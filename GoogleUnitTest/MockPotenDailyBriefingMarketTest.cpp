///////////////////////////////////////////////////////////////////////////////////////////
//
// 所有使用Mock类测试PotenDailyBriefingMarket的函数皆位于此文件中。
//
///////////////////////////////////////////////////////////////////////////////////////////
#include"stdafx.h"
#include"pch.h"

#include"globedef.h"

#include"Thread.h"
#include"WebInquirer.h"

#include"MockPotenDailyBriefingMarket.h"
using namespace testing;
using namespace testing;

using namespace std;
#include<memory>

namespace StockAnalysisTest {
  class CMockPotenDailyBirefingMarketTest : public ::testing::Test
  {
  protected:
    static void SetUpTestSuite(void) {
      ASSERT_FALSE(gl_fNormalMode);
    }
    static void TearDownTestSuite(void) {
      EXPECT_EQ(gl_pChinaStockMarket->GetCurrentStock(), nullptr) << gl_pChinaStockMarket->GetCurrentStock()->GetStockCode();
      while (gl_WebInquirer.IsReadingWebThreadRunning()) Sleep(1);
    }
    virtual void SetUp(void) override {
      while (gl_systemMessage.GetInformationDequeSize() > 0) gl_systemMessage.PopInformationMessage();
      while (gl_systemMessage.GetDayLineInfoDequeSize() > 0) gl_systemMessage.PopDayLineInfoMessage();
      while (gl_systemMessage.GetInnerSystemInformationDequeSize() > 0) gl_systemMessage.PopInnerSystemInformationMessage();
    }

    virtual void TearDown(void) override {
      // clearup

      while (gl_systemMessage.GetInformationDequeSize() > 0) gl_systemMessage.PopInformationMessage();
      while (gl_systemMessage.GetDayLineInfoDequeSize() > 0) gl_systemMessage.PopDayLineInfoMessage();
      while (gl_systemMessage.GetInnerSystemInformationDequeSize() > 0) gl_systemMessage.PopInnerSystemInformationMessage();
    }

    CMockPotenDailyBriefingMarket potenDailyBreifingMarket;
  };

  TEST_F(CMockPotenDailyBirefingMarketTest, TestThreadSavePotenData) {
    EXPECT_CALL(potenDailyBreifingMarket, SaveCurrentData)
      .Times(1);
    ThreadSavePotenData(&potenDailyBreifingMarket);
  }
}