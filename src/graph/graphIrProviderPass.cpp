#include "../prattle/pass.hpp"

using namespace prattle;
using namespace prattle::pass;

class graphIrProviderPass : public iPass {
public:
   virtual void decompose(passCatalog& c, std::list<iPass*>& updated)
   {
      updated.push_back(c.demand("graphIrBuilderPass").create());
      delete this;
   }

   void run(config& c, passLinks& l, void *pIr)
   {
      throw std::runtime_error("unimplemented");
   }
};

cdwExportPass(graphIrProviderPass,"",-1);
