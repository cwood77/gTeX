#pragma once
#include "../prattle/node.hpp"
#include "../prattle/pass.hpp"

using namespace prattle;
using namespace prattle::pass;

class paragraphWordCountingPass : public iPass {
public:
   void run(config& c, passLinks&, void *pIr);

   attribute<size_t> wCntAttr;
};
