#pragma once
#include "iWordObserverProviderPass.hpp"
#include <map>
#include <set>
#include <string>

class histogram : public iWordObserver {
public:
   histogram(const std::string& name, bool lowercase)
   : m_name(name), m_lowercase(lowercase) {}

   std::string name() const { return m_name; }
   void add(const std::string& word, const std::string& filePath);
   virtual void onWord(const std::string& w, paragraphNode& n)
   { add(w,n.filePath); }

   std::map<size_t,std::set<std::string> >& byCount();

   void clear() { counts.clear(); files.clear(); m_sortedCache.clear(); }

   std::map<std::string,size_t> counts;
   std::map<std::string,std::set<std::string> > files;

private:
   std::string m_name;
   const bool m_lowercase;
   std::map<size_t,std::set<std::string> > m_sortedCache;
};
