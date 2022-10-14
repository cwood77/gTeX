#include "../cmn/node.hpp"
#include "../prattle/log.hpp"
#include "engine.hpp"
#include <iostream>
#include <memory>

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
   loadingTargetFactory ltf(m_tCat,m_mLdr);
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
   std::unique_ptr<folderNode> pRoot(new folderNode());
   try
   {
      passManager().run(cfg,rc,pRoot.get());
   }
   catch(std::exception& x)
   {
      std::stringstream msg;

      msg << x.what() << std::endl;

      msg << "AST at time of error:" << std::endl;
      {
         log::streamLogAdapter sink(msg);
         dumpVisitor v(sink);
         pRoot->acceptVisitor(v);
      }

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
   passManager().run(c,rc);
}
