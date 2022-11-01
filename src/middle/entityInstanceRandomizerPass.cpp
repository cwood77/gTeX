#include "../cmn/nameBank.hpp"
#include "../cmn/node.hpp"
#include "../prattle/pass.hpp"
#include <memory>

using namespace prattle;
using namespace prattle::pass;

namespace {

// instances aren't always unique if they reference the same type+id pair, so keep a table
// of their names
class eInstNameTable {
public:
   eInstNameTable(std::vector<entityInstanceNode*>& nodes)
   : m_iter(0)
   {
      for(auto *pEi : nodes)
         m_nameMap[pEi->type + ":" + pEi->id];
      m_pBank.reset(new nameBank(m_nameMap.size()));
      m_iter = m_pBank->randomIterator();
   }

   std::string getNewName(const std::string& oldName)
   {
      auto& val = m_nameMap[oldName];
      if(val.empty())
      {
         val = m_pBank->get(m_iter);
         m_iter.next();
      }
      return val;
   }

private:
   std::map<std::string,std::string> m_nameMap;
   std::unique_ptr<nameBank> m_pBank;
   nameBankIter m_iter;
};

} // anonymous namespace

class entityInstanceRandomizerPass : public iPass {
public:
   void run(config& c, passLinks&, void *pIr)
   {
      auto *pRoot = reinterpret_cast<folderNode*>(pIr);

      // find all entityInstances of any type
      std::vector<entityInstanceNode*> insts;
      pRoot->searchDown<entityInstanceNode>(insts);
      eInstNameTable nTable(insts);

      // find all tables
      std::vector<tableNode*> tables;
      pRoot->searchDown<tableNode>(tables);

      for(auto it=insts.begin();it!=insts.end();++it)
      {
         for(auto tit=tables.begin();tit!=tables.end();++tit)
         {
            // for every table, update any references to the new ID
            auto oit = (*tit)->operandsToLabels.find((*it)->id);
            if(oit!=(*tit)->operandsToLabels.end())
            {
               auto copy = oit->second;
               (*tit)->operandsToLabels.erase(oit);
               (*tit)->operandsToLabels[nTable.getNewName((*it)->id)] = copy;
            }
         }

         // update the entityInstance itself
         (*it)->id = nTable.getNewName((*it)->id);
      }
   }
};

cdwExportPass(entityInstanceRandomizerPass,"",-1);
