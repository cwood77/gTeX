#include "../cmn/node.hpp"
#include "../prattle/config.hpp"
#include "../prattle/pass.hpp"
#include "iGraphIrProviderPass.hpp"
#include "node.hpp"

using namespace prattle;
using namespace prattle::pass;

namespace {

class visitor : public gTeXVisitor {
public:
   explicit visitor(graphRootNode& root) : m_root(root) {}

   virtual void visit(labelNode& n)
   {
      const bool isEntityLabel = (n.id() != n.label);

      if(!isEntityLabel)
      {
         std::vector<jumpNode*> jumps;
         n.getRoot().searchDown<jumpNode>(jumps,[&](auto&j){ return j.id == n.id(); });
         const bool isMerge = (jumps.size() == 1 && jumps[0]->markedForMerge);

         auto& v = m_root.appendChild<graphVertexNode>();
         v.pLabel = isMerge ? NULL : &n;
         v.origLblId = n.id();
         v.origLblFile = n.filePath;
         v.isMergeLabel = isMerge;
         n[m_attr] = &v;
      }

      visitChildren(n);
   }

   virtual void visit(paragraphNode& n) { visitChildren(n); }

   virtual void visit(jumpNode& n)
   {
      auto *pVertex = n.demandAncestor<labelNode>()[m_attr];
      if(n.markedForMerge)
         pVertex->origMergeLblIds.insert(n.id);
      else
         pVertex->outgoing.insert(&n);
      visitChildren(n);
   }

private:
   graphRootNode& m_root;
   attribute<graphVertexNode*> m_attr;
};

} // anonymous namespace

class graphIrBuilderPass : public iPass, public iGraphIrProviderPass {
public:
   void run(config& c, passLinks&, void *pIr)
   {
      auto *pRoot = reinterpret_cast<folderNode*>(pIr);

      visitor v(m_gRoot);
      pRoot->acceptVisitor(v);
   }

   graphRootNode& getGraphIr() { return m_gRoot; }

private:
   graphRootNode m_gRoot;
};

cdwExportPass(graphIrBuilderPass,"",-1);
