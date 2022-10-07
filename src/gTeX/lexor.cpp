#include "../prattle/node.hpp"
#include "lexor.hpp"

namespace {

const lexemeTableEntry gCommonTokens[] = {
   { lexemeTableEntry::kPunctuation,  "_#", lexor::kComment },
   { lexemeTableEntry::kPunctuation,  NULL }
};

const lexemeTableEntry gTopLevelTokens[] = {
   { lexemeTableEntry::kAlphanumeric, "_entity", lexor::kEntity },
   { lexemeTableEntry::kPunctuation,  "_:",      lexor::kLabel  },
   { lexemeTableEntry::kPunctuation,  NULL }
};

const lexemeTableEntry gEntityTokens[] = {
   { lexemeTableEntry::kPunctuation,  "{",       lexor::kLBrace  },
   { lexemeTableEntry::kPunctuation,  "}",       lexor::kRBrace  },
   { lexemeTableEntry::kAlphanumeric, "actions", lexor::kActions },
   { lexemeTableEntry::kPunctuation,  NULL }
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
   publishToken(kComment, "comment");
   publishToken(kEntity,  "entity");
   publishToken(kLBrace,  "left brace");
   publishToken(kRBrace,  "right brace");
   publishToken(kActions, "actions");
   publishToken(kLabel,   "label");
   publishToken(kWord,    "word");
}

void lexor::setup(node& n)
{
   n.filePath = getFileName();
   n.lineNumber = getLineNumber();
}
