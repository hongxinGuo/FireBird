#include"pch.h"

#include"globedef.h"
#include"WebInquirer.h"

#include"thread.h"

using namespace Testing;
using namespace testing;

namespace StockAnalysisTest {
  class CThreadReadSinaRTDataTest : public ::testing::Test
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
    CMockSinaRTWebInquiry SinaRTWebInquiry;
  };

  TEST_F(CThreadReadSinaRTDataTest, TestThreadReadSinaRTData) {
    EXPECT_CALL(SinaRTWebInquiry, ReadWebData(70, 30, 20))
      .Times(1)
      .WillOnce(Return(false));
    SinaRTWebInquiry.__TESTSetBuffer(_T("testData"));
    EXPECT_EQ(ThreadReadSinaRTData(&SinaRTWebInquiry), (UINT)1);
    EXPECT_EQ(gl_ThreadStatus.GetNumberOfRunningThread(), 0);
    EXPECT_EQ(gl_WebInquirer.GetPotenDailyBriefingDataSize(), 0);

    EXPECT_CALL(SinaRTWebInquiry, ReadWebData(70, 30, 20))
      .Times(1)
      .WillOnce(Return(true));
    SinaRTWebInquiry.__TESTSetBuffer(_T("testData"));
    EXPECT_EQ(ThreadReadSinaRTData(&SinaRTWebInquiry), (UINT)1);
    EXPECT_EQ(gl_ThreadStatus.GetNumberOfRunningThread(), 0);
    EXPECT_EQ(gl_WebInquirer.GetSinaRTDataSize(), 1);
    CWebDataPtr pWebData = gl_WebInquirer.PopSinaRTData();
    EXPECT_EQ(pWebData->m_lBufferLength, 8);
    char buffer[30];
    strcpy_s(buffer, pWebData->GetBufferAddr());
    CString str = buffer;
    EXPECT_STREQ(str, _T("testData"));
  }
}