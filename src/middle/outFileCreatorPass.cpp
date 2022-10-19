#include "../cmn/node.hpp"
#include "../prattle/pass.hpp"

using namespace prattle;
using namespace prattle::pass;

class outFileCreatorPass : public iPass {
public:
   void run(config& c, passLinks&, void *pIr)
   {
      auto *pRoot = reinterpret_cast<folderNode*>(pIr);
      auto& outF = pRoot->appendChild<fileNode>();
      outF.filePath = " OUT ";
   }
};

cdwExportPass(outFileCreatorPass,"",-1);
