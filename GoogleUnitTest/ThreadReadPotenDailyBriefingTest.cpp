#include"pch.h"

#include"globedef.h"
#include"WebInquirer.h"

#include"thread.h"

using namespace testing;

namespace StockAnalysisTest {
  class CPotenDailyBriefingThreadTest : public ::testing::Test
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
    CMockPotenDailyBriefingWebInquiry potenDailyBriefingInquiry;
  };

  TEST_F(CPotenDailyBriefingThreadTest, TestThreadReadPotenDailyBriefing) {
    int iRunningThread = gl_ThreadStatus.GetNumberOfRunningThread();
    EXPECT_CALL(potenDailyBriefingInquiry, ReadWebData(500, 50, 30))
      .Times(1)
      .WillOnce(Return(false));
    potenDailyBriefingInquiry.__TESTSetBuffer(_T("testData"));
    potenDailyBriefingInquiry.SetInquiringDate(12345678);
    EXPECT_EQ(ThreadReadPotenDailyBriefing(&potenDailyBriefingInquiry), (UINT)6);
    EXPECT_EQ(gl_ThreadStatus.GetNumberOfRunningThread(), iRunningThread);
    EXPECT_EQ(gl_WebInquirer.GetPotenDailyBriefingDataSize(), 0);

    EXPECT_CALL(potenDailyBriefingInquiry, ReadWebData(500, 50, 30))
      .Times(1)
      .WillOnce(Return(true));
    potenDailyBriefingInquiry.__TESTSetBuffer(_T("testData"));
    potenDailyBriefingInquiry.SetInquiringDate(12345678);
    EXPECT_EQ(ThreadReadPotenDailyBriefing(&potenDailyBriefingInquiry), (UINT)6);
    EXPECT_EQ(gl_ThreadStatus.GetNumberOfRunningThread(), iRunningThread);
    EXPECT_EQ(gl_WebInquirer.GetPotenDailyBriefingDataSize(), 1);
    CWebDataPtr pWebData = gl_WebInquirer.PopPotenDailyBriefingData();
    EXPECT_EQ(pWebData->m_lBufferLength, 8);
    char buffer[30];
    strcpy_s(buffer, pWebData->__TEST_GetBufferAddr());
    CString str = buffer;
    EXPECT_STREQ(str, _T("testData"));
    EXPECT_EQ(pWebData->m_lTime, 12345678000000);
    EXPECT_FALSE(gl_WebInquirer.IsReadingPotenDailyBriefing());
  }
}