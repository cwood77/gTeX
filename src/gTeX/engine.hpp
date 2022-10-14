#pragma once
#include "../prattle/config.hpp"
#include "../prattle/module.hpp"
#include "../prattle/pass.hpp"

using namespace prattle;
using namespace prattle::module;
using namespace prattle::pass;

class engine {
public:
   engine(passCatalog& pCat, targetCatalog& tCat, moduleLoader& mLdr)
   : m_pCat(pCat), m_tCat(tCat), m_mLdr(pCat,tCat,mLdr) {}

   void run();

private:
   void handleConfigTarget(config& gCfg, const std::string& configTarget);
   void setCurrentConfigVars(const std::string& configTarget, config& c);
   void runPassPhaseOnConfig(const std::string& phase, config& c);

   passCatalog& m_pCat;
   targetCatalog& m_tCat;
   incrementalModuleLoader m_mLdr;
};
