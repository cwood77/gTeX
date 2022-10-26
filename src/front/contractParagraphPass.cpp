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
      if(n.getChildren().size() == 0) return;

      visitChildren(n);

      // reconstruct the paragraph
      std::vector<paragraphNode*> words;
      n.searchDown<paragraphNode>(words,[&](auto& p){return p.getParent()==&n;});
      if(words.size() != n.getChildren().size())
         throw std::runtime_error("some children of paragraphNode aren't paragraphs!");
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

      nodeEditOperation o;
      {
         nodeEditCollector c(o);
         visitor v;
         pRoot->acceptVisitor(v);
      }
      o.commit();
   }
};

cdwExportPass(contractParagraphPass,"",-1);
