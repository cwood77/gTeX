#pragma once
#include "../prattle/log.hpp"
#include "../prattle/node.hpp"
#include <vector>

using namespace prattle;

class folderNode;
class fileNode;
class entityNode;
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

class labelNode : public node {
public:
   std::string label;

   cdwImplNode(labelNode,iGTeXVisitor);
};

class paragraphNode : public node {
public:
   std::string text;

   cdwImplNode(paragraphNode,iGTeXVisitor);
};

class entityInstanceNode : public node {
   cdwImplNode(entityInstanceNode,iGTeXVisitor);
};

class jumpNode : public node {
   cdwImplNode(jumpNode,iGTeXVisitor);
};

class linkTableNode : public node {
   cdwImplNode(linkTableNode,iGTeXVisitor);
};

class tableNode : public node {
   cdwImplNode(tableNode,iGTeXVisitor);
};

class gTeXVisitor : public iGTeXVisitor {
public:
   virtual void visit(node& n) {}
   virtual void visit(folderNode& n) { visitChildren(n); }
   virtual void visit(fileNode& n) { visitChildren(n); }
   virtual void visit(entityNode& n) {}
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
   virtual void visit(labelNode& n);
   virtual void visit(paragraphNode& n);
   virtual void visit(entityInstanceNode& n);
   virtual void visit(jumpNode& n);
   virtual void visit(linkTableNode& n);
   virtual void visit(tableNode& n);

private:
   log::iLog& m_l;
};

// TODO the complete list of imaginable transforms
//
//                                          ----- front end
// * file finder
// * file loader
//                                          ----- middle end
// 1 paragraph disassembler
//  - location jumps
//  - entities
// entity/table generator
// 1 linker
// DOT printer
//                                          ----- 2nd middle end
// 1 label randomizer
// 1 label mover
//                                          ----- back end
// 1 jump formatter
// * label formatter
// table formatter
// entity formatter
// * entity remover
// 1 paragraph reassembler
//
// * printer
//
// 1 what I need first
// 2 what I need second
// * done
//

