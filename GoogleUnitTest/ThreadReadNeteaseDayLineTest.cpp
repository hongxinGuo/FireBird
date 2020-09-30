#include"pch.h"

#include"globedef.h"
#include"WebInquirer.h"

#include"thread.h"

using namespace testing;

namespace StockAnalysisTest {
  class CThreadReadNeteaseDLTest : public ::testing::Test
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
    CMockNeteaseDLWebInquiry NeteaseDLWebInquiry;
  };

  TEST_F(CThreadReadNeteaseDLTest, TestThreadReadNeteaseDL) {
    EXPECT_CALL(NeteaseDLWebInquiry, ReadWebData(200, 30, 30))
      .Times(1)
      .WillOnce(Return(false));
    NeteaseDLWebInquiry.__TESTSetBuffer(_T("testData"));
    NeteaseDLWebInquiry.SetDownLoadingStockCode(_T("sh600601"));
    NeteaseDLWebInquiry.SetReadingWebData(true);
    EXPECT_EQ(ThreadReadNeteaseDL(&NeteaseDLWebInquiry), (UINT)4);
    EXPECT_EQ(gl_ThreadStatus.GetNumberOfRunningThread(), 0);
    EXPECT_FALSE(gl_pChinaStockMarket->GetStock(_T("sh600601"))->IsDLNeedProcess());

    EXPECT_CALL(NeteaseDLWebInquiry, ReadWebData(200, 30, 30))
      .Times(1)
      .WillOnce(Return(true));
    NeteaseDLWebInquiry.__TESTSetBuffer(_T("testData"));
    EXPECT_EQ(ThreadReadNeteaseDL(&NeteaseDLWebInquiry), (UINT)4);
    EXPECT_EQ(gl_ThreadStatus.GetNumberOfRunningThread(), 0);
    EXPECT_TRUE(gl_pChinaStockMarket->GetStock(_T("sh600601"))->IsDLNeedProcess()) << _T("顺利读取日线历史数据后，其需要处理的表示被设置");
    char buffer[30];
    strcpy_s(buffer, gl_pChinaStockMarket->GetStock(_T("sh600601"))->GetDLBufferPtr());
    CString str = buffer;
    EXPECT_STREQ(str, _T("testData"));

    gl_pChinaStockMarket->GetStock(_T("sh600601"))->SetDLNeedProcess(false);
  }
}