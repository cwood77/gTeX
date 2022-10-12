#include "../prattle/node.hpp"
#include "lexor.hpp"

namespace {

const lexemeTableEntry gCfgToken[] = {
   { lexemeTableEntry::kPunctuation,  "#",  lexor::kComment    },
   { lexemeTableEntry::kPunctuation,  ":=", lexor::kAssignment },
   { lexemeTableEntry::kPunctuation,  "[",  lexor::kLBracket   },
   { lexemeTableEntry::kPunctuation,  "]",  lexor::kRBracket   },
   { lexemeTableEntry::kAlphanumeric, "if", lexor::kIf         },
   { lexemeTableEntry::kPunctuation,  "{",  lexor::kLBrace     },
   { lexemeTableEntry::kPunctuation,  "}",  lexor::kRBrace     },
   { lexemeTableEntry::kPunctuation,  "==", lexor::kEquOp      },
   { lexemeTableEntry::kPunctuation,  "!=", lexor::kNeqOp      },
   { lexemeTableEntry::kPunctuation,  NULL                     }
};

} // anonymous namespace

scanStrategies& scanStrategies::get()
{
   static scanStrategies the;
   return the;
}

scanStrategies::scanStrategies()
: m_cfg(gCfgToken)
, cfg(m_cfg,/*anyWordToken*/lexor::kWord)
{
}

lexor::lexor(iLexorInput& src)
: lexorBase(scanStrategies::get().cfg,src)
{
   publishToken(kComment,    "comment");
   publishToken(kWord,       "word");
   publishToken(kAssignment, "assignment");
   publishToken(kLBracket,   "left bracket");
   publishToken(kRBracket,   "right bracket");
   publishToken(kIf,         "if");
   publishToken(kLBrace,     "left brace");
   publishToken(kRBrace,     "right brace");
   publishToken(kEquOp,      "equ op");
   publishToken(kNeqOp,      "neq op");

}

void lexor::setup(node& n)
{
   n.filePath = getFileName();
   n.lineNumber = getLineNumber();
}
