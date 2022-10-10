#include "node.hpp"
#include <iostream>

using namespace prattle::log;

std::string labelNode::id()
{
   std::string ans = label;
   if(!action.empty())
   {
      ans += "->";
      ans += action;
   }
   return ans;
}

void dumpVisitor::visit(node& n)
{
   m_l.s().s() << indent(m_l) << n.getName() << std::endl;
   m_l.s().s() << indent(m_l) << std::endl;
   autoIndent _i(m_l);
   visitChildren(n);
}

void dumpVisitor::visit(folderNode& n)
{
   m_l.s().s() << indent(m_l) << n.getName() << std::endl;
   m_l.s().s() << indent(m_l) << std::endl;
   autoIndent _i(m_l);
   visitChildren(n);
}

void dumpVisitor::visit(fileNode& n)
{
   m_l.s().s() << indent(m_l) << n.getName() << std::endl;
   m_l.s().s() << indent(m_l) << std::endl;
   autoIndent _i(m_l);
   visitChildren(n);
}

void dumpVisitor::visit(entityNode& n)
{
   m_l.s().s() << indent(m_l) << n.getName() << ": " << n.name << std::endl;
   m_l.s().s() << indent(m_l) << "   actions=";
   for(auto a : n.actions)
      m_l.s().s() << indent(m_l) << a;
   m_l.s().s() << indent(m_l) << std::endl;
   m_l.s().s() << indent(m_l) << std::endl;
   autoIndent _i(m_l);
   visitChildren(n);
}

void dumpVisitor::visit(labelNode& n)
{
   m_l.s().s() << indent(m_l) << n.getName() << ":" << n.label << std::endl;
   m_l.s().s() << indent(m_l) << std::endl;
   autoIndent _i(m_l);
   visitChildren(n);
}

void dumpVisitor::visit(paragraphNode& n)
{
   m_l.s().s() << indent(m_l) << n.getName() << std::endl;
   m_l.s().s() << indent(m_l) << "TEST<" << n.text << ">" << std::endl;
   m_l.s().s() << indent(m_l) << std::endl;
   autoIndent _i(m_l);
   visitChildren(n);
}

void dumpVisitor::visit(entityInstanceNode& n)
{
   m_l.s().s() << indent(m_l) << n.getName() << ":{" << n.type << ":" << n.id << "}" << std::endl;
   m_l.s().s() << indent(m_l) << std::endl;
   autoIndent _i(m_l);
   visitChildren(n);
}

void dumpVisitor::visit(jumpNode& n)
{
   m_l.s().s() << indent(m_l) << n.getName() << ":" << n.id << std::endl;
   m_l.s().s() << indent(m_l) << std::endl;
   autoIndent _i(m_l);
   visitChildren(n);
}

void dumpVisitor::visit(linkTableNode& n)
{
   m_l.s().s() << indent(m_l) << n.getName() << std::endl;
   m_l.s().s() << indent(m_l) << std::endl;
   autoIndent _i(m_l);
   visitChildren(n);
}

void dumpVisitor::visit(tableNode& n)
{
   m_l.s().s() << indent(m_l) << n.getName() << std::endl;
   m_l.s().s() << indent(m_l) << std::endl;
   autoIndent _i(m_l);
   visitChildren(n);
}
