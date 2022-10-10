#include "../cmn/node.hpp"
#include "../prattle/pass.hpp"
#include <sstream>
#include <stdexcept>

// here I
// - ensure all labels are used
// - ensure all jumps have labels
// - _do not_ ensure all entities have labels, as that would require every possible
//   action on every possible entity to be doable, which I don't want

using namespace prattle;
using namespace prattle::pass;

namespace {

class labelFinder : public gTeXVisitor {
public:
   virtual void visit(labelNode& n)
   {
      labelNode*& pN = table[n.id()];
      if(pN)
         throw std::runtime_error("label ID already in use: " + n.id());
      pN = &n;
   }

   std::map<std::string,labelNode*> table;
};

class refFinder : public gTeXVisitor {
public:
   virtual void visit(labelNode& n)
   {
      visitChildren(n);
   }

   virtual void visit(paragraphNode& n)
   {
      visitChildren(n);
   }

   virtual void visit(entityInstanceNode& n)
   {
      auto& entity = n.getRoot()
         .demandDown<entityNode>([&](auto& c){ return c.name == n.type; });

      for(auto& action : entity.actions)
      {
         std::stringstream name;
         name << n.id << "->" << action;
         m_eis[name.str()].insert(&n);
      }
   }

   virtual void visit(jumpNode& n)
   {
      m_jumps[n.id].insert(&n);
   }

   void buildTable(labelFinder& f, linkTableNode& t)
   {
      auto unusedLabels = f.table;

      buildTable(m_eis,  f,unusedLabels,t.l2ei, /*required*/false);
      buildTable(m_jumps,f,unusedLabels,t.l2j,  /*required*/true);

      if(unusedLabels.size())
      {
         std::stringstream msg;
         msg << unusedLabels.size() << " label(s) existed but are unreferenced?" << std::endl;
         for(auto it=unusedLabels.begin();it!=unusedLabels.end();++it)
            msg << "   " << it->first << std::endl;

         throw std::runtime_error(msg.str());
      }
   }

private:
   template<class T>
   void buildTable(
      std::map<std::string,std::set<T*> >& refer,
      labelFinder& f,
      std::map<std::string,labelNode*>& unusedLabels,
      std::map<labelNode*,std::set<T*> >& table,
      bool required)
   {
      for(auto it=refer.begin();it!=refer.end();++it)
      {
         labelNode *pL = f.table[it->first];
         if(!pL && !required)
            continue;
         if(!pL)
            throw std::runtime_error("referenced label not found: " + it->first);
         table[pL].insert(it->second.begin(),it->second.end());
         unusedLabels.erase(it->first);
      }
   }

   std::map<std::string,std::set<entityInstanceNode*> > m_eis;
   std::map<std::string,std::set<jumpNode*> > m_jumps;
};

} // anonymous namespace

class linkerPass : public iPass {
public:
   void run(config& c, void *pIr)
   {
      auto *pRoot = reinterpret_cast<folderNode*>(pIr);

      labelFinder L;
      pRoot->acceptVisitor(L);
      refFinder R;
      pRoot->acceptVisitor(R);

      // combine the findings
      auto& table = pRoot->appendChild<linkTableNode>();
      R.buildTable(L,table);
   }
};

cdwExportPass(linkerPass,"",-1);
