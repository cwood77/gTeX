#include "../prattle/node.hpp"
#include "iStyler.hpp"

class stylingVisitorBase {
public:
   explicit stylingVisitorBase(iStyler& s) : m_styler(s) {}

   void replaceObjects();

protected:
   iStyler& m_styler;
   std::map<prattle::node*,std::string> m_ans;
};
