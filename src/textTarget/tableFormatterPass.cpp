#include "../cmn/node.hpp"
#include "../prattle/pass.hpp"

using namespace prattle;
using namespace prattle::pass;

namespace {

class visitor : public gTeXVisitor {
public:
   virtual void visit(tableNode& n)
   {
      std::stringstream stream;
      stream << "----------------------------------------------" << std::endl;
      stream << n.action << " (" << n.entityType << ")" << std::endl;
      stream << std::endl;
      for(auto it=n.operandsToLabels.begin();it!=n.operandsToLabels.end();++it)
         stream << it->first <<  " -> " << it->second << std::endl;

      auto *p = new paragraphNode();
      p->text = stream.str();
      n.replace(*p);
   }
};

} // anonymous namespace

class tableFormatterPass : public iPass {
public:
   void run(config& c, passLinks&, void *pIr)
   {
      auto *pRoot = reinterpret_cast<folderNode*>(pIr);

      nodeEditOperation o;
      {
         nodeEditCollector c(o);
         visitor v;
         pRoot->acceptVisitor(v);
      }
      o.commit();
   }
};

cdwExportPass(tableFormatterPass,"",-1);
