#include "../prattle/pass.hpp"

using namespace prattle;
using namespace prattle::pass;

class frontTarget : public iTarget {
public:
   virtual void configure(config& c) {}
   virtual std::string getPredecessorTarget() { return ""; }
   virtual void adjustPasses(module::incrementalModuleLoader&, passCatalog& c, passSchedule& s)
   {
      // parse
      s.append(c.demand("fileEnumPass"));
      s.append(c.demand("parsePass"));

      // if
      s.append(c.demand("conditionalEvaluatorPass"));

      // inline label decomp
      s.append(c.demand("expandInlineLabelsPass"));

      // explode paras
      s.append(c.demand("expandParagraphPass"));

      // impl macros
      s.append(c.demand("implementMacroPass"));
   }
};

cdwExportTarget(frontTarget);
