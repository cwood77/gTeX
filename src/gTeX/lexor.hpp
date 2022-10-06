#pragma once
#include "../prattle/lexor.hpp"

using namespace prattle;
using namespace prattle::lex;

class scanStrategies {
private:
   tokenTable m_cmn;
   tokenTable m_topLevel;
   tokenTable m_entity;

public:
   static scanStrategies& get();

   standardStrategy topLevel;
   standardStrategy entity;

private:
   scanStrategies();
};

class lexor : public lexorBase {
public:
   enum tokens {
      kComment = lexorBase::kFirstDerivedToken,
      kEntity,
      kLBrace,
      kRBrace,
      kActions,
      kLabel,
      kWord
   };

   explicit lexor(iLexorInput& src);

   tokens getToken() const { return (tokens)_getToken(); }
};
