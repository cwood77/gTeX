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
      c.createOrFetch<stringSetting>("dot-s1:out",[](auto& s){ s.value="out1"; });
      c.createOrFetch<stringSetting>("dot-s2:out",[](auto& s){ s.value="out2"; });
   }
};

cdwExportPass(defaultTargetPass,"cfg",100);
