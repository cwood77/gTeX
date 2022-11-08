#include "../cmn/node.hpp"
#include "../prattle/pass.hpp"

using namespace prattle;
using namespace prattle::pass;

namespace {

class visitor : public gTeXVisitor {
public:
   virtual void visit(labelNode& n)
   {
      m_firstParagraph = true;
      visitChildren(n);
   }

   virtual void visit(paragraphNode& n)
   {
      std::stringstream stream;
      stream << "<p";
      if(m_firstParagraph)
         stream << " class=\"first\">";
      else
         stream << ">";
      m_firstParagraph = false;

      stream << n.text << "</p>";
      n.text = stream.str();

      visitChildren(n);
   }

private:
   bool m_firstParagraph;
};

} // anonymous namespace

class htmlParagraphPass : public iPass {
public:
   void run(config& c, passLinks&, void *pIr)
   {
      auto *pRoot = reinterpret_cast<folderNode*>(pIr);

      visitor v;
      pRoot->acceptVisitor(v);
   }
};

cdwExportPass(htmlParagraphPass,"",-1);
