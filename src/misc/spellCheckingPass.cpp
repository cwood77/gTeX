#include "../prattle/config.hpp"
#include "../prattle/pass.hpp"
#include "iDictionaryProviderPass.hpp"
#include <fstream>
#include <iostream>
#include <memory>

using namespace prattle;
using namespace prattle::pass;

class spellCheckingPass : public iPass {
public:
   void run(config& c, passLinks& l, void *pIr)
   {
      auto& dp = l.demandLink<dict::iDictionaryProviderPass>();
      auto& source = dp.getPrevious().getHistogram();
      auto& misspellings = dp.getHistogram();

      for(auto it=source.counts.begin();it!=source.counts.end();++it)
      {
         if(!dp.getDictionary().isMatch(it->first))
         {
            misspellings.counts[it->first] = it->second;
            misspellings.files[it->first] = source.files[it->first];
         }
      }

      std::cout << "   " << misspellings.counts.size() << " word(s) are misspelled" << std::endl;
   }
};

cdwExportPass(spellCheckingPass,"",-1);
