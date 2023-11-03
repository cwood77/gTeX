#pragma once
#include "../cmn/node.hpp"
#include <functional>
#include <string>
#include <vector>

class iWordObserver {
public:
   virtual void onWord(const std::string& w, paragraphNode& n) = 0;
};

class wordObserver : public iWordObserver {
public:
   virtual void onWord(const std::string& w, paragraphNode& n)
   { forEach<iWordObserver>([&](auto& o){ o.onWord(w,n); }); }

   void add(iWordObserver& w) { m_obs.push_back(&w); }

   template<class T>
   void forEach(std::function<void(T&)> f)
   {
      for(auto *pWord : m_obs)
      {
         T *pObj = dynamic_cast<T*>(pWord);
         if(pObj)
            f(*pObj);
      }
   }

private:
   std::vector<iWordObserver*> m_obs;
};

class iWordObserverProviderPass {
public:
   virtual wordObserver& getObserver() = 0;
};
