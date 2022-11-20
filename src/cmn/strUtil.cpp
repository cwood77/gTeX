#include "strUtil.hpp"
#include <cstring>
#include <sstream>

void strUtil::expandVars(const std::string& varName, const std::string& value, std::string& text)
{
   std::stringstream newText;

   const char *pThumb = text.c_str();
   const char *pStart = pThumb;
   while(true)
   {
      if(::strlen(pThumb) < varName.length())
      {
         pThumb += ::strlen(pThumb);
         break;
      }

      if(::strncmp(pThumb,varName.c_str(),varName.length())==0)
      {
         newText << std::string(pStart,pThumb-pStart);
         newText << value;
         pThumb += varName.length();
         pStart = pThumb;
      }
      else
         ++pThumb;
   }

   newText << std::string(pStart,pThumb-pStart);
   text = newText.str();
}
