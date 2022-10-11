#include "../cmn/node.hpp"
#include "../prattle/pass.hpp"

using namespace prattle;
using namespace prattle::pass;

namespace {

class visitor : public gTeXVisitor {
public:
   virtual void visit(labelNode& n)
   {
      visitChildren(n);
   }

   virtual void visit(paragraphNode& n)
   {
      visitChildren(n);
   }

   virtual void visit(jumpNode& n)
   {
      auto *p = new paragraphNode();
      p->text = n.id;
      n.replace(*p);
   }
};

} // anonymous namespace

class jumpFormatterPass : public iPass {
public:
   void run(config& c, passLinks&, void *pIr)
   {
      auto *pRoot = reinterpret_cast<folderNode*>(pIr);

      nodeEditOperation o;
      {
         nodeEditCollector c(o);
         visitor v;
         pRoot->acceptVisitor(v);
      }
      o.commit();
   }
};

cdwExportPass(jumpFormatterPass,"",-1);
