#pragma once
#include "../prattle/node.hpp"
#include <vector>

using namespace prattle;

class fileNode;
class entityNode;
class labelNode;
class paragraphNode;

class iGTeXVisitor : public iNodeVisitor {
public:
   virtual void visit(node& n) = 0;
   virtual void visit(fileNode& n) = 0;
   virtual void visit(entityNode& n) = 0;
   virtual void visit(labelNode& n) = 0;
   virtual void visit(paragraphNode& n) = 0;
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

class dumpVisitor : public iGTeXVisitor {
public:
   virtual void visit(node& n);
   virtual void visit(fileNode& n);
   virtual void visit(entityNode& n);
   virtual void visit(labelNode& n);
   virtual void visit(paragraphNode& n);
};

// TODO the complete list of imaginable transforms
//
//                                          ----- front end
// 2 file finder
// * file loader
//                                          ----- middle end
// paragraph disassembler
//  - location jumps
//  - entities
// entity/table generator
// linker
// DOT printer
//                                          ----- 2nd middle end
// label randomizer
// label mover
//                                          ----- back end
// jump formatter
// 1 label formatter
// table formatter
// entity formatter
// 1 entity remover
// paragraph reassembler
//
// 1 printer
//
// 1 what I need first
// 2 what I need second
// * done
//
