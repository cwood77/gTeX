#include "../prattle/config.hpp"
#include "../prattle/module.hpp"
#include "../prattle/pass.hpp"

using namespace prattle;
using namespace prattle::pass;

class textTargetDefaultOptionPass : public iPass {
public:
   virtual void run(config& c, passLinks&, void *pIr)
   {
      //c.createOrFetch<stringSetting>("text:out-path",[](auto&s){ s.value="out.txt"; });
   }
};

cdwExportPass(textTargetDefaultOptionPass,"cfg:target",0);

class textTarget : public iTarget {
public:
   textTarget() : m_pCfg(NULL) {}

   virtual void configure(config& c) { m_pCfg = &c; }
   virtual std::string getPredecessorTarget() { return "middleTarget"; }
   virtual void adjustPasses(module::incrementalModuleLoader& mLdr, passCatalog& c, passSchedule& s)
   {
      s.append(c.demand("jumpFormatterPass"));
      s.append(c.demand("entityInstanceFormatterPass"));
      s.append(c.demand("contractParagraphPass"));

      if(m_pCfg->fetch<stringSetting>("wcnt:log-path"))
      {
         mLdr.tryLoad("misc.dll");
         s.append(c.demand("overallWordCountingPass"));
      }

      s.append(c.demand("labelFormatterPass"));
      // tableFormatter
      // [graph] mapFormatterPass
      s.append(c.demand("textPrintPass"));
   }

private:
   config *m_pCfg;
};

cdwExportTarget(textTarget);
