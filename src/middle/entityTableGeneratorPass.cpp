#include "../cmn/node.hpp"
#include "../prattle/pass.hpp"

using namespace prattle;
using namespace prattle::pass;

namespace {

class instanceFinder : public gTeXVisitor {
public:
   explicit instanceFinder(const std::string& type) : m_type(type) {}

   virtual void visit(labelNode& n) { visitChildren(n); }
   virtual void visit(paragraphNode& n) { visitChildren(n); }
   virtual void visit(entityInstanceNode& n)
   {
      if(m_type == n.type)
         ids.insert(n.id);
   }

   std::set<std::string> ids;

private:
   std::string m_type;
};

class tablePopulator : public gTeXVisitor {
public:
   tablePopulator(std::set<std::string>& legalIds, tableNode& t)
   : m_legalIds(legalIds), m_t(t) {}

   virtual void visit(labelNode& n)
   {
      if(m_t.action != n.action) return;
      if(m_legalIds.find(n.label)==m_legalIds.end()) return;

      m_t.operandsToLabels[n.label] = n.id();
   }

private:
   std::set<std::string>& m_legalIds;
   tableNode& m_t;
};

} // anonymous namespace

class entityTableGeneratorPass : public iPass {
public:
   void run(config& c, void *pIr)
   {
      auto *pRoot = reinterpret_cast<folderNode*>(pIr);

      // find all the entities
      std::vector<entityNode*> entities;
      pRoot->searchDown<entityNode>([](auto&){ return true; },entities); // TODO lame!

      // create a table for each entity action
      for(entityNode *pE : entities)
      {
         for(auto& action : pE->actions)
         {
            auto& t = pRoot->appendChild<tableNode>();
            t.entityType = pE->name;
            t.action = action;

            instanceFinder insts(t.entityType);
            pRoot->acceptVisitor(insts);
            tablePopulator v(insts.ids,t);
            pRoot->acceptVisitor(v);
         }
      }
   }
};

cdwExportPass(entityTableGeneratorPass,"",-1);
