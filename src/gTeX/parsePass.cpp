#include "parsePass.hpp"
#include <iostream>
#include <memory>

using namespace prattle;
using namespace prattle::pass;

void loadPass::run(config& c, void*& pIr)
{
   std::cout << "loading file" << std::endl;
   std::unique_ptr<iLexorInput> pIn(fileLoader::load("testdata.txt"));
}

autoPassInfo<loadPass> gLoadPass("",-1);
