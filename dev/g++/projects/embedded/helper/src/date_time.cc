/*!
 * \file      date_time.cpp
 * \brief     Implementation file for the Date and Time helper class.
 * \author    garciay.yann@gmail.com
 * \copyright Copyright (c) 2015-2017 ygarcia. All rights reserved
 * \license   This project is released under the MIT License
 * \version   0.1
 */
#include <chrono>
#include <ctime>
#include <array>
#include <utility>
#include <cstdlib>

#include "date_time.hh"

namespace helpers {

  std::array<std::pair<std::string, uint32_t>, 12> date_time::month_desc = {
    {
      std::make_pair(std::string("Jan"), month_t::Jan),
      std::make_pair(std::string("Feb"), month_t::Feb),
      std::make_pair(std::string("Mar"), month_t::Mar),
      std::make_pair(std::string("Apr"), month_t::Apr),
      std::make_pair(std::string("May"), month_t::May),
      std::make_pair(std::string("Jun"), month_t::Jun),
      std::make_pair(std::string("Jul"), month_t::Jul),
      std::make_pair(std::string("Aug"), month_t::Aug),
      std::make_pair(std::string("Sep"), month_t::Sep),
      std::make_pair(std::string("Oct"), month_t::Oct),
      std::make_pair(std::string("Nov"), month_t::Nov),
      std::make_pair(std::string("Dec"), month_t::Dec),
    }
  };

  std::regex date_time::g_dt_regex("^(\\w{3})\\s+(\\w{3})\\s+(\\d{1,2})\\s+(\\d{2}):(\\d{2}):(\\d{2})\\s+(\\d{4})$");

  date_time::date_time() {
    // Get current time
    std::chrono::system_clock::time_point tp_now = std::chrono::system_clock::now();
    time_t now = std::chrono::system_clock::to_time_t(tp_now);
    //  uint8_t * now_info = ::ctime(&now);
    // Initialise data structures
    std::string str(::ctime(&now));
    str.erase(std::remove(str.begin(), str.end(), '\n'), str.end());
    parse_date_time(str);
  } // End of ctor
  
  void date_time::parse_date_time(const std::string & p_date_time) {
    //    std::clog << ">>> date_time::parse_date_time: " << p_date_time << std::endl;

    std::smatch match;
    std::regex_match(p_date_time, match, g_dt_regex);
    //    std::clog << "date_time::parse_date_time: matching: " << (int)match.size() << std::endl;
    _date_time.clear();
    if (match.size() == 8) {
      // Match the group and subgroups by regex parser.
      uint32_t index = 0;
      _date_time.to_string.assign(match[index++]);
      _date_time.sday = match[index++];
      _date_time.date_time_tm.tm_mon = get_month(match[index++]);
      _date_time.date_time_tm.tm_mday = std::stoi(match[index++]);
      _date_time.date_time_tm.tm_hour = std::stoi(match[index++]);
      _date_time.date_time_tm.tm_min = std::stoi(match[index++]);
      _date_time.date_time_tm.tm_sec = std::stoi(match[index++]);
      _date_time.date_time_tm.tm_year = std::stoi(match[index++]);
      _date_time.date_time_tm.tm_isdst = -1; // Not used
    }
  } // End of method parse_date_time

  uint32_t date_time::get_month(const std::string& p_month) {
    for (const auto & it : month_desc) {
      if (it.first == p_month) return it.second;
    } // End of 'for' statement
    
    return 0;
  } // End of method get_month

} // End of namespace helpers
