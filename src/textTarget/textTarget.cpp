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
      // pre-count formatter (i.e. everything contributing to word count)
      s.append(c.demand("jumpFormatterPass"));
      s.append(c.demand("entityInstanceFormatterPass"));
      s.append(c.demand("expandedParagraphStylingPass"));
      s.append(c.demand("contractParagraphPass"));

      // word counting
      if(m_pCfg->fetch<stringSetting>("wcnt:log-path"))
      {
         mLdr.tryLoad("misc.dll");
         s.append(c.demand("overallWordCountingPass"));
      }

      // post-count formatter (i.e. everything not contributing to word count)
      s.append(c.demand("labelFormatterPass"));
      s.append(c.demand("tableFormatterPass"));
      // [graph] mapFormatterPass

      // the end
      s.append(c.demand("textPrintPass"));
   }

private:
   config *m_pCfg;
};

cdwExportTarget(textTarget);
