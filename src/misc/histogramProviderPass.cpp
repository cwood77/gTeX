#include "../prattle/pass.hpp"
#include "iHistogramProviderPass.hpp"

using namespace prattle;
using namespace prattle::pass;

class histogramProviderPass : public iPass, public iHistogramProviderPass {
public:
   void run(config& c, passLinks&, void *pIr) {}
   virtual std::string getHistogramName() const { return "word-hist"; }
   virtual histogram& getHistogram() { return m_histogram; }

private:
   histogram m_histogram;
};

cdwExportPass(histogramProviderPass,"",-1);
