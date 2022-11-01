#include "../cmn/node.hpp"
#include "../prattle/config.hpp"
#include "../prattle/pass.hpp"
#include "iGraphIrProviderPass.hpp"
#include "node.hpp"

using namespace prattle;
using namespace prattle::pass;

namespace {

class labelFindingVisitor : public gTeXVisitor {
public:
   explicit labelFindingVisitor(graphRootNode& root) : m_root(root) {}

   attribute<graphVertexNode*> attr;

   virtual void visit(labelNode& n)
   {
      const bool isEntityLabel = (n.id() != n.label);

      if(!isEntityLabel && !isAttachedTable(n))
      {
         std::vector<jumpNode*> jumps;
         n.getRoot().searchDown<jumpNode>(jumps,[&](auto&j){ return j.id == n.id(); });
         const bool isMerge = (jumps.size() == 1 && jumps[0]->markedForMerge);

         auto& v = m_root.appendChild<graphVertexNode>();
         v.pLabel = isMerge ? NULL : &n;
         v.origLblId = n.id();
         v.origLblFile = n.filePath;
         v.isMergeLabel = isMerge;
         n[attr] = &v;
      }

      visitChildren(n);
   }

   virtual void visit(paragraphNode& n) { visitChildren(n); }

   static bool isAttachedTable(labelNode& n)
   {
      return
         n.getChildren().size() > 0 &&
         dynamic_cast<tableNode*>(n.getChildren()[0]);
   }

private:
   graphRootNode& m_root;
};

class jumpResolvingVisitor : public gTeXVisitor {
public:
   jumpResolvingVisitor(graphRootNode& root, attribute<graphVertexNode*>& attr)
   : m_root(root), m_attr(attr) {}

   virtual void visit(labelNode& n) { visitChildren(n); }
   virtual void visit(paragraphNode& n) { visitChildren(n); }

   virtual void visit(jumpNode& n)
   {
      auto *pVertex = n.demandAncestor<labelNode>()[m_attr];
      if(pVertex)
      {
         auto& tgt = n.getRoot().demandDown<labelNode>(
            [&](auto&l){ return l.id() == n.id; });
         if(labelFindingVisitor::isAttachedTable(tgt))
            ; // do not record in DOT
         else if(n.markedForMerge)
            pVertex->origMergeLblIds.insert(n.id);
         else
            pVertex->outgoing.insert(&n);
      }
      visitChildren(n);
   }

private:
   graphRootNode& m_root;
   attribute<graphVertexNode*>& m_attr;
};

} // anonymous namespace

class graphIrBuilderPass : public iPass, public iGraphIrProviderPass {
public:
   void run(config& c, passLinks&, void *pIr)
   {
      auto *pRoot = reinterpret_cast<folderNode*>(pIr);

      labelFindingVisitor lv(m_gRoot);
      pRoot->acceptVisitor(lv);

      jumpResolvingVisitor jv(m_gRoot,lv.attr);
      pRoot->acceptVisitor(jv);
   }

   graphRootNode& getGraphIr() { return m_gRoot; }

private:
   graphRootNode m_gRoot;
};

cdwExportPass(graphIrBuilderPass,"",-1);
