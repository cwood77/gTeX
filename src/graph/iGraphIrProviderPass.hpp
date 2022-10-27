#pragma once

class graphRootNode;

class iGraphIrProviderPass {
public:
   virtual graphRootNode& getGraphIr() = 0;
};
