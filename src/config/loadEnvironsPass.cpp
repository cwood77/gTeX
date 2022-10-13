#include "../prattle/loader.hpp"
#include "../prattle/pass.hpp"
#include "exec.hpp"
#include "lexor.hpp"
#include "node.hpp"
#include "parser.hpp"
#include <iostream>
#include <memory>

using namespace prattle;
using namespace prattle::log;
using namespace prattle::pass;

class cfgFileParsePass : public iPass {
public:
   void run(config& c, passLinks&, void *pIr)
   {
      std::unique_ptr<iLexorInput> pIn(fileLoader::load(getFileName()));
      lexor l(*pIn);
      parser p(l);

      seqNode root;
      p.parseFile(root);

      streamLogAdapter Log(std::cout);
      //cfgDumpVisitor v(Log);
      //root.acceptVisitor(v);

      execVisitor exec(c);
      root.acceptVisitor(exec);
   }

protected:
   virtual const char *getFileName() const = 0;
};

class loadEnvironsPass : public cfgFileParsePass {
protected:
   virtual const char *getFileName() const { return "environs"; }
};

cdwExportPass(loadEnvironsPass,"env",0);

class loadConfigPass : public cfgFileParsePass {
protected:
   virtual const char *getFileName() const { return "config"; }
};

cdwExportPass(loadConfigPass,"cfg",0);
