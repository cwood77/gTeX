#include "../prattle/config.hpp"
#include "../prattle/pass.hpp"

using namespace prattle;
using namespace prattle::pass;

class textTargetDefaultOptionPass : public iPass {
public:
   virtual void run(config& c, void *pIr)
   {
      auto& s = c.createOrFetch<stringSetting>("text:out-path");
      if(s.value.empty())
         s.value = "testdata.txt-out";
   }
};

cdwExportPass(textTargetDefaultOptionPass,"cfg:target",0);

class textTarget : public iTarget {
public:
   virtual void configure(config& c) {}
   virtual std::string getPredecessorTarget() { return "middleTarget"; }
   virtual void adjustPasses(passCatalog& c, passSchedule& s)
   {
      s.append(c.demand("labelFormatterPass"));
      s.append(c.demand("textPrintPass"));
   }
};

cdwExportTarget(textTarget);
