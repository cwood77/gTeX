#include "../cmn/node.hpp"
#include "../prattle/log.hpp"
#include "paragraphWordCountingPass.hpp"
#include <iostream>

using namespace prattle;
using namespace prattle::log;
using namespace prattle::pass;

namespace {

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

      streamLogAdapter Log(std::cout);
      Log.s().s() << "-- word count totals --" << std::endl;
      autoIndent _i(Log);

      visitor v(l.demandLink<paragraphWordCountingPass>().wCntAttr,Log);
      pRoot->acceptVisitor(v);
   }
};

cdwExportPass(wordCountReporterPass,"",-1);
