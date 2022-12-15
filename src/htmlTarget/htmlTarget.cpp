#include "../prattle/config.hpp"
#include "../prattle/module.hpp"
#include "../prattle/pass.hpp"

using namespace prattle;
using namespace prattle::pass;

class htmlTargetDefaultOptionPass : public iPass {
public:
   virtual void run(config& c, passLinks&, void *pIr)
   {
      c.createOrFetch<stringSetting>("html:label-fmt",[](auto&s)
         {
            s.value="<center><font size='16'><b>{label}</b></font></center>";
         });
      c.createOrFetch<stringSetting>("html:chap-fmt-before",[](auto&s)
         {
            s.value="<center><font size='18'>";
         });
      c.createOrFetch<stringSetting>("html:chap-fmt-after",[](auto&s)
         {
            s.value="</font></center>";
         });
      c.createOrFetch<stringSetting>("html:table-header-style",[](auto&s){ s.value=""; });
   }
};

cdwExportPass(htmlTargetDefaultOptionPass,"cfg:target",0);

class htmlTarget : public iTarget {
public:
   htmlTarget() : m_pCfg(NULL) {}

   virtual void configure(config& c) { m_pCfg = &c; }
   virtual std::string getPredecessorTarget() { return "textPreTarget"; }
   virtual void adjustPasses(module::incrementalModuleLoader& mLdr, passCatalog& c, passSchedule& s)
   {
      // encase paragraphs in <p></p> tags
      s.append(c.demand("htmlParagraphPass"));

      // post-count formatter (i.e. everything not contributing to word count)
      s.append(c.demand("htmlLabelFormatterPass"));
      s.append(c.demand("htmlTableFormatterPass"));

      // the end
      s.append(c.demand("htmlPrintPass"));
   }

private:
   config *m_pCfg;
};

cdwExportTarget(htmlTarget);
