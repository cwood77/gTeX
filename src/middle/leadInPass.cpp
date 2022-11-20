#include "../cmn/node.hpp"
#include "../prattle/config.hpp"
#include "../prattle/pass.hpp"

using namespace prattle;
using namespace prattle::pass;

// TODO - move into prattle config
long asL(const std::string& s)
{
   long n = 0;
   auto rVal = ::sscanf(s.c_str(),"%ld",&n);
   if(rVal != 1)
      throw std::runtime_error("can't parse long int: " + s);
   return n;
}

class leadInPass : public iPass {
public:
   void run(config& c, passLinks&, void *pIr)
   {
      auto nWords = asL(c.demand<stringSetting>("lead-in:nWords").value);
      if(nWords < 1)
         return;

//
// _sch Title _sch
//    _sch
//    Title
//    _sch
//
// Alas, poor Yorick, I knew him Horatio; a fellow...
//    Alas,
//    poor
//    Yorick,
//    ...

      auto *pRoot = reinterpret_cast<folderNode*>(pIr);
      std::vector<stylingNode*> chapters;
      pRoot->searchDown<stylingNode>(chapters,[](auto&n){ return n.style == "ch"; });

      for(auto *pChap : chapters)
      {
         auto& lineWithChapter = pChap->demandParent();
         auto& section = lineWithChapter.demandParent();
         auto idx = section.getIndexOfChild(lineWithChapter);

         // find the line after the chapter heading, if any
         paragraphNode *pLineToLeadIn = NULL;
         if(idx+1 < section.getChildren().size())
            pLineToLeadIn = dynamic_cast<paragraphNode*>(
               section.getChildren()[idx+1]);

         if(pLineToLeadIn)
         {
            // style the first N atoms of the line
            long wordsLeadIn = 0;
            for(auto *pWord : pLineToLeadIn->getChildren())
            {
               if(wordsLeadIn == nWords)
                  break; // done

               if(tryLeadInNode(*pWord))
                  wordsLeadIn++;
            }
         }
      }
   }

private:
   bool tryLeadInNode(node& n)
   {
      auto *pN = dynamic_cast<paragraphNode*>(&n);
      if(!pN)
         return false;

      std::stringstream noob;
      for(size_t i=0;i<pN->text.length();i++)
      {
         char c = pN->text.c_str()[i];

         // uppercase it
         if(('a' <= c) && (c <= 'z'))
            c -= ('a' - 'A');

         noob << c;
      }

      pN->text = noob.str();
      return true;
   }
};

cdwExportPass(leadInPass,"",-1);
