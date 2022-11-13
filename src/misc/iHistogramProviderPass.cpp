#include "iHistogramProviderPass.hpp"
#include <sstream>

void histogram::add(const std::string& word, const std::string& filePath)
{
   std::stringstream noob;
   bool isNumber = true;
   for(size_t i=0;i<word.length();i++)
   {
      char c = word.c_str()[i];

      // some puncutation is discardable
      if(c==' ' ||
         c=='.' || c=='!' || c=='?' ||
         c=='"' || c==',' || c==';' || c==':' ||
         c=='(' || c==')' ||
         c=='[' || c==']')
         continue;

      // lowercase it
      if(('A' <= c) && (c <= 'Z'))
         c += ('a' - 'A');

      // keep track if it's a number
      if(isNumber &&
         !(('0' <= c) && (c <= '9')))
         isNumber = false;

      // add it
      noob << c;
   }

   // if it's only numbers, then it's a jump, so ignore it
   if(isNumber)
      return;

   counts[noob.str()]++;
   files[noob.str()].insert(filePath);
   m_sortedCache.clear();
}

std::map<size_t,std::set<std::string> >& histogram::byCount()
{
   if(m_sortedCache.size() == 0)
      for(auto it=counts.begin();it!=counts.end();++it)
         m_sortedCache[it->second].insert(it->first);
   return m_sortedCache;
}
