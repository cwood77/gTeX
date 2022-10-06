#include "lexor.hpp"

namespace {

const tokenTableEntry gCommonTokens[] = {
   { tokenTableEntry::kPunctuation,  "_#", lexor::kComment, "comment" },
   { tokenTableEntry::kPunctuation,  NULL }
};

const tokenTableEntry gTopLevelTokens[] = {
   { tokenTableEntry::kAlphanumeric, "_entity", lexor::kEntity, "entity" },
   { tokenTableEntry::kPunctuation,  "_:",      lexor::kLabel,  "label" },
   { tokenTableEntry::kPunctuation,  NULL }
};

const tokenTableEntry gEntityTokens[] = {
   { tokenTableEntry::kPunctuation,  "{",       lexor::kLBrace,  "left brace" },
   { tokenTableEntry::kPunctuation,  "}",       lexor::kRBrace,  "right brace" },
   { tokenTableEntry::kAlphanumeric, "actions", lexor::kActions, "actions" },
   { tokenTableEntry::kPunctuation,  NULL }
};

} // anonymous namespace

scanStrategies& scanStrategies::get()
{
   static scanStrategies the;
   return the;
}

scanStrategies::scanStrategies()
: m_cmn(gCommonTokens)
, m_topLevel(gCommonTokens)
, m_entity(gCommonTokens)
, topLevel(m_topLevel,/*anyWordToken*/lexor::kWord)
, entity(m_entity,/*anyWordToken*/lexor::kWord)
{
   m_topLevel.add(gTopLevelTokens);
   m_entity.add(gEntityTokens);
}

lexor::lexor(iLexorInput& src)
: lexorBase(scanStrategies::get().topLevel,src)
{
}
