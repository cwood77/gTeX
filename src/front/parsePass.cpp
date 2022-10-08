#include "../cmn/node.hpp"
#include "../prattle/loader.hpp"
#include "../prattle/pass.hpp"
#include "lexor.hpp"
#include "parser.hpp"
#include <iostream>
#include <memory>

using namespace prattle;
using namespace prattle::pass;

class loadPass : public iPass {
public:
   void run(config& c, void *pIr)
   {
      fileNode *pRoot = reinterpret_cast<fileNode*>(pIr);

      std::cout << "  loading file testdata.txt" << std::endl;
      std::unique_ptr<iLexorInput> pIn(fileLoader::load("testdata.txt"));

      lexor l(*pIn);
      parser p(l);
      p.parseFile(*pRoot);
   }
};

cdwExportPass(loadPass,"",-1);
