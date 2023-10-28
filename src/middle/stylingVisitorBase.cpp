#include "../cmn/node.hpp"
#include "stylingVisitorBase.hpp"

void stylingVisitorBase::replaceObjects()
{
   for(auto it=m_ans.begin();it!=m_ans.end();++it)
   {
      auto *pVarRef = it->first;

      auto& sBeginNode = pVarRef->demandParent().appendChild<stylingNode>();
      sBeginNode.style = it->second;

      auto& sEndNode = pVarRef->demandParent().appendChild<stylingNode>();
      sEndNode.style = it->second;

      auto& nodes = pVarRef->demandParent().getChildren();
      auto vrIdx = pVarRef->demandParent().getIndexOfChild(*pVarRef);
      auto jit = nodes.begin();
      nodes.insert(std::next(jit,vrIdx),&sBeginNode);
      jit = nodes.begin();
      nodes.insert(std::next(jit,vrIdx+1+1),&sEndNode);
      nodes.resize(nodes.size()-2);
   }
}
