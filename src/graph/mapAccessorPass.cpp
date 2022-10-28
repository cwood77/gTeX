#include "../cmn/node.hpp"
#include "../prattle/config.hpp"
#include "../prattle/log.hpp"
#include "../prattle/pass.hpp"
#include "iGraphIrProviderPass.hpp"
#include "iMapAccessorPass.hpp"
#include "node.hpp"

using namespace prattle;
using namespace prattle::log;
using namespace prattle::pass;

namespace {

class visitor : public iGraphNodeVisitor {
public:
   explicit visitor(log::iLog& l) : m_l(l) {}

   virtual void visit(node& n) { visitChildren(n); }

   virtual void visit(graphRootNode& n)
   {
   }

   virtual void visit(graphSubgraphNode& n)
   {
   }

   virtual void visit(graphVertexNode& n)
   {
   }

private:
   log::iLog& m_l;
};

} // anonymous namespace

class mapAccessorPass : public iPass, public iMapAccessorPass {
public:
   void run(config& c, passLinks& l, void *)
   {
      auto& root = l.demandLink<iGraphIrProviderPass>().getGraphIr();

      auto& mapNames = c.demand<stringArraySetting>("maps");
      for(auto mapName : mapNames.value)
      {
         //auto& config = c.demand<stringArraySetting>(mapName + ":mcfg");

         std::stringstream stream;
         streamLogAdapter Log(stream);
         visitor v(Log);
         root.acceptVisitor(v);

         m_maps[mapName] = stream.str();
      }
   }

   std::string getMap(const std::string& id) { return m_maps[id]; }

private:
   std::map<std::string,std::string> m_maps;
};

cdwExportPass(mapAccessorPass,"",-1);
