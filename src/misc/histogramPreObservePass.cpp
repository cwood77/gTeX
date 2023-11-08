#include "../prattle/config.hpp"
#include "../prattle/pass.hpp"
#include "histogram.hpp"
#include "iWordObserverProviderPass.hpp"

using namespace prattle;
using namespace prattle::pass;

class histogramPreObservePass : public iPass {
public:
   virtual ~histogramPreObservePass()
   {
      for(auto *pHist : m_hists)
         delete pHist;
   }

   void run(config& c, passLinks& l, void *)
   {
      m_hists.push_back(new histogram("word-hist",/*lowercase*/true));

      bool doesSpellCheck = (c.fetch<stringArraySetting>("dict:names") != NULL);
      if(doesSpellCheck)
      {
         m_hists.push_back(new histogram("word-hist-truecase",/*lowercase*/false));
         m_hists.push_back(new histogram("misspellings",/*lowercase*/false)); // TODO: actually, this is wrong.  Instead, I should add a case-sensitive histogram, which the spell checker uses as a source.  The spellChecker needs to create it's own dest histogram, which it publishes to the wordobserver late so the printer prints all three.
      }

      auto& obs = l.demandLink<iWordObserverProviderPass>().getObserver();
      for(auto *pHist : m_hists)
         obs.add(*pHist);
   }

private:
   std::list<histogram*> m_hists;
};

cdwExportPass(histogramPreObservePass,"",-1);
