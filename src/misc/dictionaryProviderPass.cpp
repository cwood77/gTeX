#include "../prattle/config.hpp"
#include "../prattle/pass.hpp"
#include "iDictionaryProviderPass.hpp"
#include <fstream>
#include <iostream>
#include <memory>
#include <stdexcept>

using namespace prattle;
using namespace prattle::pass;

class dictionaryProviderPass : public iPass, public dict::iDictionaryProviderPass {
public:
   void run(config& c, passLinks& l, void *pIr)
   {
      // TODO HACK - this is terrible, but meh; there's only one gamebook for now
      std::string dictPathBase = "C:\\cygwin64\\home\\chris\\dev\\gTeX\\dict\\";

      if(auto *pNames = c.fetch<stringArraySetting>("dict:names"))
      {
         for(auto& name : pNames->value)
         {
            std::unique_ptr<dict::dictionary> pDict(new dict::dictionary());
            std::cout << "   loading dictionary " << name << std::endl;
            std::string fullName = name;
            if(name.c_str()[0] == '/')
               fullName = dictPathBase + (name.c_str()+1);
            std::ifstream fStream(fullName);
            if(!fStream.good())
               throw std::runtime_error("can't open " + fullName);
            pDict->load(fStream);
            std::cout << "      found " << pDict->getSize() << " entries" << std::endl;
            m_dict.add(*pDict.release());
         }
      }
   }

   virtual dict::iDictionary& getDictionary() { return m_dict; }

private:
   dict::compositeDictionary m_dict;
};

cdwExportPass(dictionaryProviderPass,"",-1);
