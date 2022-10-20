#include "lexor.hpp"
#include "parser.hpp"
#include <cstring>
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
      parseFile(f);
   }
   else if(m_l.getToken() == lexor::kLabel)
   {
      auto& n = f.appendChild<labelNode>();
      m_l.setup(n);
      m_l.advance();

      skipComments().demand(lexor::kWord);
      n.label = m_l.getLexeme();
      const char *pArrow = ::strstr(n.label.c_str(),"->");
      if(pArrow)
      {
         n.action = pArrow+2;
         n.label = std::string(n.label.c_str(),pArrow-n.label.c_str());
      }

      m_l.advance();
      parseParaSet(n);
      parseFile(f);
   }
   else if(m_l.getToken() == lexor::kEOI)
      return;
   else
      m_l.expected({ lexor::kEntity, lexor::kComment });
}

void parser::expandParagraph(paragraphNode& p)
{
   skipComments(scanStrategies::get().paragraphStart);

   if(m_l.getToken() == lexor::kWord)
   {
      auto& n = p.appendChild<paragraphNode>();
      dupSetup(p,n);
      n.text = m_l.getLexeme();
      skipComments(scanStrategies::get().paragraphStart)
         .advance(scanStrategies::get().paragraphStart);
      expandParagraph(p);
   }
   else if(m_l.getToken() == lexor::kGoto)
   {
      auto& n = p.appendChild<jumpNode>();
      dupSetup(p,n);
      skipComments(scanStrategies::get().paragraphStart)
         .advance(scanStrategies::get().paragraphStart);

      m_l.demand(lexor::kWord);
      n.id = m_l.getLexeme();
      skipComments(scanStrategies::get().paragraphStart)
         .advance(scanStrategies::get().paragraphStart);
      expandParagraph(p);
   }
   else if(m_l.getToken() == lexor::kEntity)
   {
      auto& n = p.appendChild<entityInstanceNode>();
      dupSetup(p,n);
      skipComments(scanStrategies::get().paragraphEnd)
         .advance(scanStrategies::get().paragraphEnd);

      m_l.demand(lexor::kWord);
      n.type = m_l.getLexeme();
      skipComments(scanStrategies::get().paragraphEnd)
         .advance(scanStrategies::get().paragraphEnd);
      m_l.demandAndEat(lexor::kColon,scanStrategies::get().paragraphEnd);

      m_l.demand(lexor::kWord);
      n.id = m_l.getLexeme();
      skipComments(scanStrategies::get().paragraphEnd)
         .advance(scanStrategies::get().paragraphEnd);
      m_l.demandAndEat(lexor::kRBrace,scanStrategies::get().paragraphStart);

      expandParagraph(p);
   }
   else if(m_l.getToken() == lexor::kEOI)
   {
      return;
   }
   else
      m_l.expected({ lexor::kWord, lexor::kGoto, lexor::kEntity });
}

// decide which words belong to this paragraph
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

// attach all subsequent paragraphs to this label
void parser::parseParaSet(node& l)
{
   if(m_l.getToken() == lexor::kWord)
   {
      auto& n = l.appendChild<paragraphNode>();
      m_l.setup(n);
      parseWords(n);

      parseParaSet(l);
   }
   else if(m_l.getToken() == lexor::kIf)
   {
      auto& If = l.appendChild<ifNode>();
      m_l.setup(If);
      m_l.advance(scanStrategies::get().ifNode);

      if(m_l.getToken() == lexor::kBang)
      {
         If.isFalse = true;
         m_l.advance();
      }

      m_l.demand(lexor::kWord);
      If.varName = m_l.getLexeme();
      m_l.advance();

      parseParaSet(If);

      m_l.demandAndEat(lexor::kEndIf);

      parseParaSet(l);
   }
   else
      return;
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

void parser::dupSetup(const node& src, node& dest)
{
   dest.filePath = src.filePath;
   dest.lineNumber = src.lineNumber;
}
