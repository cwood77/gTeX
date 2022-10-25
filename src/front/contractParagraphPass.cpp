#include "../cmn/node.hpp"
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
      // reconstruct the paragraph
      std::vector<paragraphNode*> words;
      n.searchDown<paragraphNode>(words,[&](auto& p){return &p!=&n;});
      std::stringstream stream;
      paragraphNode *pLast = NULL;
      for(auto *pWord : words)
      {
         const bool isPunct = ::ispunct(pWord->text.c_str()[0]);

         if(pLast && !(isPunct && pLast->combineWithFollowingPunct))
            stream << " ";

         stream << pWord->text;

         pLast = pWord;
      }
      n.text = stream.str();

      // delete children
      auto copy = n.getChildren();
      for(auto *pN : copy)
         pN->Delete();
   }
};

} // anonymous namespace

class contractParagraphPass : public iPass {
public:
   void run(config& c, passLinks&, void *pIr)
   {
      auto *pRoot = reinterpret_cast<folderNode*>(pIr);

      visitor v;
      pRoot->acceptVisitor(v);
   }
};

cdwExportPass(contractParagraphPass,"",-1);
