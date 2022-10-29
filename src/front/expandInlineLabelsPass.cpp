#include "../cmn/node.hpp"
#include "../prattle/pass.hpp"

using namespace prattle;
using namespace prattle::pass;

namespace {

class visitor : public gTeXVisitor {
public:
   virtual void visit(inlineLabelNode& n)
   {
      // the sibling previous to 'n' will receive the merge jump
      auto& prevSibling = *n.demandParent().getChildren()
         [n.demandParent().getIndexOfChild(n)-1];

      // create a jump on a new paragraph an a temp tree
      auto *pRoot = new node();
      auto& p = pRoot->appendChild<paragraphNode>();
      dupSetup(n,p);
      auto& j = p.appendChild<jumpNode>();
      dupSetup(n,j);
      j.id = n.label;
      j.markedForMerge = true;

      auto *pL = new labelNode();
      dupSetup(n,*pL);
      pL->label = n.label;

      n.reparentChildren(*pL);
      p.reparent(prevSibling);
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

#if 0
      nodeEditOperation o;
      {
         nodeEditCollector c(o);
         visitor v;
         pRoot->acceptVisitor(v);
      }
      o.commit();
#endif

      while(true)
      {
         std::vector<inlineLabelNode*> nodes;
         pRoot->searchDown<inlineLabelNode>(nodes);
         if(nodes.size() == 0) break;
         auto& n = *nodes[0];

         nodeEditOperation o;
         {
            nodeEditCollector c(o);
            visitor v;
            n.acceptVisitor(v);
         }
         o.commit();
      }
   }
};

cdwExportPass(expandInlineLabelsPass,"",-1);
