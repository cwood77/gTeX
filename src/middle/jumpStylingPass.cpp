#include "../cmn/node.hpp"
#include "../prattle/pass.hpp"
#include "stylingVisitorBase.hpp"
#include <cstring>

using namespace prattle;
using namespace prattle::pass;

namespace {

class visitor : public stylingVisitorBase, public gTeXVisitor {
public:
   explicit visitor(iStyler& s) : stylingVisitorBase(s) {}

   virtual void visit(labelNode& n) { visitChildren(n); }
   virtual void visit(paragraphNode& n) { visitChildren(n); }

   virtual void visit(jumpNode& n)
   {
      if(n.markedForMerge) return;

      auto style = m_styler.getFmt(iStyler::kJump);
      if(style.empty())
         return;

      m_ans[&n] = style;
   }
};

} // anonymous namespace

class jumpStylingPass : public iPass {
public:
   void run(config&, passLinks& l, void *pIr)
   {
      auto *pRoot = reinterpret_cast<folderNode*>(pIr);
      auto& styler = l.demandLink<iStyler>();

      visitor v(styler);
      pRoot->acceptVisitor(v);
      v.replaceObjects();
   }
};

cdwExportPass(jumpStylingPass,"",-1);
