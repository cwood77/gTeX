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
   virtual void visit(graphSubgraphNode& n)
   {
      n.reparentChildren(n.demandParent(),&n);
      n.Delete();
   }
   virtual void visit(graphVertexNode& n) {}
};

} // anonymous namespace

class subgraphRemovingPass : public iPass {
public:
   void run(config& c, passLinks& l, void *)
   {
      auto& root = l.demandLink<iGraphIrProviderPass>().getGraphIr();

      nodeEditOperation o;
      {
         nodeEditCollector c(o);
         visitor v;
         root.acceptVisitor(v);
      }
      o.commit();
   }
};

cdwExportPass(subgraphRemovingPass,"",-1);
