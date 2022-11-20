#pragma once
#include <ostream>

class iFormatProvider {
public:
   enum fmts {
      kBold,
      kItalic,
      kHRule,
      kChapter
   };

   static fmts getFmtCode(const std::string& txt)
   {
      if(txt == "b") return kBold;
      if(txt == "i") return kItalic;
      if(txt == "hr") return kHRule;
      if(txt == "ch") return kChapter;
      throw std::runtime_error("unknown styling syntax: " + txt);
   }

   virtual void beginFormat(fmts f, std::ostream& o) = 0;
   virtual void endFormat(fmts f, std::ostream& o) = 0;
};
