#include "../cmn/node.hpp"
#include "../cmn/output.hpp"
#include "../prattle/config.hpp"
#include "../prattle/log.hpp"
#include "../prattle/pass.hpp"
#include "iGraphIrProviderPass.hpp"
#include "node.hpp"
#include <fstream>
#include <iostream>

using namespace prattle;
using namespace prattle::log;
using namespace prattle::pass;

namespace {

class visitor : public iGraphNodeVisitor {
public:
   explicit visitor(log::iLog& l) : m_l(l), m_clusterCnt(0), m_nxtAlias(0) {}

   virtual void visit(node& n) { visitChildren(n); }

   virtual void visit(graphRootNode& n)
   {
      m_l.s().s() << indent(m_l) << "digraph G {" << std::endl;
      m_l.s().s() << std::endl;

      {
         autoIndent _i(m_l);
         m_l.s().s() << indent(m_l) << "node [shape=box]" << std::endl;
         m_l.s().s() << std::endl;

         visitChildren(n);
         m_l.s().s() << std::endl;

         // now, lets do linkages
         std::vector<graphVertexNode*> verticies;
         n.searchDown<graphVertexNode>(verticies);
         for(auto *pVertex : verticies)
            drawEdge(*pVertex);
      }

      m_l.s().s() << std::endl;
      m_l.s().s() << indent(m_l) << "}" << std::endl;
   }

   virtual void visit(graphSubgraphNode& n)
   {
      m_l.s().s() << indent(m_l) << "subgraph cluster_" << (m_clusterCnt++) << " {" << std::endl;
      m_l.s().s() << std::endl;

      {
         autoIndent _i(m_l);
         m_l.s().s() << indent(m_l) << "label = \"" << prepPath(n.name) << "\"" << std::endl;
         m_l.s().s() << std::endl;

         visitChildren(n);
      }

      m_l.s().s() << std::endl;
      m_l.s().s() << indent(m_l) << "}" << std::endl;
   }

   virtual void visit(graphVertexNode& n)
   {
      auto& alias = n[m_vertexAliases];
      {
         std::stringstream stream;
         stream << "n" << (m_nxtAlias++);
         alias = stream.str();
      }

      m_l.s().s() << indent(m_l)
         << alias
         << " [label=\"" << n.origLblId << "\"";

      if(n.origLblId == "START")
         m_l.s().s() << " shape=ellipse";

      m_l.s().s() << "]"
         << std::endl;

      visitChildren(n);
   }

private:
   void drawEdge(graphVertexNode& n)
   {
      for(auto *pJmp : n.outgoing)
      {
         auto& dst = n.getRoot().demandDown<graphVertexNode>(
            [&](auto&v){ return v.pLabel->id() == pJmp->id; });
         m_l.s().s() << indent(m_l)
            << n[m_vertexAliases] << " -> "
            << dst[m_vertexAliases]
            << std::endl;
      }
   }

   std::string prepPath(const std::string& p)
   {
      std::stringstream stream;
      bool first = true;
      const char *pThumb = p.c_str();
      const char *pStart = pThumb;
      for(;*pThumb!=0;++pThumb)
      {
         if(*pThumb == '\\')
         {
            std::string word(pStart,pThumb-pStart);
            if(!first)
               stream << word;
            first = false;
            pStart = pThumb+1;
         }
      }
      std::string word(pStart,pThumb-pStart);
      stream << word;

      return stream.str();
   }

   log::iLog& m_l;
   size_t m_clusterCnt;
   attribute<std::string> m_vertexAliases;
   size_t m_nxtAlias;
};

} // anonymous namespace

class dotPrintPass : public iPass {
public:
   void run(config& c, passLinks& l, void *)
   {
      auto& ir = l.demandLink<iGraphIrProviderPass>().getGraphIr();

      auto outPath = output(c).ensurePath(
         c.demand<stringSetting>("dot:out").value + ".dot");
      std::cout << "  writing to " << outPath << std::endl;

      std::ofstream fStream(outPath);
      streamLogAdapter Log(fStream);
      visitor v(Log);
      ir.acceptVisitor(v);
   }
};

cdwExportPass(dotPrintPass,"",-1);
