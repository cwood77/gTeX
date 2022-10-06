#include "../prattle/config.hpp"
#include "../prattle/pass.hpp"

using namespace prattle;
using namespace prattle::pass;

int main(int,const char*[])
{
   // setup a config
   config cfg;

   {
      // run some basic passes on the config
      auto cat = passCatalog::get().getPhase("cfg");
      passSchedule sched;
      passScheduler().schedule(cat,sched);

      passRunChain rc; // this happens twice now? make this easier?
      passScheduler().inflate(sched,rc);
      void *pIr = NULL;
      passManager().run(cfg,rc,pIr);
   }

   // we should now have a final pass
   //auto targetName = cfg.demand<stringSetting>().value;
   std::string targetName = cfg.createOrFetch<stringSetting>().value;

   // run real passses from target chain
   targetChain tc;
   targetChainBuilder().build(cfg,targetCatalog::get(),targetName,tc);
   passSchedule sched;
   tc.adjustPasses(passCatalog::get(),sched);

   passRunChain rc; // DUP
   passScheduler().inflate(sched,rc);
   void *pIr = NULL;
   passManager().run(cfg,rc,pIr);

   return 0;
}
