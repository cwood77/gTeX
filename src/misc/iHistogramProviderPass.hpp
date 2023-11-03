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
   void checkWord(const std::string& word, const std::string& filePath);

   std::map<size_t,std::set<std::string> > m_sortedCache;
};

class iHistogramProviderPass {
public:
   virtual std::string getHistogramName() const = 0;
   virtual histogram& getHistogram() = 0;
};

