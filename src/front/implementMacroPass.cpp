#include "../cmn/node.hpp"
#include "../prattle/config.hpp"
#include "../prattle/pass.hpp"

using namespace prattle;
using namespace prattle::pass;

namespace {

class replaceMacroVisitor : public gTeXVisitor {
public:
   virtual void visit(labelNode& n) { visitChildren(n); }
   virtual void visit(paragraphNode& n) { visitChildren(n); }

   virtual void visit(callMacroNode& n)
   {
      visitChildren(n);

      auto& decl = n.getRoot().demandDown<declMacroNode>(
         [&](auto&m){ return m.name == n.name; });

      treeCloningVisitor clone;
      decl.getChildren()[0]->acceptVisitor(clone);
      n.replace(*clone.pClone.release());
   }
};

class removeMacroDeclsVisitor : public gTeXVisitor {
public:
   virtual void visit(declMacroNode& n)
   {
      n.Delete();
   }
};

} // anonymous namespace

class implementMacroPass : public iPass {
public:
   void run(config& c, passLinks&, void *pIr)
   {
      auto *pRoot = reinterpret_cast<folderNode*>(pIr);

      while(true)
      {
         nodeEditOperation o;
         {
            nodeEditCollector c(o);
            replaceMacroVisitor v;
            pRoot->acceptVisitor(v);
         }
         if(o.commit() == 0)
            break;
      }

      nodeEditOperation o;
      {
         nodeEditCollector c(o);
         removeMacroDeclsVisitor v;
         pRoot->acceptVisitor(v);
      }
      o.commit();
   }
};

cdwExportPass(implementMacroPass,"",-1);
