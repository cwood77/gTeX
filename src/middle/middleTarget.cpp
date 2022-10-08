#include "../prattle/pass.hpp"

using namespace prattle;
using namespace prattle::pass;

class middleTarget : public iTarget {
public:
   virtual void configure(config& c) {}
   virtual std::string getPredecessorTarget() { return "front"; }
   virtual void adjustPasses(passCatalog& c, passSchedule& s)
   {
      extern const char *kEntityRemovalPassName;
      s.append(c.demand(kEntityRemovalPassName));
   }
};

autoTargetInfo<middleTarget> gMiddleTgt("middle");
