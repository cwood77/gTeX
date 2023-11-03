#include "iDictionaryProviderPass.hpp"
#include <cstring>

namespace dict {

bool dictionary::isMatch(const std::string& word) const
{
   if(m_words.find(word) != m_words.end())
      return true;

   std::string copy(word.c_str());
   ::strlwr(const_cast<char*>(copy.c_str()));
   if(m_words.find(copy) != m_words.end())
      return true;

   return false;
}

void dictionary::load(std::istream& i)
{
   while(i.good())
   {
      std::string line;
      std::getline(i,line);

      if(line.empty())
         continue;
      if(line.c_str()[0] == '#')
         continue;

      m_words.insert(line);
   }
}

compositeDictionary::~compositeDictionary()
{
   while(m_parts.size())
   {
      delete *m_parts.begin();
      m_parts.erase(m_parts.begin());
   }
}

bool compositeDictionary::isMatch(const std::string& word) const
{
   for(auto *pDict : m_parts)
      if(pDict->isMatch(word))
         return true;
   return false;
}

size_t compositeDictionary::getSize() const
{
   size_t n = 0;
   for(auto *pDict : m_parts)
      n += pDict->getSize();
   return n;
}

} // namespace dict
