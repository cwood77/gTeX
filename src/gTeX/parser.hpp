#pragma once
#include "node.hpp"

class lexor;

// <file> ::== _entity <entity> <file>
//           | <label> <paragraph> <file>
//           | <paragraph> <file>
//
class parser {
public:
   explicit parser(lexor& l) : m_l(l) {}

   void parseFile(fileNode& f);

private:
   void parseWords(paragraphNode& n);

   lexor& skipComments(const iScanStrategy& s);
   lexor& skipComments();

   lexor& m_l;
};
