#include "../prattle/pass.hpp"
#include "iGraphIrProviderPass.hpp"
#include "node.hpp"

using namespace prattle;
using namespace prattle::pass;

namespace {

class visitor : public iGraphNodeVisitor {
public:
   virtual void visit(node& n) { visitChildren(n); }
   virtual void visit(graphRootNode& n) { visitChildren(n); }
   virtual void visit(graphSubgraphNode& n) { visitChildren(n); }

   virtual void visit(graphVertexNode& n)
   {
      if(n.isMergeLabel)
         return;

      gatherEdgesFromAllMerges(n,n);

      n.origMergeLblIds.clear();
   }

private:
   void gatherEdgesFromAllMerges(graphVertexNode& root, graphVertexNode& candidate)
   {
      for(auto mergeLbl : candidate.origMergeLblIds)
      {
         auto& dst = root.getRoot().demandDown<graphVertexNode>(
            [&](auto&v){ return v.isMergeLabel && (v.origLblId == mergeLbl); });

         for(auto *pJmp : dst.outgoing)
            root.outgoing.insert(pJmp);

         gatherEdgesFromAllMerges(root,dst);
      }
   }
};

} // anonymous namespace

class mergeVertexRemovingPass : public iPass {
public:
   void run(config& c, passLinks& l, void *)
   {
      auto& root = l.demandLink<iGraphIrProviderPass>().getGraphIr();

      nodeEditOperation o;
      {
         nodeEditCollector c(o);

         // combine jumps into the first non-merge node
         visitor v;
         root.acceptVisitor(v);

         // delete all merge nodes
         std::vector<graphVertexNode*> merges;
         root.searchDown<graphVertexNode>(merges,[](auto&v){ return v.isMergeLabel; });
         for(auto *pDeadGuy : merges)
            pDeadGuy->Delete();
      }
      o.commit();
   }
};

cdwExportPass(mergeVertexRemovingPass,"",-1);
