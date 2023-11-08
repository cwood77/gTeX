#include "../cmn/node.hpp"
#include "../prattle/lexor.hpp"
#include "../prattle/loader.hpp"
#include "../prattle/node.hpp"
#include "../prattle/pass.hpp"
#include "iWordObserverProviderPass.hpp"
#include <cstring>
#include <sstream>

using namespace prattle;
using namespace prattle::lex;
using namespace prattle::pass;

namespace {

class wordChecker {
public:
   static void transform(const std::string& word, std::vector<std::string>& words)
   {
      std::string w = word;
      transformWord(w);
      transformSplit(w,words);
   }

private:
   static void transformWord(std::string& word)
   {
      transformWordPerChar(word);
      transformWordPatterns(word);
   }

   static void transformWordPerChar(std::string& word)
   {
      std::stringstream noob;
      bool isNumber = true;
      for(size_t i=0;i<word.length();i++)
      {
         char c = word.c_str()[i];

         // some puncutation is discardable
         if(c==' ' ||
            c=='_' ||
            c=='.' || c=='!' || c=='?' ||
            c=='"' || c==',' || c==';' || c==':' ||
            c=='(' || c==')' ||
            c=='[' || c==']')
            continue;

         // keep track if it's a number
         if(isNumber &&
            !((('0' <= c) && (c <= '9')) || c=='-' || c=='+' ))
            isNumber = false;

         // add it
         noob << c;
      }

      if(isNumber)
         word = "";
      else
         word = noob.str();
   }

   static void transformWordPatterns(std::string& word)
   {
      // more complex rules discovered by example

      // remove leading single quotes: e.g. 'em
      removeLeading(word,"'");

      // remove trailing single quotes: e.g. ya'
      removeTrailing(word,"'");

      // remove leading dashes: e.g. --can't
      removeLeading(word,"--");

      // remove trailing dashes: e.g. agents--
      removeTrailing(word,"--");

      // remove apostrophe s: e.g. alyona's
      removeTrailing(word,"'s");
      removeTrailing(word,"'S");
   }

   static void transformSplit(const std::string& word, std::vector<std::string>& words)
   {
      if(word.length() == 0)
         return;

      const char *pHyphen = ::strstr(word.c_str(),"-");
      if(pHyphen == NULL)
         words.push_back(word);
      else
      {
         transformSplit(std::string(word.c_str(),pHyphen-word.c_str()),words);
         transformSplit(pHyphen+2,words);
      }
   }

   static void removeLeading(std::string& word, const std::string& pattern)
   {
      if(word.length() > pattern.length()
         && ::strncmp(word.c_str(),pattern.c_str(),pattern.length())==0)
         word = word.c_str()+pattern.length();
   }

   static void removeTrailing(std::string& word, const std::string& pattern)
   {
      if(word.length() > pattern.length()
         && ::strncmp(word.c_str()+word.length()-pattern.length(),
               pattern.c_str(),pattern.length())==0)
         word = std::string(word.c_str(),word.length()-pattern.length());
   }
};

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
         onWord(l.getLexeme(),n);
         l.advance();
      }
   }

private:
   void onWord(const std::string& word, paragraphNode& n)
   {
      std::vector<std::string> words;
      wordChecker::transform(word,words);
      for(auto word : words)
         m_obs.onWord(word,n);
   }

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
