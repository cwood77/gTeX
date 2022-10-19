#include "../cmn/nameBank.hpp"
#include "../cmn/node.hpp"
#include "../prattle/pass.hpp"
#include <stdexcept>

using namespace prattle;
using namespace prattle::pass;

namespace {

class labelSorter {
public:
   typedef labelNode *elt_t;

   bool operator()(const elt_t& lhs, const elt_t& rhs) const
   {
      auto L = getPri(*lhs);
      auto R = getPri(*rhs);
      if(L != R)
         return L < R;
      else
         return lhs < rhs;
   }

private:
   static unsigned long getPri(const labelNode& l)
   {
      unsigned long x = 0;
      ::sscanf(l.id().c_str(),"%lu",&x);
      return x;
   }
};

} // anonymous namespace

class labelMoverPass : public iPass {
public:
   void run(config& c, passLinks&, void *pIr)
   {
      auto *pRoot = reinterpret_cast<folderNode*>(pIr);
      auto& outF = pRoot->demandDown<fileNode>(
         [](auto&f){ return f.filePath == " OUT "; });

      std::vector<labelNode*> labels;
      pRoot->searchDown<labelNode>(labels);
      std::set<labelNode*,labelSorter> sorted(labels.begin(),labels.end());
      for(auto *pLabel : sorted)
         pLabel->reparent(outF);
   }
};

cdwExportPass(labelMoverPass,"",-1);
