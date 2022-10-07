#pragma once
#include "../prattle/loader.hpp"
#include "../prattle/pass.hpp"

using namespace prattle;
using namespace prattle::pass;

class loadPass : public iPass {
public:
   virtual void run(config& c, void *pIr);
};
