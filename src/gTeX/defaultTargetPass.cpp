#include "../prattle/config.hpp"
#include "../prattle/pass.hpp"

using namespace prattle;
using namespace prattle::pass;

class defaultTargetPass : public iPass {
public:
   virtual void run(config& c, passLinks&, void *pIr)
   {
      {
         auto& s = c.createOrFetch<stringSetting>("target");
         if(s.value.empty())
            s.value = "text";
      }

      {
         auto& s = c.createOrFetch<stringSetting>("in");
         if(s.value.empty())
            s.value = ".";
      }
   }
};

cdwExportPass(defaultTargetPass,"cfg",0);
