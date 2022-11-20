#pragma once
#include <string>

class strUtil {
public:
   static void expandVars(
      const std::string& varName,
      const std::string& value,
      std::string& text);
};
