#include "node.hpp"
#include <iostream>

using namespace prattle::log;

std::string labelNode::id() const
{
   std::string ans = label;
   if(!action.empty())
   {
      ans += "->";
      ans += action;
   }
   return ans;
}

void linkTableNode::getAllLabels(std::map<std::string,labelNode*>& names)
{
   for(auto it=l2j.begin();it!=l2j.end();++it)
      names[it->first->id()] = it->first;
   for(auto it=l2ei.begin();it!=l2ei.end();++it)
      names[it->first->id()] = it->first;
   for(auto it=l2t.begin();it!=l2t.end();++it)
      names[it->first->id()] = it->first;
}

void linkTableNode::renameLabels(std::map<labelNode*,std::string>& names)
{
   for(auto it=names.begin();it!=names.end();++it)
   {
      // update jumps
      auto& jumps = l2j[it->first];
      for(jumpNode *pJ : jumps)
         pJ->id = it->second;

      // update tables
      auto& ts = l2t[it->first];
      for(tableNode *pTable : ts)
      {
         for(auto jit=pTable->operandsToLabels.begin();
            jit!=pTable->operandsToLabels.end();++jit)
         {
            if(jit->second == it->first->id())
               jit->second = it->second;
         }
      }

      // update the label itself
      it->first->label = it->second;
      it->first->action = "";

      // no need to update the linkTable itself as it's name agnostic
   }
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
      m_l.s().s() << " " << a;
   m_l.s().s() << indent(m_l) << std::endl;
   m_l.s().s() << indent(m_l) << std::endl;
   autoIndent _i(m_l);
   visitChildren(n);
}

void dumpVisitor::visit(ifNode& n)
{
   m_l.s().s() << indent(m_l) << n.getName() << ": " << "{" << (n.isFalse ? "!" : "") << "}" << n.varName << std::endl;
   m_l.s().s() << indent(m_l) << std::endl;
   autoIndent _i(m_l);
   visitChildren(n);
}

void dumpVisitor::visit(labelNode& n)
{
   m_l.s().s() << indent(m_l) << n.getName() << ":" << n.id() << std::endl;
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

   m_l.s().s() << indent(m_l) << "jumps {" << std::endl;
   {
      autoIndent _i(m_l);
      for(auto it=n.l2j.begin();it!=n.l2j.end();++it)
         m_l.s().s() << indent(m_l) << it->first->id() << std::endl;
   }
   m_l.s().s() << indent(m_l) << "}" << std::endl;

   m_l.s().s() << indent(m_l) << "entityInstances {" << std::endl;
   {
      autoIndent _i(m_l);
      for(auto it=n.l2ei.begin();it!=n.l2ei.end();++it)
         m_l.s().s() << indent(m_l) << it->first->id() << std::endl;
   }
   m_l.s().s() << indent(m_l) << "}" << std::endl;

   m_l.s().s() << indent(m_l) << "tables {" << std::endl;
   {
      autoIndent _i(m_l);
      for(auto it=n.l2t.begin();it!=n.l2t.end();++it)
         m_l.s().s() << indent(m_l) << it->first->id() << std::endl;
   }
   m_l.s().s() << indent(m_l) << "}" << std::endl;

   m_l.s().s() << indent(m_l) << std::endl;
   autoIndent _i(m_l);
   visitChildren(n);
}

void dumpVisitor::visit(tableNode& n)
{
   m_l.s().s() << indent(m_l) << n.getName() << std::endl;
   m_l.s().s() << indent(m_l) << n.action << "; " << n.operandsToLabels.size() << " entries" << std::endl;
   {
      autoIndent _i(m_l);
      for(auto it=n.operandsToLabels.begin();it!=n.operandsToLabels.end();++it)
         m_l.s().s() << indent(m_l) << it->first << " -> " << it->second << std::endl;
   }
   m_l.s().s() << indent(m_l) << std::endl;
   autoIndent _i(m_l);
   visitChildren(n);
}

void dumpVisitor::visit(declMacroNode& n)
{
   m_l.s().s() << indent(m_l) << n.getName() << std::endl;
   m_l.s().s() << indent(m_l) << n.name << "; " << n.args.size() << " args" << std::endl;
   {
      autoIndent _i(m_l);
      for(auto it=n.args.begin();it!=n.args.end();++it)
         m_l.s().s() << indent(m_l) << *it << std::endl;
   }
   m_l.s().s() << indent(m_l) << std::endl;
   autoIndent _i(m_l);
   visitChildren(n);
}

void dumpVisitor::visit(callMacroNode& n)
{
   m_l.s().s() << indent(m_l) << n.getName() << std::endl;
   m_l.s().s() << indent(m_l) << n.name << "; " << n.args.size() << " args" << std::endl;
   {
      autoIndent _i(m_l);
      for(auto it=n.args.begin();it!=n.args.end();++it)
         m_l.s().s() << indent(m_l) << *it << std::endl;
   }
   m_l.s().s() << indent(m_l) << std::endl;
   autoIndent _i(m_l);
   visitChildren(n);
}
