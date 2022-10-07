#include "../prattle/config.hpp"
#include "../prattle/pass.hpp"

using namespace prattle;
using namespace prattle::pass;

class defaultTargetPass : public iPass {
public:
   virtual void run(config& c, void *pIr)
   {
      auto& s = c.createOrFetch<stringSetting>("target");
      if(s.value.empty())
         s.value = "text";
   }
};

autoPassInfo<defaultTargetPass> gDefTgtPass("cfg",0);

class textTargetDefaultOptionPass : public iPass {
public:
   virtual void run(config& c, void *pIr)
   {
      auto& s = c.createOrFetch<stringSetting>("text:out-path");
      if(s.value.empty())
         s.value = "testdata.txt-out";
   }
};

autoPassInfo<textTargetDefaultOptionPass> txtDefOptPass("cfg",1);

class textTarget : public iTarget {
public:
   virtual void configure(config& c) {}
   virtual std::string getPredecessorTarget() { return "middle"; }
   virtual void adjustPasses(passCatalog& c, passSchedule& s)
   {
      extern const char *kTextPrintPassName;
      s.append(c.demand(kTextPrintPassName));
   }
};

autoTargetInfo<textTarget> gTextTgt("text");
