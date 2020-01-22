#include"pch.h"

#include"globedef.h"
#include"Stock.h"
#include"PotenDailyBriefingMarket.h"

// CVirtualMarket�޷�����ʵ�����ʶ��亯���Ĳ��Է������
namespace StockAnalysisTest {
  class CPotenDailyBriefingMarketTest : public ::testing::Test
  {
  protected:
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

    static void SetUpTestCase() { // ��������ĳ�ʼ������
      ASSERT_FALSE(gl_fNormalMode);
    }

    static void TearDownTestCase() {
      gl_PotenDailyBriefingMarket.SetDatabaseLoaded(false);
      gl_PotenDailyBriefingMarket.SetPermitResetMarket(true);
      gl_PotenDailyBriefingMarket.SetReadyToRun(true);
      gl_PotenDailyBriefingMarket.SetResetMarket(true);
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
    gl_PotenDailyBriefingMarket.TaskResetMarket(92459);
    EXPECT_TRUE(gl_PotenDailyBriefingMarket.IsResetMarket());
    gl_PotenDailyBriefingMarket.TaskResetMarket(93001);
    EXPECT_TRUE(gl_PotenDailyBriefingMarket.IsPermitResetMarket());
    EXPECT_TRUE(gl_PotenDailyBriefingMarket.IsResetMarket());
    gl_PotenDailyBriefingMarket.TaskResetMarket(92500);
    EXPECT_FALSE(gl_PotenDailyBriefingMarket.IsPermitResetMarket());
    EXPECT_TRUE(gl_PotenDailyBriefingMarket.IsResetMarket());

    gl_PotenDailyBriefingMarket.SetPermitResetMarket(true); // ���ô˱�ʶ
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
}