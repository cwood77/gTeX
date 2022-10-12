#pragma once
#include "../prattle/lexor.hpp"

namespace prattle {
class node;
} // namespace prattle

using namespace prattle;
using namespace prattle::lex;

class scanStrategies {
private:
   lexemeTable m_cfg;

public:
   static scanStrategies& get();

   standardStrategy cfg;

private:
   scanStrategies();
};

class lexor : public lexorBase {
public:
   enum {
      kComment = lexorBase::kFirstDerivedToken,
      kWord,
      kAssignment,
      kLBracket,
      kRBracket,
      kIf,
      kLBrace,
      kRBrace,
      kEquOp,
      kNeqOp,
   };

   explicit lexor(iLexorInput& src);

   void setup(node& n);
};
