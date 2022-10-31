#include "../cmn/node.hpp"
#include "../prattle/pass.hpp"

using namespace prattle;
using namespace prattle::pass;

namespace {

class visitor : public gTeXVisitor {
public:
   visitor() : m_cached(false) {}

   virtual void visit(labelNode& n) { visitChildren(n); }
   virtual void visit(paragraphNode& n) { visitChildren(n); }

   virtual void visit(attachActionNode& n)
   {
      demandEntity(n,n.entityType)
         .actions.push_back(n.action);
   }

private:
   entityNode& demandEntity(node& n, const std::string& name)
   {
      if(!m_cached)
      {
         m_cached = true;
         std::vector<entityNode*> entities;
         n.getRoot().searchDown<entityNode>(entities);
         for(auto *pE : entities)
            m_entities[pE->name] = pE;
      }
      return *m_entities[name];
   }

   bool m_cached;
   std::map<std::string,entityNode*> m_entities;
};

} // anonymous namespace

class gatherAndAttachEntityActionPass : public iPass {
public:
   void run(config& c, passLinks&, void *pIr)
   {
      auto *pRoot = reinterpret_cast<folderNode*>(pIr);

      visitor v;
      pRoot->acceptVisitor(v);
   }
};

cdwExportPass(gatherAndAttachEntityActionPass,"",-1);
