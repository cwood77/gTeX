#pragma once

class lexor;
class seqNode;

// # foo               <comment>
// foo := bar          <assignment>
// foo := [ bar, baz]
// foo := T
//
// if foo == bar       <condition>
// {
//    if foo != bar
//    {
//    }
// }
//
// load bar            <include>
//
// <file> ::== <comment> <word> <set> <file>
//             <comment> 'if' <if> <file>
//             |-
//
// <set> ::== ':=' <value>
//            ':=' '[' <value> <value'>
// <value'> ::== <value> <value'>
//               ']'
//
// <if> ::== <word> <op> <value> <lbrace> <set> <rbrace>
//
class parser {
public:
   explicit parser(lexor& l) : m_l(l) {}

   void parseFile(seqNode& n);

private:
   void parseSeq(seqNode& n);
   void parseSet(setNode& n);
   void parseValue(node& n);
   void parseIf(ifNode& n);

   lexor& skipComments();

   lexor& m_l;
};
