#define WIN32_LEAN_AND_MEAN
#include "output.hpp"
#include "windows.h"
#include <sstream>

std::string output::ensurePath(const std::string& path)
{
   std::stringstream totalPath;
   totalPath << m_c.demand<stringSetting>("out").value;
   makeIf(totalPath.str());

   totalPath << "\\";
   totalPath << m_c.demand<stringSetting>("config").value;
   makeIf(totalPath.str());

   totalPath << "\\";
   totalPath << m_c.demand<stringSetting>("target").value;
   makeIf(totalPath.str());

   totalPath << "\\" << path;

   return totalPath.str();
}

void output::makeIf(const std::string& path)
{
   ::CreateDirectoryA(path.c_str(),NULL);
}
