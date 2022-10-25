#include "../cmn/node.hpp"
#include "../prattle/pass.hpp"
#include <sstream>

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

   virtual void visit(entityInstanceNode& n)
   {
      std::stringstream text;
      text << "(" <<  n.id << ")";

      auto *p = new paragraphNode();
      p->text = text.str();
      p->combineWithFollowingPunct = true;
      n.replace(*p);
   }
};

} // anonymous namespace

class entityInstanceFormatterPass : public iPass {
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

cdwExportPass(entityInstanceFormatterPass,"",-1);
