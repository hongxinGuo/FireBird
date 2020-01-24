#include"stdafx.h"
#include"pch.h"

#include"globedef.h"
#include"SystemTime.h"

namespace StockAnalysisTest {
  TEST(SystemTimeTest, TestInitialize) {
    ASSERT_FALSE(gl_fNormalMode);
    long l = gl_systemMessage.GetInformationDequeSize();
    CSystemTime systemtime; // 生成第二个实例（第一个为全局变量，系统启动时就生成了）
    EXPECT_EQ(gl_systemMessage.GetInformationDequeSize(), l + 1); // 系统报警队列
    for (int i = 0; i < l + 1; i++) {
      CString str = gl_systemMessage.PopInformationMessage(); // 清除信息队列
    }
    EXPECT_EQ(gl_systemMessage.GetInformationDequeSize(), 0);
  }

  TEST(SystemTimeTest, TestGett_time) {
    EXPECT_NE(gl_systemTime.Gett_time(), 0);
    gl_systemTime.__Test_Sett_time(100100100100);
    EXPECT_EQ(gl_systemTime.Gett_time(), 100100100100);
  }
  TEST(SystemTimeTest, TestCalculateLocalTime) {
    ASSERT_FALSE(gl_fNormalMode);
    time_t ttime;
    tm tm_;
    time(&ttime);
    localtime_s(&tm_, &ttime);

    gl_systemTime.__Test_Sett_time(ttime);
    gl_systemTime.CalculateLastTradeDay();

    EXPECT_EQ(gl_systemTime.GetDayOfWeek(), tm_.tm_wday);

    long day = (tm_.tm_year + 1900) * 10000 + (tm_.tm_mon + 1) * 100 + tm_.tm_mday;
    EXPECT_EQ(gl_systemTime.GetDay(), day);
    EXPECT_EQ(gl_systemTime.GetMonthOfYear(), tm_.tm_mon + 1);

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

    gl_systemTime.CalculateLocalTime();
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

  TEST(SystemTimeTest, TestIsWorkingDay) {
    CTime time1(2019, 11, 25, 0, 0, 0); // 此日为星期一
    EXPECT_TRUE(gl_systemTime.IsWorkingDay(time1));
    EXPECT_TRUE(gl_systemTime.IsWorkingDay(20191125));
    CTime time2(2019, 11, 26, 0, 0, 0); // 此日为星期二
    EXPECT_TRUE(gl_systemTime.IsWorkingDay(time2));
    EXPECT_TRUE(gl_systemTime.IsWorkingDay(20191126));
    CTime time3(2019, 11, 27, 0, 0, 0); // 此日为星期三
    EXPECT_TRUE(gl_systemTime.IsWorkingDay(time3));
    EXPECT_TRUE(gl_systemTime.IsWorkingDay(20191127));
    CTime time4(2019, 11, 28, 0, 0, 0); // 此日为星期四
    EXPECT_TRUE(gl_systemTime.IsWorkingDay(time4));
    EXPECT_TRUE(gl_systemTime.IsWorkingDay(20191128));
    CTime time5(2019, 11, 29, 0, 0, 0); // 此日为星期五
    EXPECT_TRUE(gl_systemTime.IsWorkingDay(time5));
    EXPECT_TRUE(gl_systemTime.IsWorkingDay(20191129));
    CTime time6(2019, 11, 30, 0, 0, 0); // 此日为星期六
    EXPECT_FALSE(gl_systemTime.IsWorkingDay(time6));
    EXPECT_FALSE(gl_systemTime.IsWorkingDay(20191130));
    CTime time7(2019, 12, 1, 0, 0, 0); // 此日为星期日
    EXPECT_FALSE(gl_systemTime.IsWorkingDay(time7));
    EXPECT_FALSE(gl_systemTime.IsWorkingDay(20191201));
  }

  TEST(SystemTimeTest, TestGetDayOfWeek) {
    ASSERT_FALSE(gl_fNormalMode);
    time_t ttime;
    tm tm_;
    time(&ttime);
    localtime_s(&tm_, &ttime);

    gl_systemTime.__Test_Sett_time(ttime);
    EXPECT_EQ(gl_systemTime.GetDayOfWeek(), tm_.tm_wday);
  }

  TEST(SystemTimeTest, TestGetDay) {
    ASSERT_FALSE(gl_fNormalMode);
    time_t ttime;
    tm tm_;
    time(&ttime);
    localtime_s(&tm_, &ttime);

    gl_systemTime.__Test_Sett_time(ttime);
    long day = (tm_.tm_year + 1900) * 10000 + (tm_.tm_mon + 1) * 100 + tm_.tm_mday;
    EXPECT_EQ(gl_systemTime.GetDay(), day);
  }

  TEST(SystemTimeTest, TestGetMonthOfYear) {
    ASSERT_FALSE(gl_fNormalMode);
    time_t ttime;
    tm tm_;
    time(&ttime);
    localtime_s(&tm_, &ttime);

    gl_systemTime.__Test_Sett_time(ttime);
    EXPECT_EQ(gl_systemTime.GetMonthOfYear(), tm_.tm_mon + 1);
    EXPECT_EQ(gl_systemTime.GetDayOfMonth(), tm_.tm_mday);
    EXPECT_EQ(gl_systemTime.GetYear(), tm_.tm_year + 1900);
  }

  TEST(SystemTimeTest, TestIsEarlyThen) {
    EXPECT_TRUE(gl_systemTime.IsEarlyThen(20190101, 20190104, 2));
    EXPECT_TRUE(gl_systemTime.IsEarlyThen(20180101, 20190101, 354));
    EXPECT_TRUE(gl_systemTime.IsEarlyThen(20190130, 20190201, 1));
    EXPECT_TRUE(gl_systemTime.IsEarlyThen(20190101, 20190110, 8));
    EXPECT_TRUE(gl_systemTime.IsEarlyThen(20190101, 20190104, 2));
    EXPECT_FALSE(gl_systemTime.IsEarlyThen(20191231, 20200101, 2));
    EXPECT_FALSE(gl_systemTime.IsEarlyThen(20191131, 20200101, 33));
    EXPECT_FALSE(gl_systemTime.IsEarlyThen(20190131, 20190201, 2));
    EXPECT_FALSE(gl_systemTime.IsEarlyThen(20190101, 20190110, 10));
  }
}