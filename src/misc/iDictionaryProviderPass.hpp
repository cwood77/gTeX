#pragma once
#include <istream>
#include <set>
#include <string>

// dictionaries are lists of correctly-spelled words
//
// you can have multiple dictionaries that stack (i.e. some words may be document-specific,
// like character names).
// 
// any upper-case words in a dictionary are considered case-sensitive (i.e. proper nouns).

namespace dict {

class iDictionary {
public:
   virtual ~iDictionary() {}
   virtual bool isMatch(const std::string& word) const = 0;
   virtual size_t getSize() const = 0;
};

class dictionary : public iDictionary {
public:
   virtual bool isMatch(const std::string& word) const;
   virtual size_t getSize() const { return m_words.size(); }
   void load(std::istream& i);

private:
   std::set<std::string> m_words;
};

class dictionarySorter {
public:
   typedef iDictionary *arg_t;

   bool operator()(const arg_t& lhs, const arg_t& rhs) const
   {
      return lhs < rhs;
   }
};

class compositeDictionary : public iDictionary {
public:
   virtual ~compositeDictionary();
   virtual bool isMatch(const std::string& word) const;
   virtual size_t getSize() const;
   void add(iDictionary& d) { m_parts.insert(&d); }

private:
   std::set<iDictionary*,dictionarySorter> m_parts;
};

class iDictionaryProviderPass {
public:
   virtual iDictionary& getDictionary() = 0;
};

} // namespace dict
