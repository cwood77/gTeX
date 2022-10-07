#include "lexor.hpp"
#include "node.hpp"
#include "parsePass.hpp"
#include "parser.hpp"
#include <iostream>
#include <memory>

using namespace prattle;
using namespace prattle::pass;

void loadPass::run(config& c, void *pIr)
{
   std::cout << "loading file" << std::endl;
   std::unique_ptr<iLexorInput> pIn(fileLoader::load("testdata.txt"));

   lexor l(*pIn);
   parser p(l);
   std::unique_ptr<fileNode> pRoot(new fileNode());
   p.parseFile(*pRoot);

   dumpVisitor v;
   pRoot->acceptVisitor(v);
}

autoPassInfo<loadPass> gLoadPass("",-1);
