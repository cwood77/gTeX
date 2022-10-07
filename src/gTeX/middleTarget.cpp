#include "../prattle/pass.hpp"

using namespace prattle;
using namespace prattle::pass;

class middleTarget : public iTarget {
public:
   virtual void configure(config& c) {}
   virtual std::string getPredecessorTarget() { return ""; }
   virtual void adjustPasses(passCatalog& c, passSchedule& s)
   {
      extern const char *kLoadPassName;
      s.append(c.demand(kLoadPassName));
   }
};

autoTargetInfo<middleTarget> gMiddleTgt("middle");
