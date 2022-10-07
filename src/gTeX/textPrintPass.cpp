#include "../prattle/config.hpp"
#include "../prattle/loader.hpp"
#include "../prattle/pass.hpp"
#include "lexor.hpp"
#include "node.hpp"
#include "parser.hpp"
#include <fstream>
#include <iostream>
#include <memory>

using namespace prattle;
using namespace prattle::pass;

namespace {

class textPrinter : public gTeXVisitor {
public:
   explicit textPrinter(std::ostream& o) : m_out(o) {}

   virtual void visit(fileNode& n)
   {
      visitChildren(n);
   }

   virtual void visit(paragraphNode& n)
   {
      m_out << n.text << std::endl;
      m_out << std::endl;
   }

private:
   std::ostream& m_out;
};

} // anonymous namespace

class textPrintPass : public iPass {
public:
   void run(config& c, void *pIr)
   {
      fileNode *pRoot = reinterpret_cast<fileNode*>(pIr);

      auto path = c.demand<stringSetting>("text:out-path").value;
      std::cout << "writing to " << path << std::endl;
      std::ofstream out(path.c_str());
      if(!out.good())
         throw std::runtime_error("can't open file for output: " + path);

      textPrinter v(out);
      pRoot->acceptVisitor(v);
   }
};

autoPassInfo<textPrintPass> gTextPrintPass("",-1);
const char *kTextPrintPassName = typeid(textPrintPass).name();
