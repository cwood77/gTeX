#include "../cmn/node.hpp"
#include "../prattle/config.hpp"
#include "../prattle/log.hpp"
#include "paragraphWordCountingPass.hpp"
#include "wordCountReport.hpp"
#include <fstream>
#include <iostream>

using namespace prattle;
using namespace prattle::log;
using namespace prattle::pass;

namespace {

// counts by date
// <date> <total> <change from yesterday>
//
// NaNoWriMo stats
// monthly goal: 30000
// days left in November: 30
// words left: 30000
// words left / day left: 1000
// days written: 0/0
//
// historical log
// <date>
// overall X
//   file -> X
//
// <date>
// overall X
//   file -> X

class visitor : public gTeXVisitor {
public:
   visitor(attribute<size_t>& attr, iLog& l) : m_attr(attr), m_log(l) {}

   virtual void visit(folderNode& n)
   {
      m_log.s().s() << indent(m_log) << "overall: " << n[m_attr] << std::endl;
      autoIndent _i(m_log);
      visitChildren(n);
   }

   virtual void visit(fileNode& n)
   {
      m_log.s().s() << indent(m_log) << n.filePath << " -> "  << n[m_attr] << std::endl;
   }

private:
   attribute<size_t>& m_attr;
   iLog& m_log;
};

} // anonymous namespace

class wordCountReporterPass : public iPass {
public:
   void run(config& c, passLinks& l, void *pIr)
   {
      auto *pRoot = reinterpret_cast<folderNode*>(pIr);
      auto& attr = l.demandLink<paragraphWordCountingPass>().wCntAttr;

      // construct the log
      wcnt::wordCountReport wcr;

      wcnt::dateSummaryTable byDate;
      wcr.sections.push_back(&byDate);

      wcnt::monthlyStats nanowrimo;
      wcr.sections.push_back(&nanowrimo);

      wcnt::historicalLog hLog;
      wcr.sections.push_back(&hLog);

      // load the log
      {
         std::ifstream fStream(c.demand<stringSetting>("wcnt:log-path").value);
         wcr.load(fStream);
      }

      // update the date summary log
      {
         std::stringstream todayStream;
         todayStream << wcnt::today();
         byDate.total[todayStream.str()] = (*pRoot)[attr];
      }

      // update the monthly goals
      {
      }

      // update the historical log
      {
         streamLogAdapter Log(hLog.addendum());
         Log.s().s() << wcnt::today() << std::endl;
         Log.s().s() << "-- word count totals --" << std::endl;
         {
            autoIndent _i(Log);
            visitor v(attr,Log);
            pRoot->acceptVisitor(v);
         }
      }

      // save the log
      {
         std::ofstream fStream(c.demand<stringSetting>("wcnt:log-path").value);
         wcr.save(fStream);
      }
   }
};

cdwExportPass(wordCountReporterPass,"",-1);
