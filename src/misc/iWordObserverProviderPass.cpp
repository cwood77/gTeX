#include "../prattle/pass.hpp"
#include "iWordObserverProviderPass.hpp"

using namespace prattle;
using namespace prattle::pass;

class wordObserverProviderPass : public iPass, public iWordObserverProviderPass {
public:
   void run(config& c, passLinks&, void *pIr) {}
   virtual wordObserver& getObserver() { return m_obs; }

private:
   wordObserver m_obs;
};

cdwExportPass(wordObserverProviderPass,"",-1);
