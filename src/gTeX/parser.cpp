#include "lexor.hpp"
#include "parser.hpp"
#include <sstream>

void parser::parseFile(fileNode& f)
{
   skipComments();

   if(m_l.getToken() == lexor::kEntity)
   {
      auto& n = f.appendChild<entityNode>();
      m_l.setup(n);
      skipComments().advance();
      m_l.demand(lexor::kWord);
      n.name = m_l.getLexeme();
      skipComments(scanStrategies::get().entity).advance(scanStrategies::get().entity);
      skipComments(scanStrategies::get().entity).demandAndEat(lexor::kLBrace,scanStrategies::get().entity);
      skipComments(scanStrategies::get().entity).demandAndEat(lexor::kActions,scanStrategies::get().entity);
      skipComments(scanStrategies::get().entity).demandAndEat(lexor::kLBrace,scanStrategies::get().entity);
      while(skipComments(scanStrategies::get().entity).getToken() == lexor::kWord)
      {
         n.actions.push_back(m_l.getLexeme());
         m_l.advance(scanStrategies::get().entity);
      }
      m_l.demandAndEat(lexor::kRBrace,scanStrategies::get().entity);
      skipComments(scanStrategies::get().entity).demandAndEat(lexor::kRBrace);
   }
   else if(m_l.getToken() == lexor::kLabel)
   {
      auto& n = f.appendChild<labelNode>();
      m_l.setup(n);
      m_l.advance();
      skipComments().demand(lexor::kWord);
      n.label = m_l.getLexeme();
      m_l.advance();
   }
   else if(m_l.getToken() == lexor::kWord)
   {
      auto& n = f.appendChild<paragraphNode>();
      m_l.setup(n);
      parseWords(n);
   }
   else if(m_l.getToken() == lexor::kEOI)
      return;
   else
      m_l.expected({ lexor::kEntity, lexor::kComment, lexor::kWord });

   parseFile(f);
}

void parser::parseWords(paragraphNode& n)
{
   std::stringstream stream;

   while(true)
   {
      stream << m_l.getLexeme();
      auto ln = m_l.getLineNumber();

      m_l.advance();
      skipComments();

      if(ln != m_l.getLineNumber())
         break;
      if(m_l.getToken() != lexor::kWord)
         break;

      stream << " ";
   }

   n.text = stream.str();
}

lexor& parser::skipComments(const iScanStrategy& s)
{
   while(m_l.getToken() == lexor::kComment)
   {
      m_l.scanToEndOfLine();
      m_l.advance(s);
   }
   return m_l;
}

lexor& parser::skipComments()
{
   return skipComments(scanStrategies::get().topLevel);
}
