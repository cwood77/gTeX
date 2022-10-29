#include "../cmn/nameBank.hpp"
#include "../cmn/node.hpp"
#include "../prattle/pass.hpp"
#include <stdexcept>

using namespace prattle;
using namespace prattle::pass;

class mergeLabelsPass : public iPass {
public:
   void run(config& c, passLinks&, void *pIr)
   {
      auto *pRoot = reinterpret_cast<folderNode*>(pIr);
      auto& links = pRoot->demandDown<linkTableNode>();

      while(true)
      {
         // find all merge jumps
         std::vector<jumpNode*> merges;
         pRoot->searchDown<jumpNode>(merges,[](auto&j){ return j.markedForMerge; });

         // handle one at a time
         if(merges.size() == 0) break;
         auto *pMerge = merges[0];

         // find jump target label
         auto& lbl =
            pRoot->demandDown<labelNode>(
               [&](auto&l){ return l.id() == pMerge->id; });

         // make sure it's safe to eraticate label
         if(links.l2j[&lbl].size() != 1)
            throw std::runtime_error("target of a merge jump should have any other jumps");

         // transfer all label children to the jump's label
         auto& lblOfJmp = pMerge->demandAncestor<labelNode>();
         auto& paraOfJmp = dynamic_cast<paragraphNode&>(*pMerge->getParent());
         lbl.reparentChildren(lblOfJmp,&paraOfJmp); // TODO this needs an after sibling!

         // delete nodes
         lbl.Delete();
         pMerge->Delete();
         // if the merge jump had nothing else in its paragraph, delete that paragraph also
         if(paraOfJmp.getChildren().size() == 0)
            paraOfJmp.Delete();
      }
   }
};

cdwExportPass(mergeLabelsPass,"",-1);
