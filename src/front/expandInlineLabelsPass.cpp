#include "../cmn/node.hpp"
#include "../prattle/pass.hpp"

using namespace prattle;
using namespace prattle::pass;

namespace {

class visitor : public gTeXVisitor {
public:
   virtual void visit(inlineLabelNode& n)
   {
      auto& prevLbl = dynamic_cast<labelNode&>(*n.demandParent().getChildren()
         [n.demandParent().getIndexOfChild(n)-1]);
      auto *pRoot = new node();
      auto& j = pRoot->appendChild<jumpNode>();
      dupSetup(n,j);
      j.id = n.label;

      auto *pL = new labelNode();
      dupSetup(n,*pL);
      pL->label = n.label;
      pL->markedForMerge = true;

      n.reparentChildren(*pL);
      j.reparent(prevLbl);
      n.replace(*pL);
      nodeEditCollector::head()->op.defer([=](){ delete pRoot; });
   }

private:
   void dupSetup(const node& src, node& dest)
   {
      dest.filePath = src.filePath;
      dest.lineNumber = src.lineNumber;
   }
};

} // anonymous namespace

class expandInlineLabelsPass : public iPass {
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

cdwExportPass(expandInlineLabelsPass,"",-1);
