#include "../cmn/node.hpp"
#include "../prattle/loader.hpp"
#include "../prattle/pass.hpp"
#include "lexor.hpp"
#include "parser.hpp"
#include <iostream>
#include <memory>

using namespace prattle;
using namespace prattle::pass;

namespace {

class visitor : public gTeXVisitor {
public:
   virtual void visit(fileNode& n)
   {
      std::cout << "  loading file " << n.filePath  << std::endl;

      std::unique_ptr<iLexorInput> pIn(fileLoader::load(n.filePath));
      lexor l(*pIn);
      parser p(l);
      p.parseFile(n);
   }
};

} // anonymous namespace

class parsePass : public iPass {
public:
   void run(config& c, passLinks&, void *pIr)
   {
      auto *pRoot = reinterpret_cast<folderNode*>(pIr);

      visitor v;
      pRoot->acceptVisitor(v);
   }
};

cdwExportPass(parsePass,"",-1);
