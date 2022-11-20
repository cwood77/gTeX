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

   virtual void beginFormat(fmts f, std::ostream& o) = 0;
   virtual void endFormat(fmts f, std::ostream& o) = 0;
};
