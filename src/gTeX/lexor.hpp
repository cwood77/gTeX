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

public:
   static scanStrategies& get();

   standardStrategy topLevel;
   standardStrategy entity;

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
      kActions,
      kLabel,
      kWord
   };

   explicit lexor(iLexorInput& src);

   void setup(node& n);
};
