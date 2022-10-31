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
      std::vector<labelNode*> labels;
      n.getRoot().searchDown<labelNode>(labels,[&](auto&l){ return l.id() == n.id; });
      bool isAttachedTableLabel =
         labels.size() == 1 &&
         labels[0]->getChildren().size() > 0 &&
         dynamic_cast<tableNode*>(labels[0]->getChildren()[0]);

      std::stringstream text;
      if(isAttachedTableLabel)
         text << "(" <<  n.id << ")";
      else
         text << n.id;

      auto *p = new paragraphNode();
      p->text = text.str();
      p->combineWithFollowingPunct = true;
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
