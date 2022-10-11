#include "../cmn/node.hpp"
#include "../prattle/lexor.hpp"
#include "../prattle/loader.hpp"
#include "paragraphWordCountingPass.hpp"

using namespace prattle::lex;

namespace {

const lexemeTableEntry gNoTokens[] = {
   { lexemeTableEntry::kPunctuation,  NULL }
};

class wordCountingLexor : public lexorBase {
public:
   enum {
      kWord = kFirstDerivedToken
   };

   wordCountingLexor(const iScanStrategy& defaultStrat, iLexorInput& src)
   : lexorBase(defaultStrat,src) {}
};

class visitor : public gTeXVisitor {
public:
   explicit visitor(attribute<size_t>& attr)
   : m_attr(attr)
   , m_lTable(gNoTokens)
   , m_scanStrat(m_lTable,/*anyWordToken*/wordCountingLexor::kFirstDerivedToken)
   {
   }

   virtual void visit(labelNode& n) { visitChildren(n); }
   virtual void visit(paragraphNode& n)
   {
      auto& cnt = n[m_attr];

      memorySource mSrc(n.text.c_str());
      wordCountingLexor l(m_scanStrat,mSrc);
      while(l.getToken() != wordCountingLexor::kEOI)
      {
         cnt++;
         l.advance();
      }
   }

private:
   attribute<size_t>& m_attr;
   lexemeTable m_lTable;
   standardStrategy m_scanStrat;
};

} // anonymous namespace

void paragraphWordCountingPass::run(config& c, passLinks&, void *pIr)
{
   auto *pRoot = reinterpret_cast<folderNode*>(pIr);

   visitor v(wCntAttr);
   pRoot->acceptVisitor(v);
}

cdwExportPass(paragraphWordCountingPass,"",-1);
