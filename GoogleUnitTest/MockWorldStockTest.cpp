///////////////////////////////////////////////////////////////////////////////////////////
//
// 所有使用Mock类测试ChinaMarket的函数皆位于此文件中。
//
///////////////////////////////////////////////////////////////////////////////////////////
#include"pch.h"

#include"globedef.h"

#include"Thread.h"

#include"WorldStock.h"

#include"WebInquirer.h"

#include"MockWorldStock.h"
using namespace std;
using namespace testing;
#include<memory>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

namespace StockAnalysisTest {
  class CMockWorldStockTest : public ::testing::Test
  {
  protected:
    static void SetUpTestSuite(void) {
      ASSERT_FALSE(gl_fNormalMode);
    }

    static void TearDownTestSuite(void) {
    }

    virtual void SetUp(void) override {
      EXPECT_THAT(gl_systemMessage.GetInformationDequeSize(), 0) << gl_systemMessage.PopInformationMessage();
      EXPECT_THAT(gl_systemMessage.GetInnerSystemInformationDequeSize(), 0) << gl_systemMessage.PopInnerSystemInformationMessage();
      EXPECT_THAT(gl_systemMessage.GetDayLineInfoDequeSize(), 0) << gl_systemMessage.PopDayLineInfoMessage();
    }

    virtual void TearDown(void) override {
      // clearup

      while (gl_systemMessage.GetInformationDequeSize() > 0) gl_systemMessage.PopInformationMessage();
      while (gl_systemMessage.GetDayLineInfoDequeSize() > 0) gl_systemMessage.PopDayLineInfoMessage();
      while (gl_systemMessage.GetInnerSystemInformationDequeSize() > 0) gl_systemMessage.PopInnerSystemInformationMessage();
    }
  };

  TEST_F(CMockWorldStockTest, TestUpdateEPSSurpriseDB) {
    CMockWorldStock stock;
    EXPECT_CALL(stock, UpdateEPSSurpriseDB)
      .Times(1);
    EXPECT_EQ(ThreadUpdateEPSSurpriseDB(&stock), 41);
  }
}