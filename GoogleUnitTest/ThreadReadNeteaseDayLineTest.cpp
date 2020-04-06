#include"pch.h"

#include"globedef.h"
#include"WebInquirer.h"

#include"thread.h"

using namespace Testing;
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
    EXPECT_CALL(NeteaseDayLineWebInquiry, ReadWebData(200, 30, 30))
      .Times(1)
      .WillOnce(Return(false));
    NeteaseDayLineWebInquiry.__TESTSetBuffer(_T("testData"));
    NeteaseDayLineWebInquiry.SetDownLoadingStockCode(_T("sh600601"));
    EXPECT_EQ(ThreadReadNeteaseDayLine(&NeteaseDayLineWebInquiry), 4);
    EXPECT_EQ(gl_ThreadStatus.GetNumberOfRunningThread(), 0);
    EXPECT_FALSE(gl_pChinaStockMarket->GetStock(_T("sh600601"))->IsDayLineNeedProcess());

    EXPECT_CALL(NeteaseDayLineWebInquiry, ReadWebData(200, 30, 30))
      .Times(1)
      .WillOnce(Return(true));
    NeteaseDayLineWebInquiry.__TESTSetBuffer(_T("testData"));
    EXPECT_EQ(ThreadReadNeteaseDayLine(&NeteaseDayLineWebInquiry), 4);
    EXPECT_EQ(gl_ThreadStatus.GetNumberOfRunningThread(), 0);
    EXPECT_TRUE(gl_pChinaStockMarket->GetStock(_T("sh600601"))->IsDayLineNeedProcess());
    char buffer[30];
    strcpy_s(buffer, gl_pChinaStockMarket->GetStock(_T("sh600601"))->GetDayLineBufferPtr());
    CString str = buffer;
    EXPECT_STREQ(str, _T("testData"));

    gl_pChinaStockMarket->GetStock(_T("sh600601"))->SetDayLineNeedProcess(false);
  }
}