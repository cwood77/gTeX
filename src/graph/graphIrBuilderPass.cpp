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
         auto& v = m_root.appendChild<graphVertexNode>();
         v.pLabel = &n;
         v.origLblId = n.id();
         n[m_attr] = &v;
      }

      visitChildren(n);
   }

   virtual void visit(paragraphNode& n) { visitChildren(n); }

   virtual void visit(jumpNode& n)
   {
      n.demandAncestor<labelNode>()[m_attr]->outgoing.insert(&n);
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
