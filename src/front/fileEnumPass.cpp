#define WIN32_LEAN_AND_MEAN
#include "../cmn/node.hpp"
#include "../prattle/config.hpp"
#include "../prattle/pass.hpp"
#include <iostream>
#include <windows.h>

using namespace prattle;
using namespace prattle::pass;

class fileEnumPass : public iPass {
public:
   void run(config& c, passLinks&, void *pIr)
   {
      auto *pRoot = reinterpret_cast<folderNode*>(pIr);

      auto& in = c.demand<stringSetting>("in").value;

      WIN32_FIND_DATA fData;
      HANDLE hFind = ::FindFirstFileA((in + "\\*").c_str(),&fData);
      if(hFind == INVALID_HANDLE_VALUE)
         throw std::runtime_error("can't find input file(s)");
      do
      {
         if(fData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
            continue;

         std::string name = fData.cFileName;
         if(name.length() < 5 || ::strnicmp(name.c_str()+name.length()-5,".gtex",5)!=0)
            continue;

         auto& n = pRoot->appendChild<fileNode>();
         n.filePath = in + "\\" + fData.cFileName;
         std::cout << "  found file " << n.filePath << std::endl;

      } while(::FindNextFile(hFind,&fData));

      if(pRoot->getChildren().size() == 0)
         throw std::runtime_error("no input files found");
   }
};

cdwExportPass(fileEnumPass,"",-1);
