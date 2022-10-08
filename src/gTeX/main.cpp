#include "../cmn/node.hpp"
#include "../prattle/config.hpp"
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

   // setup a config
   config cfg;

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
   auto targetName = cfg.demand<stringSetting>("target").value;

   // try loading a module for the target, just in case
   mLdr.tryLoad(targetName + "Target.dll");
   mLdr.collect(passCatalog::get(),targetCatalog::get());

   // run additional passess to configure the targt
   {
      auto cat = passCatalog::get().getPhase("cfg:target");
      passSchedule sched;
      passScheduler().schedule(cat,sched);

      passRunChain rc;
      passScheduler().inflate(sched,rc);
      passManager().run(cfg,rc);
   }

   // run real passses from target chain
   targetChain tc;
   targetChainBuilder().build(cfg,targetCatalog::get(),targetName,tc);
   passSchedule sched;
   tc.adjustPasses(passCatalog::get(),sched);

   passRunChain rc;
   passScheduler().inflate(sched,rc);
   std::unique_ptr<fileNode> pRoot(new fileNode());
   passManager().run(cfg,rc,pRoot.get());

#if 1
   // diag dump
   dumpVisitor v;
   pRoot->acceptVisitor(v);
#endif

   std::cout << "bye" << std::endl;

   return 0;
}
