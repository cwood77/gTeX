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
      m_hists.push_back(new histogram("word-hist"));

      bool doesSpellCheck = (c.fetch<stringArraySetting>("dict:names") != NULL);
      if(doesSpellCheck)
         m_hists.push_back(new histogram("misspellings"));

      auto& obs = l.demandLink<iWordObserverProviderPass>().getObserver();
      for(auto *pHist : m_hists)
         obs.add(*pHist);
   }

private:
   std::list<histogram*> m_hists;
};

cdwExportPass(histogramPreObservePass,"",-1);
