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
      std::stringstream msg;
      msg << "--- " << n.label << " ----------------------------------------";

      auto *p = new paragraphNode();
      p->text = msg.str();

      // 'flatten' my children under me
      n.reparentChildren(n.demandParent(),&n);

      // replace myself with a paragraph
      n.replace(*p);
   }
};

} // anonymous namespace

class labelFormatterPass : public iPass {
public:
   void run(config& c, void *pIr)
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

cdwExportPass(labelFormatterPass,"",-1);
