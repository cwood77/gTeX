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
   { lexemeTableEntry::kPunctuation,  "\"", lexor::kQuote      },
   { lexemeTableEntry::kPunctuation,  NULL                     }
};

} // anonymous namespace

void quotedTextScanStrategy::scan(kernel& k) const
{
   if(k.token != lexorBase::kUnknown)
      return;

   const char *pStart = k.pThumb;
   for(;*k.pThumb!='"'&&*k.pThumb!=0;++k.pThumb);
   k.token = m_token;
   k.lexeme = std::string(pStart,k.pThumb-pStart);
}

scanStrategies& scanStrategies::get()
{
   static scanStrategies the;
   return the;
}

scanStrategies::scanStrategies()
: m_cfg(gCfgToken)
, cfg(m_cfg,/*anyWordToken*/lexor::kWord)
, quoted(lexor::kWord)
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
   publishToken(kQuote,      "double quote");
}

void lexor::setup(node& n)
{
   n.filePath = getFileName();
   n.lineNumber = getLineNumber();
}
