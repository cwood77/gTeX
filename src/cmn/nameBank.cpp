#include "nameBank.hpp"
#include <numeric>
#include <sstream>
#include <stdexcept>

nameBank::nameBank(size_t n)
: m_lNames(n)
{
   std::iota(m_lNames.begin(),m_lNames.end(),1);
}

nameBankIter nameBank::randomIterator()
{
   return nameBankIter(m_lNames.size());
}

std::string nameBank::get(nameBankIter& i)
{
   std::stringstream stream;
   stream << m_lNames[i.get()];
   return stream.str();
}

nameBankIter::nameBankIter(size_t n)
: m_n(n)
{
   ::srand(17); // always use the same seed so runs are reproducible
   next();
}

unsigned long nameBankIter::get()
{
   if(isDone() && 0)
      throw std::runtime_error("ise nbi 40");
   return m_i;
}

void nameBankIter::next()
{
   if(isDone()) return;

   m_i = ::rand() % m_n;
   auto it=m_burned.find(m_i);
   if(it==m_burned.end())
      m_burned.insert(m_i);
   else
      next(); // keep trying
}
