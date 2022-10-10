#pragma once
#include <set>
#include <string>
#include <vector>

class nameBankIter;

class nameBank {
public:
   explicit nameBank(size_t n);

   nameBankIter randomIterator();

   std::string get(nameBankIter& i);

private:
   std::vector<unsigned long> m_lNames;
};

class nameBankIter {
public:
   explicit nameBankIter(size_t n);

   unsigned long get();

   void next();

   bool isDone() const { return m_n == m_burned.size(); }

private:
   size_t m_n;
   unsigned long m_i;
   std::set<unsigned long> m_burned;
};
