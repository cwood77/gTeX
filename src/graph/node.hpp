#pragma once
#include "../cmn/node.hpp"
#include "../prattle/log.hpp"
#include <set>

class graphRootNode;
class graphSubgraphNode;
class graphVertexNode;

class iGraphNodeVisitor : public iNodeVisitor {
public:
   virtual void visit(node& n) = 0;
   virtual void visit(graphRootNode& n) = 0;
   virtual void visit(graphSubgraphNode& n) = 0;
   virtual void visit(graphVertexNode& n) = 0;
};

class graphRootNode : public node {
   cdwImplNode(graphRootNode,iGraphNodeVisitor);
};

class graphSubgraphNode : public node {
public:
   std::string name;

   cdwImplNode(graphSubgraphNode,iGraphNodeVisitor);
};

class graphVertexNode : public node {
public:
   graphVertexNode() : pLabel(NULL), isMergeLabel(false) {}

   labelNode *pLabel;
   std::string origLblId;
   std::string origLblFile;
   bool isMergeLabel;

   std::set<jumpNode*> outgoing;
   std::set<std::string> origMergeLblIds;

   cdwImplNode(graphVertexNode,iGraphNodeVisitor);
};
