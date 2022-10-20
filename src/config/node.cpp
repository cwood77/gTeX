#include "node.hpp"

using namespace prattle::log;

void cfgDumpVisitor::visit(node& n)
{
   m_l.s().s() << indent(m_l) << n.getName() << std::endl;
   m_l.s().s() << indent(m_l) << std::endl;
   autoIndent _i(m_l);
   visitChildren(n);
}

void cfgDumpVisitor::visit(seqNode& n)
{
   m_l.s().s() << indent(m_l) << n.getName() << std::endl;
   m_l.s().s() << indent(m_l) << std::endl;
   autoIndent _i(m_l);
   visitChildren(n);
}

void cfgDumpVisitor::visit(setNode& n)
{
   m_l.s().s() << indent(m_l) << n.getName() << ":" << n.varName << std::endl;
   m_l.s().s() << indent(m_l) << std::endl;
   autoIndent _i(m_l);
   visitChildren(n);
}

void cfgDumpVisitor::visit(ifNode& n)
{
   m_l.s().s() << indent(m_l) << n.getName() << ":" << n.varName << " " << n.op << std::endl;
   m_l.s().s() << indent(m_l) << std::endl;
   autoIndent _i(m_l);
   visitChildren(n);
}

void cfgDumpVisitor::visit(strLitNode& n)
{
   m_l.s().s() << indent(m_l) << n.getName() << ":" << n.val << std::endl;
   m_l.s().s() << indent(m_l) << std::endl;
   autoIndent _i(m_l);
   visitChildren(n);
}

void cfgDumpVisitor::visit(arrayLitNode& n)
{
   m_l.s().s() << indent(m_l) << n.getName() << std::endl;
   m_l.s().s() << indent(m_l) << "[";
   for(auto& v : n.vals)
      m_l.s().s() << " " << v;
   m_l.s().s() << " ]" << std::endl;
   m_l.s().s() << indent(m_l) << std::endl;
   autoIndent _i(m_l);
   visitChildren(n);
}

void cfgDumpVisitor::visit(boolLitNode& n)
{
   m_l.s().s() << indent(m_l) << n.getName() << ":" << n.val << std::endl;
   m_l.s().s() << indent(m_l) << std::endl;
   autoIndent _i(m_l);
   visitChildren(n);
}
