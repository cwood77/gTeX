#include "../prattle/config.hpp"
#include "../prattle/module.hpp"
#include "../prattle/pass.hpp"

using namespace prattle;
using namespace prattle::pass;

class htmlTarget : public iTarget {
public:
   htmlTarget() : m_pCfg(NULL) {}

   virtual void configure(config& c) { m_pCfg = &c; }
   virtual std::string getPredecessorTarget() { return "textPreTarget"; }
   virtual void adjustPasses(module::incrementalModuleLoader& mLdr, passCatalog& c, passSchedule& s)
   {
      mLdr.tryLoad("textTarget.dll");
   }

private:
   config *m_pCfg;
};

cdwExportTarget(htmlTarget);
