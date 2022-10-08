#include "../prattle/module.hpp"
#include "../prattle/pass.hpp"

using namespace prattle;
using namespace prattle::module;
using namespace prattle::pass;

namespace {

class myModule : public iModule {
public:
   virtual void solicit(std::list<std::string>& depMods)
   {
      depMods.push_back("front.dll");
   }

   virtual void collect(passCatalog& p, targetCatalog& t)
   {
      passCatalog::get().publishTo(p);
      targetCatalog::get().publishTo(t);
   }

} gMod;

} // anonymous namespace

cdwImplModule(gMod);
