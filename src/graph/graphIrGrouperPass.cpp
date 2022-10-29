#include "../cmn/node.hpp"
#include "../prattle/pass.hpp"
#include "iGraphIrProviderPass.hpp"
#include "node.hpp"

using namespace prattle;
using namespace prattle::pass;

namespace {

class visitor : public iGraphNodeVisitor {
public:
   virtual void visit(node& n) { visitChildren(n); }

   virtual void visit(graphRootNode& n)
   {
      visitChildren(n);

      for(auto it=m_verticiesByCategory.begin();it!=m_verticiesByCategory.end();++it)
      {
         auto& subGraph = n.appendChild<graphSubgraphNode>();
         subGraph.name = it->first;
         for(auto *pVertex : it->second)
            pVertex->reparent(subGraph);
      }
   }

   virtual void visit(graphSubgraphNode& n) { visitChildren(n); }

   virtual void visit(graphVertexNode& n)
   {
      auto& set = m_verticiesByCategory[n.origLblFile];
      set.insert(&n);
   }

private:
   std::map<std::string,std::set<graphVertexNode*> > m_verticiesByCategory;
};

} // anonymous namespace

class graphIrGrouperPass : public iPass {
public:
   void run(config& c, passLinks& l, void *)
   {
      auto& root = l.demandLink<iGraphIrProviderPass>().getGraphIr();

      visitor v;
      root.acceptVisitor(v);
   }
};

cdwExportPass(graphIrGrouperPass,"",-1);
