#include "../../prattle/pass.hpp"
#include "../node.hpp"

using namespace prattle;
using namespace prattle::pass;

namespace {

class visitor : public gTeXVisitor {
public:
   virtual void visit(entityNode& n)
   {
      n.Delete();
   }
};

} // anonymous namespace

class entityRemovalPass : public iPass {
public:
   void run(config& c, void *pIr)
   {
      fileNode *pRoot = reinterpret_cast<fileNode*>(pIr);

      nodeEditOperation o;
      {
         nodeEditCollector c(o);
         visitor v;
         pRoot->acceptVisitor(v);
      }
      o.commit();
   }
};

autoPassInfo<entityRemovalPass> gEntityRemovalPass("",-1);
const char *kEntityRemovalPassName = typeid(entityRemovalPass).name();
