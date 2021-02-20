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
    int iRunningThread = gl_ThreadStatus.GetNumberOfSavingThread();
    EXPECT_CALL(NeteaseRTWebInquiry, ReadWebData())
      .Times(1)
      .WillOnce(Return(false));
    NeteaseRTWebInquiry.__TESTSetBuffer(_T("testData"));
    EXPECT_EQ(ThreadReadNeteaseRTData(&NeteaseRTWebInquiry), (UINT)3);
    EXPECT_EQ(gl_ThreadStatus.GetNumberOfSavingThread(), iRunningThread);
    EXPECT_EQ(gl_WebInquirer.GetPotenDailyBriefingDataSize(), 0);

    EXPECT_CALL(NeteaseRTWebInquiry, ReadWebData())
      .Times(1)
      .WillOnce(Return(true));
    NeteaseRTWebInquiry.__TESTSetBuffer(_T("testData"));
    EXPECT_EQ(ThreadReadNeteaseRTData(&NeteaseRTWebInquiry), (UINT)3);
    EXPECT_EQ(gl_ThreadStatus.GetNumberOfSavingThread(), iRunningThread);
    EXPECT_EQ(gl_WebInquirer.GetNeteaseRTDataSize(), 1);
    CWebDataPtr pWebData = gl_WebInquirer.PopNeteaseRTData();
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