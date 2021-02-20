#include"pch.h"

#include"globedef.h"
#include"WebInquirer.h"

#include"thread.h"

using namespace testing;

namespace StockAnalysisTest {
  class CThreadReadNeteaseDayLineTest : public ::testing::Test
  {
  protected:
    static void SetUpTestSuite(void) {
    }

    static void TearDownTestSuite(void) {
    }

    virtual void SetUp(void) override {
    }

    virtual void TearDown(void) override {
    }
    CMockNeteaseDayLineWebInquiry NeteaseDayLineWebInquiry;
  };

  TEST_F(CThreadReadNeteaseDayLineTest, TestThreadReadNeteaseDayLine) {
    int iRunningThread = gl_ThreadStatus.GetNumberOfSavingThread();
    EXPECT_CALL(NeteaseDayLineWebInquiry, ReadWebData3(200, 30, 30))
      .Times(1)
      .WillOnce(Return(false));
    NeteaseDayLineWebInquiry.__TESTSetBuffer(_T("testData"));
    NeteaseDayLineWebInquiry.SetDownLoadingStockCode(_T("600601.SS"));
    NeteaseDayLineWebInquiry.SetReadingWebData(true);
    EXPECT_EQ(ThreadReadNeteaseDayLine(&NeteaseDayLineWebInquiry), (UINT)4);
    EXPECT_EQ(gl_ThreadStatus.GetNumberOfSavingThread(), iRunningThread);
    EXPECT_FALSE(gl_pChinaStockMarket->GetStock(_T("600601.SS"))->IsDayLineNeedProcess());

    EXPECT_FALSE(NeteaseDayLineWebInquiry.IsReadingWebData());
    NeteaseDayLineWebInquiry.SetReadingWebData(true);
    EXPECT_CALL(NeteaseDayLineWebInquiry, ReadWebData3(200, 30, 30))
      .Times(1)
      .WillOnce(Return(true));
    NeteaseDayLineWebInquiry.__TESTSetBuffer(_T("testData"));
    EXPECT_EQ(ThreadReadNeteaseDayLine(&NeteaseDayLineWebInquiry), (UINT)4);
    EXPECT_EQ(gl_ThreadStatus.GetNumberOfSavingThread(), iRunningThread);
    EXPECT_TRUE(gl_pChinaStockMarket->GetStock(_T("600601.SS"))->IsDayLineNeedProcess()) << _T("顺利读取日线历史数据后，其需要处理的表示被设置");

    gl_pChinaStockMarket->GetStock(_T("600601.SS"))->SetDayLineNeedProcess(false);
  }
}