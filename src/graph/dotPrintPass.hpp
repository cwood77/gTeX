#pragma once
#include "../prattle/pass.hpp"

using namespace prattle;
using namespace prattle::pass;

class dotPrintPass : public iPass {
public:
   virtual void run(config& c, passLinks& l, void *);

   std::string outCfgKeyName;
};
