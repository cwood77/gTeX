#include "../cmn/nameBank.hpp"
#include "../cmn/node.hpp"
#include "../prattle/config.hpp"
#include "../prattle/pass.hpp"
#include <cstring>

using namespace prattle;
using namespace prattle::pass;

namespace {

class refVisitor : public gTeXVisitor {
public:
   explicit refVisitor(config& c) : m_cfg(c) {}

   ~refVisitor()
   {
      for(auto it=m_banks.begin();it!=m_banks.end();++it)
         delete it->second;
   }

   virtual void visit(labelNode& n) { visitChildren(n); }
   virtual void visit(paragraphNode& n) { visitChildren(n); }

   virtual void visit(varRefNode& n)
   {
      auto& decl = n.getRoot().demandDown<varDeclNode>(
         [&](auto&d){ return d.name == n.baseName; });

      if(decl.type.length() <= 7 && ::strncmp(decl.type.c_str(),"random<",7)!=0)
         return;

      if(!n.suffix.empty())
         throw std::runtime_error("unknown suffix on random-type variable: " + n.suffix);

      std::string& ans = m_values[n.baseName];
      if(ans.empty())
      {
         // compute this variables random assignment

         std::string varName(decl.type.c_str() + 7, decl.type.length() - 7 - 1);
         nameBank*& pBank = m_banks[varName];

         auto& sett = m_cfg.demand<stringArraySetting>(varName);
         if(pBank == NULL)
         {
            // build a name bank for this variable group
            pBank = new nameBank(sett.value.size());
            m_iters[varName] = pBank->randomIterator();
         }

         if(m_values.size() > sett.value.size())
            throw std::runtime_error("not enough values are provided for random variable: " + varName);

         ans = sett.value[m_iters[varName].get()];
         m_iters[varName].next();
      }

      auto *p = new paragraphNode();
      p->text = ans;
      p->combineWithFollowingPunct = true;
      n.replace(*p);
   }

private:
   std::map<std::string,nameBank*> m_banks;
   std::map<std::string,nameBankIter> m_iters;
   std::map<std::string,std::string> m_values;
   config& m_cfg;
};

class declVisitor : public gTeXVisitor {
public:
   virtual void visit(varDeclNode& n)
   {
      if(n.type.length() <= 7 && ::strncmp(n.type.c_str(),"random<",7)!=0)
         return;

      n.Delete();
   }
};

} // anonymous namespace

class evaluateRandomVarPass : public iPass {
public:
   void run(config& c, passLinks&, void *pIr)
   {
      auto *pRoot = reinterpret_cast<folderNode*>(pIr);

      nodeEditOperation o;
      {
         nodeEditCollector _c(o);
         refVisitor v(c);
         pRoot->acceptVisitor(v);
      }
      {
         nodeEditCollector c(o);
         declVisitor v;
         pRoot->acceptVisitor(v);
      }
      o.commit();
   }
};

cdwExportPass(evaluateRandomVarPass,"",-1);
