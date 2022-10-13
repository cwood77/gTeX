#include "../cmn/node.hpp"
#include "../prattle/config.hpp"
#include "../prattle/log.hpp"
#include "../prattle/module.hpp"
#include "../prattle/pass.hpp"
#include <iostream>
#include <memory>

using namespace prattle;
using namespace prattle::module;
using namespace prattle::pass;

int main(int,const char*[])
{
   std::cout << "gTeX " << __DATE__ << " " << __TIME__ << std::endl;

   // module loader unloads modules on dtor, so put it first. This allows deallocs
   // (e.g. config's dtor) to use module heaps
   moduleLoader mLdr;

   std::unique_ptr<folderNode> pRoot(new folderNode());
   try
   {
      // setup a config
      config gCfg;

      incrementalModuleLoader imLdr(passCatalog::get(),targetCatalog::get(),mLdr);
      imLdr.tryLoad("config.dll");

      // run some basic passes on the config
      {
         auto cat = passCatalog::get().getPhase("env");
         passSchedule sched;
         passScheduler().schedule(cat,sched);

         passRunChain rc;
         passScheduler().inflate(sched,rc);
         passManager().run(gCfg,rc);
      }

      auto& configs = gCfg.demand<stringArraySetting>("config-targets").value;
      for(auto it=configs.begin();it!=configs.end();++it)
      {
         std::cout << "---- running config-target " << *it << " ----" << std::endl;

         config cfg;
         gCfg.cloneInto(cfg);

         // run some basic passes on the config
         {
            auto cat = passCatalog::get().getPhase("cfg");
            passSchedule sched;
            passScheduler().schedule(cat,sched);

            passRunChain rc;
            passScheduler().inflate(sched,rc);
            passManager().run(cfg,rc);
         }

         // we should now have a final pass
         auto targetName = cfg.demand<stringSetting>("target").value + "Target";

         // load the target chain, pulling in modules as necessary
         loadingTargetFactory ltf(targetCatalog::get(),imLdr);
         targetChain tc;
         targetChainBuilder().build(cfg,ltf,targetName,tc);

         // run additional passess to fill in the config
         {
            auto cat = passCatalog::get().getPhase("cfg:target");
            passSchedule sched;
            passScheduler().schedule(cat,sched);

            passRunChain rc;
            passScheduler().inflate(sched,rc);
            passManager().run(cfg,rc);
         }

         std::cout << "completed settings:" << std::endl;
         cfg.dump(std::cout);

         // run real passses from target chain
         passSchedule sched;
         tc.adjustPasses(imLdr,passCatalog::get(),sched);

         passRunChain rc;
         passScheduler().inflate(sched,rc);
         passManager().run(cfg,rc,pRoot.get());

#if 0
         {
            // diag dump
            log::streamLogAdapter sink(std::cout);
            dumpVisitor v(sink);
            pRoot->acceptVisitor(v);
         }
#endif
      }
   }
   catch(std::exception& x)
   {
      std::cerr << "ERROR: " << x.what() << std::endl;

      std::cerr << "AST at time of error:" << std::endl;
      {
         log::streamLogAdapter sink(std::cerr);
         dumpVisitor v(sink);
         pRoot->acceptVisitor(v);
      }

      std::cout << "aborting" << std::endl;
      return -2;
   }

   std::cout << "bye" << std::endl;
   return 0;
}
