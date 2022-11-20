#include "../cmn/node.hpp"
#include "../cmn/strUtil.hpp"
#include "../prattle/config.hpp"
#include "../prattle/pass.hpp"

using namespace prattle;
using namespace prattle::pass;

namespace {

class visitor : public gTeXVisitor {
public:
   explicit visitor(const std::string& pattern) : m_pattern(pattern) {}

   virtual void visit(labelNode& n)
   {
      // 'flatten' my children under me
      n.reparentChildren(n.demandParent(),&n);

      if(n.id() == "0")
      {
         // the 'start' label gets no formatting
         n.Delete();
      }
      else
      {
         std::string text = m_pattern;
         strUtil::expandVars("{label}",n.id(),text);

         auto *p = new paragraphNode();
         p->text = text;

         // replace myself with a paragraph
         n.replace(*p);
      }
   }

private:
   std::string m_pattern;
};

} // anonymous namespace

class htmlLabelFormatterPass : public iPass {
public:
   void run(config& c, passLinks&, void *pIr)
   {
      auto& pattern = c.demand<stringSetting>("html:label-fmt").value;
      auto *pRoot = reinterpret_cast<folderNode*>(pIr);

      nodeEditOperation o;
      {
         nodeEditCollector c(o);
         visitor v(pattern);
         pRoot->acceptVisitor(v);
      }
      o.commit();
   }
};

cdwExportPass(htmlLabelFormatterPass,"",-1);
