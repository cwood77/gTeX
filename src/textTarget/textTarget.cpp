#include "../prattle/config.hpp"
#include "../prattle/module.hpp"
#include "../prattle/pass.hpp"

using namespace prattle;
using namespace prattle::pass;

class textTargetDefaultOptionPass : public iPass {
public:
   virtual void run(config& c, passLinks&, void *pIr)
   {
      {
         auto& s = c.createOrFetch<stringSetting>("text:out-path");
         if(s.value.empty())
            s.value = "out.txt";
      }
      {
         auto& s = c.createOrFetch<stringSetting>("wcnt:log-path");
         if(s.value.empty())
            s.value = "wordcount-log.txt";
      }
   }
};

cdwExportPass(textTargetDefaultOptionPass,"cfg:target",0);

class textTarget : public iTarget {
public:
   textTarget() : m_pCfg(NULL) {}

   virtual void configure(config& c) { m_pCfg = &c; }
   virtual std::string getPredecessorTarget() { return "middleTarget"; }
   virtual void adjustPasses(module::moduleLoader& mLdr, passCatalog& c, passSchedule& s)
   {
      s.append(c.demand("jumpFormatterPass"));
      s.append(c.demand("entityInstanceFormatterPass"));
      s.append(c.demand("contractParagraphPass"));

      //if(auto *pLogPath = m_pCfg->fetch<stringSetting>("wcnt:log-path")) // TODO HACK
      if(!m_pCfg->createOrFetch<stringSetting>("wcnt:log-path").value.empty())
      {
         mLdr.tryLoad("misc.dll");
         mLdr.collect(c,targetCatalog::get()); // TODO HACK
         s.append(c.demand("paragraphWordCountingPass"));
         s.append(c.demand("wordCountRollupPass"));
         s.append(c.demand("wordCountReporterPass"));
      }

      s.append(c.demand("labelFormatterPass"));
      s.append(c.demand("textPrintPass"));
   }

private:
   config *m_pCfg;
};

cdwExportTarget(textTarget);
