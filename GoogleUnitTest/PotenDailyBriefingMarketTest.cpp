#include"pch.h"

#include"globedef.h"
#include"Stock.h"
#include"PotenDailyBriefingMarket.h"

// CVirtualMarket无法生成实例，故而其函数的测试放在这里。
namespace StockAnalysisTest {
  class CPotenDailyBriefingMarketTest : public ::testing::Test
  {
  protected:
    static void SetUpTestSuite() { // 本测试类的初始化函数
      ASSERT_FALSE(gl_fNormalMode);
    }

    static void TearDownTestSuite() {
      gl_PotenDailyBriefingMarket.SetDatabaseLoaded(false);
      gl_PotenDailyBriefingMarket.SetPermitResetMarket(true);
      gl_PotenDailyBriefingMarket.SetReadyToRun(true);
      gl_PotenDailyBriefingMarket.SetResetMarket(true);
    }

    virtual void SetUp(void) override {
      ASSERT_FALSE(gl_fNormalMode);
      ASSERT_TRUE(gl_fTestMode);
      EXPECT_FALSE(gl_PotenDailyBriefingMarket.IsDatabaseLoaded());
      EXPECT_EQ(gl_PotenDailyBriefingMarket.GetNewestUpdateDay(), 20180411);
      EXPECT_TRUE(gl_PotenDailyBriefingMarket.IsPermitResetMarket());
      EXPECT_TRUE(gl_PotenDailyBriefingMarket.IsReadyToRun());
      EXPECT_TRUE(gl_PotenDailyBriefingMarket.IsResetMarket());
    }

    virtual void TearDown(void) override {
      // clearup
    }
  };

  TEST_F(CPotenDailyBriefingMarketTest, TestInitialize) {
    EXPECT_FALSE(gl_PotenDailyBriefingMarket.IsDatabaseLoaded());
    EXPECT_EQ(gl_PotenDailyBriefingMarket.GetNewestUpdateDay(), 20180411);
    EXPECT_TRUE(gl_PotenDailyBriefingMarket.IsPermitResetMarket());
  }

  TEST_F(CPotenDailyBriefingMarketTest, TestTaskResetMarket) {
    EXPECT_TRUE(gl_PotenDailyBriefingMarket.IsPermitResetMarket());
    EXPECT_TRUE(gl_PotenDailyBriefingMarket.IsResetMarket());
    gl_PotenDailyBriefingMarket.TaskResetMarket(85959);
    EXPECT_TRUE(gl_PotenDailyBriefingMarket.IsResetMarket());
    gl_PotenDailyBriefingMarket.TaskResetMarket(93001);
    EXPECT_TRUE(gl_PotenDailyBriefingMarket.IsPermitResetMarket());
    EXPECT_TRUE(gl_PotenDailyBriefingMarket.IsResetMarket());
    gl_PotenDailyBriefingMarket.TaskResetMarket(90000);
    EXPECT_FALSE(gl_PotenDailyBriefingMarket.IsPermitResetMarket());
    EXPECT_TRUE(gl_PotenDailyBriefingMarket.IsResetMarket());
    gl_PotenDailyBriefingMarket.SetPermitResetMarket(true); // 重置此标识
    gl_PotenDailyBriefingMarket.TaskResetMarket(93000);
    EXPECT_FALSE(gl_PotenDailyBriefingMarket.IsPermitResetMarket());
    EXPECT_TRUE(gl_PotenDailyBriefingMarket.IsResetMarket());

    gl_PotenDailyBriefingMarket.SetPermitResetMarket(true); // 重置此标识
  }

  TEST_F(CPotenDailyBriefingMarketTest, TestIsReadyToRun) {
    EXPECT_TRUE(gl_PotenDailyBriefingMarket.IsReadyToRun());
    gl_PotenDailyBriefingMarket.SetReadyToRun(false);
    EXPECT_FALSE(gl_PotenDailyBriefingMarket.IsReadyToRun());
    gl_PotenDailyBriefingMarket.SetReadyToRun(true);
    EXPECT_TRUE(gl_PotenDailyBriefingMarket.IsReadyToRun());
  }

  TEST_F(CPotenDailyBriefingMarketTest, TestIsPermitResetMarket) {
    EXPECT_TRUE(gl_PotenDailyBriefingMarket.IsPermitResetMarket());
    gl_PotenDailyBriefingMarket.SetPermitResetMarket(false);
    EXPECT_FALSE(gl_PotenDailyBriefingMarket.IsPermitResetMarket());
    gl_PotenDailyBriefingMarket.SetPermitResetMarket(true);
    EXPECT_TRUE(gl_PotenDailyBriefingMarket.IsPermitResetMarket());
  }

  TEST_F(CPotenDailyBriefingMarketTest, TestIsResetMarket) {
    EXPECT_TRUE(gl_PotenDailyBriefingMarket.IsResetMarket());
    gl_PotenDailyBriefingMarket.SetResetMarket(false);
    EXPECT_FALSE(gl_PotenDailyBriefingMarket.IsResetMarket());
    gl_PotenDailyBriefingMarket.SetResetMarket(true);
    EXPECT_TRUE(gl_PotenDailyBriefingMarket.IsResetMarket());
  }

