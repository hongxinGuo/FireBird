#include"stdafx.h"
#include"pch.h"

#include"SystemTime.h"

namespace StockAnalysisTest {
  TEST(SystemTimeTest, TestGett_time) {
    CSystemTime systemtime;
    EXPECT_EQ(systemtime.Gett_time(), 0);
    systemtime.Sett_time(100100100100);
    EXPECT_EQ(systemtime.Gett_time(), 100100100100);
  }
  TEST(SystemTimeTest, TestCalculateTime) {
    time_t ttime;
    tm tm_;
    time(&ttime);
    localtime_s(&tm_, &ttime);

    CSystemTime systemtime;
    systemtime.CalculateTime();
    systemtime.CalculateLastTradeDay();

    long day = (tm_.tm_year + 1900) * 10000 + (tm_.tm_mon + 1) * 100 + tm_.tm_mday;
    EXPECT_EQ(systemtime.GetDay(), day);

    switch (tm_.tm_wday) {
    case 1: // 星期一
      ttime -= 3 * 24 * 3600; // 
      break;
    case 0: //星期日
      ttime -= 3 * 24 * 3600; // 
      break;
    case 6: // 星期六
      ttime -= 2 * 24 * 3600; // 
      break;
    default: // 其他
      ttime -= 24 * 3600; // 
    }
    localtime_s(&tm_, &ttime);
    long LastTradeDay = (tm_.tm_year + 1900) * 10000 + (tm_.tm_mon + 1) * 100 + tm_.tm_mday;

    EXPECT_EQ(systemtime.GetLastTradeDay(), LastTradeDay);
  }

}