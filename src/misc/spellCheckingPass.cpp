#include "../prattle/config.hpp"
#include "../prattle/pass.hpp"
#include "histogram.hpp"
#include "iDictionaryProviderPass.hpp"
#include "iWordObserverProviderPass.hpp"
#include <iostream>

using namespace prattle;
using namespace prattle::pass;

class spellCheckingPass : public iPass {
public:
   void run(config& c, passLinks& l, void *pIr)
   {
      auto& dict = l.demandLink<dict::iDictionaryProviderPass>().getDictionary();
      auto& source = getSourceHistogram(l);
      auto& misspellings = getDestHistogram(l);
      misspellings.clear();

      for(auto it=source.counts.begin();it!=source.counts.end();++it)
      {
         if(!dict.isMatch(it->first))
         {
            misspellings.counts[it->first] = it->second;
            misspellings.files[it->first] = source.files[it->first];
         }
      }

      std::cout << "   " << misspellings.counts.size() << " word(s) are misspelled" << std::endl;
   }

private:
   histogram& getSourceHistogram(passLinks& l) { return demandHistogram(l,"word-hist-truecase"); }
   histogram& getDestHistogram(passLinks& l) { return demandHistogram(l,"misspellings"); }
   histogram& demandHistogram(passLinks& l, const std::string& name)
   {
      histogram *pHit = NULL;
      l.demandLink<iWordObserverProviderPass>().getObserver()
         .forEach<histogram>([&](auto& h){ if(h.name() == name) pHit = &h; });
      if(!pHit)
         throw std::runtime_error("ISE spellChecking 49");
      return *pHit;
   }
};

cdwExportPass(spellCheckingPass,"",-1);
