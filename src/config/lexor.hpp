#pragma once
#include "../prattle/lexor.hpp"

namespace prattle {
class node;
} // namespace prattle

using namespace prattle;
using namespace prattle::lex;

class quotedTextScanStrategy : public iScanStrategy {
public:
   explicit quotedTextScanStrategy(size_t token) : m_token(token) {}
   virtual void scan(kernel& k) const;

private:
   const size_t m_token;
};

class scanStrategies {
private:
   lexemeTable m_cfg;

public:
   static scanStrategies& get();

   standardStrategy cfg;
   quotedTextScanStrategy quoted;

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
      kQuote,
   };

   explicit lexor(iLexorInput& src);

   void setup(node& n);
};
