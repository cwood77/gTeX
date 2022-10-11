#include "wordCountReport.hpp"

namespace wcnt {

int today::month() const
{
}

bool dateSummaryTable::tryEatLine(const std::string& line)
{
   if(!m_reading)
   {
      if(line == "by date")
      {
         m_reading = true;
         return true;
      }
      return false;
   }
   else
   {
      if(line.empty())
         m_reading = false;
      else
      {
         char key[1024];
         long value = 0;
         ::sscanf(line.c_str(),"%[^ ] = %ld",key,&value);
         total[key] = value;
      }

      return true;
   }
}

void dateSummaryTable::write(std::ostream& o) const
{
   o << "by date" << std::endl;
   long last = 0;
   for(auto it=total.begin();it!=total.end();++it)
   {
      o << it->first << " = " << it->second;
      long delta = it->second - last;
      last = it->second;
      o << " " << (delta > 0 ? "+" : "") << delta << std::endl;
   }
   o << std::endl;
}

bool monthlyStats::tryEatLine(const std::string& line)
{
   if(!m_reading)
   {
      if(line == "monthly goals")
      {
         m_reading = true;
         return true;
      }
      return false;
   }
   else
   {
      if(line.empty())
         m_reading = false;
      else
         ; // this section doesn't store anything and recreates everything
           // upon write

      return true;
   }
}

void monthlyStats::write(std::ostream& o) const
{
   if(month == 0)
      return; // disabled

   //if(today().month == 

   // NaNoWriMo stats
   // monthly goal: 30000
   // days left in November: 30
   // words left: 30000
   // words left / day left: 1000
   // days written: 0/0
}

bool historicalLog::tryEatLine(const std::string& line)
{
   m_existing.push_back(line);
   return true;
}

void historicalLog::write(std::ostream& o) const
{
   for(auto l : m_existing)
      o << l << std::endl;

   if(m_existing.size() == 0)
      o << "historical log" << std::endl;

   o << m_addendum.str();
}

void wordCountReport::load(std::istream& i)
{
   while(i.good())
   {
      std::string line;
      std::getline(i,line);
      for(auto *pSec : sections)
         if(pSec->tryEatLine(line))
            break;
   }
}

void wordCountReport::save(std::ostream& o)
{
   for(auto *pSec : sections)
      pSec->write(o);
}

} // namespace wcnt

std::ostream& operator<<(std::ostream& o, const wcnt::today& t)
{
   struct tm *pLt = ::localtime(&t.now);
   char buffer[1024];
   //::strftime(buffer,1024,"%d-%b-%Y",pLt);
   ::strftime(buffer,1024,"%m/%d/%Y",pLt);
   o << buffer;
   return o;
}
