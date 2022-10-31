#pragma once
#include "../prattle/log.hpp"
#include "../prattle/node.hpp"
#include <map>
#include <memory>
#include <set>
#include <vector>

using namespace prattle;

class folderNode;
class fileNode;
class entityNode;
class ifNode;
class labelNode;
class inlineLabelNode;
class paragraphNode;
class entityInstanceNode;
class jumpNode;
class linkTableNode;
class tableNode;
class declMacroNode;
class callMacroNode;
class varDeclNode;
class varRefNode;
class attachActionNode;

class iGTeXVisitor : public iNodeVisitor {
public:
   virtual void visit(node& n) = 0;
   virtual void visit(folderNode& n) = 0;
   virtual void visit(fileNode& n) = 0;
   virtual void visit(entityNode& n) = 0;
   virtual void visit(ifNode& n) = 0;
   virtual void visit(labelNode& n) = 0;
   virtual void visit(inlineLabelNode& n) = 0;
   virtual void visit(paragraphNode& n) = 0;
   virtual void visit(entityInstanceNode& n) = 0;
   virtual void visit(jumpNode& n) = 0;
   virtual void visit(linkTableNode& n) = 0;
   virtual void visit(tableNode& n) = 0;
   virtual void visit(declMacroNode& n) = 0;
   virtual void visit(callMacroNode& n) = 0;
   virtual void visit(varDeclNode& n) = 0;
   virtual void visit(varRefNode& n) = 0;
   virtual void visit(attachActionNode& n) = 0;

protected:
   template<class T>
   void unimplemented(T& n) { _unimplemented(typeid(T).name()); }

private:
   void _unimplemented(const std::string& nodeType) const;
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

class inlineLabelNode : public node {
public:
   std::string label;

   cdwImplNode(inlineLabelNode,iGTeXVisitor);
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
   jumpNode() : markedForMerge(false) {}

   std::string id; // gets edited by linker

   bool markedForMerge;

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

class declMacroNode : public node {
public:
   std::string name;
   std::vector<std::string> args;
   std::string body;

   cdwImplNode(declMacroNode,iGTeXVisitor);
};

class callMacroNode : public node {
public:
   std::string name;
   std::vector<std::string> args;

   cdwImplNode(callMacroNode,iGTeXVisitor);
};

class varDeclNode : public node {
public:
   std::string name;
   std::string type;

   cdwImplNode(varDeclNode,iGTeXVisitor);
};

class varRefNode : public node {
public:
   std::string baseName;
   std::string suffix;

   cdwImplNode(varRefNode,iGTeXVisitor);
};

class attachActionNode : public node {
public:
   std::string actionPrefix;
   std::string action;
   std::string entityType;

   cdwImplNode(attachActionNode,iGTeXVisitor);
};

class gTeXVisitor : public iGTeXVisitor {
public:
   virtual void visit(node& n) {}
   virtual void visit(folderNode& n) { visitChildren(n); }
   virtual void visit(fileNode& n) { visitChildren(n); }
   virtual void visit(entityNode& n) {}
   virtual void visit(ifNode& n) {}
   virtual void visit(labelNode& n) {}
   virtual void visit(inlineLabelNode& n) {}
   virtual void visit(paragraphNode& n) {}
   virtual void visit(entityInstanceNode& n) {}
   virtual void visit(jumpNode& n) {}
   virtual void visit(linkTableNode& n) {}
   virtual void visit(tableNode& n) {}
   virtual void visit(declMacroNode& n) {}
   virtual void visit(callMacroNode& n) {}
   virtual void visit(varDeclNode& n) {}
   virtual void visit(varRefNode& n) {}
   virtual void visit(attachActionNode& n) {}
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
   virtual void visit(inlineLabelNode& n);
   virtual void visit(paragraphNode& n);
   virtual void visit(entityInstanceNode& n);
   virtual void visit(jumpNode& n);
   virtual void visit(linkTableNode& n);
   virtual void visit(tableNode& n);
   virtual void visit(declMacroNode& n);
   virtual void visit(callMacroNode& n);
   virtual void visit(varDeclNode& n);
   virtual void visit(varRefNode& n);
   virtual void visit(attachActionNode& n);

private:
   log::iLog& m_l;
};

class fieldCopyingNodeVisitor : public iGTeXVisitor {
public:
   explicit fieldCopyingNodeVisitor(node& src) : m_src(src) {}

