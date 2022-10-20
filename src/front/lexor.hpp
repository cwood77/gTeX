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

public:
   static scanStrategies& get();

   standardStrategy topLevel;
   standardStrategy entity;
   standardStrategy ifNode;
   standardStrategy paragraphStart;
   standardStrategy paragraphEnd;

private:
   scanStrategies();
};

class lexor : public lexorBase {
public:
   enum {
      kComment = lexorBase::kFirstDerivedToken,
      kEntity,
      kLBrace,
      kRBrace,
      kColon,
      kActions,
      kLabel,
      kWord,
      kGoto,
      kIf,
      kEndIf,
      kBang,
   };

   explicit lexor(iLexorInput& src);

   void setup(node& n);
};
