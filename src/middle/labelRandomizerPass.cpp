#include "../cmn/nameBank.hpp"
#include "../cmn/node.hpp"
#include "../prattle/pass.hpp"
#include <stdexcept>

using namespace prattle;
using namespace prattle::pass;

class labelRandomizerPass : public iPass {
public:
   void run(config& c, passLinks&, void *pIr)
   {
      auto *pRoot = reinterpret_cast<folderNode*>(pIr);
      auto& links = pRoot->demandDown<linkTableNode>();

      // acquire the current manifest of labels
      std::map<std::string,labelNode*> oldNames;
      links.getAllLabels(oldNames);

      // generate a bank of numeric names reproducibly randomly sorted
      nameBank bank(oldNames.size());
      auto nit = bank.randomIterator();

      // build the future manifest of labels and apply it
      std::map<labelNode*,std::string> newNames;
      for(auto it=oldNames.begin();it!=oldNames.end();++it,nit.next())
         newNames[it->second] = bank.get(nit);
      links.renameLabels(newNames);

      // rename the START label
      auto *pStart = pRoot->findDown<labelNode>([](auto& c){ return c.id() == "START"; });
      if(!pStart)
         throw std::runtime_error("there must be a START label");
      pStart->label = "0";
   }
};

cdwExportPass(labelRandomizerPass,"",-1);
