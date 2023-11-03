#pragma once
#include "iWordObserverProviderPass.hpp"
#include <map>
#include <set>
#include <string>

class histogram : public iWordObserver {
public:
   explicit histogram(const std::string& name) : m_name(name) {}

   std::string name() const { return m_name; }
   void add(const std::string& word, const std::string& filePath);
   virtual void onWord(const std::string& w, paragraphNode& n)
   { add(w,n.filePath); }

   std::map<size_t,std::set<std::string> >& byCount();

   std::map<std::string,size_t> counts;
   std::map<std::string,std::set<std::string> > files;

private:
   void checkWord(const std::string& word, const std::string& filePath);

   std::string m_name;
   std::map<size_t,std::set<std::string> > m_sortedCache;
};
