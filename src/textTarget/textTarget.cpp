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

      addWordCountingIf(mLdr,c,s);
   }

private:
   virtual void addWordCountingIf(module::incrementalModuleLoader& mLdr, passCatalog& c, passSchedule& s)
   {
      if(!doesWordCounting())
         return;

      // pre-observe
      mLdr.tryLoad("misc.dll");
      s.append(c.demand("wordObserverProviderPass"));
      if(writesWordCountLog())
         s.append(c.demand("wordCountPreObservePass"));
      //s.append(c.demand("histogramPreObservePass"));

      // observe
      s.append(c.demand("wordObserverPass"));

      // post-observe
      if(writesWordCountLog())
         s.append(c.demand("wordCountPostObservePass"));
      //s.append(c.demand("histogramPostObservePass"));
   }

   std::string getPassName(const std::string& baseName)
   {
      auto& tgt = m_pCfg->demand<stringSetting>("target");
      return tgt.value + baseName;
   }

   bool writesWordCountLog() { return m_pCfg->fetch<stringSetting>("wcnt:log-path"); }
   bool doesSpellCheck() { return m_pCfg->fetch<stringArraySetting>("dict:names"); }
   bool doesWordCounting() { return writesWordCountLog() || doesSpellCheck(); }

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
