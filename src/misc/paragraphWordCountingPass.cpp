#include "../cmn/node.hpp"
#include "../prattle/lexor.hpp"
#include "../prattle/loader.hpp"
#include "iHistogramProviderPass.hpp"
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
   visitor(attribute<size_t>& attr, histogram& hist)
   : m_attr(attr)
   , m_histogram(hist)
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
         m_histogram.add(l.getLexeme(),n.filePath);
         cnt++;
         l.advance();
      }
   }

private:
   attribute<size_t>& m_attr;
   histogram& m_histogram;
   lexemeTable m_lTable;
   standardStrategy m_scanStrat;
};

} // anonymous namespace

void paragraphWordCountingPass::run(config& c, passLinks& l, void *pIr)
{
   auto *pRoot = reinterpret_cast<folderNode*>(pIr);

   histogram dummyHistogram;
   auto *pHist = &dummyHistogram;
   if(auto *pProv = l.findLink<iHistogramProviderPass>())
      pHist = &pProv->getHistogram();

   visitor v(wCntAttr,*pHist);
   pRoot->acceptVisitor(v);
}

cdwExportPass(paragraphWordCountingPass,"",-1);
