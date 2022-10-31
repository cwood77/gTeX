#pragma once
#include "../cmn/node.hpp"

class lexor;

// <file> ::== '_entity' <entity> <file>
//           | '_macro' <macro-decl> <file>
//           | '_var' <var-decl> <file>
//           | <label> <para-set> <file>
//           | <inline-label> <para-set> <file>
// <para-set> ::== <words-on-same-line> <para-set>
//                  | 'if' 'varName' '{' <para-set> '}'
//                  | e
// <macro-decl> ::== <name> '{' '}' <para-set> '_macro'
// <var-decl> ::== <name> <type>
//
// <exploded-paragraph> ::== <word> <goto> <merge> <entity> <call> <var-ref> <attach> <word>
//
class parser {
public:
   explicit parser(lexor& l) : m_l(l) {}

   void parseFile(fileNode& f);
   void expandParagraph(paragraphNode& p);

private:
   void parseWords(paragraphNode& n);
   void parseParaSet(node& n);

   lexor& skipComments(const iScanStrategy& s);
   lexor& skipComments();
   void dupSetup(const node& src, node& dest);

   lexor& m_l;
};
