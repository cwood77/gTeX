#include "histogram.hpp"
#include <sstream>

void histogram::add(const std::string& word, const std::string& filePath)
{
   std::stringstream noob;
   for(size_t i=0;i<word.length();i++)
   {
      char c = word.c_str()[i];

      // lowercase it
      if(m_lowercase)
      {
         if(('A' <= c) && (c <= 'Z'))
            c += ('a' - 'A');
      }

      // add it
      noob << c;
   }

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
