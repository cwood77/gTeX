#pragma once
#include <map>
#include <set>
#include <string>

class histogram {
public:
   void add(const std::string& word, const std::string& filePath);

   std::map<size_t,std::set<std::string> >& byCount();

   std::map<std::string,size_t> counts;
   std::map<std::string,std::set<std::string> > files;

private:
   std::map<size_t,std::set<std::string> > m_sortedCache;
};

class iHistogramProviderPass {
public:
   virtual histogram& getHistogram() = 0;
};

