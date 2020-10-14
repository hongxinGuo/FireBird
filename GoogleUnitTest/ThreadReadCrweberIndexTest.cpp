#include"pch.h"

#include"globedef.h"
#include"WebInquirer.h"

#include"thread.h"

using namespace testing;

namespace StockAnalysisTest {
  class CThreadCrweberIndexTest : public ::testing::Test
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
    CMockCrweberIndexWebInquiry crweberIndexInquiry;
  };

  TEST_F(CThreadCrweberIndexTest, TestThreadCrweberIndex) {
    int iRunningThread = gl_ThreadStatus.GetNumberOfRunningThread();
    EXPECT_CALL(crweberIndexInquiry, ReadWebData(1000, 100, 100))
      .Times(1)
      .WillOnce(Return(false));
    crweberIndexInquiry.__TESTSetBuffer(_T("testData"));
    EXPECT_EQ(ThreadReadCrweberIndex(&crweberIndexInquiry), (UINT)5);
    EXPECT_EQ(gl_ThreadStatus.GetNumberOfRunningThread(), iRunningThread);
    EXPECT_EQ(gl_WebInquirer.GetPotenDailyBriefingDataSize(), 0);

    EXPECT_CALL(crweberIndexInquiry, ReadWebData(1000, 100, 100))
      .Times(1)
      .WillOnce(Return(true));
    crweberIndexInquiry.__TESTSetBuffer(_T("testData"));
    EXPECT_EQ(ThreadReadCrweberIndex(&crweberIndexInquiry), (UINT)5);
    EXPECT_EQ(gl_ThreadStatus.GetNumberOfRunningThread(), iRunningThread);
    EXPECT_EQ(gl_WebInquirer.GetCrweberDataSize(), 1);
    CWebDataPtr pWebData = gl_WebInquirer.PopCrweberData();
    EXPECT_EQ(pWebData->m_lBufferLength, 8);
    char buffer[30];
    strcpy_s(buffer, pWebData->__TEST_GetBufferAddr());
    CString str = buffer;
    EXPECT_STREQ(str, _T("testData"));
    EXPECT_FALSE(gl_WebInquirer.IsReadingCrweberIndex());
  }
}