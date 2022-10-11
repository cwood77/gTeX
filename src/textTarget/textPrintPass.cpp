#include "../cmn/node.hpp"
#include "../prattle/config.hpp"
#include "../prattle/pass.hpp"
#include <fstream>
#include <iostream>

using namespace prattle;
using namespace prattle::pass;

namespace {

class visitor : public gTeXVisitor {
public:
   explicit visitor(std::ostream& o) : m_out(o) {}

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
   void run(config& c, passLinks&, void *pIr)
   {
      auto *pRoot = reinterpret_cast<folderNode*>(pIr);

      auto path = c.demand<stringSetting>("text:out-path").value;
      std::cout << "  writing to " << path << std::endl;
      std::ofstream out(path.c_str());
      if(!out.good())
         throw std::runtime_error("can't open file for output: " + path);

      visitor v(out);
      pRoot->acceptVisitor(v);
   }
};

cdwExportPass(textPrintPass,"",-1);
