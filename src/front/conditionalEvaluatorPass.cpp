#include "../cmn/node.hpp"
#include "../prattle/config.hpp"
#include "../prattle/pass.hpp"

using namespace prattle;
using namespace prattle::pass;

namespace {

class visitor : public gTeXVisitor {
public:
   explicit visitor(config& c) : m_c(c) {}

   virtual void visit(ifNode& n)
   {
      visitChildren(n);

      auto& v = m_c.demand<boolSetting>(n.varName).value;
      if((v && !n.isFalse) || (!v && n.isFalse))
         n.reparentChildren(n.demandParent(),&n);
      n.Delete();
   }

   virtual void visit(labelNode& n) { visitChildren(n); }

private:
   config& m_c;
};

} // anonymous namespace

class conditionalEvaluatorPass : public iPass {
public:
   void run(config& c, passLinks&, void *pIr)
   {
      auto *pRoot = reinterpret_cast<folderNode*>(pIr);

      nodeEditOperation o;
      {
         nodeEditCollector _c(o);
         visitor v(c);
         pRoot->acceptVisitor(v);
      }
      o.commit();
   }
};

cdwExportPass(conditionalEvaluatorPass,"",-1);
