/**
 * @file      date_time.h
 * @brief     Header file for the Date and Time helper class.
 * @author    garciay.yann@gmail.com
 * @copyright Copyright (c) 2015-2017 ygarcia. All rights reserved
 * @license   This project is released under the MIT License
 * @version   0.1
 */
#pragma once

#include <iostream>
#include <string>
#include <cstdint>
#include <regex>

#include <string.h>

namespace helpers {

  /**
   * @enum month_t
   * @brief List of the months, starting from 0 (refer to std::tm::mon)
   */
  typedef enum {
    Jan = 0, Feb, Mar, Apr, May, Jun, Jul, Aug, Sep, Oct, Nov, Dec
  } month_t;

  /**
   * @class date_time
   * @brief This class provides date and time helper methods
   */
  class date_time {
  public:
    /**
     * @brief Default constructor, date and time are set to the current time
     */
    date_time();
    /**
     * @brief Specialized constructor, date and time are parsed from the provided parameter
     * @param[in] p_date_time Date/Time string value to parse (e.g. Thu May 15 04:36:00 2014)
     */
    date_time(const std::string & p_date_time) { parse_date_time(p_date_time); };
    /**
     * @brief Default destructor
     */
    virtual ~date_time() { };

    /**
     * @struct date_time_t
     * @brief concrete daytime structure to store the data
     */
    typedef struct {
      struct tm date_time_tm;
      std::string sday;
      std::string to_string;

      void clear() {
        ::memset((void *)&date_time_tm, 0x00, sizeof(struct tm));
        sday.clear();
        to_string.clear();
      };
    } date_time_t;

    /**
     * @brief Retrieve the day in the month of the date/time
     * @return The day value (1 to 31)
     */
    inline uint32_t get_day() const { return static_cast<uint32_t>(_date_time.date_time_tm.tm_mday); }
    /**
     * @brief Retrieve the month of the date/time
     * @return The month value (1 to 12)
     */
    inline uint32_t get_month() const { return static_cast<uint32_t>(_date_time.date_time_tm.tm_mon + 1); }
    /**
     * @brief Retrieve the year of the date/time
     * @return The year value (e.g. 2015)
     */
    inline uint32_t get_year() const { return static_cast<uint32_t>(_date_time.date_time_tm.tm_year); }
    /**
     * @brief Retrieve the hour of the date/time
     * @return The hour value (0 to 23)
     */
    inline uint32_t get_hour() const { return static_cast<uint32_t>(_date_time.date_time_tm.tm_hour); }
    /**
     * @brief Retrieve the minutes of the date/time
     * @return The minutes value (0 to 59)
     */
    inline uint32_t get_minutes() const { return static_cast<uint32_t>(_date_time.date_time_tm.tm_min); }
    /**
     * @brief Retrieve the seconds of the date/time
     * @return The seconds value (0 to 59)
     */
    inline uint32_t get_seconds() const { return static_cast<uint32_t>(_date_time.date_time_tm.tm_sec); }
    /**
     * @brief Retrieve the day of the date/time in string format
     * @return The day value (e.g. Mon for Monday)
     */
    inline std::string get_sday() const { return _date_time.sday; }

  private:
    /**
     * @brief Date format regex string
     * @remark Format: Thu May 15 04:36:00 2014 
     * @see http://www.regexr.com/
     */
    static std::regex g_dt_regex;
    
    void parse_date_time(const std::string & p_date_time);
    uint32_t get_month(const std::string & p_month);
  
    date_time_t _date_time;
    /**
     * @brief Lookup table for month string to integer convertion
     */
    static std::array<std::pair<std::string, uint32_t>, 12> month_desc;

  }; // End of class date_time

} // End of namespace helpers

using namespace helpers;
