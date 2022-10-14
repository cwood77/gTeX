#pragma once
#include <list>
#include <map>
#include <string>
#include <sstream>
#include <ostream>
#include <istream>

namespace wcnt {

class today {
public:
   today() : now(::time(NULL)) {}

   int day() const;
   int month() const;
   int year() const;

   time_t now;
};

class iReportSection {
public:
   virtual bool tryEatLine(const std::string& line) = 0;
   virtual void write(std::ostream& o) const = 0;
};

class dateSummaryTable : public iReportSection {
public:
   dateSummaryTable() : m_reading(false) {}

   virtual bool tryEatLine(const std::string& line);
   virtual void write(std::ostream& o) const;

   std::map<std::string,long> total;

private:
   bool m_reading;
};

class monthlyStats : public iReportSection {
public:
   monthlyStats() : m_reading(false) {}

   virtual bool tryEatLine(const std::string& line);
   virtual void write(std::ostream& o) const;

   size_t monthlyGoal;
   int month;
   int daysInMonth;

   size_t wordsLeft;

   void updateFrom(const dateSummaryTable& d);

private:
   bool m_reading;

   size_t m_daysWritten;
};

class historicalLog : public iReportSection {
public:
   virtual bool tryEatLine(const std::string& line);
   virtual void write(std::ostream& o) const;

   std::ostream& addendum() { return m_addendum; }

private:
   std::list<std::string> m_existing;
   std::stringstream m_addendum;
};

class wordCountReport {
public:
   std::list<iReportSection*> sections;

   void load(std::istream& i);
   void save(std::ostream& o);

private:
};

} // namespace wcnt

std::ostream& operator<<(std::ostream& o, const wcnt::today& t);
