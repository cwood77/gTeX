#include "../cmn/node.hpp"
#include "paragraphWordCountingPass.hpp"

using namespace prattle;
using namespace prattle::pass;

namespace {

class visitor : public gTeXVisitor {
public:
   explicit visitor(attribute<size_t>& attr) : m_attr(attr) {}

   virtual void visit(folderNode& n)
   {
      m_folderTot = 0;
      visitChildren(n);
      n[m_attr] = m_folderTot;
   }

   virtual void visit(fileNode& n)
   {
      m_fileTot = 0;
      visitChildren(n);
      n[m_attr] = m_fileTot;
      m_folderTot += m_fileTot;
   }

   virtual void visit(labelNode& n) { visitChildren(n); }

   virtual void visit(paragraphNode& n)
   {
      m_fileTot += n[m_attr];
   }

private:
   attribute<size_t>& m_attr;
   size_t m_folderTot;
   size_t m_fileTot;
};

} // anonymous namespace

class wordCountRollupPass : public iPass {
public:
   void run(config& c, passLinks& l, void *pIr)
   {
      auto *pRoot = reinterpret_cast<folderNode*>(pIr);

      visitor v(l.demandLink<paragraphWordCountingPass>().wCntAttr);
      pRoot->acceptVisitor(v);
   }
};

cdwExportPass(wordCountRollupPass,"",-1);
