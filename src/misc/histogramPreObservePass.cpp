#include "../prattle/pass.hpp"
#include "iWordObserverProviderPass.hpp"

using namespace prattle;
using namespace prattle::pass;

class histogramPreObservePass : public iPass, public iWordObserver {
public:
   void run(config&, passLinks& l, void *)
   {
      l.demandLink<iWordObserverProviderPass>().getObserver().add(*this);
   }

   virtual void onWord(const std::string& w, paragraphNode& n)
   {
   }
};

cdwExportPass(histogramPreObservePass,"",-1);
