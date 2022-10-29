#include "../prattle/config.hpp"
#include "../prattle/module.hpp"
#include "../prattle/pass.hpp"

using namespace prattle;
using namespace prattle::pass;

class middleTarget : public iTarget {
public:
   middleTarget() : m_pCfg(NULL) {}

   virtual void configure(config& c) { m_pCfg = &c; }
   virtual std::string getPredecessorTarget() { return "frontTarget"; }
   virtual void adjustPasses(module::incrementalModuleLoader& mLdr, passCatalog& c, passSchedule& s)
   {
      s.append(c.demand("evaluateCounterVarPass"));
      s.append(c.demand("ensureNoVarsPass"));

      s.append(c.demand("outFileCreatorPass"));

      s.append(c.demand("entityTableGeneratorPass"));
      s.append(c.demand("entityInstanceRandomizerPass"));

      if(usesDot() || usesMaps())
      {
         mLdr.tryLoad("graph.dll");
         s.append(c.demand("graphIrProviderPass"));
      }

      // merge (with its own linker)
      s.append(c.demand("linkerPass"));
      s.append(c.demand("mergeLabelsPass"));
      s.append(c.demand("unlinkerPass"));

      // randomize (with its own linker)
      s.append(c.demand("linkerPass"));
      s.append(c.demand("entityRemovalPass"));
      s.append(c.demand("labelRandomizerPass"));
      s.append(c.demand("unlinkerPass"));

      s.append(c.demand("labelMoverPass"));

      if(usesDot())
         s.append(c.demand("dotPrintPass"));

      if(usesMaps())
         // [graph] mapGeneratorPass * one for each config
         // [graph] mapGeneratorPass *
         // [graph] mapGeneratorPass *
         // [graph] mapAccessorPass <- decomposes.. needs config?!  conditional
         //                           maybe skip decomposition here
         //                           and just handle at run time
         s.append(c.demand("mapAccessorPass"));
   }

private:
   bool usesDot()
   {
      auto *pSet = m_pCfg->fetch<boolSetting>("dot:on");
      if(!pSet)
         return false;
      return pSet->value;
   }

   bool usesMaps()
   {
      auto *pSet = m_pCfg->fetch<stringArraySetting>("maps");
      if(!pSet)
         return false;
      return !pSet->value.empty();
   }

   config *m_pCfg;
};

cdwExportTarget(middleTarget);
