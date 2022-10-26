#pragma once
#include "../prattle/log.hpp"
#include "../prattle/node.hpp"
#include <map>
#include <set>
#include <vector>

using namespace prattle;

class folderNode;
class fileNode;
class entityNode;
class ifNode;
class labelNode;
class paragraphNode;
class entityInstanceNode;
class jumpNode;
class linkTableNode;
class tableNode;

class iGTeXVisitor : public iNodeVisitor {
public:
   virtual void visit(node& n) = 0;
   virtual void visit(folderNode& n) = 0;
   virtual void visit(fileNode& n) = 0;
   virtual void visit(entityNode& n) = 0;
   virtual void visit(ifNode& n) = 0;
   virtual void visit(labelNode& n) = 0;
   virtual void visit(paragraphNode& n) = 0;
   virtual void visit(entityInstanceNode& n) = 0;
   virtual void visit(jumpNode& n) = 0;
   virtual void visit(linkTableNode& n) = 0;
   virtual void visit(tableNode& n) = 0;
};

class folderNode : public node {
   cdwImplNode(folderNode,iGTeXVisitor);
};

class fileNode : public node {
   cdwImplNode(fileNode,iGTeXVisitor);
};

class entityNode : public node {
public:
   std::string name;
   std::vector<std::string> actions;

   cdwImplNode(entityNode,iGTeXVisitor);
};

class ifNode : public node {
public:
   ifNode() : isFalse(false) {}

   std::string varName;
   bool isFalse;

   cdwImplNode(ifNode,iGTeXVisitor);
};

// could be a entity label or a jump label
// i.e.
//      _$LEFT
//        --or--
//      _$Bartender->rob
//
class labelNode : public node {
public:
   std::string label;      // only the first part if entity label
   std::string action;     // empty if not an entity label
   std::string id() const; // calculate the whole id

   cdwImplNode(labelNode,iGTeXVisitor);
};

class paragraphNode : public node {
public:
   paragraphNode() : combineWithFollowingPunct(false) {}

   std::string text;
   bool combineWithFollowingPunct;

   cdwImplNode(paragraphNode,iGTeXVisitor);
};

class entityInstanceNode : public node {
public:
   std::string type;
   std::string id; // gets edited by linker

   cdwImplNode(entityInstanceNode,iGTeXVisitor);
};

class jumpNode : public node {
public:
   std::string id; // gets edited by linker

   cdwImplNode(jumpNode,iGTeXVisitor);
};

// need label -> refer so I can number/randomize labels
class linkTableNode : public node {
public:
   void getAllLabels(std::map<std::string,labelNode*>& names);
   void renameLabels(std::map<labelNode*,std::string>& names);

   std::map<labelNode*,std::set<jumpNode*> > l2j;
   std::map<labelNode*,std::set<entityInstanceNode*> > l2ei;
   std::map<labelNode*,std::set<tableNode*> > l2t;

   cdwImplNode(linkTableNode,iGTeXVisitor);
};

class tableNode : public node {
public:
   std::string entityType;
   std::string action;
   std::map<std::string,std::string> operandsToLabels;

   cdwImplNode(tableNode,iGTeXVisitor);
};

class gTeXVisitor : public iGTeXVisitor {
public:
   virtual void visit(node& n) {}
   virtual void visit(folderNode& n) { visitChildren(n); }
   virtual void visit(fileNode& n) { visitChildren(n); }
   virtual void visit(entityNode& n) {}
   virtual void visit(ifNode& n) {}
   virtual void visit(labelNode& n) {}
   virtual void visit(paragraphNode& n) {}
   virtual void visit(entityInstanceNode& n) {}
   virtual void visit(jumpNode& n) {}
   virtual void visit(linkTableNode& n) {}
   virtual void visit(tableNode& n) {}
};

class dumpVisitor : public iGTeXVisitor {
public:
   explicit dumpVisitor(log::iLog& l) : m_l(l) {}

   virtual void visit(node& n);
   virtual void visit(folderNode& n);
   virtual void visit(fileNode& n);
   virtual void visit(entityNode& n);
   virtual void visit(ifNode& n);
   virtual void visit(labelNode& n);
   virtual void visit(paragraphNode& n);
   virtual void visit(entityInstanceNode& n);
   virtual void visit(jumpNode& n);
   virtual void visit(linkTableNode& n);
   virtual void visit(tableNode& n);

private:
   log::iLog& m_l;
};
