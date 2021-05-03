#include"pch.h"

#include"globedef.h"
#include"WebInquirer.h"

#include"thread.h"

using namespace testing;

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

namespace StockAnalysisTest {
  class CThreadReadTengxunRTDataTest : public ::testing::Test
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
    CMockTengxunRTWebInquiry TengxunRTWebInquiry;
  };

  TEST_F(CThreadReadTengxunRTDataTest, TestThreadReadTengxunRTData) {
    int iRunningThread = gl_ThreadStatus.GetNumberOfWebInquiringThread();
    EXPECT_CALL(TengxunRTWebInquiry, ReadWebData())
      .Times(1)
      .WillOnce(Return(false));
    TengxunRTWebInquiry.__TESTSetBuffer(_T("testData"));
    EXPECT_EQ(ThreadReadTengxunRTData(&TengxunRTWebInquiry), (UINT)2);
    EXPECT_EQ(gl_ThreadStatus.GetNumberOfWebInquiringThread(), iRunningThread);
    EXPECT_EQ(gl_WebInquirer.GetTengxunRTDataSize(), 0);

    EXPECT_CALL(TengxunRTWebInquiry, ReadWebData())
      .Times(1)
      .WillOnce(Return(true));
    TengxunRTWebInquiry.__TESTSetBuffer(_T("testData"));
    EXPECT_EQ(ThreadReadTengxunRTData(&TengxunRTWebInquiry), (UINT)2);
    EXPECT_EQ(gl_ThreadStatus.GetNumberOfWebInquiringThread(), iRunningThread);
    EXPECT_EQ(gl_WebInquirer.GetTengxunRTDataSize(), 1);
    CWebDataPtr pWebData = gl_WebInquirer.PopTengxunRTData();
    EXPECT_EQ(pWebData->GetBufferLength(), 8);
    char buffer[30];
    int i = 0;
    while ((i < 30) && (pWebData->GetData(i) != 0x000)) {
      buffer[i] = pWebData->GetData(i);
      i++;
    }
    buffer[i] = 0x000;
    CString str = buffer;
    EXPECT_STREQ(str, _T("testData"));
  }
}