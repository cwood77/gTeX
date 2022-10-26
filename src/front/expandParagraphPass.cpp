#include "../cmn/node.hpp"
#include "../prattle/loader.hpp"
#include "../prattle/pass.hpp"
#include "lexor.hpp"
#include "parser.hpp"

using namespace prattle;
using namespace prattle::pass;

namespace {

class visitor : public gTeXVisitor {
public:
   virtual void visit(fileNode& n)
   {
      visitChildren(n);
   }

   virtual void visit(labelNode& n)
   {
      visitChildren(n);
   }

   virtual void visit(paragraphNode& n)
   {
      memorySource in(n.text.c_str());
      lexor l(scanStrategies::get().paragraphStart,in);
      parser p(l);
      p.expandParagraph(n);
   }
};

} // anonymous namespace

class expandParagraphPass : public iPass {
public:
   void run(config& c, passLinks&, void *pIr)
   {
      auto *pRoot = reinterpret_cast<folderNode*>(pIr);

      visitor v;
      pRoot->acceptVisitor(v);
   }
};

cdwExportPass(expandParagraphPass,"",-1);
