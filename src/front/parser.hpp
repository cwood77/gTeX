#pragma once
#include "../cmn/node.hpp"

class lexor;

// <file> ::== _entity <entity> <file>
//           | <label> <para-set> <file>
// <para-set> ::== <words-on-same-line> <para-set>
//                  | 'if' 'varName' '{' <para-set> '}'
//                  | e
//
// <exploded-paragraph> ::== <word> <goto> <entity> <word>
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
