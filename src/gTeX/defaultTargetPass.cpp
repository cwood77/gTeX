#include "../prattle/config.hpp"
#include "../prattle/pass.hpp"

using namespace prattle;
using namespace prattle::pass;

class defaultTargetPass : public iPass {
public:
   virtual void run(config& c, passLinks&, void *pIr)
   {
      c.createOrFetch<stringSetting>("in",[](auto& s){ s.value="."; });
   }
};

cdwExportPass(defaultTargetPass,"cfg",100);
