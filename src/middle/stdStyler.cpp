#include "../cmn/node.hpp"
#include "../prattle/config.hpp"
#include "../prattle/pass.hpp"
#include "iStyler.hpp"
#include <sstream>

using namespace prattle;
using namespace prattle::pass;

class stdStyler : public iPass, public iStyler {
public:
   stdStyler() : m_pConfig(NULL) {}

   void run(config& c, passLinks&, void *) { m_pConfig = &c; }

   virtual std::string getFmt(objects o)
   {
      std::stringstream name;
      name << m_pConfig->demand<stringSetting>("target").value << ":";

      if(o == kRndVar)
         name << "rnd-var";
      else if(o == kJump)
         name << "jmp";
      else
         throw std::runtime_error("unsupported object type");

      name << ":fmt";

      auto *pV = m_pConfig->fetch<stringSetting>(name.str());
      if(!pV)
         return "";
      else
         return pV->value;
   }

private:
   config *m_pConfig;
};

cdwExportPass(stdStyler,"",-1);
