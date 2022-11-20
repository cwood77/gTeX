#include "../cmn/node.hpp"
#include "../middle/iFormatProvider.hpp"
#include "../prattle/pass.hpp"
#include <sstream>

using namespace prattle;
using namespace prattle::pass;

class htmlFormatProviderPass : public iPass, public iFormatProvider {
public:
   void run(config& c, passLinks&, void *pIr) { /* noop */ }

   virtual void beginFormat(fmts f, std::ostream& o)
   {
      if(f == kBold)
         o << "<b>";
      else if(f == kItalic)
         o << "<i>";
      else if(f == kHRule)
         o << "<hr>";
      else
         throw std::runtime_error("unknown format code");
   }

   virtual void endFormat(fmts f, std::ostream& o)
   {
      if(f == kBold)
         o << "</b>";
      else if(f == kItalic)
         o << "</i>";
      else if(f == kHRule)
         ;
      else
         throw std::runtime_error("unknown format code");
   }
};

cdwExportPass(htmlFormatProviderPass,"",-1);
