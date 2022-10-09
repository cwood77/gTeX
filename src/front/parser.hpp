#pragma once
#include "../cmn/node.hpp"

class lexor;

// <file> ::== _entity <entity> <file>
//           | <label> <label-paras> <file>
//           | <paragraph> <file>               TODO is this really ok?
//
// <label-paras> ::== <paragraph> <label-paras>
//                  | e
//
class parser {
public:
   explicit parser(lexor& l) : m_l(l) {}

   void parseFile(fileNode& f);

private:
   void parseWords(paragraphNode& n);
   void parseLabelParas(labelNode& n);

   lexor& skipComments(const iScanStrategy& s);
   lexor& skipComments();

   lexor& m_l;
};
