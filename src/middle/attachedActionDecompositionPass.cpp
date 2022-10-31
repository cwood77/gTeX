#include "../cmn/node.hpp"
#include "../prattle/pass.hpp"

using namespace prattle;
using namespace prattle::pass;

class attachedActionDecompositionPass : public iPass {
public:
   void run(config& c, passLinks&, void *pIr)
   {
      auto *pRoot = reinterpret_cast<folderNode*>(pIr);

      std::vector<attachActionNode*> attachments;
      pRoot->searchDown<attachActionNode>(attachments);

      for(auto *pAttach : attachments)
      {
         auto& table = pRoot->demandDown<tableNode>(
            [&](auto&t){ return 
               t.entityType == pAttach->entityType &&
               t.action == pAttach->action; });

         // amend the table action with the prefix
         if(!pAttach->actionPrefix.empty())
            table.action = pAttach->actionPrefix + " " + table.action;
         // zero-out the entityType on the table
         table.entityType = "";

         // replace attachment with jump
         std::stringstream stream;
         stream << "lbl_" << pAttach->action << "_" << pAttach->entityType;
         auto *pJ = new jumpNode();
         pJ->id = stream.str();
         pAttach->replace(*pJ);

         // inject a label above the table
         auto *pRoot = new node();
         auto& L = pRoot->appendChild<labelNode>();
         L.label = stream.str();
         L.reparent(table.demandParent(),&table);
         delete pRoot;
         table.reparent(L);
      }

      //throw std::runtime_error("dump AST after new xfrm");
   }
};

cdwExportPass(attachedActionDecompositionPass,"",-1);
