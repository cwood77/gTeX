#include "../prattle/config.hpp"
#include "node.hpp"

using namespace prattle;

class execVisitor : public iCfgNodeVisitor {
public:
   execVisitor(config& c) : m_c(c) {}

   virtual void visit(node& n) { visitChildren(n); }
   virtual void visit(seqNode& n) { visitChildren(n); }
   virtual void visit(setNode& n);
   virtual void visit(ifNode& n);
   virtual void visit(strLitNode& n) {}
   virtual void visit(arrayLitNode& n) {}

private:
   config& m_c;
};

class setVisitor : public iCfgNodeVisitor {
public:
   setVisitor(config& c, const std::string& name) : m_c(c), m_name(name) {}

   virtual void visit(node& n) {}
   virtual void visit(seqNode& n) {}
   virtual void visit(setNode& n) { visitChildren(n); }
   virtual void visit(ifNode& n) {}
   virtual void visit(strLitNode& n);
   virtual void visit(arrayLitNode& n);

private:
   config& m_c;
   std::string m_name;
};

class equVisitor : public iCfgNodeVisitor {
public:
   equVisitor(config& c, const std::string& name) : m_c(c), m_name(name) {}

   virtual void visit(node& n) {}
   virtual void visit(seqNode& n) {}
   virtual void visit(setNode& n) {}
   virtual void visit(ifNode& n) { visitChildren(n); }
   virtual void visit(strLitNode& n);
   virtual void visit(arrayLitNode& n);

   bool equ;

private:
   config& m_c;
   std::string m_name;
};
