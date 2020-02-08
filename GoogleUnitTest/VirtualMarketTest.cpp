#include"pch.h"

#include"globedef.h"
#include"VirtualMarket.h"

namespace StockAnalysisTest {
  CVirtualMarket gl_VirtualMarket;

  class CVirtualMarketTest : public ::testing::Test
  {
  protected:
    virtual void SetUp(void) override {
      ASSERT_FALSE(gl_fNormalMode);
      ASSERT_TRUE(gl_fTestMode);
      gl_VirtualMarket.SetPermitResetMarket(true);
      gl_VirtualMarket.SetReadyToRun(true);
      gl_VirtualMarket.SetResetMarket(true);
    }

    virtual void TearDown(void) override {
      // clearup
    }
  };

  TEST_F(CVirtualMarketTest, TestIsReadyToRun) {
    EXPECT_TRUE(gl_VirtualMarket.IsReadyToRun());
    gl_VirtualMarket.SetReadyToRun(false);
    EXPECT_FALSE(gl_VirtualMarket.IsReadyToRun());
    gl_VirtualMarket.SetReadyToRun(true);
    EXPECT_TRUE(gl_VirtualMarket.IsReadyToRun());
  }

  TEST_F(CVirtualMarketTest, TestIsPermitResetMarket) {
    EXPECT_TRUE(gl_VirtualMarket.IsPermitResetMarket()) << "PermitResetMarket should be true\n";
    gl_VirtualMarket.SetPermitResetMarket(false);
    EXPECT_FALSE(gl_VirtualMarket.IsPermitResetMarket());
    gl_VirtualMarket.SetPermitResetMarket(true);
    EXPECT_TRUE(gl_VirtualMarket.IsPermitResetMarket());
  }

  TEST_F(CVirtualMarketTest, TestCalculateMarketTime) {
    ASSERT_FALSE(gl_fNormalMode);
    time_t ttime;
    tm tm_, tmLocal;
    time(&ttime);
    gl_VirtualMarket.CalculateTime();
    EXPECT_EQ(ttime, gl_VirtualMarket.GetLocalTime());
    EXPECT_EQ(ttime - gl_VirtualMarket.GetTimeZoneOffset(), gl_VirtualMarket.GetMarketTime());
    localtime_s(&tmLocal, &ttime);
    ttime -= gl_VirtualMarket.GetTimeZoneOffset();
    gmtime_s(&tm_, &ttime);
    long lTimeZone;
    _get_timezone(&lTimeZone);
    gl_VirtualMarket.CalculateLastTradeDay();
    long lTime = gl_VirtualMarket.GetTime();
    EXPECT_EQ(gl_VirtualMarket.GetDayOfWeek(), tm_.tm_wday);

    long day = (tm_.tm_year + 1900) * 10000 + (tm_.tm_mon + 1) * 100 + tm_.tm_mday;
    EXPECT_EQ(gl_VirtualMarket.GetDay(), day);
    EXPECT_EQ(gl_VirtualMarket.GetMonthOfYear(), tm_.tm_mon + 1);
    EXPECT_EQ(gl_VirtualMarket.GetDayOfMonth(), tm_.tm_mday);
    EXPECT_EQ(gl_VirtualMarket.GetYear(), tm_.tm_year + 1900);

    long time = tm_.tm_hour * 10000 + tm_.tm_min * 100 + tm_.tm_sec;
    EXPECT_EQ(gl_VirtualMarket.GetTime(), time);
    char buffer[30];
    sprintf_s(buffer, "%02d:%02d:%02d ", tmLocal.tm_hour, tmLocal.tm_min, tmLocal.tm_sec);
    CString str;
    str = buffer;

    EXPECT_EQ(str.Compare(gl_VirtualMarket.GetTimeString()), 0);

    sprintf_s(buffer, "%02d:%02d:%02d ", tm_.tm_hour, tm_.tm_min, tm_.tm_sec);
    str = buffer;
    EXPECT_EQ(str.Compare(gl_VirtualMarket.GetMarketTimeString()), 0);

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
    gmtime_s(&tm_, &ttime);
    long LastTradeDay = (tm_.tm_year + 1900) * 10000 + (tm_.tm_mon + 1) * 100 + tm_.tm_mday;
    EXPECT_EQ(gl_VirtualMarket.GetLastTradeDay(), LastTradeDay);
  }

  TEST_F(CVirtualMarketTest, TestGetLastTradeDay) {
    time_t ttime;
    tm tm_, tm2;

    for (int i = 0; i < 7; i++) {
      time(&ttime);
      ttime += i * 60 * 60 * 24;
      gmtime_s(&tm2, &ttime);
      tm_ = tm2;
      gl_VirtualMarket.__TEST_SetMarketTime(ttime);
      gl_VirtualMarket.__TEST_SetMarketTM(tm2);

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
      gmtime_s(&tm_, &ttime);
      long LastTradeDay = (tm_.tm_year + 1900) * 10000 + (tm_.tm_mon + 1) * 100 + tm_.tm_mday;
      EXPECT_EQ(gl_VirtualMarket.GetLastTradeDay(), LastTradeDay);
    }
  }

