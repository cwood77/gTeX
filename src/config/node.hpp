#pragma once
#include "../prattle/log.hpp"
#include "../prattle/node.hpp"
#include <vector>

using namespace prattle;

class seqNode;
class setNode;
class ifNode;
class strLitNode;
class arrayLitNode;
class boolLitNode;

class iCfgNodeVisitor : public iNodeVisitor {
public:
   virtual void visit(node& n) = 0;
   virtual void visit(seqNode& n) = 0;
   virtual void visit(setNode& n) = 0;
   virtual void visit(ifNode& n) = 0;
   virtual void visit(strLitNode& n) = 0;
   virtual void visit(arrayLitNode& n) = 0;
   virtual void visit(boolLitNode& n) = 0;
};

class seqNode : public node {
   cdwImplNode(seqNode,iCfgNodeVisitor);
};

class setNode : public node {
public:
   std::string varName;

   cdwImplNode(setNode,iCfgNodeVisitor);
};

class ifNode : public node {
public:
   std::string varName;
   std::string op;

   cdwImplNode(ifNode,iCfgNodeVisitor);
};

class strLitNode : public node {
public:
   std::string val;

   cdwImplNode(strLitNode,iCfgNodeVisitor);
};

class arrayLitNode : public node {
public:
   std::vector<std::string> vals;

   cdwImplNode(arrayLitNode,iCfgNodeVisitor);
};

class boolLitNode : public node {
public:
   bool val;

   cdwImplNode(boolLitNode,iCfgNodeVisitor);
};

class cfgDumpVisitor : public iCfgNodeVisitor {
public:
   explicit cfgDumpVisitor(log::iLog& l) : m_l(l) {}

   virtual void visit(node& n);
   virtual void visit(seqNode& n);
   virtual void visit(setNode& n);
   virtual void visit(ifNode& n);
   virtual void visit(strLitNode& n);
   virtual void visit(arrayLitNode& n);
   virtual void visit(boolLitNode& n);

private:
   log::iLog& m_l;
};
