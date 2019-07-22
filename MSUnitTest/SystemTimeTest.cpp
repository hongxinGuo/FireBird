#include"stdafx.h"

#include "pch.h"
#include "CppUnitTest.h"

//#include"SystemStatus.h"
#include"SystemTime.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace StockAnalysisUnitTest
{
	TEST_CLASS(SystemTimeTest) {
	public:
		TEST_METHOD(TestGett_time) {
      CSystemTime systemtime;
      Assert::AreEqual(systemtime.Gett_time(), (time_t)0);
      systemtime.Sett_time(100100100100100100);
      Assert::AreEqual(systemtime.Gett_time(), (time_t)100100100100100100);
		}

    TEST_METHOD(TestGetDay) {
      CSystemTime systemtime;
      Assert::AreEqual(systemtime.GetDay(), (long)0);
    }

    TEST_METHOD(TestCalculateTime) {
      CSystemTime systemtime;
      time_t ttime;
      tm tm_;
      time(&ttime);
      localtime_s(&tm_, &ttime);

      systemtime.Sett_time(ttime);
      systemtime.CalculateLastTradeDay();

      Assert::AreEqual(systemtime.GetDayOfWeek(), (long)tm_.tm_wday);

      long day = (tm_.tm_year + 1900) * 10000 + (tm_.tm_mon + 1) * 100 + tm_.tm_mday;
      Assert::AreEqual(systemtime.GetDay(), day);

      long time = tm_.tm_hour * 10000 + tm_.tm_min * 100 + tm_.tm_sec;
      Assert::AreEqual(systemtime.GetTime(), time);

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

      Assert::AreEqual(systemtime.GetLastTradeDay(), LastTradeDay);

      systemtime.CalculateTime();
      ttime = systemtime.Gett_time();
      localtime_s(&tm_, &ttime);

      Assert::AreEqual(systemtime.GetDayOfWeek(), (long)tm_.tm_wday);

      day = (tm_.tm_year + 1900) * 10000 + (tm_.tm_mon + 1) * 100 + tm_.tm_mday;
      Assert::AreEqual(systemtime.GetDay(), day);

      time = tm_.tm_hour * 10000 + tm_.tm_min * 100 + tm_.tm_sec;
      Assert::AreEqual(systemtime.GetTime(), time);

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
      LastTradeDay = (tm_.tm_year + 1900) * 10000 + (tm_.tm_mon + 1) * 100 + tm_.tm_mday;
      Assert::AreEqual(systemtime.GetLastTradeDay(), LastTradeDay);
    }
	};
}
