#include "../prattle/config.hpp"
#include "../prattle/pass.hpp"

using namespace prattle;
using namespace prattle::pass;

class defaultTargetPass : public iPass {
public:
   virtual void run(config& c, passLinks&, void *pIr)
   {
      c.createOrFetch<stringSetting>("in",[](auto& s){ s.value="."; });
      c.createOrFetch<stringSetting>("out",[](auto& s){ s.value="out"; });

      c.createOrFetch<boolSetting>("dot:on",[](auto& s){ s.value=false; });
      c.createOrFetch<stringSetting>("dot:out",[](auto& s){ s.value="out"; });
   }
};

cdwExportPass(defaultTargetPass,"cfg",100);
