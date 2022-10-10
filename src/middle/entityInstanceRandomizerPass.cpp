#include "../cmn/nameBank.hpp"
#include "../cmn/node.hpp"
#include "../prattle/pass.hpp"

using namespace prattle;
using namespace prattle::pass;

class entityInstanceRandomizerPass : public iPass {
public:
   void run(config& c, void *pIr)
   {
      auto *pRoot = reinterpret_cast<folderNode*>(pIr);

      // find all entityInstances, of any type
      std::vector<entityInstanceNode*> insts;
      pRoot->searchDown<entityInstanceNode>([](auto&){return true;},insts); // TODO LAME

      // generate a bank of numeric names reproducibly randomly sorted
      nameBank bank(insts.size());
      auto nit = bank.randomIterator();

      for(auto it=insts.begin();it!=insts.end();++it,nit.next())
      {
         // update any label referencing this entityInstance
         std::vector<labelNode*> refs;
         pRoot->searchDown<labelNode>(
            [&](auto& c){ return !c.action.empty() && c.label == (*it)->id; },
            refs);
         for(auto *pLabel : refs)
            pLabel->label = bank.get(nit);

         // update the entityInstance itself
         (*it)->id = bank.get(nit);
      }
   }
};

cdwExportPass(entityInstanceRandomizerPass,"",-1);
