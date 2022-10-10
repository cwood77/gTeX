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

      // find all tables
      std::vector<tableNode*> tables;
      pRoot->searchDown<tableNode>([](auto&){return true;},tables); // TODO LAME

      // generate a bank of numeric names reproducibly randomly sorted
      nameBank bank(insts.size());
      auto nit = bank.randomIterator();

      for(auto it=insts.begin();it!=insts.end();++it,nit.next())
      {
         for(auto tit=tables.begin();tit!=tables.end();++tit)
         {
            // for every table, update any references to the new ID
            auto oit = (*tit)->operandsToLabels.find((*it)->id);
            if(oit!=(*tit)->operandsToLabels.end())
            {
               auto copy = oit->second;
               (*tit)->operandsToLabels.erase(oit);
               (*tit)->operandsToLabels[bank.get(nit)] = copy;
            }
         }

         // update the entityInstance itself
         (*it)->id = bank.get(nit);
      }
   }
};

cdwExportPass(entityInstanceRandomizerPass,"",-1);
