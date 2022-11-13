#include "../prattle/pass.hpp"
#include "dotPrintPass.hpp"

using namespace prattle;
using namespace prattle::pass;

class overallDotPrintPass : public iPass {
public:
   virtual void decompose(passCatalog& c, std::list<iPass*>& updated)
   {
      auto *pPass = dynamic_cast<dotPrintPass*>(c.demand("dotPrintPass").create());
      pPass->outCfgKeyName = "dot:out";
      updated.push_back(pPass);

      updated.push_back(c.demand("mergeVertexRemovingPass").create());

      pPass = dynamic_cast<dotPrintPass*>(c.demand("dotPrintPass").create());
      pPass->outCfgKeyName = "dot-s1:out";
      updated.push_back(pPass);

      updated.push_back(c.demand("subgraphRemovingPass").create());

      pPass = dynamic_cast<dotPrintPass*>(c.demand("dotPrintPass").create());
      pPass->outCfgKeyName = "dot-s2:out";
      updated.push_back(pPass);

      delete this;
   }

   void run(config& c, passLinks& l, void *pIr)
   {
      throw std::runtime_error("unimplemented");
   }
};

cdwExportPass(overallDotPrintPass,"",-1);
