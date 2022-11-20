#include "../cmn/node.hpp"
#include "../prattle/pass.hpp"
#include "iFormatProvider.hpp"

using namespace prattle;
using namespace prattle::pass;

namespace {

class styleWriter : public gTeXVisitor {
public:
   explicit styleWriter(iFormatProvider& fmter) : m_fmter(fmter) {}

   virtual void visit(labelNode& n)
   {
      visitChildren(n);
      if(m_fStack.size() != 0)
      {
         std::stringstream msg;
         msg << "dangling style detected inside label starting on line " << n.lineNumber << ", " << n.filePath;
         throw std::runtime_error(msg.str());
      }
   }

   virtual void visit(paragraphNode& n) { visitChildren(n); }
   virtual void visit(stylingNode& n)
   {
      auto *p = new paragraphNode();
      bool isBegin;
      p->text = formatStyle(n,isBegin);
      stylingResidue.push_back(std::make_pair<>(p,buildConcatCode(n,isBegin)));

      n.replace(*p);
   }

   std::list<std::pair<paragraphNode*,std::string> > stylingResidue;

private:
   iFormatProvider::fmts getFmtCode(const std::string& txt)
   {
      if(txt == "b") return iFormatProvider::kBold;
      if(txt == "i") return iFormatProvider::kItalic;
      if(txt == "hr") return iFormatProvider::kHRule;
      throw std::runtime_error("unknown styling syntax: " + txt);
   }

   std::string formatStyle(stylingNode& n, bool& isBegin)
   {
      std::stringstream stream;

      auto code = getFmtCode(n.style);
      if(m_fStack.size() == 0 || m_fStack.back() != code)
      {
         m_fStack.push_back(code);
         m_fmter.beginFormat(code,stream);
         isBegin = true;
      }
      else
      {
         m_fStack.pop_back();
         m_fmter.endFormat(code,stream);
         isBegin = false;
      }

      return stream.str();
   }

   std::string buildConcatCode(stylingNode& n, bool isBegin)
   {
      if(isBegin)
         return n.suffix + ">";
      else
         return "<" + n.suffix;
   }

   iFormatProvider& m_fmter;

   std::list<iFormatProvider::fmts> m_fStack;
};

class styleConcatenator {
public:
   void process(paragraphNode& residue, const std::string& code)
   {
      auto idx = residue.demandParent().getIndexOfChild(residue);

      paragraphNode *pBefore = NULL;
      if(idx)
         pBefore = dynamic_cast<paragraphNode*>(residue.demandParent().getChildren()[idx-1]);

      paragraphNode *pAfter = NULL;
      if(idx+1 < residue.demandParent().getChildren().size())
         pAfter = dynamic_cast<paragraphNode*>(residue.demandParent().getChildren()[idx+1]);

      // merge with previous node
      if(pBefore && code.length() && code.c_str()[0] == '<')
      {
         residue.text = pBefore->text + residue.text;
         pBefore->Delete();
      }

      // merge with the subsequent node
      if(pAfter && (code == ">" || (code.length() == 2 && code.c_str()[1] == '>' )))
      {
         residue.text += pAfter->text;
         residue.combineWithFollowingPunct = pAfter->combineWithFollowingPunct;
         pAfter->Delete();
      }
      else
         residue.combineWithFollowingPunct = true;
   }
};

} // anonymous namespace

class expandedParagraphStylingPass : public iPass {
public:
   void run(config& c, passLinks& l, void *pIr)
   {
      auto *pRoot = reinterpret_cast<folderNode*>(pIr);
      auto& fmter = l.demandLink<iFormatProvider>();

      styleWriter writer(fmter);
      {
         nodeEditOperation o;
         {
            nodeEditCollector c(o);
            pRoot->acceptVisitor(writer);
         }
         o.commit();
      }

      styleConcatenator cat;
      for(auto& tuple : writer.stylingResidue)
         cat.process(*tuple.first,tuple.second);

      //throw std::runtime_error("show AST");
   }
};

cdwExportPass(expandedParagraphStylingPass,"",-1);
