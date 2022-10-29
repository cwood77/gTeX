#include "../prattle/node.hpp"
#include "lexor.hpp"

namespace {

const lexemeTableEntry gCommonTokens[] = {
   { lexemeTableEntry::kPunctuation,  "_#", lexor::kComment },
   { lexemeTableEntry::kPunctuation,  NULL }
};

const lexemeTableEntry gTopLevelTokens[] = {
   { lexemeTableEntry::kAlphanumeric, "_entity", lexor::kEntity       },
   { lexemeTableEntry::kAlphanumeric, "_macro",  lexor::kMacro        },
   { lexemeTableEntry::kPunctuation,  "_:",      lexor::kLabel        },
   { lexemeTableEntry::kPunctuation,  "_-",      lexor::kInlineLabel  },
   { lexemeTableEntry::kAlphanumeric,  "_if",    lexor::kIf           },
   { lexemeTableEntry::kAlphanumeric,  "_endif", lexor::kEndIf        },
   { lexemeTableEntry::kAlphanumeric,  "_var",   lexor::kVar          },
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
   { lexemeTableEntry::kPunctuation,  "_$",      lexor::kGoto   },
   { lexemeTableEntry::kPunctuation,  "_<",      lexor::kMerge  },
   { lexemeTableEntry::kPunctuation,  "_{",      lexor::kEntity },
   { lexemeTableEntry::kAlphanumeric,  "_call",  lexor::kCall   },
   { lexemeTableEntry::kPunctuation,  "_@",      lexor::kAt     },
   { lexemeTableEntry::kPunctuation,  NULL }
};

const lexemeTableEntry gParagraphEndTokens[] = {
   { lexemeTableEntry::kPunctuation,  "}", lexor::kRBrace },
   { lexemeTableEntry::kPunctuation,  ":", lexor::kColon },
   { lexemeTableEntry::kPunctuation,  NULL }
};

const lexemeTableEntry gMacroTokens[] = {
   { lexemeTableEntry::kPunctuation,  "{", lexor::kLBrace },
   { lexemeTableEntry::kPunctuation,  "}", lexor::kRBrace },
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
, m_macro(gMacroTokens)
, topLevel(m_topLevel,/*anyWordToken*/lexor::kWord)
, entity(m_entity,/*anyWordToken*/lexor::kWord)
, ifNode(m_ifNode,/*anyWordToken*/lexor::kWord)
, paragraphStart(m_paragraphStart,/*anyWordToken*/lexor::kWord)
, paragraphEnd(m_paragraphEnd,/*anyWordToken*/lexor::kWord)
, macro(m_macro,/*anyWordToken*/lexor::kWord)
{
   m_topLevel.add(gTopLevelTokens);
   m_entity.add(gEntityTokens);
   m_ifNode.add(gIfTokens);
}

void lexor::setup(node& n)
{
   n.filePath = getFileName();
   n.lineNumber = getLineNumber();
}

void lexor::publishTokens()
{
   publishToken(kComment,     "comment");

   publishToken(kEntity,      "entity");
   publishToken(kMacro,       "macro declaration");
   publishToken(kCall,        "macro call");
   publishToken(kVar,         "variable declaration");

   publishToken(kLBrace,      "left brace");
   publishToken(kRBrace,      "right brace");
   publishToken(kColon,       "colon");
   publishToken(kBang,        "exclamation point");
   publishToken(kAt,          "at symbol");

   publishToken(kActions,     "actions");

   publishToken(kLabel,       "label");
   publishToken(kInlineLabel, "inline label");
   publishToken(kWord,        "word");
   publishToken(kGoto,        "goto");
   publishToken(kMerge,       "merge goto");

   publishToken(kIf,          "if");
   publishToken(kEndIf,       "end if");
}
