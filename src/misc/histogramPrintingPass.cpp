#include "../cmn/output.hpp"
#include "../prattle/config.hpp"
#include "../prattle/pass.hpp"
#include "iHistogramProviderPass.hpp"
#include <fstream>
#include <iostream>
#include <set>

using namespace prattle;
using namespace prattle::pass;

class histogramPrintingPass : public iPass {
public:
   void run(config& c, passLinks& l, void *pIr)
   {
      {
         auto path = output(c).ensurePath("out-histo.txt");
         std::cout << "  writing to " << path << std::endl;
         std::ofstream out(path.c_str());
         if(!out.good())
            throw std::runtime_error("can't open file for output: " + path);

         auto& h = l.demandLink<iHistogramProviderPass>().getHistogram();

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
      {
         auto path = output(c).ensurePath("out-histo2.txt");
         std::cout << "  writing to " << path << std::endl;
         std::ofstream out(path.c_str());
         if(!out.good())
            throw std::runtime_error("can't open file for output: " + path);

         auto& h = l.demandLink<iHistogramProviderPass>().getHistogram();

         for(auto it=h.counts.begin();it!=h.counts.end();++it)
            out << it->first << std::endl;
      }
   }
};

cdwExportPass(histogramPrintingPass,"",-1);
