#include"stdafx.h"
#include"pch.h"

#include"globedef.h"
#include"SystemTime.h"

using namespace testing;

namespace StockAnalysisTest {
  
  TEST(SystemTimeTest, TestInitialize) {
    ASSERT_FALSE(gl_fNormalMode);
    long l = gl_systemMessage.GetInformationDequeSize();
    CSystemTime systemtime; // 生成第二个实例（第一个为全局变量，系统启动时就生成了）
    EXPECT_EQ(gl_systemMessage.GetInformationDequeSize(), l + 1); // 系统报警队列
    for (int i = 0; i < l+1; i++) {
      CString str = gl_systemMessage.PopInformationMessage(); // 清除信息队列
    }
    EXPECT_EQ(gl_systemMessage.GetInformationDequeSize(), 0);
  }
  

  TEST(SystemTimeTest, TestGett_time) {
    EXPECT_NE(gl_systemTime.Gett_time(), 0);
    gl_systemTime.Sett_time(100100100100);
    EXPECT_EQ(gl_systemTime.Gett_time(), 100100100100);
  }
  TEST(SystemTimeTest, TestCalculateTime) {
    time_t ttime;
    tm tm_;
    time(&ttime);
    localtime_s(&tm_, &ttime);

    gl_systemTime.Sett_time(ttime);
    gl_systemTime.CalculateLastTradeDay();

    EXPECT_EQ(gl_systemTime.GetDayOfWeek(), tm_.tm_wday);

    long day = (tm_.tm_year + 1900) * 10000 + (tm_.tm_mon + 1) * 100 + tm_.tm_mday;
    EXPECT_EQ(gl_systemTime.GetDay(), day);

    long time = tm_.tm_hour * 10000 + tm_.tm_min * 100 + tm_.tm_sec;
    EXPECT_EQ(gl_systemTime.GetTime(), time);
    char buffer[30];
    sprintf_s(buffer, "%02d:%02d:%02d ", tm_.tm_hour, tm_.tm_min, tm_.tm_sec);
    CString str;
    str = buffer;

    EXPECT_EQ(str.Compare(gl_systemTime.GetTimeString()), 0);

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

    EXPECT_EQ(gl_systemTime.GetLastTradeDay(), LastTradeDay);

    gl_systemTime.CalculateTime();
    ttime = gl_systemTime.Gett_time();
    localtime_s(&tm_, &ttime);

    EXPECT_EQ(gl_systemTime.GetDayOfWeek(), tm_.tm_wday);

    day = (tm_.tm_year + 1900) * 10000 + (tm_.tm_mon + 1) * 100 + tm_.tm_mday;
    EXPECT_EQ(gl_systemTime.GetDay(), day);

    time = tm_.tm_hour * 10000 + tm_.tm_min * 100 + tm_.tm_sec;
    EXPECT_EQ(gl_systemTime.GetTime(), time);

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
    EXPECT_EQ(gl_systemTime.GetLastTradeDay(), LastTradeDay);
  }

}