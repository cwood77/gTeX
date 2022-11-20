#include "../prattle/config.hpp"
#include "../prattle/module.hpp"
#include "../prattle/pass.hpp"

using namespace prattle;
using namespace prattle::pass;

class textTargetDefaultOptionPass : public iPass {
public:
   virtual void run(config& c, passLinks&, void *pIr)
   {
      c.createOrFetch<stringSetting>("lead-in:nWords",[](auto&s){ s.value="5"; });
   }
};

cdwExportPass(textTargetDefaultOptionPass,"cfg:target",0);

class textPreTarget : public iTarget {
public:
   textPreTarget() : m_pCfg(NULL) {}

   virtual void configure(config& c) { m_pCfg = &c; }
   virtual std::string getPredecessorTarget() { return "middleTarget"; }
   virtual void adjustPasses(module::incrementalModuleLoader& mLdr, passCatalog& c, passSchedule& s)
   {
      // pre-count formatter (i.e. everything contributing to word count)
      s.append(c.demand("jumpFormatterPass"));
      s.append(c.demand("entityInstanceFormatterPass"));
      s.append(c.demand("leadInPass"));
      s.append(c.demand(getPassName("FormatProviderPass")));
      s.append(c.demand("expandedParagraphStylingPass"));
      s.append(c.demand("contractParagraphPass"));

      // word counting
      if(m_pCfg->fetch<stringSetting>("wcnt:log-path"))
      {
         mLdr.tryLoad("misc.dll");
         s.append(c.demand("histogramProviderPass"));
         s.append(c.demand("overallWordCountingPass"));
         s.append(c.demand("histogramPrintingPass"));
      }
   }

private:
   std::string getPassName(const std::string& baseName)
   {
      auto& tgt = m_pCfg->demand<stringSetting>("target");
      return tgt.value + baseName;
   }

   config *m_pCfg;
};

cdwExportTarget(textPreTarget);

class textTarget : public iTarget {
public:
   textTarget() : m_pCfg(NULL) {}

   virtual void configure(config& c) { m_pCfg = &c; }
   virtual std::string getPredecessorTarget() { return "textPreTarget"; }
   virtual void adjustPasses(module::incrementalModuleLoader& mLdr, passCatalog& c, passSchedule& s)
   {
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
