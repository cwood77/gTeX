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
   { lexemeTableEntry::kAlphanumeric,  "_if",    lexor::kIf     },
   { lexemeTableEntry::kAlphanumeric,  "_endif", lexor::kEndIf  },
   { lexemeTableEntry::kPunctuation,  NULL }
};

const lexemeTableEntry gIfTokens[] = {
   { lexemeTableEntry::kPunctuation,  "!",       lexor::kBang    },
   { lexemeTableEntry::kPunctuation,  NULL }
};

const lexemeTableEntry gEntityTokens[] = {
   { lexemeTableEntry::kPunctuation,  "{",       lexor::kLBrace  },
   { lexemeTableEntry::kPunctuation,  "}",       lexor::kRBrace  },
   { lexemeTableEntry::kAlphanumeric, "actions", lexor::kActions },
   { lexemeTableEntry::kPunctuation,  NULL }
};

const lexemeTableEntry gParagraphStartTokens[] = {
   { lexemeTableEntry::kPunctuation,  "_$", lexor::kGoto },
   { lexemeTableEntry::kPunctuation,  "_{", lexor::kEntity },
   { lexemeTableEntry::kPunctuation,  NULL }
};

const lexemeTableEntry gParagraphEndTokens[] = {
   { lexemeTableEntry::kPunctuation,  "}", lexor::kRBrace },
   { lexemeTableEntry::kPunctuation,  ":", lexor::kColon },
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
, m_ifNode(gCommonTokens)
, m_paragraphStart(gParagraphStartTokens)
, m_paragraphEnd(gParagraphEndTokens)
, topLevel(m_topLevel,/*anyWordToken*/lexor::kWord)
, entity(m_entity,/*anyWordToken*/lexor::kWord)
, ifNode(m_ifNode,/*anyWordToken*/lexor::kWord)
, paragraphStart(m_paragraphStart,/*anyWordToken*/lexor::kWord)
, paragraphEnd(m_paragraphEnd,/*anyWordToken*/lexor::kWord)
{
   m_topLevel.add(gTopLevelTokens);
   m_entity.add(gEntityTokens);
   m_ifNode.add(gIfTokens);
}

lexor::lexor(iLexorInput& src)
: lexorBase(scanStrategies::get().topLevel,src)
{
   publishToken(kComment, "comment");
   publishToken(kEntity,  "entity");
   publishToken(kLBrace,  "left brace");
   publishToken(kRBrace,  "right brace");
   publishToken(kColon,   "colon");
   publishToken(kActions, "actions");
   publishToken(kLabel,   "label");
   publishToken(kWord,    "word");
   publishToken(kGoto,    "goto");
   publishToken(kIf,      "if");
   publishToken(kEndIf,   "end if");
   publishToken(kBang,    "exclamation point");
}

void lexor::setup(node& n)
{
   n.filePath = getFileName();
   n.lineNumber = getLineNumber();
}
