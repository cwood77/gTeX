#include "node.hpp"
#include <iostream>

void dumpVisitor::visit(node& n)
{
   std::cout << n.getName() << std::endl;
   std::cout << std::endl;
   visitChildren(n);
}

void dumpVisitor::visit(fileNode& n)
{
   std::cout << n.getName() << std::endl;
   std::cout << std::endl;
   visitChildren(n);
}

void dumpVisitor::visit(entityNode& n)
{
   std::cout << n.getName() << ": " << n.name << std::endl;
   std::cout << "   actions=";
   for(auto a : n.actions)
      std::cout << a;
   std::cout << std::endl;
   std::cout << std::endl;
   visitChildren(n);
}

void dumpVisitor::visit(labelNode& n)
{
   std::cout << n.getName() << ":" << n.label << std::endl;
   std::cout << std::endl;
   visitChildren(n);
}

void dumpVisitor::visit(paragraphNode& n)
{
   std::cout << n.getName() << std::endl;
   std::cout << "TEST<" << n.text << ">" << std::endl;
   std::cout << std::endl;
   visitChildren(n);
}
