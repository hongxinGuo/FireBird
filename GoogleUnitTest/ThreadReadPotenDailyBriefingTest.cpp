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
    int iRunningThread = gl_ThreadStatus.GetNumberOfSavingThread();
    EXPECT_CALL(potenDailyBriefingInquiry, ReadWebData())
      .Times(1)
      .WillOnce(Return(false));
    potenDailyBriefingInquiry.__TESTSetBuffer(_T("testData"));
    potenDailyBriefingInquiry.SetInquiringDate(12345678);
    EXPECT_EQ(ThreadReadPotenDailyBriefing(&potenDailyBriefingInquiry), (UINT)6);
    EXPECT_EQ(gl_ThreadStatus.GetNumberOfSavingThread(), iRunningThread);
    EXPECT_EQ(gl_WebInquirer.GetPotenDailyBriefingDataSize(), 0);

    EXPECT_CALL(potenDailyBriefingInquiry, ReadWebData())
      .Times(1)
      .WillOnce(Return(true));
    potenDailyBriefingInquiry.__TESTSetBuffer(_T("testData"));
    potenDailyBriefingInquiry.SetInquiringDate(12345678);
    EXPECT_EQ(ThreadReadPotenDailyBriefing(&potenDailyBriefingInquiry), (UINT)6);
    EXPECT_EQ(gl_ThreadStatus.GetNumberOfSavingThread(), iRunningThread);
    EXPECT_EQ(gl_WebInquirer.GetPotenDailyBriefingDataSize(), 1);
    CWebDataPtr pWebData = gl_WebInquirer.PopPotenDailyBriefingData();
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
    EXPECT_EQ(pWebData->GetTime(), 12345678000000);
    EXPECT_FALSE(gl_WebInquirer.IsReadingPotenDailyBriefing());
  }
}