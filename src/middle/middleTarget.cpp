#include "../prattle/pass.hpp"

using namespace prattle;
using namespace prattle::pass;

class middleTarget : public iTarget {
public:
   virtual void configure(config& c) {}
   virtual std::string getPredecessorTarget() { return "frontTarget"; }
   virtual void adjustPasses(module::incrementalModuleLoader&, passCatalog& c, passSchedule& s)
   {
      s.append(c.demand("evaluateCounterVarPass"));
      s.append(c.demand("ensureNoVarsPass"));

      s.append(c.demand("outFileCreatorPass"));

      s.append(c.demand("entityTableGeneratorPass"));
      s.append(c.demand("entityInstanceRandomizerPass"));

      // link for merge
      // merge
      // unlink for merge

      s.append(c.demand("linkerPass"));
      s.append(c.demand("entityRemovalPass"));
      // DOT printer
      s.append(c.demand("labelRandomizerPass"));
      s.append(c.demand("unlinkerPass"));

      s.append(c.demand("labelMoverPass"));
   }
};

cdwExportTarget(middleTarget);
