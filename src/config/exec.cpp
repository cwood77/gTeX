#include "exec.hpp"

void execVisitor::visit(setNode& n)
{
   setVisitor v(m_c,n.varName);
   n.acceptVisitor(v);
}

void execVisitor::visit(ifNode& n)
{
   equVisitor v(m_c,n.varName);
   n.acceptVisitor(v);

   if((n.op == "==" && v.equ) || (n.op == "!=" && !v.equ))
   {
      std::vector<seqNode*> seqs;
      n.searchDown<seqNode>(seqs);
      seqs[0]->acceptVisitor(*this);
   }
}

void setVisitor::visit(strLitNode& n)
{
   m_c.createOrFetch<stringSetting>(m_name).value = n.val;
}

void setVisitor::visit(arrayLitNode& n)
{
   m_c.createOrFetch<stringArraySetting>(m_name).value = n.vals;
}

void setVisitor::visit(boolLitNode& n)
{
   m_c.createOrFetch<boolSetting>(m_name).value = n.val;
}

void equVisitor::visit(strLitNode& n)
{
   equ = (m_c.demand<stringSetting>(m_name).value == n.val);
}

void equVisitor::visit(arrayLitNode& n)
{
   equ = (m_c.demand<stringArraySetting>(m_name).value == n.vals);
}

void equVisitor::visit(boolLitNode& n)
{
   equ = (m_c.demand<boolSetting>(m_name).value == n.val);
}
