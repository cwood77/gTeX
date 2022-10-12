#include "lexor.hpp"
#include "node.hpp"
#include "parser.hpp"

void parser::parseFile(seqNode& n)
{
   parseSeq(n);
   m_l.demand(lexor::kEOI);
}

void parser::parseSeq(seqNode& n)
{
   skipComments();

   if(m_l.getToken() == lexor::kWord)
   {
      auto& s = n.appendChild<setNode>();
      m_l.setup(s);
      s.varName = m_l.getLexeme();
      m_l.advance();
      parseSet(s);
      parseSeq(n);
   }
   else if(m_l.getToken() == lexor::kIf)
   {
      auto& i = n.appendChild<ifNode>();
      m_l.setup(i);
      m_l.advance();
      parseIf(i);
      parseSeq(n);
   }
}

void parser::parseSet(setNode& n)
{
   m_l.demandAndEat(lexor::kAssignment);
   parseValue(n);
}

void parser::parseValue(node& n)
{
   if(m_l.getToken() == lexor::kLBracket)
   {
      // array

      auto& v = n.appendChild<arrayLitNode>();
      m_l.setup(v);

      while(true)
      {
         m_l.advance();
         if(m_l.getToken() == lexor::kRBracket)
         {
            m_l.advance();
            break;
         }

         m_l.demand(lexor::kWord);
         v.vals.push_back(m_l.getLexeme());
      }
   }
   else
   {
      // scalar

      m_l.demand(lexor::kWord);
      auto& v = n.appendChild<strLitNode>();
      m_l.setup(v);
      v.val = m_l.getLexeme();
      m_l.advance();
   }
}

void parser::parseIf(ifNode& n)
{
   m_l.demand(lexor::kWord);
   n.varName = m_l.getLexeme();
   m_l.advance();

   if(m_l.getToken() != lexor::kEquOp && m_l.getToken() != lexor::kNeqOp)
      m_l.expected({ lexor::kEquOp, lexor::kNeqOp });
   n.op = m_l.getLexeme();
   m_l.advance();

   parseValue(n);

   m_l.demandAndEat(lexor::kLBrace);
   auto& s = n.appendChild<seqNode>();
   parseSeq(s);
   m_l.demandAndEat(lexor::kRBrace);
}

lexor& parser::skipComments()
{
   while(m_l.getToken() == lexor::kComment)
   {
      m_l.scanToEndOfLine();
      m_l.advance();
   }
   return m_l;
}
