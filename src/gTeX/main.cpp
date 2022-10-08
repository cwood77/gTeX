#include "../prattle/config.hpp"
#include "../prattle/module.hpp"
#include "../prattle/pass.hpp"
#include "node.hpp"
#include <memory>

using namespace prattle;
using namespace prattle::module;
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
   auto targetName = cfg.createOrFetch<stringSetting>("target").value;

   // TODO all
   {
      if(targetName.empty())
         targetName = "text";
      auto& s = cfg.createOrFetch<stringSetting>("text:out-path");
      if(s.value.empty())
         s.value = "testdata.txt-out";
   }

   // try loading some modules, just in case
   moduleLoader mLdr;
   mLdr.tryLoad(targetName + "Target.dll");
   mLdr.collect(passCatalog::get(),targetCatalog::get());

   // run real passses from target chain
   targetChain tc;
   targetChainBuilder().build(cfg,targetCatalog::get(),targetName,tc);
   passSchedule sched;
   tc.adjustPasses(passCatalog::get(),sched);

   passRunChain rc; // DUP
   passScheduler().inflate(sched,rc);
   std::unique_ptr<fileNode> pRoot(new fileNode());
   passManager().run(cfg,rc,pRoot.get());

#if 1
   // diag dump
   dumpVisitor v;
   pRoot->acceptVisitor(v);
#endif

   return 0;
}
