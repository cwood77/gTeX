#pragma once
#include "../prattle/config.hpp"

using namespace prattle;

class output {
public:
   explicit output(config& c) : m_c(c) {}

   std::string ensurePath(const std::string& path);

private:
   void makeIf(const std::string& path);

   config& m_c;
};
