#include "../cmn/node.hpp"
#include "../prattle/pass.hpp"

using namespace prattle;
using namespace prattle::pass;

namespace {

class counterRefVisitor : public gTeXVisitor {
public:
   counterRefVisitor() : doTotals(false) {}

   bool doTotals;

   virtual void visit(labelNode& n) { visitChildren(n); }
   virtual void visit(paragraphNode& n) { visitChildren(n); }

   virtual void visit(varRefNode& n)
   {
      auto& decl = n.getRoot().demandDown<varDeclNode>(
         [&](auto&d){ return d.name == n.baseName; });

      if(decl.type != "counter")
         return;

      std::stringstream ans;
      if(n.suffix == "total")
      {
         if(doTotals)
         {
            ans << m_nextId[n.baseName];
         }
      }
      else if(n.suffix.empty())
      {
         if(!doTotals)
         {
            // value mode
            auto& attr = m_ids[n.baseName];
            auto& lbl = n.demandAncestor<labelNode>();
            auto& id = lbl[attr];
            if(id == 0)
            {
               // uninitialized
               auto& nxt = m_nextId[n.baseName];
               id = ++nxt;
            }
            ans << id;
         }
      }
      else
         throw std::runtime_error("don't know var suffix: " + n.suffix);

      if(!ans.str().empty())
      {
         auto *p = new paragraphNode();
         p->text = ans.str();
         p->combineWithFollowingPunct = true;
         n.replace(*p);
      }
   }

private:
   std::map<std::string,attribute<size_t> > m_ids;
   std::map<std::string,size_t> m_nextId;
};

class counterDeclVisitor : public gTeXVisitor {
public:
   virtual void visit(varDeclNode& n)
   {
      if(n.type != "counter")
         return;

      n.Delete();
   }
};

} // anonymous namespace

class evaluateCounterVarPass : public iPass {
public:
   void run(config& c, passLinks&, void *pIr)
   {
      auto *pRoot = reinterpret_cast<folderNode*>(pIr);

      nodeEditOperation o;
      {
         nodeEditCollector c(o);
         counterRefVisitor v;
         pRoot->acceptVisitor(v);
         v.doTotals = true;
         pRoot->acceptVisitor(v);
      }
      {
         nodeEditCollector c(o);
         counterDeclVisitor v;
         pRoot->acceptVisitor(v);
      }
      o.commit();
   }
};

cdwExportPass(evaluateCounterVarPass,"",-1);
