#include "../cmn/node.hpp"
#include "../prattle/pass.hpp"
#include <numeric>
#include <sstream>
#include <stdexcept>

using namespace prattle;
using namespace prattle::pass;

namespace {

class nameBankIter;

class nameBank {
public:
   explicit nameBank(size_t n)
   : m_lNames(n)
   { std::iota(m_lNames.begin(),m_lNames.end(),1); }

   nameBankIter randomIterator();

   std::string get(nameBankIter& i);

private:
   std::vector<unsigned long> m_lNames;
};

class nameBankIter {
public:
   explicit nameBankIter(size_t n)
   : m_n(n)
   {
      ::srand(17); // always use the same seed so runs are reproducible
      next();
   }

   unsigned long get()
   {
      if(isDone() && 0)
         throw std::runtime_error("ise nbi 40");
      return m_i;
   }

   void next()
   {
      if(isDone()) return;

      m_i = ::rand() % m_n;
      auto it=m_burned.find(m_i);
      if(it==m_burned.end())
         m_burned.insert(m_i);
      else
         next(); // keep trying
   }

   bool isDone() const { return m_n == m_burned.size(); }

private:
   size_t m_n;
   unsigned long m_i;
   std::set<unsigned long> m_burned;
};

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

} // anonymous namespace

class labelRandomizerPass : public iPass {
public:
   void run(config& c, void *pIr)
   {
      auto *pRoot = reinterpret_cast<folderNode*>(pIr);
      auto& links = pRoot->demandDown<linkTableNode>();

      // acquire the current manifest of labels
      std::map<std::string,labelNode*> oldNames;
      links.getAllLabels(oldNames);

      // generate a bank of numeric names reproducibly randomly sorted
      nameBank bank(oldNames.size());
      auto nit = bank.randomIterator();

      // build the future manifest of labels and apply it
      std::map<labelNode*,std::string> newNames;
      for(auto it=oldNames.begin();it!=oldNames.end();++it,nit.next())
         newNames[it->second] = bank.get(nit);
      links.renameLabels(newNames);
   }
};

cdwExportPass(labelRandomizerPass,"",-1);
