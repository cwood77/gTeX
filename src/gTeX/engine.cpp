#include "../cmn/node.hpp"
#include "../cmn/output.hpp"
#include "../prattle/log.hpp"
#include "engine.hpp"
#include <fstream>
#include <iostream>
#include <memory>

class myLoadingTargetFactory : public loadingTargetFactory {
public:
   explicit myLoadingTargetFactory(targetCatalog& tCat, incrementalModuleLoader& iml)
   : loadingTargetFactory(tCat,iml)
   , m_tCat(tCat), m_imLdr(iml) {}

   virtual pass::iTarget *create(const std::string& name)
   {
      // handle idiomatic name
      if(name == "textPreTarget")
      {
         m_imLdr.tryLoad("textTarget.dll");
         return m_tCat.create(name);
      }
      else
         return loadingTargetFactory::create(name);
   }

private:
   pass::targetCatalog& m_tCat;
   incrementalModuleLoader& m_imLdr;
};

void engine::run()
{
   // load the environs file
   m_mLdr.tryLoad("config.dll");
   config gCfg;
   runPassPhaseOnConfig("env",gCfg);

   // handle each config-target combination
   auto& configs = gCfg.demand<stringArraySetting>("config-targets").value;
   for(auto it=configs.begin();it!=configs.end();++it)
      handleConfigTarget(gCfg,*it);
}

void engine::handleConfigTarget(config& gCfg, const std::string& configTarget)
{
   std::cout << "---- running config-target " << configTarget << " ----" << std::endl;

   // create a sub-config and set it up
   config cfg;
   gCfg.cloneInto(cfg);
   setCurrentConfigVars(configTarget,cfg);
   runPassPhaseOnConfig("cfg",cfg);

   // we should now have a target name
   auto targetName = cfg.demand<stringSetting>("target").value + "Target";

   // load the target chain, pulling in modules as necessary
   myLoadingTargetFactory ltf(m_tCat,m_mLdr);
   targetChain tc;
   targetChainBuilder().build(cfg,ltf,targetName,tc);

   // run additional passess to fill in the config for target-specific defaults
   runPassPhaseOnConfig("cfg:target",cfg);

   std::cout << "completed settings:" << std::endl;
   cfg.dump(std::cout);

   // run real passses from target chain
   passSchedule sched;
   tc.adjustPasses(m_mLdr,m_pCat,sched);

   passRunChain rc;
   passScheduler().inflate(sched,rc);
   passScheduler().decompose(m_pCat,rc);
   std::unique_ptr<folderNode> pRoot(new folderNode());
   try
   {
      passManager().run(cfg,rc,pRoot.get());
   }
   catch(std::exception& x)
   {
      std::stringstream msg;

      msg << x.what() << std::endl;

      auto path = output(cfg).ensurePath("crash.ast");
      {
         std::ofstream oFile(path.c_str());
         log::streamLogAdapter sink(oFile);
         dumpVisitor v(sink);
         pRoot->acceptVisitor(v);
      }
      msg << "AST at time of error saved to '" << path << "'" << std::endl;

      throw std::runtime_error(msg.str());
   }
}

void engine::setCurrentConfigVars(const std::string& configTarget, config& cfg)
{
   cfg.createOrFetch<stringSetting>("config-target").value = configTarget;

   const char *pSlash = ::strstr(configTarget.c_str(),"/");
   std::string activeCfg(configTarget.c_str(),pSlash-configTarget.c_str());
   std::string activeTgt(pSlash+1);

   cfg.createOrFetch<stringSetting>("config").value = activeCfg;
   cfg.createOrFetch<stringSetting>("target").value = activeTgt;
}

void engine::runPassPhaseOnConfig(const std::string& phase, config& c)
{
   auto cat = m_pCat.getPhase(phase);
   passSchedule sched;
   passScheduler().schedule(cat,sched);

   passRunChain rc;
   passScheduler().inflate(sched,rc);
   passScheduler().decompose(m_pCat,rc);
   passManager().run(c,rc);
}
