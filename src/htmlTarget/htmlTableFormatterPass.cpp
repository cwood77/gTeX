#include "../cmn/node.hpp"
#include "../middle/iFormatProvider.hpp"
#include "../prattle/config.hpp"
#include "../prattle/pass.hpp"

using namespace prattle;
using namespace prattle::pass;

namespace {

class visitor : public gTeXVisitor {
public:
   visitor(config& c, iFormatProvider& fmter)
   : m_fmter(fmter)
   {
      m_tableTitleFmtTxt = c.demand<stringSetting>("html:table-header-style").value;
      if(!m_tableTitleFmtTxt.empty())
         m_tableTitleFmt = iFormatProvider::getFmtCode(m_tableTitleFmtTxt);
   }

   virtual void visit(tableNode& n)
   {
      std::stringstream stream;

      if(n.entityType.empty())
      {
         // attached action

         stream << "<p class=\"first\">";
         if(!m_tableTitleFmtTxt.empty())
            m_fmter.beginFormat(m_tableTitleFmt,stream);
         stream << n.action;
         if(!m_tableTitleFmtTxt.empty())
            m_fmter.endFormat(m_tableTitleFmt,stream);
         stream << "</p>" << std::endl;
      }
      else
         throw std::runtime_error("unimpled - html table - normalentity");

      stream << "<table>" << std::endl;
      for(auto it=n.operandsToLabels.begin();it!=n.operandsToLabels.end();++it)
         stream
            << "<tr><td>" << it->first
            <<  "</td><td>" << it->second
            << "</td></tr>" << std::endl;
      stream << "</table>";

      auto *p = new paragraphNode();
      p->text = stream.str();
      n.replace(*p);
   }

private:
   std::string m_tableTitleFmtTxt;
   iFormatProvider& m_fmter;
   iFormatProvider::fmts m_tableTitleFmt;
};

} // anonymous namespace

class htmlTableFormatterPass : public iPass {
public:
   void run(config& c, passLinks& l, void *pIr)
   {
      auto *pRoot = reinterpret_cast<folderNode*>(pIr);
      auto& fmter = l.demandLink<iFormatProvider>();

      nodeEditOperation o;
      {
         nodeEditCollector _c(o);
         visitor v(c,fmter);
         pRoot->acceptVisitor(v);
      }
      o.commit();
   }
};

cdwExportPass(htmlTableFormatterPass,"",-1);
