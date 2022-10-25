#include "../prattle/module.hpp"
#include "engine.hpp"
#include <iostream>

using namespace prattle;
using namespace prattle::module;
using namespace prattle::pass;

int main(int argc,const char*[])
{
   if(argc>1)
      std::cout << "gTeX" << std::endl;
   else
      std::cout << "gTeX built " << __DATE__ << " " << __TIME__ << std::endl;

   // module loader unloads modules on dtor, so put it first. This allows deallocs
   // (e.g. config's dtor) to use module heaps
   moduleLoader mLdr;

   try
   {
      engine(passCatalog::get(),targetCatalog::get(),mLdr).run();
   }
   catch(std::exception& x)
   {
      std::cerr << "ERROR: " << x.what() << std::endl;

      std::cout << "aborting" << std::endl;
      return -2;
   }

   std::cout << "bye" << std::endl;
   return 0;
}
