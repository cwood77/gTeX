#include "../cmn/node.hpp"
#include "../cmn/output.hpp"
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

class htmlPrintPass : public iPass {
public:
   void run(config& c, passLinks&, void *pIr)
   {
      auto *pRoot = reinterpret_cast<folderNode*>(pIr);

      auto path = output(c).ensurePath("out.html");
      std::cout << "  writing to " << path << std::endl;
      std::ofstream out(path.c_str());
      if(!out.good())
         throw std::runtime_error("can't open file for output: " + path);

      out << "<html><head><style>" << std::endl;

      // styling
      out << "p {text-indent: 25px;}" << std::endl;
      out << "p.first {text-indent: 0px;}" << std::endl;
      out << "p {margin-top: 0;}" << std::endl;
      out << "p {margin-bottom: 0;}" << std::endl;
      out << "</style></head><body>" << std::endl;

      // body
      visitor v(out);
      pRoot->acceptVisitor(v);

      out << "</body></html>" << std::endl;
   }
};

cdwExportPass(htmlPrintPass,"",-1);
