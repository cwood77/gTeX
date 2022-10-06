#pragma once
#include "../prattle/loader.hpp"
#include "../prattle/pass.hpp"

class loadPass : public prattle::pass::iPass {
public:
   virtual void run(prattle::config& c, void*& pIr);
};

extern prattle::pass::autoPassInfo<loadPass> gLoadPass;