  TEST_F(CVirtualMarketTest, TestIsWorkingDay) {
    CTime time1(2019, 11, 25, 0, 0, 0); // 此日为星期一
    EXPECT_TRUE(gl_VirtualMarket.IsWorkingDay(time1));
    EXPECT_TRUE(gl_VirtualMarket.IsWorkingDay(20191125));
    CTime time2(2019, 11, 26, 0, 0, 0); // 此日为星期二
    EXPECT_TRUE(gl_VirtualMarket.IsWorkingDay(time2));
    EXPECT_TRUE(gl_VirtualMarket.IsWorkingDay(20191126));
    CTime time3(2019, 11, 27, 0, 0, 0); // 此日为星期三
    EXPECT_TRUE(gl_VirtualMarket.IsWorkingDay(time3));
    EXPECT_TRUE(gl_VirtualMarket.IsWorkingDay(20191127));
    CTime time4(2019, 11, 28, 0, 0, 0); // 此日为星期四
    EXPECT_TRUE(gl_VirtualMarket.IsWorkingDay(time4));
    EXPECT_TRUE(gl_VirtualMarket.IsWorkingDay(20191128));
    CTime time5(2019, 11, 29, 0, 0, 0); // 此日为星期五
    EXPECT_TRUE(gl_VirtualMarket.IsWorkingDay(time5));
    EXPECT_TRUE(gl_VirtualMarket.IsWorkingDay(20191129));
    CTime time6(2019, 11, 30, 0, 0, 0); // 此日为星期六
    EXPECT_FALSE(gl_VirtualMarket.IsWorkingDay(time6));
    EXPECT_FALSE(gl_VirtualMarket.IsWorkingDay(20191130));
    CTime time7(2019, 12, 1, 0, 0, 0); // 此日为星期日
    EXPECT_FALSE(gl_VirtualMarket.IsWorkingDay(time7));
    EXPECT_FALSE(gl_VirtualMarket.IsWorkingDay(20191201));
  }

  TEST_F(CVirtualMarketTest, TestGetDayOfWeek) {
    ASSERT_FALSE(gl_fNormalMode);
    time_t ttime;
    tm tm_;
    time(&ttime);
    ttime -= gl_VirtualMarket.GetTimeZoneOffset();
    gmtime_s(&tm_, &ttime);

    gl_VirtualMarket.CalculateTime();
    EXPECT_EQ(gl_VirtualMarket.GetDayOfWeek(), tm_.tm_wday);
  }

  TEST_F(CVirtualMarketTest, TestTaskResetMarketFlagAtMidnight) { // 这个其实是测试的CVirtualMarket类中的函数。
    EXPECT_TRUE(gl_VirtualMarket.IsPermitResetMarket());
    gl_VirtualMarket.SetPermitResetMarket(false);
    gl_VirtualMarket.TaskResetMarketFlagAtMidnight(0);
    EXPECT_TRUE(gl_VirtualMarket.IsPermitResetMarket());
    gl_VirtualMarket.SetPermitResetMarket(false);
    gl_VirtualMarket.TaskResetMarketFlagAtMidnight(1501);
    EXPECT_FALSE(gl_VirtualMarket.IsPermitResetMarket());
    gl_VirtualMarket.TaskResetMarketFlagAtMidnight(1500);
    EXPECT_TRUE(gl_VirtualMarket.IsPermitResetMarket());
  }

  TEST_F(CVirtualMarketTest, TestSchedulingTask) {
    EXPECT_TRUE(gl_VirtualMarket.SchedulingTask()) << "第一次调用时，内部时间初值为0，故而返回真";
    EXPECT_FALSE(gl_VirtualMarket.SchedulingTask()) << "第二次调用时，内部时间已经设置为当前时间了，间隔不超过一秒，故而返回假";
  }

  TEST_F(CVirtualMarketTest, TestSchedulingTaskPerSecond) {
    EXPECT_TRUE(gl_VirtualMarket.SchedulingTaskPerSecond(90000));
  }

  TEST_F(CVirtualMarketTest, TestSchedulingTaskPer1Minute) {
    EXPECT_TRUE(gl_VirtualMarket.SchedulingTaskPer1Minute(60, 10000));
    EXPECT_FALSE(gl_VirtualMarket.SchedulingTaskPer1Minute(59, 12000));
    EXPECT_TRUE(gl_VirtualMarket.SchedulingTaskPer1Minute(1, 12010));
  }

  TEST_F(CVirtualMarketTest, TestSchedulingTaskPer10Second) {
    EXPECT_TRUE(gl_VirtualMarket.SchedulingTaskPer10Second(10, 10000));
    EXPECT_FALSE(gl_VirtualMarket.SchedulingTaskPer10Second(9, 12000));
    EXPECT_TRUE(gl_VirtualMarket.SchedulingTaskPer10Second(1, 12010));
  }
  TEST_F(CVirtualMarketTest, TestSchedulingTaskPer5Minute) {
    EXPECT_TRUE(gl_VirtualMarket.SchedulingTaskPer5Minute(300, 10000));
    EXPECT_FALSE(gl_VirtualMarket.SchedulingTaskPer5Minute(299, 12000));
    EXPECT_TRUE(gl_VirtualMarket.SchedulingTaskPer5Minute(1, 12010));
  }
  TEST_F(CVirtualMarketTest, TestSchedulingTaskPerHour) {
    EXPECT_TRUE(gl_VirtualMarket.SchedulingTaskPerHour(3600, 10000));
    EXPECT_FALSE(gl_VirtualMarket.SchedulingTaskPerHour(3599, 12000));
    EXPECT_TRUE(gl_VirtualMarket.SchedulingTaskPerHour(1, 12010));
  }
}