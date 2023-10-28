#pragma once
#include <string>

class iStyler {
public:
   enum objects {
      kRndVar,
      kJump
   };

   virtual std::string getFmt(objects o) = 0;
};
