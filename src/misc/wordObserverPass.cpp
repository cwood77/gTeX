#include "../cmn/node.hpp"
#include "../prattle/lexor.hpp"
#include "../prattle/loader.hpp"
#include "../prattle/node.hpp"
#include "../prattle/pass.hpp"
#include "iWordObserverProviderPass.hpp"

using namespace prattle;
using namespace prattle::lex;
using namespace prattle::pass;

namespace {

class wordCountingLexor : public lexorBase {
public:
   enum {
      kWord = kFirstDerivedToken
   };

   wordCountingLexor(const iScanStrategy& defaultStrat, iLexorInput& src)
   : lexorBase(defaultStrat,src)
   {
      publishToken(kWord,"word");
   }
};

const lexemeTableEntry gNoTokens[] = {
   { lexemeTableEntry::kPunctuation,  NULL }
};

class visitor : public gTeXVisitor {
public:
   explicit visitor(iWordObserver& o)
   : m_obs(o)
   , m_lTable(gNoTokens)
   , m_scanStrat(m_lTable,/*anyWordToken*/wordCountingLexor::kWord)
   {
   }

   virtual void visit(labelNode& n) { visitChildren(n); }
   virtual void visit(paragraphNode& n)
   {
      memorySource mSrc(n.text.c_str());
      wordCountingLexor l(m_scanStrat,mSrc);
      while(l.getToken() != wordCountingLexor::kEOI)
      {
         m_obs.onWord(l.getLexeme(),n);
         l.advance();
      }
   }

private:
   iWordObserver& m_obs;
   lexemeTable m_lTable;
   standardStrategy m_scanStrat;
};

} // anonymous namespace

class wordObserverPass : public iPass {
public:
   void run(config&, passLinks& l, void *pIr)
   {
      auto& obs = l.demandLink<iWordObserverProviderPass>().getObserver();

      visitor v(obs);
      auto *pRoot = reinterpret_cast<folderNode*>(pIr);
      pRoot->acceptVisitor(v);
   }
};

cdwExportPass(wordObserverPass,"",-1);
