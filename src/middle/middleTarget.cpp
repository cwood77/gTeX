#include "../prattle/pass.hpp"

using namespace prattle;
using namespace prattle::pass;

class middleTarget : public iTarget {
public:
   virtual void configure(config& c) {}
   virtual std::string getPredecessorTarget() { return "frontTarget"; }
   virtual void adjustPasses(passCatalog& c, passSchedule& s)
   {
      s.append(c.demand("linkerPass"));
      s.append(c.demand("entityRemovalPass"));
   }
};

cdwExportTarget(middleTarget);
