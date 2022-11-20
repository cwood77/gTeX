#include "../cmn/node.hpp"
#include "../middle/iFormatProvider.hpp"
#include "../prattle/config.hpp"
#include "../prattle/pass.hpp"
#include <sstream>

using namespace prattle;
using namespace prattle::pass;

class htmlFormatProviderPass : public iPass, public iFormatProvider {
public:
   htmlFormatProviderPass() : m_pCfg(NULL) {}

   void run(config& c, passLinks&, void *) { m_pCfg = &c; }

   virtual void beginFormat(fmts f, std::ostream& o)
   {
      if(f == kBold)
         o << "<b>";
      else if(f == kItalic)
         o << "<i>";
      else if(f == kHRule)
         o << "<hr>";
      else if(f == kChapter)
         o << "_" // special character hint to paragraph pass
           << m_pCfg->demand<stringSetting>("html:chap-fmt-before").value;
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
      else if(f == kChapter)
         o << m_pCfg->demand<stringSetting>("html:chap-fmt-after").value;
      else
         throw std::runtime_error("unknown format code");
   }

private:
   config *m_pCfg;
};

cdwExportPass(htmlFormatProviderPass,"",-1);
