#include"pch.h"

#include"globedef.h"
#include"WebInquirer.h"

#include"thread.h"

using namespace Testing;
using namespace testing;

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
    EXPECT_CALL(TengxunRTWebInquiry, ReadWebData(100, 30, 20))
      .Times(1)
      .WillOnce(Return(false));
    TengxunRTWebInquiry.__TESTSetBuffer(_T("testData"));
    EXPECT_EQ(ThreadReadTengxunRTData(&TengxunRTWebInquiry), (UINT)2);
    EXPECT_EQ(gl_ThreadStatus.GetNumberOfRunningThread(), 0);
    EXPECT_EQ(gl_WebInquirer.GetPotenDailyBriefingDataSize(), 0);

    EXPECT_CALL(TengxunRTWebInquiry, ReadWebData(100, 30, 20))
      .Times(1)
      .WillOnce(Return(true));
    TengxunRTWebInquiry.__TESTSetBuffer(_T("testData"));
    EXPECT_EQ(ThreadReadTengxunRTData(&TengxunRTWebInquiry), (UINT)2);
    EXPECT_EQ(gl_ThreadStatus.GetNumberOfRunningThread(), 0);
    EXPECT_EQ(gl_WebInquirer.GetTengxunRTDataSize(), 1);
    CWebDataPtr pWebData = gl_WebInquirer.PopTengxunRTData();
    EXPECT_EQ(pWebData->m_lBufferLength, 8);
    char buffer[30];
    strcpy_s(buffer, pWebData->GetBufferAddr());
    CString str = buffer;
    EXPECT_STREQ(str, _T("testData"));
  }
}