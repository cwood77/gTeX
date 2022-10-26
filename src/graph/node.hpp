#pragma once
#include "../prattle/log.hpp"
#include "../cmn/node.hpp"

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
   graphVertexNode();

   labelNode *pLabel;

   cdwImplNode(graphVertexNode,iGraphNodeVisitor);
};

class graphBuilder : public iGTeXVisitor {
public:
   graphBuilder() : pRoot(new graphRootNode()) {}

   std::unique_ptr<graphRootNode> pRoot;

   virtual void visit(node& n) { visitChildren(n); }
   virtual void visit(folderNode& n) { visitChildren(n); }
   virtual void visit(fileNode& n) { visitChildren(n); }
   virtual void visit(entityNode& n) { visitChildren(n); }
   virtual void visit(ifNode& n) { visitChildren(n); }
   virtual void visit(labelNode& n);
   virtual void visit(paragraphNode& n) { visitChildren(n); }
   virtual void visit(entityInstanceNode& n) { visitChildren(n); }
   virtual void visit(jumpNode& n);
   virtual void visit(linkTableNode& n) { visitChildren(n); }
   virtual void visit(tableNode& n) { visitChildren(n); }
   virtual void visit(declMacroNode& n) { visitChildren(n); }
   virtual void visit(callMacroNode& n) { visitChildren(n); }
   virtual void visit(varDeclNode& n) { visitChildren(n); }
   virtual void visit(varRefNode& n) { visitChildren(n); }
};

class dotWriter : public iGraphNodeVisitor {
public:
   explicit dotWriter(log::iLog& l) : m_l(l) {}

private:
   log::iLog& m_l;
};
