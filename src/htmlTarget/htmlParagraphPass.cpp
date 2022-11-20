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

      stream << n.text << "</p>";
      if(n.text.length() && n.text.c_str()[0] == '_')
         // this is a special paragraph that requests no <p></p> tags
         // (e.g. a chapter heading)
         n.text = n.text.c_str()+1;
      else
      {
         // wrap it with <p></p>
         n.text = stream.str();
         m_firstParagraph = false;
      }

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
