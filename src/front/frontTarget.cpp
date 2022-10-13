#include "../prattle/pass.hpp"

using namespace prattle;
using namespace prattle::pass;

class frontTarget : public iTarget {
public:
   virtual void configure(config& c) {}
   virtual std::string getPredecessorTarget() { return ""; }
   virtual void adjustPasses(module::incrementalModuleLoader&, passCatalog& c, passSchedule& s)
   {
      s.append(c.demand("fileEnumPass"));
      s.append(c.demand("parsePass"));
      s.append(c.demand("expandParagraphPass"));
   }
};

cdwExportTarget(frontTarget);
