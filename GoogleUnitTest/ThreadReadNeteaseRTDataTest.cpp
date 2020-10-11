#include"pch.h"

#include"globedef.h"
#include"WebInquirer.h"

#include"thread.h"

using namespace testing;

namespace StockAnalysisTest {
  class CThreadReadNeteaseRTDataTest : public ::testing::Test
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
    CMockNeteaseRTWebInquiry NeteaseRTWebInquiry;
  };

  TEST_F(CThreadReadNeteaseRTDataTest, TestThreadReadNeteaseRTData) {
    EXPECT_CALL(NeteaseRTWebInquiry, ReadWebData(150, 30, 30))
      .Times(1)
      .WillOnce(Return(false));
    NeteaseRTWebInquiry.__TESTSetBuffer(_T("testData"));
    EXPECT_EQ(ThreadReadNeteaseRTData(&NeteaseRTWebInquiry), (UINT)3);
    EXPECT_EQ(gl_ThreadStatus.GetNumberOfRunningThread(), 0);
    EXPECT_EQ(gl_WebInquirer.GetPotenDailyBriefingDataSize(), 0);

    EXPECT_CALL(NeteaseRTWebInquiry, ReadWebData(150, 30, 30))
      .Times(1)
      .WillOnce(Return(true));
    NeteaseRTWebInquiry.__TESTSetBuffer(_T("testData"));
    EXPECT_EQ(ThreadReadNeteaseRTData(&NeteaseRTWebInquiry), (UINT)3);
    EXPECT_EQ(gl_ThreadStatus.GetNumberOfRunningThread(), 0);
    EXPECT_EQ(gl_WebInquirer.GetNeteaseRTDataSize(), 1);
    CWebDataPtr pWebData = gl_WebInquirer.PopNeteaseRTData();
    EXPECT_EQ(pWebData->m_lBufferLength, 8);
    char buffer[30];
    strcpy_s(buffer, pWebData->__TEST_GetBufferAddr());
    CString str = buffer;
    EXPECT_STREQ(str, _T("testData"));
  }
}