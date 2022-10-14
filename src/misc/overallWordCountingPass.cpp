#include "../prattle/pass.hpp"

using namespace prattle;
using namespace prattle::pass;

class overallWordCountingPass : public iPass {
public:
   virtual void decompose(passCatalog& c, std::list<iPass*>& updated)
   {
      updated.push_back(c.demand("paragraphWordCountingPass").create());
      updated.push_back(c.demand("wordCountRollupPass").create());
      updated.push_back(c.demand("wordCountReporterPass").create());
      delete this;
   }

   void run(config& c, passLinks& l, void *pIr)
   {
      throw std::runtime_error("unimplemented");
   }
};

cdwExportPass(overallWordCountingPass,"",-1);
