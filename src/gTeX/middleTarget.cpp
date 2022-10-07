#include "../prattle/pass.hpp"
#include "parsePass.hpp"

using namespace prattle;
using namespace prattle::pass;

class middleTarget : public iTarget {
public:
   virtual void configure(config& c) {}
   virtual std::string getPredecessorTarget() { return ""; }
   virtual void adjustPasses(passCatalog& c, passSchedule& s)
   {
      s.append(c.demand<loadPass>());
   }
};

autoTargetInfo<middleTarget> gMiddleTgt("middle");