  TEST_F(CPotenDailyBriefingMarketTest, TestCalculateMarketTime) {
    ASSERT_FALSE(gl_fNormalMode);
    time_t ttime;
    tm tm_, tmLocal;
    time(&ttime);
    localtime_s(&tmLocal, &ttime);
    ttime -= gl_PotenDailyBriefingMarket.GetTimeZoneOffset();
    gmtime_s(&tm_, &ttime);
    long lTimeZone;
    _get_timezone(&lTimeZone);
    gl_PotenDailyBriefingMarket.CalculateTime();
    gl_PotenDailyBriefingMarket.CalculateLastTradeDay();
    long lTime = gl_PotenDailyBriefingMarket.GetTime();
    EXPECT_EQ(gl_PotenDailyBriefingMarket.GetDayOfWeek(), tm_.tm_wday);

    long day = (tm_.tm_year + 1900) * 10000 + (tm_.tm_mon + 1) * 100 + tm_.tm_mday;
    EXPECT_EQ(gl_PotenDailyBriefingMarket.GetDay(), day);
    EXPECT_EQ(gl_PotenDailyBriefingMarket.GetMonthOfYear(), tm_.tm_mon + 1);
    EXPECT_EQ(gl_PotenDailyBriefingMarket.GetDayOfMonth(), tm_.tm_mday);
    EXPECT_EQ(gl_PotenDailyBriefingMarket.GetYear(), tm_.tm_year + 1900);

    long time = tm_.tm_hour * 10000 + tm_.tm_min * 100 + tm_.tm_sec;
    EXPECT_EQ(gl_PotenDailyBriefingMarket.GetTime(), time);
    char buffer[30];
    sprintf_s(buffer, "%02d:%02d:%02d ", tmLocal.tm_hour, tmLocal.tm_min, tmLocal.tm_sec);
    CString str;
    str = buffer;

    EXPECT_EQ(str.Compare(gl_PotenDailyBriefingMarket.GetTimeString()), 0);

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
    EXPECT_EQ(gl_PotenDailyBriefingMarket.GetLastTradeDay(), LastTradeDay);
  }

  TEST_F(CPotenDailyBriefingMarketTest, TestIsWorkingDay) {
    CTime time1(2019, 11, 25, 0, 0, 0); // 此日为星期一
    EXPECT_TRUE(gl_PotenDailyBriefingMarket.IsWorkingDay(time1));
    EXPECT_TRUE(gl_PotenDailyBriefingMarket.IsWorkingDay(20191125));
    CTime time2(2019, 11, 26, 0, 0, 0); // 此日为星期二
    EXPECT_TRUE(gl_PotenDailyBriefingMarket.IsWorkingDay(time2));
    EXPECT_TRUE(gl_PotenDailyBriefingMarket.IsWorkingDay(20191126));
    CTime time3(2019, 11, 27, 0, 0, 0); // 此日为星期三
    EXPECT_TRUE(gl_PotenDailyBriefingMarket.IsWorkingDay(time3));
    EXPECT_TRUE(gl_PotenDailyBriefingMarket.IsWorkingDay(20191127));
    CTime time4(2019, 11, 28, 0, 0, 0); // 此日为星期四
    EXPECT_TRUE(gl_PotenDailyBriefingMarket.IsWorkingDay(time4));
    EXPECT_TRUE(gl_PotenDailyBriefingMarket.IsWorkingDay(20191128));
    CTime time5(2019, 11, 29, 0, 0, 0); // 此日为星期五
    EXPECT_TRUE(gl_PotenDailyBriefingMarket.IsWorkingDay(time5));
    EXPECT_TRUE(gl_PotenDailyBriefingMarket.IsWorkingDay(20191129));
    CTime time6(2019, 11, 30, 0, 0, 0); // 此日为星期六
    EXPECT_FALSE(gl_PotenDailyBriefingMarket.IsWorkingDay(time6));
    EXPECT_FALSE(gl_PotenDailyBriefingMarket.IsWorkingDay(20191130));
    CTime time7(2019, 12, 1, 0, 0, 0); // 此日为星期日
    EXPECT_FALSE(gl_PotenDailyBriefingMarket.IsWorkingDay(time7));
    EXPECT_FALSE(gl_PotenDailyBriefingMarket.IsWorkingDay(20191201));
  }

  TEST_F(CPotenDailyBriefingMarketTest, TestGetDayOfWeek) {
    ASSERT_FALSE(gl_fNormalMode);
    time_t ttime;
    tm tm_;
    time(&ttime);
    ttime -= gl_PotenDailyBriefingMarket.GetTimeZoneOffset();
    gmtime_s(&tm_, &ttime);

    gl_PotenDailyBriefingMarket.CalculateTime();
    EXPECT_EQ(gl_PotenDailyBriefingMarket.GetDayOfWeek(), tm_.tm_wday);
  }
}