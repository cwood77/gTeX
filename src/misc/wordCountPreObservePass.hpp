#pragma once
#include "../prattle/node.hpp"
#include "../prattle/pass.hpp"
#include "iWordObserverProviderPass.hpp"

using namespace prattle;
using namespace prattle::pass;

class wordCountPreObservePass : public iPass, public iWordObserver {
public:
   void run(config&, passLinks& l, void *)
   {
      l.demandLink<iWordObserverProviderPass>().getObserver().add(*this);
   }

   virtual void onWord(const std::string& w, paragraphNode& n)
   {
      auto& cnt = n[wCntAttr];
      cnt++;
   }

   attribute<size_t> wCntAttr;
};
