#include "../cmn/node.hpp"
#include "../prattle/pass.hpp"

using namespace prattle;
using namespace prattle::pass;

class ensureNoVarsPass : public iPass {
public:
   void run(config& c, passLinks&, void *pIr)
   {
      auto *pRoot = reinterpret_cast<folderNode*>(pIr);

      {
         std::vector<varRefNode*> ans;
         pRoot->searchDown(ans);
         if(ans.size() != 0)
            throw std::runtime_error("unresolved variable references");
      }
      {
         std::vector<varDeclNode*> ans;
         pRoot->searchDown(ans);
         if(ans.size() != 0)
            throw std::runtime_error("unresolved variable declarations");
      }
   }
};

cdwExportPass(ensureNoVarsPass,"",-1);
