#include "../cmn/node.hpp"
#include "../middle/iFormatProvider.hpp"
#include "../prattle/pass.hpp"
#include <sstream>

using namespace prattle;
using namespace prattle::pass;

class textFormatProviderPass : public iPass, public iFormatProvider {
public:
   void run(config& c, passLinks&, void *pIr) { /* noop */ }

   virtual void beginFormat(fmts f, std::ostream& o)
   {
      o << "_";
   }

   virtual void endFormat(fmts f, std::ostream& o)
   {
      o << "_";
   }
};

cdwExportPass(textFormatProviderPass,"",-1);
