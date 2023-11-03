#include "../cmn/output.hpp"
#include "../prattle/config.hpp"
#include "../prattle/pass.hpp"
#include "histogram.hpp"
#include "iWordObserverProviderPass.hpp"
#include <fstream>
#include <iostream>
#include <set>

using namespace prattle;
using namespace prattle::pass;

class histogramPrintingPass : public iPass {
public:
   void run(config& c, passLinks& l, void *pIr)
   {
      l.demandLink<iWordObserverProviderPass>()
         .getObserver()
            .forEach<histogram>([&](auto& h){ print(c,h); });
   }

private:
   void print(config& c, histogram& h)
   {
      printWithFiles(c,h);
      printFlat(c,h);
   }

   void printWithFiles(config& c, histogram& h)
   {
      auto path = output(c).ensurePath(h.name() + ".txt");
      std::cout << "  writing to " << path << std::endl;
      std::ofstream out(path.c_str());
      if(!out.good())
         throw std::runtime_error("can't open file for output: " + path);

      for(auto it=h.byCount().rbegin();it!=h.byCount().rend();it++)
      {
         auto& words = it->second;
         for(auto& word : words)
         {
            out
               << it->first << ": "
               << word << " -> " << word << " [";

            auto& files = h.files[word];
            for(auto& file : files)
               out << file << " ";

            out << "]" << std::endl;
         }
      }
   }

   void printFlat(config& c, histogram& h)
   {
      auto path = output(c).ensurePath(h.name() + "-flat.txt");
      std::cout << "  writing to " << path << std::endl;
      std::ofstream out(path.c_str());
      if(!out.good())
         throw std::runtime_error("can't open file for output: " + path);

      for(auto it=h.counts.begin();it!=h.counts.end();++it)
         out << it->first << std::endl;
   }
};

cdwExportPass(histogramPrintingPass,"",-1);
