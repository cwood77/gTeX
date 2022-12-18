#include "../cmn/nameBank.hpp"
#include "../cmn/node.hpp"
#include "../prattle/config.hpp"
#include "../prattle/pass.hpp"
#include "iStyler.hpp"
#include <cstring>

using namespace prattle;
using namespace prattle::pass;

namespace {

class visitor : public gTeXVisitor {
public:
   visitor(config& c, iStyler& s) : m_cfg(c), m_styler(s) {}

   virtual void visit(labelNode& n) { visitChildren(n); }
   virtual void visit(paragraphNode& n) { visitChildren(n); }

   virtual void visit(varRefNode& n)
   {
      // par        par
      //   par        par
      //   varRef     style  **
      //   par        varRef
      //              style  **
      //              par

      auto& decl = n.getRoot().demandDown<varDeclNode>(
         [&](auto&d){ return d.name == n.baseName; });

      if(decl.type.length() <= 7 && ::strncmp(decl.type.c_str(),"random<",7)!=0)
         return;

      auto style = m_styler.getFmt(iStyler::kRndVar);
      if(style.empty())
         return;

      m_ans[&n] = style;
   }

   void replaceObjects()
   {
      for(auto it=m_ans.begin();it!=m_ans.end();++it)
      {
         auto *pVarRef = it->first;

         auto& sBeginNode = pVarRef->demandParent().appendChild<stylingNode>();
         sBeginNode.style = it->second;

         auto& sEndNode = pVarRef->demandParent().appendChild<stylingNode>();
         sEndNode.style = it->second;

         auto& nodes = pVarRef->demandParent().getChildren();
         auto vrIdx = pVarRef->demandParent().getIndexOfChild(*pVarRef);
         auto jit = nodes.begin();
         nodes.insert(std::next(jit,vrIdx),&sBeginNode);
         jit = nodes.begin();
         nodes.insert(std::next(jit,vrIdx+1+1),&sEndNode);
         nodes.resize(nodes.size()-2);
      }
   }

private:
   config& m_cfg;
   iStyler& m_styler;
   std::map<varRefNode*,std::string> m_ans;
};

} // anonymous namespace

class varRefStylingPass : public iPass {
public:
   void run(config& c, passLinks& l, void *pIr)
   {
      auto *pRoot = reinterpret_cast<folderNode*>(pIr);
      auto& styler = l.demandLink<iStyler>();

      visitor v(c,styler);
      pRoot->acceptVisitor(v);
      v.replaceObjects();
   }
};

cdwExportPass(varRefStylingPass,"",-1);
