#pragma once
#include "../prattle/lexor.hpp"

namespace prattle {
class node;
} // namespace prattle

using namespace prattle;
using namespace prattle::lex;

class scanStrategies {
private:
   lexemeTable m_cmn;
   lexemeTable m_topLevel;
   lexemeTable m_entity;
   lexemeTable m_ifNode;
   lexemeTable m_paragraphStart;
   lexemeTable m_paragraphEnd;
   lexemeTable m_macro;

public:
   static scanStrategies& get();

   standardStrategy topLevel;
   standardStrategy entity;
   standardStrategy ifNode;
   standardStrategy paragraphStart;
   standardStrategy paragraphEnd;
   standardStrategy macro;

private:
   scanStrategies();
};

class lexor : public lexorBase {
public:
   enum {
      kComment = lexorBase::kFirstDerivedToken,

      kEntity,
      kMacro,
      kCall,
      kVar,

      kLBrace,
      kRBrace,
      kColon,
      kBang,
      kAt,

      kActions,

      kLabel,
      kInlineLabel,
      kWord,
      kGoto,
      kMerge,
      kAttach,

      kIf,
      kEndIf,
   };

   explicit lexor(iLexorInput& src)
   : lexorBase(scanStrategies::get().topLevel,src) { publishTokens(); }

   lexor(const iScanStrategy& defaultStrat, iLexorInput& src)
   : lexorBase(defaultStrat,src) { publishTokens(); }

   void setup(node& n);

private:
   void publishTokens();
};