   virtual void visit(node& n) {}
   virtual void visit(folderNode& n) { unimplemented(n); }
   virtual void visit(fileNode& n) { unimplemented(n); }
   virtual void visit(entityNode& n) { unimplemented(n); }
   virtual void visit(ifNode& n) { unimplemented(n); }
   virtual void visit(labelNode& n) { unimplemented(n); }
   virtual void visit(inlineLabelNode& n) { unimplemented(n); }
   virtual void visit(paragraphNode& n);
   virtual void visit(entityInstanceNode& n) { unimplemented(n); }
   virtual void visit(jumpNode& n) { unimplemented(n); }
   virtual void visit(linkTableNode& n) { unimplemented(n); }
   virtual void visit(tableNode& n) { unimplemented(n); }
   virtual void visit(declMacroNode& n) { unimplemented(n); }
   virtual void visit(callMacroNode& n) { unimplemented(n); }
   virtual void visit(varDeclNode& n) { unimplemented(n); }
   virtual void visit(varRefNode& n);
   virtual void visit(attachActionNode& n) { unimplemented(n); }

private:
   node& m_src;
};

class treeCloningVisitor : public iGTeXVisitor {
public:
   std::unique_ptr<node> pClone;

   virtual void visit(node& n) { cloneNodeWithDefaultCTor<node>(n); }
   virtual void visit(folderNode& n) { cloneNodeWithDefaultCTor<folderNode>(n); }
   virtual void visit(fileNode& n) { cloneNodeWithDefaultCTor<fileNode>(n); }
   virtual void visit(entityNode& n) { cloneNodeWithDefaultCTor<entityNode>(n); }
   virtual void visit(ifNode& n) { cloneNodeWithDefaultCTor<ifNode>(n); }
   virtual void visit(labelNode& n) { cloneNodeWithDefaultCTor<labelNode>(n); }
   virtual void visit(inlineLabelNode& n) { cloneNodeWithDefaultCTor<inlineLabelNode>(n); }
   virtual void visit(paragraphNode& n) { cloneNodeWithDefaultCTor<paragraphNode>(n); }
   virtual void visit(entityInstanceNode& n) { cloneNodeWithDefaultCTor<entityInstanceNode>(n); }
   virtual void visit(jumpNode& n) { cloneNodeWithDefaultCTor<jumpNode>(n); }
   virtual void visit(linkTableNode& n) { cloneNodeWithDefaultCTor<linkTableNode>(n); }
   virtual void visit(tableNode& n) { cloneNodeWithDefaultCTor<tableNode>(n); }
   virtual void visit(declMacroNode& n) { cloneNodeWithDefaultCTor<declMacroNode>(n); }
   virtual void visit(callMacroNode& n) { cloneNodeWithDefaultCTor<callMacroNode>(n); }
   virtual void visit(varDeclNode& n) { cloneNodeWithDefaultCTor<varDeclNode>(n); }
   virtual void visit(varRefNode& n) { cloneNodeWithDefaultCTor<varRefNode>(n); }
   virtual void visit(attachActionNode& n) { cloneNodeWithDefaultCTor<attachActionNode>(n); }

private:
   template<class T>
   void cloneNodeWithDefaultCTor(T& n)
   {
      pClone.reset(new T());
      postClone(n);
   }

   void postClone(node& src);
};

// TODO: what's left
// ok attachment table formatting polish
// - DOT barfs on attachments
// - attachment jump formatting polish
// - entity instances should be repeatable without bumping the IDs
