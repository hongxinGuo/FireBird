#include"stdafx.h"
#include"pch.h"

#include"ChinaMarket.h"
#include"ChinaStock.h"

#include"SetStockCode.h"
#include"WebInquirer.h"

using namespace std;
#include<memory>
namespace StockAnalysisTest {
  class CChinaMarketTest : public ::testing::Test
  {
  protected:
    static void SetUpTestCase(void) {
      ASSERT_FALSE(gl_fNormalMode);
      EXPECT_EQ(gl_pChinaStockMarket->GetCurrentStock(), nullptr);
      EXPECT_EQ(gl_pChinaStockMarket->GetDayLineNeedUpdateNumber(), 12000);
    }
    static void TearDownTestCase(void) {
      while (gl_WebInquirer.IsReadingWebThreadRunning()) Sleep(1);
    }
    virtual void SetUp(void) override {
      ASSERT_FALSE(gl_fNormalMode);
      gl_pChinaStockMarket->CalculateTime();
      gl_pChinaStockMarket->ResetNeteaseRTDataInquiringIndex();
      gl_pChinaStockMarket->ResetNeteaseDayLineDataInquiringIndex();
      gl_pChinaStockMarket->ResetSinaRTDataInquiringIndex();
      gl_pChinaStockMarket->ResetTengxunRTDataInquiringIndex();
      gl_pChinaStockMarket->SetSystemReady(true); // 测试市场时，默认系统已经准备好
      gl_pChinaStockMarket->SetPermitResetMarket(true);
      gl_pChinaStockMarket->SetCheckActiveStock(true);
      EXPECT_TRUE(gl_pChinaStockMarket->IsResetMarket());

      while (gl_systemMessage.GetInformationDequeSize() > 0) gl_systemMessage.PopInformationMessage();
      while (gl_systemMessage.GetDayLineInfoDequeSize() > 0) gl_systemMessage.PopDayLineInfoMessage();
      while (gl_systemMessage.GetInnerSystemInformationDequeSize() > 0) gl_systemMessage.PopInnerSystemInformationMessage();
    }

    virtual void TearDown(void) override {
      // clearup
      gl_pChinaStockMarket->SetRTDataSetCleared(false);
      gl_pChinaStockMarket->SetUpdateStockCodeDB(false);
      gl_pChinaStockMarket->SetUpdateOptionDB(false);
      gl_pChinaStockMarket->ClearChoicedRTDataQueue();
      gl_pChinaStockMarket->SetResetMarket(true);
      gl_pChinaStockMarket->ResetNeteaseRTDataInquiringIndex();
      gl_pChinaStockMarket->ResetNeteaseDayLineDataInquiringIndex();
      gl_pChinaStockMarket->ResetSinaRTDataInquiringIndex();
      gl_pChinaStockMarket->ResetTengxunRTDataInquiringIndex();
      while (gl_systemMessage.GetInformationDequeSize() > 0) gl_systemMessage.PopInformationMessage();
      gl_pChinaStockMarket->SetCurrentStockChanged(false);
      gl_pChinaStockMarket->SetPermitResetMarket(true);
      gl_pChinaStockMarket->SetCheckActiveStock(true);
      gl_pChinaStockMarket->SetSystemReady(true); // 离开此测试时，默认系统已准备好。
      for (int i = 0; i < gl_pChinaStockMarket->GetTotalStock(); i++) {
        CChinaStockPtr pStock = gl_pChinaStockMarket->GetStock(i);
        if (!pStock->IsDayLineNeedUpdate()) pStock->SetDayLineNeedUpdate(true);
      }

      while (gl_systemMessage.GetInformationDequeSize() > 0) gl_systemMessage.PopInformationMessage();
      while (gl_systemMessage.GetDayLineInfoDequeSize() > 0) gl_systemMessage.PopDayLineInfoMessage();
      while (gl_systemMessage.GetInnerSystemInformationDequeSize() > 0) gl_systemMessage.PopInnerSystemInformationMessage();
    }
  };

  TEST_F(CChinaMarketTest, TestInitialize) {
    CChinaStockPtr pStock = nullptr;
    EXPECT_GT(gl_pChinaStockMarket->GetTotalActiveStock(), 0);
    EXPECT_FALSE(gl_pChinaStockMarket->IsLoadSelectedStock());
    EXPECT_TRUE(gl_pChinaStockMarket->IsSystemReady());
    EXPECT_FALSE(gl_pChinaStockMarket->IsCurrentEditStockChanged());
    EXPECT_FALSE(gl_pChinaStockMarket->IsMarketOpened());
    EXPECT_FALSE(gl_pChinaStockMarket->IsCurrentStockChanged());
    EXPECT_EQ(gl_pChinaStockMarket->GetTotalAttackBuyAmount(), 0);
    EXPECT_EQ(gl_pChinaStockMarket->GetTotalAttackSellAmount(), 0);
    EXPECT_TRUE(gl_pChinaStockMarket->IsGetRTData());
    EXPECT_FALSE(gl_pChinaStockMarket->IsTodayTempRTDataLoaded());
    EXPECT_EQ(gl_pChinaStockMarket->GetCountDownSlowReadingRTData(), 3);
    EXPECT_EQ(gl_pChinaStockMarket->GetTotalStock(), 12000);   // 在全局变量gl_ChinaStockMarket初始化时就生成了全部股票代码池
    EXPECT_EQ(gl_pChinaStockMarket->GetTotalStockMapIndexSize(), 12000);
    pStock = gl_pChinaStockMarket->GetStock(0);
    EXPECT_STREQ(pStock->GetStockCode(), _T("sh600000"));
    EXPECT_EQ(pStock->GetMarket(), __SHANGHAI_MARKET__);
    pStock = gl_pChinaStockMarket->GetStock(6000);
    EXPECT_STREQ(pStock->GetStockCode(), _T("sz000000"));
    EXPECT_EQ(pStock->GetMarket(), __SHENZHEN_MARKET__);
    for (int i = 0; i < 12000; i++) {
      pStock = gl_pChinaStockMarket->GetStock(i);
      EXPECT_EQ(pStock->GetOffset(), i);
      EXPECT_TRUE(pStock->IsDayLineNeedUpdate()) << pStock->GetStockCode();
      EXPECT_FALSE(pStock->IsDayLineNeedProcess());
      EXPECT_FALSE(pStock->IsDayLineNeedSaving());
      if ((pStock->GetStockCode() >= _T("sh000000")) && (pStock->GetStockCode() <= _T("sh000999"))) {
        EXPECT_FALSE(pStock->IsNeedProcessRTData());
      }
      else if ((pStock->GetStockCode() >= _T("sz399000")) && (pStock->GetStockCode() <= _T("sz399999"))) {
        EXPECT_FALSE(pStock->IsNeedProcessRTData());
      }
      else EXPECT_TRUE(pStock->IsNeedProcessRTData());
    }
    EXPECT_EQ(gl_pChinaStockMarket->GetTotalStockIndex(_T("sh600000")), 0);
    EXPECT_EQ(gl_pChinaStockMarket->GetTotalStockIndex(_T("sz000000")), 6000);

    EXPECT_TRUE(gl_pChinaStockMarket->IsPermitResetMarket());
    EXPECT_EQ(gl_pChinaStockMarket->GetDayLineNeedProcessNumber(), 0);
    EXPECT_EQ(gl_pChinaStockMarket->GetDayLineNeedSaveNumber(), 0);
    EXPECT_TRUE(gl_pChinaStockMarket->IsUsingSinaRTDataReceiver());
    EXPECT_TRUE(gl_pChinaStockMarket->IsUsingTengxunRTDataReceiver());
    EXPECT_FALSE(gl_pChinaStockMarket->IsUsingNeteaseRTDataReceiver());

    EXPECT_EQ(gl_pChinaStockMarket->GetNeteaseDayLineDataInquiringIndex(), 0);
    EXPECT_EQ(gl_pChinaStockMarket->GetNeteaseRTDataInquiringIndex(), 0);
    EXPECT_EQ(gl_pChinaStockMarket->GetSinaRTDataInquiringIndex(), 0);
    EXPECT_EQ(gl_pChinaStockMarket->GetTengxunRTDataInquiringIndex(), 0);
  }

  TEST_F(CChinaMarketTest, TestClearUpdateStockCodeDBFlag) {
    EXPECT_FALSE(gl_pChinaStockMarket->IsDayLineDBUpdated());
    for (int i = 0; i < gl_pChinaStockMarket->GetTotalStock(); i++) {
      CChinaStockPtr pStock = gl_pChinaStockMarket->GetStock(i);
      pStock->SetDayLineDBUpdated(true);
    }
    EXPECT_TRUE(gl_pChinaStockMarket->IsDayLineDBUpdated());
    gl_pChinaStockMarket->ClearDayLineDBUpdatedFlag();
    EXPECT_FALSE(gl_pChinaStockMarket->IsDayLineDBUpdated());
  }

  TEST_F(CChinaMarketTest, TestGetTengxunInquiringStockStr) {
    CChinaStockPtr pStock = nullptr;
    gl_pChinaStockMarket->SetSystemReady(true);
    gl_pChinaStockMarket->ResetTengxunRTDataInquiringIndex();
    CString str = gl_pChinaStockMarket->GetTengxunInquiringStockStr(900, false);
    EXPECT_EQ(gl_pChinaStockMarket->GetTengxunRTDataInquiringIndex(), 899);
    str = gl_pChinaStockMarket->GetTengxunInquiringStockStr(900, false);
    CString strCompare, str2;
    pStock = gl_pChinaStockMarket->GetStock(899);
    strCompare = pStock->GetStockCode();
    str2 = str.Left(8);
    EXPECT_EQ(str2.Compare(strCompare), 0);
    str = gl_pChinaStockMarket->GetTengxunInquiringStockStr(900, false);
    EXPECT_EQ(gl_pChinaStockMarket->GetTengxunRTDataInquiringIndex(), 2697);
    pStock = gl_pChinaStockMarket->GetStock(1798);
    strCompare = pStock->GetStockCode();
    str2 = str.Left(8);
    EXPECT_EQ(str2.Compare(strCompare), 0);
    str = gl_pChinaStockMarket->GetTengxunInquiringStockStr(900, false);
    EXPECT_EQ(gl_pChinaStockMarket->GetTengxunRTDataInquiringIndex(), 3596);
    pStock = gl_pChinaStockMarket->GetStock(2697);
    strCompare = pStock->GetStockCode();
    str2 = str.Left(8);
    EXPECT_EQ(str2.Compare(strCompare), 0);
    str = gl_pChinaStockMarket->GetTengxunInquiringStockStr(900, false);
    EXPECT_EQ(gl_pChinaStockMarket->GetTengxunRTDataInquiringIndex(), 4495);
    pStock = gl_pChinaStockMarket->GetStock(3596);
    strCompare = pStock->GetStockCode();
    str2 = str.Left(8);
    EXPECT_EQ(str2.Compare(strCompare), 0);
    str = gl_pChinaStockMarket->GetTengxunInquiringStockStr(900, false);
    EXPECT_EQ(gl_pChinaStockMarket->GetTengxunRTDataInquiringIndex(), 5394);
    pStock = gl_pChinaStockMarket->GetStock(4495);
    strCompare = pStock->GetStockCode();
    str2 = str.Left(8);
    EXPECT_EQ(str2.Compare(strCompare), 0);
    str = gl_pChinaStockMarket->GetTengxunInquiringStockStr(900, false);
    EXPECT_EQ(gl_pChinaStockMarket->GetTengxunRTDataInquiringIndex(), 6293);
    pStock = gl_pChinaStockMarket->GetStock(5394);
    strCompare = pStock->GetStockCode();
    str2 = str.Left(8);
    EXPECT_EQ(str2.Compare(strCompare), 0);
    str = gl_pChinaStockMarket->GetTengxunInquiringStockStr(900, false);
    EXPECT_EQ(gl_pChinaStockMarket->GetTengxunRTDataInquiringIndex(), 7192);
    pStock = gl_pChinaStockMarket->GetStock(6293);
    strCompare = pStock->GetStockCode();
    str2 = str.Left(8);
    EXPECT_EQ(str2.Compare(strCompare), 0);
    str = gl_pChinaStockMarket->GetTengxunInquiringStockStr(900, false);
    EXPECT_EQ(gl_pChinaStockMarket->GetTengxunRTDataInquiringIndex(), 8091);
    pStock = gl_pChinaStockMarket->GetStock(7192);
    strCompare = pStock->GetStockCode();
    str2 = str.Left(8);
    EXPECT_EQ(str2.Compare(strCompare), 0);
    str = gl_pChinaStockMarket->GetTengxunInquiringStockStr(900, false);
    pStock = gl_pChinaStockMarket->GetStock(8091);
    strCompare = pStock->GetStockCode();
    str2 = str.Left(8);
    EXPECT_EQ(str2.Compare(strCompare), 0);
    str = gl_pChinaStockMarket->GetTengxunInquiringStockStr(900, false);
    pStock = gl_pChinaStockMarket->GetStock(8990);
    strCompare = pStock->GetStockCode();
    str2 = str.Left(8);
    EXPECT_EQ(str2.Compare(strCompare), 0);
    str = gl_pChinaStockMarket->GetTengxunInquiringStockStr(900, false);
    EXPECT_EQ(gl_pChinaStockMarket->GetTengxunRTDataInquiringIndex(), 10788);
    pStock = gl_pChinaStockMarket->GetStock(9889);
    strCompare = pStock->GetStockCode();
    str2 = str.Left(8);
    EXPECT_EQ(str2.Compare(strCompare), 0);
    str = gl_pChinaStockMarket->GetTengxunInquiringStockStr(900, false);
    EXPECT_EQ(gl_pChinaStockMarket->GetTengxunRTDataInquiringIndex(), 11687);
    pStock = gl_pChinaStockMarket->GetStock(10788);
    strCompare = pStock->GetStockCode();
    str2 = str.Left(8);
    EXPECT_EQ(str2.Compare(strCompare), 0);
    str = gl_pChinaStockMarket->GetTengxunInquiringStockStr(900, false);
    EXPECT_EQ(gl_pChinaStockMarket->GetTengxunRTDataInquiringIndex(), 0);
    pStock = gl_pChinaStockMarket->GetStock(11687);
    strCompare = pStock->GetStockCode();
    str2 = str.Left(8);
    EXPECT_EQ(str2.Compare(strCompare), 0);
    str = gl_pChinaStockMarket->GetTengxunInquiringStockStr(900, false);
    EXPECT_EQ(gl_pChinaStockMarket->GetTengxunRTDataInquiringIndex(), 899);
    pStock = gl_pChinaStockMarket->GetStock(0);
    strCompare = pStock->GetStockCode();
    str2 = str.Left(8);
    EXPECT_EQ(str2.Compare(strCompare), 0);
    gl_pChinaStockMarket->ResetTengxunRTDataInquiringIndex();
  }

  TEST_F(CChinaMarketTest, TestGetSinaInquiringStockStr2) {
    CChinaStockPtr pStock = nullptr;
    gl_pChinaStockMarket->ResetSinaRTDataInquiringIndex();
    CString str = gl_pChinaStockMarket->GetSinaInquiringStockStr(900, false);
    EXPECT_EQ(gl_pChinaStockMarket->GetSinaRTDataInquiringIndex(), 899);
    str = gl_pChinaStockMarket->GetSinaInquiringStockStr(900, false);
    EXPECT_EQ(gl_pChinaStockMarket->GetSinaRTDataInquiringIndex(), 1798);
    CString strCompare, str2;
    pStock = gl_pChinaStockMarket->GetStock(899);
    strCompare = pStock->GetStockCode();
    str2 = str.Left(8);
    EXPECT_EQ(str2.Compare(strCompare), 0);
    str = gl_pChinaStockMarket->GetSinaInquiringStockStr(900, false);
    pStock = gl_pChinaStockMarket->GetStock(1798);
    strCompare = pStock->GetStockCode();
    str2 = str.Left(8);
    EXPECT_EQ(str2.Compare(strCompare), 0);
    str = gl_pChinaStockMarket->GetSinaInquiringStockStr(900, false);
    pStock = gl_pChinaStockMarket->GetStock(2697);
    strCompare = pStock->GetStockCode();
    str2 = str.Left(8);
    EXPECT_EQ(str2.Compare(strCompare), 0);
    str = gl_pChinaStockMarket->GetSinaInquiringStockStr(900, false);
    pStock = gl_pChinaStockMarket->GetStock(3596);
    strCompare = pStock->GetStockCode();
    str2 = str.Left(8);
    EXPECT_EQ(str2.Compare(strCompare), 0);
    str = gl_pChinaStockMarket->GetSinaInquiringStockStr(900, false);
    pStock = gl_pChinaStockMarket->GetStock(4495);
    strCompare = pStock->GetStockCode();
    str2 = str.Left(8);
    EXPECT_EQ(str2.Compare(strCompare), 0);
    str = gl_pChinaStockMarket->GetSinaInquiringStockStr(900, false);
    pStock = gl_pChinaStockMarket->GetStock(5394);
    strCompare = pStock->GetStockCode();
    str2 = str.Left(8);
    EXPECT_EQ(str2.Compare(strCompare), 0);
    str = gl_pChinaStockMarket->GetSinaInquiringStockStr(900, false);
    pStock = gl_pChinaStockMarket->GetStock(6293);
    strCompare = pStock->GetStockCode();
    str2 = str.Left(8);
    EXPECT_EQ(str2.Compare(strCompare), 0);
    str = gl_pChinaStockMarket->GetSinaInquiringStockStr(900, false);
    pStock = gl_pChinaStockMarket->GetStock(7192);
    strCompare = pStock->GetStockCode();
    str2 = str.Left(8);
    EXPECT_EQ(str2.Compare(strCompare), 0);
    str = gl_pChinaStockMarket->GetSinaInquiringStockStr(900, false);
    pStock = gl_pChinaStockMarket->GetStock(8091);
    strCompare = pStock->GetStockCode();
    str2 = str.Left(8);
    EXPECT_EQ(str2.Compare(strCompare), 0);
    str = gl_pChinaStockMarket->GetSinaInquiringStockStr(900, false);
    pStock = gl_pChinaStockMarket->GetStock(8990);
    strCompare = pStock->GetStockCode();
    str2 = str.Left(8);
    EXPECT_EQ(str2.Compare(strCompare), 0);
    str = gl_pChinaStockMarket->GetSinaInquiringStockStr(900, false);
    pStock = gl_pChinaStockMarket->GetStock(9889);
    strCompare = pStock->GetStockCode();
    str2 = str.Left(8);
    EXPECT_EQ(str2.Compare(strCompare), 0);
    str = gl_pChinaStockMarket->GetSinaInquiringStockStr(900, false);
    pStock = gl_pChinaStockMarket->GetStock(10788);
    strCompare = pStock->GetStockCode();
    str2 = str.Left(8);
    EXPECT_EQ(str2.Compare(strCompare), 0);
    str = gl_pChinaStockMarket->GetSinaInquiringStockStr(900, false);
    EXPECT_EQ(gl_pChinaStockMarket->GetSinaRTDataInquiringIndex(), 0);
    pStock = gl_pChinaStockMarket->GetStock(11687);
    strCompare = pStock->GetStockCode();
    str2 = str.Left(8);
    EXPECT_EQ(str2.Compare(strCompare), 0);
    str = gl_pChinaStockMarket->GetSinaInquiringStockStr(900, false);
    pStock = gl_pChinaStockMarket->GetStock(0);
    strCompare = pStock->GetStockCode();
    str2 = str.Left(8);
    EXPECT_EQ(str2.Compare(strCompare), 0);
  }

  TEST_F(CChinaMarketTest, TestGetInquiringStockStr) {
    CString str2;

    gl_pChinaStockMarket->SetSystemReady(true);
    gl_pChinaStockMarket->ResetSinaRTDataInquiringIndex();
    CString str = gl_pChinaStockMarket->GetSinaInquiringStockStr(900);
    str = gl_pChinaStockMarket->GetSinaInquiringStockStr(900);
    str = gl_pChinaStockMarket->GetSinaInquiringStockStr(900);
    str = gl_pChinaStockMarket->GetSinaInquiringStockStr(900);
    str = gl_pChinaStockMarket->GetSinaInquiringStockStr(900);
    if (gl_pChinaStockMarket->GetSinaRTDataInquiringIndex() == 0) {
      str = gl_pChinaStockMarket->GetSinaInquiringStockStr(900); // 目前不到五千个活跃股票，故而六次即可遍历一次
      str2 = str.Left(9);
      EXPECT_EQ(str2, _T("sh600000,"));
    }
    else {
      str = gl_pChinaStockMarket->GetSinaInquiringStockStr(900);
      str = gl_pChinaStockMarket->GetSinaInquiringStockStr(900);
      str2 = str.Left(9);
      EXPECT_STREQ(str2, _T("sh600000,"));
    }
  }

  TEST_F(CChinaMarketTest, TestGetNeteaseInquiringStockStr) {
    CChinaStockPtr pStock = nullptr;
    gl_pChinaStockMarket->SetSystemReady(true);
    gl_pChinaStockMarket->ResetNeteaseRTDataInquiringIndex();
    CString str = gl_pChinaStockMarket->GetNeteaseInquiringStockStr(900, false);
    EXPECT_EQ(gl_pChinaStockMarket->GetNeteaseRTDataInquiringIndex(), 899);
    str = gl_pChinaStockMarket->GetNeteaseInquiringStockStr(900, false);
    CString strCompare, str2;
    pStock = gl_pChinaStockMarket->GetStock(899);
    strCompare = pStock->GetStockCode();
    str2 = str.Left(7);
    EXPECT_STREQ(str2.Right(6), strCompare.Right(6));
    str = gl_pChinaStockMarket->GetNeteaseInquiringStockStr(900, false);
    pStock = gl_pChinaStockMarket->GetStock(1798);
    strCompare = pStock->GetStockCode();
    str2 = str.Left(7);
    EXPECT_STREQ(str2.Right(6), strCompare.Right(6));
    str = gl_pChinaStockMarket->GetNeteaseInquiringStockStr(900, false);
    pStock = gl_pChinaStockMarket->GetStock(2697);
    strCompare = pStock->GetStockCode();
    str2 = str.Left(7);
    EXPECT_STREQ(str2.Right(6), strCompare.Right(6));
    str = gl_pChinaStockMarket->GetNeteaseInquiringStockStr(900, false);
    pStock = gl_pChinaStockMarket->GetStock(3596);
    strCompare = pStock->GetStockCode();
    str2 = str.Left(7);
    EXPECT_STREQ(str2.Right(6), strCompare.Right(6));
    str = gl_pChinaStockMarket->GetNeteaseInquiringStockStr(900, false);
    pStock = gl_pChinaStockMarket->GetStock(4495);
    strCompare = pStock->GetStockCode();
    str2 = str.Left(7);
    EXPECT_STREQ(str2.Right(6), strCompare.Right(6));
    str = gl_pChinaStockMarket->GetNeteaseInquiringStockStr(900, false);
    pStock = gl_pChinaStockMarket->GetStock(5394);
    strCompare = pStock->GetStockCode();
    str2 = str.Left(7);
    EXPECT_STREQ(str2.Right(6), strCompare.Right(6));
    str = gl_pChinaStockMarket->GetNeteaseInquiringStockStr(900, false);
    pStock = gl_pChinaStockMarket->GetStock(6293);
    strCompare = pStock->GetStockCode();
    str2 = str.Left(7);
    EXPECT_STREQ(str2.Right(6), strCompare.Right(6));
    str = gl_pChinaStockMarket->GetNeteaseInquiringStockStr(900, false);
    pStock = gl_pChinaStockMarket->GetStock(7192);
    strCompare = pStock->GetStockCode();
    str2 = str.Left(7);
    EXPECT_STREQ(str2.Right(6), strCompare.Right(6));
    str = gl_pChinaStockMarket->GetNeteaseInquiringStockStr(900, false);
    pStock = gl_pChinaStockMarket->GetStock(8091);
    strCompare = pStock->GetStockCode();
    str2 = str.Left(7);
    EXPECT_STREQ(str2.Right(6), strCompare.Right(6));
    str = gl_pChinaStockMarket->GetNeteaseInquiringStockStr(900, false);
    pStock = gl_pChinaStockMarket->GetStock(8990);
    strCompare = pStock->GetStockCode();
    str2 = str.Left(7);
    EXPECT_STREQ(str2.Right(6), strCompare.Right(6));
    str = gl_pChinaStockMarket->GetNeteaseInquiringStockStr(900, false);
    pStock = gl_pChinaStockMarket->GetStock(9889);
    strCompare = pStock->GetStockCode();
    str2 = str.Left(7);
    EXPECT_STREQ(str2.Right(6), strCompare.Right(6));
    str = gl_pChinaStockMarket->GetNeteaseInquiringStockStr(900, false);
    pStock = gl_pChinaStockMarket->GetStock(10788);
    strCompare = pStock->GetStockCode();
    str2 = str.Left(7);
    EXPECT_STREQ(str2.Right(6), strCompare.Right(6));
    str = gl_pChinaStockMarket->GetNeteaseInquiringStockStr(900, false);
    EXPECT_EQ(gl_pChinaStockMarket->GetNeteaseRTDataInquiringIndex(), 0);
    pStock = gl_pChinaStockMarket->GetStock(11687);
    strCompare = pStock->GetStockCode();
    str2 = str.Left(7);
    EXPECT_STREQ(str2.Right(6), strCompare.Right(6));
    str = gl_pChinaStockMarket->GetNeteaseInquiringStockStr(900, false);
    pStock = gl_pChinaStockMarket->GetStock(0);
    strCompare = pStock->GetStockCode();
    str2 = str.Left(7);
    EXPECT_STREQ(str2.Right(6), strCompare.Right(6));
    gl_pChinaStockMarket->ResetNeteaseRTDataInquiringIndex();
    gl_pChinaStockMarket->ResetSinaRTDataInquiringIndex();
  }

  TEST_F(CChinaMarketTest, TestGetNeteaseDayLineInquiringStr) {
    CString str;
    CChinaStockPtr pStock = gl_pChinaStockMarket->GetStock(0);
    EXPECT_TRUE(pStock->IsDayLineNeedUpdate()) << _T("测试时使用teststock数据库，此数据库比较旧，最后更新时间不是昨日，故而活跃股票也需要更新日线");
    long lDay = pStock->GetDayLineEndDay();
    pStock->SetDayLineEndDay(gl_pChinaStockMarket->GetDay());
    pStock->SetDayLineNeedUpdate(false);
    pStock->SetDayLineEndDay(lDay); // 恢复原状
    pStock = gl_pChinaStockMarket->GetStock(1);
    EXPECT_TRUE(pStock->IsDayLineNeedUpdate());
    pStock = gl_pChinaStockMarket->GetStock(2);
    EXPECT_TRUE(pStock->IsDayLineNeedUpdate());
    str = gl_pChinaStockMarket->CreateNeteaseDayLineInquiringStr();
    EXPECT_TRUE(str.GetLength() > 0);
    EXPECT_STREQ(str, _T("0600001")) << _T("第一个股票已设置为无需查询日线历史数据");
    pStock = gl_pChinaStockMarket->GetStock(1);
    EXPECT_FALSE(pStock->IsDayLineNeedUpdate());
    pStock = gl_pChinaStockMarket->GetStock(2);
    EXPECT_TRUE(pStock->IsDayLineNeedUpdate());
    long lIPOStatus = pStock->GetIPOStatus();
    pStock->SetIPOStatus(__STOCK_NULL__);
    str = gl_pChinaStockMarket->CreateNeteaseDayLineInquiringStr();
    EXPECT_TRUE(str.GetLength() > 0);
    EXPECT_STREQ(str, _T("0600003")) << _T("第三个股票设置为无效股票");
    pStock->SetIPOStatus(lIPOStatus); // 恢复原状
    pStock = gl_pChinaStockMarket->GetStock(3);
    EXPECT_FALSE(pStock->IsDayLineNeedUpdate());
    pStock = gl_pChinaStockMarket->GetStock(4);
    lDay = pStock->GetDayLineEndDay();
    pStock->SetDayLineEndDay(gl_pChinaStockMarket->GetDay());
    EXPECT_TRUE(pStock->IsDayLineNeedUpdate()) << _T("标识尚未更新");
    str = gl_pChinaStockMarket->CreateNeteaseDayLineInquiringStr();
    EXPECT_TRUE(str.GetLength() > 0);
    EXPECT_STREQ(str, _T("0600005")) << _T("0600004的日线结束日已设置为最新，故而无需再更新日线");
    pStock->SetDayLineEndDay(lDay); // 恢复原状。
    pStock = gl_pChinaStockMarket->GetStock(5);
    EXPECT_FALSE(pStock->IsDayLineNeedUpdate());
    pStock = gl_pChinaStockMarket->GetStock(4);
    EXPECT_FALSE(pStock->IsDayLineNeedUpdate()) << _T("标识在查询下载股票时更新了");
    // 恢复原状
    pStock = gl_pChinaStockMarket->GetStock(0);
    pStock->SetDayLineEndDay(lDay);
    for (int i = 0; i < 12000; i++) {
      pStock = gl_pChinaStockMarket->GetStock(i);
      if (!pStock->IsDayLineNeedUpdate()) {
        pStock->SetDayLineNeedUpdate(true);
      }
    }
  }

  TEST_F(CChinaMarketTest, TestIsAStock) {
    CChinaStockPtr pstock = make_shared<CChinaStock>();
    pstock->SetStockCode(_T("sh600000"));
    EXPECT_TRUE(gl_pChinaStockMarket->IsAStock(pstock));
    pstock->SetStockCode(_T("ss600000"));
    EXPECT_FALSE(gl_pChinaStockMarket->IsAStock(pstock));
    pstock->SetStockCode(_T("sz000001"));
    EXPECT_TRUE(gl_pChinaStockMarket->IsAStock(pstock));
    pstock->SetStockCode(_T("sz10001"));
    EXPECT_FALSE(gl_pChinaStockMarket->IsAStock(pstock));
  }

  TEST_F(CChinaMarketTest, TestIsAStock2) {
    EXPECT_TRUE(gl_pChinaStockMarket->IsAStock(_T("sh600000")));
    EXPECT_TRUE(gl_pChinaStockMarket->IsAStock(_T("sh601198")));
    EXPECT_FALSE(gl_pChinaStockMarket->IsAStock(_T("sh602102")));
    EXPECT_FALSE(gl_pChinaStockMarket->IsAStock(_T("sh603604")));
    EXPECT_FALSE(gl_pChinaStockMarket->IsAStock(_T("sh604604")));
    EXPECT_FALSE(gl_pChinaStockMarket->IsAStock(_T("sh605604")));
    EXPECT_FALSE(gl_pChinaStockMarket->IsAStock(_T("sh606604")));
    EXPECT_FALSE(gl_pChinaStockMarket->IsAStock(_T("sh607604")));
    EXPECT_FALSE(gl_pChinaStockMarket->IsAStock(_T("sh608604")));
    EXPECT_FALSE(gl_pChinaStockMarket->IsAStock(_T("sh609604")));

    EXPECT_FALSE(gl_pChinaStockMarket->IsAStock(_T("sz600000")));
    EXPECT_FALSE(gl_pChinaStockMarket->IsAStock(_T("ss000001")));
    EXPECT_FALSE(gl_pChinaStockMarket->IsAStock(_T("az000001")));
    EXPECT_FALSE(gl_pChinaStockMarket->IsAStock(_T("sz200001")));
    EXPECT_TRUE(gl_pChinaStockMarket->IsAStock(_T("sz000001")));
    EXPECT_TRUE(gl_pChinaStockMarket->IsAStock(_T("sz002389")));
    EXPECT_FALSE(gl_pChinaStockMarket->IsAStock(_T("sz003389")));
    EXPECT_FALSE(gl_pChinaStockMarket->IsAStock(_T("sz004389")));
    EXPECT_FALSE(gl_pChinaStockMarket->IsAStock(_T("sz005389")));
    EXPECT_FALSE(gl_pChinaStockMarket->IsAStock(_T("sz006389")));
    EXPECT_FALSE(gl_pChinaStockMarket->IsAStock(_T("sz007389")));
    EXPECT_FALSE(gl_pChinaStockMarket->IsAStock(_T("sz008389")));
    EXPECT_FALSE(gl_pChinaStockMarket->IsAStock(_T("sz009389")));
    EXPECT_FALSE(gl_pChinaStockMarket->IsAStock(_T("sz001389")));
  }

  TEST_F(CChinaMarketTest, TestIsStock) {
    EXPECT_GT(gl_pChinaStockMarket->GetTotalActiveStock(), 1);
    EXPECT_TRUE(gl_pChinaStockMarket->IsStock(_T("sh600000")));
    EXPECT_FALSE(gl_pChinaStockMarket->IsStock(_T("sh60000")));
  }

  TEST_F(CChinaMarketTest, TestGetStockName) {
    //未实现.由于stockName存储时使用的是UniCode制式，而本系统默认是Ansi制式，导致无法进行字符串对比。暂时不进行测试了。
    //EXPECT_STREQ(gl_pChinaStockMarket->GetStockName(_T("sh600000")), _T("浦发银行"));
    EXPECT_STREQ(gl_pChinaStockMarket->GetStockName(_T("sh60000")), _T("")); // 没找到返回空字符串
  }

  TEST_F(CChinaMarketTest, TestGetStockIndex) {
    long lIndex = -2;
    EXPECT_TRUE(gl_pChinaStockMarket->GetStockIndex(_T("sh600000"), lIndex));
    EXPECT_STREQ(gl_pChinaStockMarket->GetStock(lIndex)->GetStockCode(), _T("sh600000"));
    EXPECT_FALSE(gl_pChinaStockMarket->GetStockIndex(_T("sh60000"), lIndex));
    EXPECT_EQ(lIndex, -1);
  }

  TEST_F(CChinaMarketTest, TestIncreaseTotalActiveStock) {
    long l = gl_pChinaStockMarket->GetTotalActiveStock();
    gl_pChinaStockMarket->IncreaseActiveStockNumber();
    EXPECT_EQ(gl_pChinaStockMarket->GetTotalActiveStock(), l + 1);
  }

  TEST_F(CChinaMarketTest, TestGetStockCode) {
    EXPECT_EQ(gl_pChinaStockMarket->GetStock(_T("sh66000")), nullptr);
    EXPECT_FALSE(gl_pChinaStockMarket->GetStock(_T("sh600001")) == nullptr);

    EXPECT_EQ(gl_pChinaStockMarket->GetStock(-1), nullptr);
    EXPECT_EQ(gl_pChinaStockMarket->GetStock(12000), nullptr);
    EXPECT_FALSE(gl_pChinaStockMarket->GetStock(0) == nullptr);
    EXPECT_FALSE(gl_pChinaStockMarket->GetStock(11999) == nullptr);
  }

  TEST_F(CChinaMarketTest, TestGetCurrentStock) {
    CChinaStockPtr pStock = gl_pChinaStockMarket->GetStock(7);
    CChinaStockPtr pStock2 = gl_pChinaStockMarket->GetStock(4);

    gl_pChinaStockMarket->SetCurrentStock(pStock);
    EXPECT_EQ(gl_pChinaStockMarket->GetCurrentStock(), pStock);
    EXPECT_TRUE(pStock->IsRecordRTData());
    EXPECT_TRUE(gl_pChinaStockMarket->IsCurrentStockChanged());
    gl_pChinaStockMarket->SetCurrentStockChanged(false);
    gl_pChinaStockMarket->SetCurrentStock(pStock);
    EXPECT_FALSE(gl_pChinaStockMarket->IsCurrentStockChanged());
    EXPECT_FALSE(pStock2->IsRecordRTData());
    gl_pChinaStockMarket->SetCurrentStock(pStock2);
    EXPECT_TRUE(gl_pChinaStockMarket->IsCurrentStockChanged());
    EXPECT_TRUE(pStock2->IsRecordRTData());
    gl_pChinaStockMarket->SetCurrentStock(_T("sh600000"));
    EXPECT_STREQ(gl_pChinaStockMarket->GetCurrentStock()->GetStockCode(), _T("sh600000"));
  }

  TEST_F(CChinaMarketTest, TestClearDayLineContainer) {
    EXPECT_TRUE(gl_pChinaStockMarket->ClearDayLineContainer());
  }

  TEST_F(CChinaMarketTest, TestMarketReady) {
    gl_pChinaStockMarket->SetSystemReady(true);
    EXPECT_TRUE(gl_pChinaStockMarket->IsSystemReady());
    gl_pChinaStockMarket->SetSystemReady(false);
    EXPECT_FALSE(gl_pChinaStockMarket->IsSystemReady());
  }

  TEST_F(CChinaMarketTest, TestIsTodayStockProcessed) {
    gl_pChinaStockMarket->SetTodayStockProcessed(true);
    EXPECT_TRUE(gl_pChinaStockMarket->IsTodayStockProcessed());
    gl_pChinaStockMarket->SetTodayStockProcessed(false);
    EXPECT_FALSE(gl_pChinaStockMarket->IsTodayStockProcessed());
  }

  TEST_F(CChinaMarketTest, TestIsDayLineNeedSaving) {
    EXPECT_FALSE(gl_pChinaStockMarket->IsDayLineNeedSaving());
    CChinaStockPtr pStock = gl_pChinaStockMarket->GetStock(0);
    pStock->SetDayLineNeedSaving(true);
    EXPECT_TRUE(gl_pChinaStockMarket->IsDayLineNeedSaving());
    pStock->SetDayLineNeedSaving(false);
  }

  TEST_F(CChinaMarketTest, TestIsDayLineNeedUpdate) {
    CChinaStockPtr pStock = nullptr;
    EXPECT_TRUE(gl_pChinaStockMarket->IsDayLineNeedUpdate());
    for (int i = 0; i < gl_pChinaStockMarket->GetTotalStock(); i++) {
      pStock = gl_pChinaStockMarket->GetStock(i);
      if (pStock->IsDayLineNeedUpdate()) pStock->SetDayLineNeedUpdate(false);
    }
    EXPECT_FALSE(gl_pChinaStockMarket->IsDayLineNeedUpdate());
    for (int i = 0; i < gl_pChinaStockMarket->GetTotalStock(); i++) {
      pStock = gl_pChinaStockMarket->GetStock(i);
      pStock->SetDayLineNeedUpdate(true);
    }
  }

  TEST_F(CChinaMarketTest, TestIsLoadSelectedStock) {
    EXPECT_FALSE(gl_pChinaStockMarket->IsLoadSelectedStock());
    gl_pChinaStockMarket->SetLoadSelectedStock(true);
    EXPECT_TRUE(gl_pChinaStockMarket->IsLoadSelectedStock());
    gl_pChinaStockMarket->SetLoadSelectedStock(false);
    EXPECT_FALSE(gl_pChinaStockMarket->IsLoadSelectedStock());
  }

  TEST_F(CChinaMarketTest, TestIsRecordingRTData) {
    EXPECT_FALSE(gl_pChinaStockMarket->IsRecordingRTData());
    gl_pChinaStockMarket->SetRecordRTData(true);
    EXPECT_TRUE(gl_pChinaStockMarket->IsRecordingRTData());
    gl_pChinaStockMarket->SetRecordRTData(false);
    EXPECT_FALSE(gl_pChinaStockMarket->IsRecordingRTData());
  }

  TEST_F(CChinaMarketTest, TestSetUpdateStockCodeDB) {
    EXPECT_FALSE(gl_pChinaStockMarket->IsUpdateStockCodeDB());
    gl_pChinaStockMarket->SetUpdateStockCodeDB(true);
    EXPECT_TRUE(gl_pChinaStockMarket->IsUpdateStockCodeDB());
    gl_pChinaStockMarket->SetUpdateStockCodeDB(false);
    EXPECT_FALSE(gl_pChinaStockMarket->IsUpdateStockCodeDB());
  }
  TEST_F(CChinaMarketTest, TestSetUpdateOptionDB) {
    EXPECT_FALSE(gl_pChinaStockMarket->IsUpdateOptionDB());
    gl_pChinaStockMarket->SetUpdateOptionDB(true);
    EXPECT_TRUE(gl_pChinaStockMarket->IsUpdateOptionDB());
    gl_pChinaStockMarket->SetUpdateOptionDB(false);
    EXPECT_FALSE(gl_pChinaStockMarket->IsUpdateOptionDB());
  }

  TEST_F(CChinaMarketTest, TestSetRTDataSetCleared) {
    EXPECT_FALSE(gl_pChinaStockMarket->IsRTDataSetCleared());
    gl_pChinaStockMarket->SetRTDataSetCleared(true);
    EXPECT_TRUE(gl_pChinaStockMarket->IsRTDataSetCleared());
    gl_pChinaStockMarket->SetRTDataSetCleared(false);
    EXPECT_FALSE(gl_pChinaStockMarket->IsRTDataSetCleared());
  }

  TEST_F(CChinaMarketTest, TestTaskResetMarket) {
    tm tm_;
    tm_.tm_wday = 1; // 星期一
    gl_pChinaStockMarket->__TEST_SetMarketTM(tm_);
    EXPECT_TRUE(gl_pChinaStockMarket->IsPermitResetMarket());
    EXPECT_TRUE(gl_pChinaStockMarket->IsResetMarket());
    gl_pChinaStockMarket->SetResetMarket(false);
    gl_pChinaStockMarket->TaskResetMarket(91259);
    EXPECT_FALSE(gl_pChinaStockMarket->IsResetMarket());
    gl_pChinaStockMarket->TaskResetMarket(91400);
    EXPECT_FALSE(gl_pChinaStockMarket->IsResetMarket()) << _T("第一次重启市场，其结束时间必须在9:14之前，这样才能保证只运行了一次（此函数必须每分钟调度一次");
    gl_pChinaStockMarket->SetSystemReady(true);
    gl_pChinaStockMarket->TaskResetMarket(91300);
    EXPECT_TRUE(gl_pChinaStockMarket->IsResetMarket());
    EXPECT_TRUE(gl_pChinaStockMarket->IsPermitResetMarket());
    EXPECT_FALSE(gl_pChinaStockMarket->IsSystemReady());
  }

  TEST_F(CChinaMarketTest, TestTaskResetMarket2) {
    tm tm_;
    tm_.tm_wday = 0; // 星期日
    gl_pChinaStockMarket->__TEST_SetMarketTM(tm_);
    EXPECT_TRUE(gl_pChinaStockMarket->IsPermitResetMarket());
    gl_pChinaStockMarket->SetResetMarket(false);
    gl_pChinaStockMarket->TaskResetMarket(91259);
    EXPECT_FALSE(gl_pChinaStockMarket->IsResetMarket());
    gl_pChinaStockMarket->TaskResetMarket(91401);
    EXPECT_FALSE(gl_pChinaStockMarket->IsResetMarket());
    gl_pChinaStockMarket->TaskResetMarket(91300);
    EXPECT_FALSE(gl_pChinaStockMarket->IsResetMarket());
    EXPECT_TRUE(gl_pChinaStockMarket->IsPermitResetMarket());
  }

  TEST_F(CChinaMarketTest, TestTaskResetMarketAgain) {
    tm tm_;
    tm_.tm_wday = 1;
    gl_pChinaStockMarket->SetSystemReady(false);
    gl_pChinaStockMarket->__TEST_SetMarketTM(tm_);
    EXPECT_TRUE(gl_pChinaStockMarket->IsPermitResetMarket());
    EXPECT_FALSE(gl_pChinaStockMarket->IsSystemReady());
    EXPECT_TRUE(gl_pChinaStockMarket->IsResetMarket());
    EXPECT_TRUE(gl_pChinaStockMarket->TaskResetMarketAgain(92459));
    EXPECT_FALSE(gl_pChinaStockMarket->IsSystemReady());
    EXPECT_TRUE(gl_pChinaStockMarket->IsResetMarket());
    EXPECT_TRUE(gl_pChinaStockMarket->TaskResetMarketAgain(93001));
    EXPECT_FALSE(gl_pChinaStockMarket->IsSystemReady());
    EXPECT_FALSE(gl_pChinaStockMarket->IsPermitResetMarket());
    EXPECT_TRUE(gl_pChinaStockMarket->IsResetMarket());
    gl_pChinaStockMarket->SetResetMarket(false);
    gl_pChinaStockMarket->SetSystemReady(false);
    gl_pChinaStockMarket->SetPermitResetMarket(true);
    EXPECT_TRUE(gl_pChinaStockMarket->TaskResetMarketAgain(92500));
    EXPECT_FALSE(gl_pChinaStockMarket->IsSystemReady());
    EXPECT_FALSE(gl_pChinaStockMarket->IsPermitResetMarket());
    EXPECT_TRUE(gl_pChinaStockMarket->IsResetMarket());
  }

  TEST_F(CChinaMarketTest, TestTaskClearChoicedRTDataSet) {
    EXPECT_FALSE(gl_pChinaStockMarket->IsRTDataSetCleared());
    gl_pChinaStockMarket->TaskClearChoicedRTDataSet(92900);
    EXPECT_FALSE(gl_pChinaStockMarket->IsRTDataSetCleared());
    gl_pChinaStockMarket->TaskClearChoicedRTDataSet(92901);
    EXPECT_TRUE(gl_pChinaStockMarket->IsRTDataSetCleared());
  }

  TEST_F(CChinaMarketTest, TestIsCurrentEditStockChanged) {
    EXPECT_FALSE(gl_pChinaStockMarket->IsCurrentEditStockChanged());
    gl_pChinaStockMarket->SetCurrentEditStockChanged(true);
    EXPECT_TRUE(gl_pChinaStockMarket->IsCurrentEditStockChanged());
    gl_pChinaStockMarket->SetCurrentEditStockChanged(false);
    EXPECT_FALSE(gl_pChinaStockMarket->IsCurrentEditStockChanged());
  }

  TEST_F(CChinaMarketTest, TestCheckMarketOpen) {
    tm tm_;
    tm_.tm_wday = 1;
    gl_pChinaStockMarket->__TEST_SetMarketTM(tm_);
    EXPECT_FALSE(gl_pChinaStockMarket->TaskCheckMarketOpen(92800));
    EXPECT_FALSE(gl_pChinaStockMarket->IsMarketOpened());
    EXPECT_TRUE(gl_pChinaStockMarket->TaskCheckMarketOpen(92801));
    EXPECT_TRUE(gl_pChinaStockMarket->IsMarketOpened());
    EXPECT_TRUE(gl_pChinaStockMarket->TaskCheckMarketOpen(150259));
    EXPECT_TRUE(gl_pChinaStockMarket->IsMarketOpened());
    EXPECT_FALSE(gl_pChinaStockMarket->TaskCheckMarketOpen(150300));
    tm_.tm_wday = 0;
    gl_pChinaStockMarket->__TEST_SetMarketTM(tm_);
    EXPECT_FALSE(gl_pChinaStockMarket->TaskCheckMarketOpen(92859));
    EXPECT_FALSE(gl_pChinaStockMarket->IsMarketOpened());
    EXPECT_FALSE(gl_pChinaStockMarket->TaskCheckMarketOpen(92900));
    EXPECT_FALSE(gl_pChinaStockMarket->IsMarketOpened());
    EXPECT_FALSE(gl_pChinaStockMarket->TaskCheckMarketOpen(150259));
    EXPECT_FALSE(gl_pChinaStockMarket->IsMarketOpened());
    EXPECT_FALSE(gl_pChinaStockMarket->TaskCheckMarketOpen(150300));
    EXPECT_FALSE(gl_pChinaStockMarket->IsMarketOpened());
  }

  TEST_F(CChinaMarketTest, TestCheckStartReceivingData) {
    tm tm_;
    tm_.tm_wday = 1;
    gl_pChinaStockMarket->__TEST_SetMarketTM(tm_);
    EXPECT_FALSE(gl_pChinaStockMarket->TaskCheckStartReceivingData(91159));
    EXPECT_FALSE(gl_pChinaStockMarket->IsStartReceivingData());
    EXPECT_TRUE(gl_pChinaStockMarket->TaskCheckStartReceivingData(91200));
    EXPECT_TRUE(gl_pChinaStockMarket->IsStartReceivingData());
    EXPECT_TRUE(gl_pChinaStockMarket->TaskCheckStartReceivingData(113500));
    EXPECT_TRUE(gl_pChinaStockMarket->IsStartReceivingData());
    EXPECT_FALSE(gl_pChinaStockMarket->TaskCheckStartReceivingData(113501));
    EXPECT_FALSE(gl_pChinaStockMarket->IsStartReceivingData());
    EXPECT_FALSE(gl_pChinaStockMarket->TaskCheckStartReceivingData(125459));
    EXPECT_FALSE(gl_pChinaStockMarket->IsStartReceivingData());
    EXPECT_TRUE(gl_pChinaStockMarket->TaskCheckStartReceivingData(125500));
    EXPECT_TRUE(gl_pChinaStockMarket->IsStartReceivingData());
    EXPECT_TRUE(gl_pChinaStockMarket->TaskCheckStartReceivingData(150630));
    EXPECT_TRUE(gl_pChinaStockMarket->IsStartReceivingData());
    EXPECT_FALSE(gl_pChinaStockMarket->TaskCheckStartReceivingData(150631));
    tm_.tm_wday = 0;
    gl_pChinaStockMarket->__TEST_SetMarketTM(tm_);
    EXPECT_FALSE(gl_pChinaStockMarket->TaskCheckStartReceivingData(91459));
    EXPECT_FALSE(gl_pChinaStockMarket->IsStartReceivingData());
    EXPECT_FALSE(gl_pChinaStockMarket->TaskCheckStartReceivingData(91500));
    EXPECT_FALSE(gl_pChinaStockMarket->IsStartReceivingData());
    EXPECT_FALSE(gl_pChinaStockMarket->TaskCheckStartReceivingData(113459));
    EXPECT_FALSE(gl_pChinaStockMarket->IsStartReceivingData());
    EXPECT_FALSE(gl_pChinaStockMarket->TaskCheckStartReceivingData(113500));
    EXPECT_FALSE(gl_pChinaStockMarket->IsStartReceivingData());
    EXPECT_FALSE(gl_pChinaStockMarket->TaskCheckStartReceivingData(125459));
    EXPECT_FALSE(gl_pChinaStockMarket->IsStartReceivingData());
    EXPECT_FALSE(gl_pChinaStockMarket->TaskCheckStartReceivingData(125500));
    EXPECT_FALSE(gl_pChinaStockMarket->IsStartReceivingData());
    EXPECT_FALSE(gl_pChinaStockMarket->TaskCheckStartReceivingData(150630));
    EXPECT_FALSE(gl_pChinaStockMarket->IsStartReceivingData());
    EXPECT_FALSE(gl_pChinaStockMarket->TaskCheckStartReceivingData(150631));
    EXPECT_FALSE(gl_pChinaStockMarket->IsStartReceivingData());
  }

  TEST_F(CChinaMarketTest, TestTaskUpdateStockCodeDB) {
    gl_pChinaStockMarket->SetSaveDayLine(true);
    gl_pChinaStockMarket->SetDayLineNeedSaveNumber(1);
    gl_pChinaStockMarket->SetDayLineNeedProcessNumber(1);
    gl_pChinaStockMarket->SetDayLineNeedUpdateNumber(1);
    gl_pChinaStockMarket->TaskCheckDayLineDB();
    EXPECT_TRUE(gl_pChinaStockMarket->IsSaveDayLine());
    gl_pChinaStockMarket->SetDayLineNeedProcessNumber(0);
    gl_pChinaStockMarket->TaskCheckDayLineDB();
    EXPECT_TRUE(gl_pChinaStockMarket->IsSaveDayLine());
    gl_pChinaStockMarket->SetDayLineNeedSaveNumber(0);
    gl_pChinaStockMarket->TaskCheckDayLineDB();
    EXPECT_TRUE(gl_pChinaStockMarket->IsSaveDayLine());
    gl_pChinaStockMarket->SetDayLineNeedUpdateNumber(0);
    gl_pChinaStockMarket->TaskCheckDayLineDB();
    EXPECT_FALSE(gl_pChinaStockMarket->IsSaveDayLine());
  }

  TEST_F(CChinaMarketTest, TestIncreaseNeteaseDayLineInquiringIndex) {
    long k = 0;
    EXPECT_EQ(gl_pChinaStockMarket->GetTotalStock(), 12000);
    int i = gl_pChinaStockMarket->IncreaseStockInquiringIndex(k);
    EXPECT_EQ(i, 1);
    EXPECT_EQ(k, 1);
    i = gl_pChinaStockMarket->IncreaseStockInquiringIndex(k);
    EXPECT_EQ(i, 2);
    EXPECT_EQ(k, 2);
    k = 11999;
    i = gl_pChinaStockMarket->IncreaseStockInquiringIndex(k);
    EXPECT_EQ(k, 0);
    EXPECT_EQ(i, 0);
  }

  TEST_F(CChinaMarketTest, TestStepToActiveStockIndex) {
    long i = 0; //
    gl_pChinaStockMarket->StepToActiveStockIndex(i);
    EXPECT_EQ(i, 0); // sh600000为活跃股票
    i++;
    gl_pChinaStockMarket->StepToActiveStockIndex(i);
    EXPECT_EQ(i, 4); // sh600004为活跃股票，之间三个是退市股
    for (int j = 1; j < 4; j++) {
      EXPECT_FALSE(gl_pChinaStockMarket->GetStock(j)->IsActive());
    }
  }

  TEST_F(CChinaMarketTest, TestProcessTodayStock) {
    tm tm_;
    tm_.tm_wday = 1; // 星期一
    gl_pChinaStockMarket->__TEST_SetMarketTM(tm_);
    gl_pChinaStockMarket->SetSystemReady(false);
    gl_pChinaStockMarket->SetTodayStockProcessed(false);
    EXPECT_FALSE(gl_pChinaStockMarket->TaskProcessTodayStock(150359));
    EXPECT_FALSE(gl_pChinaStockMarket->TaskProcessTodayStock(150400));
    EXPECT_FALSE(gl_pChinaStockMarket->IsTodayStockProcessed());
    gl_pChinaStockMarket->SetSystemReady(true);
    gl_pChinaStockMarket->SetTodayStockProcessed(true);
    EXPECT_FALSE(gl_pChinaStockMarket->TaskProcessTodayStock(150359));
    EXPECT_FALSE(gl_pChinaStockMarket->TaskProcessTodayStock(150400));
    EXPECT_TRUE(gl_pChinaStockMarket->IsTodayStockProcessed());
    gl_pChinaStockMarket->SetSystemReady(true);
    gl_pChinaStockMarket->SetTodayStockProcessed(false);
    EXPECT_FALSE(gl_pChinaStockMarket->TaskProcessTodayStock(150359));
    EXPECT_TRUE(gl_pChinaStockMarket->TaskProcessTodayStock(150400));
    EXPECT_TRUE(gl_pChinaStockMarket->IsTodayStockProcessed());

    tm_.tm_wday = 0; // 星期日
    gl_pChinaStockMarket->__TEST_SetMarketTM(tm_);
    gl_pChinaStockMarket->SetTodayStockProcessed(false);
    EXPECT_FALSE(gl_pChinaStockMarket->TaskProcessTodayStock(150400));
    EXPECT_FALSE(gl_pChinaStockMarket->IsTodayStockProcessed()) << _T("休息日不处理");
  }

  TEST_F(CChinaMarketTest, TestSetCheckActiveStockFlag) {
    EXPECT_TRUE(gl_pChinaStockMarket->IsCheckActiveStock());
    gl_pChinaStockMarket->TaskSetCheckActiveStockFlag(91459);
    EXPECT_FALSE(gl_pChinaStockMarket->IsCheckActiveStock());
    gl_pChinaStockMarket->TaskSetCheckActiveStockFlag(91500);
    EXPECT_TRUE(gl_pChinaStockMarket->IsCheckActiveStock());
    gl_pChinaStockMarket->TaskSetCheckActiveStockFlag(92859);
    EXPECT_TRUE(gl_pChinaStockMarket->IsCheckActiveStock());
    gl_pChinaStockMarket->TaskSetCheckActiveStockFlag(92900);
    EXPECT_FALSE(gl_pChinaStockMarket->IsCheckActiveStock());
    gl_pChinaStockMarket->TaskSetCheckActiveStockFlag(113059);
    EXPECT_FALSE(gl_pChinaStockMarket->IsCheckActiveStock());
    gl_pChinaStockMarket->TaskSetCheckActiveStockFlag(113100);
    EXPECT_TRUE(gl_pChinaStockMarket->IsCheckActiveStock());
    gl_pChinaStockMarket->TaskSetCheckActiveStockFlag(125859);
    EXPECT_TRUE(gl_pChinaStockMarket->IsCheckActiveStock());
    gl_pChinaStockMarket->TaskSetCheckActiveStockFlag(125900);
    EXPECT_FALSE(gl_pChinaStockMarket->IsCheckActiveStock());
    gl_pChinaStockMarket->TaskSetCheckActiveStockFlag(150000);
    EXPECT_FALSE(gl_pChinaStockMarket->IsCheckActiveStock());
  }

  TEST_F(CChinaMarketTest, TestLoadStockCodeDB) {
    // 股票代码数据库在全局环境设置时即已装入测试系统，故而直接测试即可。
    CChinaStockPtr pStock = nullptr;
    pStock = gl_pChinaStockMarket->GetStock(0);
    EXPECT_EQ(pStock->GetIPOStatus(), __STOCK_IPOED__);
    EXPECT_STREQ(pStock->GetStockCode(), _T("sh600000"));
    EXPECT_EQ(pStock->GetDayLineStartDay(), 19991110);
    EXPECT_TRUE(pStock->IsActive());
    pStock = gl_pChinaStockMarket->GetStock(1);
    EXPECT_EQ(pStock->GetIPOStatus(), __STOCK_DELISTED__);
    EXPECT_STREQ(pStock->GetStockCode(), _T("sh600001"));
    EXPECT_EQ(pStock->GetDayLineStartDay(), 19980122);
    EXPECT_EQ(pStock->GetDayLineEndDay(), 20091215);
    EXPECT_FALSE(pStock->IsActive());
  }

  TEST_F(CChinaMarketTest, TestGetLastLoginDay) {
    gl_pChinaStockMarket->SetLastLoginDay(19900102);
    EXPECT_EQ(gl_pChinaStockMarket->GetLastLoginDay(), 19900102);
  }

  TEST_F(CChinaMarketTest, TestGetRelativeStrongStartDay) {
    gl_pChinaStockMarket->SetRelativeStrongStartDay(19900202);
    EXPECT_EQ(gl_pChinaStockMarket->GetRelativeStrongStartDay(), 19900202);
  }

  TEST_F(CChinaMarketTest, TestGetRelativeStrongEndDay) {
    gl_pChinaStockMarket->SetRelativeStrongEndDay(19900302);
    EXPECT_EQ(gl_pChinaStockMarket->GetRelativeStrongEndDay(), 19900302);
  }

  TEST_F(CChinaMarketTest, TestGetNewestTransactionTime) {
    gl_pChinaStockMarket->SetNewestTransactionTime(10101010);
    EXPECT_EQ(gl_pChinaStockMarket->GetNewestTransactionTime(), 10101010);
  }

  TEST_F(CChinaMarketTest, TestGetStockCodeForInquiringSinaRTData) {
    gl_pChinaStockMarket->SetStockCodeForInquiringSinaRTData(_T("sh601919"));
    EXPECT_EQ(gl_pChinaStockMarket->GetStockCodeForInquiringSinaRTData(), _T("sh601919"));
  }

  TEST_F(CChinaMarketTest, TestGetReadingNeteaseDayLineDataTime) {
    gl_pChinaStockMarket->SetStockCodeForInquiringNeteaseDayLine(_T("0600000"));
    EXPECT_EQ(gl_pChinaStockMarket->GetStockCodeForInquiringNeteaseDayLine(), _T("0600000"));
  }

  TEST_F(CChinaMarketTest, TestGetReadingTengxunRTDataTime) {
    time_t tt = 1010101010;
    gl_pChinaStockMarket->SetReadingTengxunRTDataTime(tt);
    EXPECT_EQ(gl_pChinaStockMarket->GetReadingTengxunRTDataTime(), tt);
  }

  TEST_F(CChinaMarketTest, TesstIsTodayTempRTDataLoaded) {
    EXPECT_FALSE(gl_pChinaStockMarket->IsTodayTempRTDataLoaded());
    gl_pChinaStockMarket->SetTodayTempRTDataLoaded(true);
    EXPECT_TRUE(gl_pChinaStockMarket->IsTodayTempRTDataLoaded());
    gl_pChinaStockMarket->SetTodayTempRTDataLoaded(false);
    EXPECT_FALSE(gl_pChinaStockMarket->IsTodayTempRTDataLoaded());
  }

  TEST_F(CChinaMarketTest, TesstIsCheckActiveStock) {
    EXPECT_TRUE(gl_pChinaStockMarket->IsCheckActiveStock());
    gl_pChinaStockMarket->SetCheckActiveStock(false);
    EXPECT_FALSE(gl_pChinaStockMarket->IsCheckActiveStock());
    gl_pChinaStockMarket->SetCheckActiveStock(true);
    EXPECT_TRUE(gl_pChinaStockMarket->IsCheckActiveStock());
  }

  TEST_F(CChinaMarketTest, TestGetTotalActiveStock) {
    long l = gl_pChinaStockMarket->GetTotalActiveStock();
    gl_pChinaStockMarket->SetTotalActiveStock(4000);
    EXPECT_EQ(gl_pChinaStockMarket->GetTotalActiveStock(), 4000);
    gl_pChinaStockMarket->SetTotalActiveStock(l);
  }

  TEST_F(CChinaMarketTest, TestGetStockPtr) {
    CChinaStockPtr pStock = nullptr;
    long lIndex = -1;
    pStock = gl_pChinaStockMarket->GetStock(_T("sh600000"));
    EXPECT_TRUE(gl_pChinaStockMarket->GetStockIndex(_T("sh600000"), lIndex));
    EXPECT_EQ(lIndex, 0);
    EXPECT_STREQ(pStock->GetStockCode(), _T("sh600000"));
  }

  TEST_F(CChinaMarketTest, TestIsInvalidNeteaseRTData) {
    CWebDataPtr pWebDataReceived;
    pWebDataReceived = make_shared<CWebData>();
    CString str = _T("_ntes_quote_callback({ });");
    pWebDataReceived->m_pDataBuffer = new char[50];
    strcpy_s(pWebDataReceived->m_pDataBuffer, 30, (LPSTR)str.GetBuffer());
    pWebDataReceived->m_lBufferLength = str.GetLength();
    pWebDataReceived->ResetCurrentPos();
    EXPECT_TRUE(gl_pChinaStockMarket->IsInvalidNeteaseRTData(pWebDataReceived));
    str = _T("_ntes_quote_callback({});");
    strcpy_s(pWebDataReceived->m_pDataBuffer, 30, (LPSTR)str.GetBuffer());
    pWebDataReceived->m_lBufferLength = str.GetLength();
    pWebDataReceived->ResetCurrentPos();
    EXPECT_FALSE(gl_pChinaStockMarket->IsInvalidNeteaseRTData(pWebDataReceived));
  }

  TEST_F(CChinaMarketTest, TestIsValidNeteaseRTDataPrefix) {
    CWebDataPtr pWebDataReceived;
    pWebDataReceived = make_shared<CWebData>();
    CString str = _T("_ntes_quote_callback({\"");
    pWebDataReceived->m_pDataBuffer = new char[50];
    strcpy_s(pWebDataReceived->m_pDataBuffer, 30, (LPSTR)str.GetBuffer());
    pWebDataReceived->m_lBufferLength = str.GetLength();
    pWebDataReceived->ResetCurrentPos();
    EXPECT_TRUE(gl_pChinaStockMarket->IsValidNeteaseRTDataPrefix(pWebDataReceived));
    str = _T("_ntes_quo_callback({\"");
    strcpy_s(pWebDataReceived->m_pDataBuffer, 30, (LPSTR)str.GetBuffer());
    pWebDataReceived->m_lBufferLength = str.GetLength();
    pWebDataReceived->ResetCurrentPos();
    EXPECT_FALSE(gl_pChinaStockMarket->IsValidNeteaseRTDataPrefix(pWebDataReceived));
  }

  TEST_F(CChinaMarketTest, TestIsValidTengxunRTDataPrefix) {
    CWebDataPtr pWebDataReceived;
    pWebDataReceived = make_shared<CWebData>();
    CString str = _T("v_pv_none_match=\"1\";\n");
    pWebDataReceived->m_pDataBuffer = new char[50];
    strcpy_s(pWebDataReceived->m_pDataBuffer, 30, (LPSTR)str.GetBuffer());
    pWebDataReceived->m_lBufferLength = str.GetLength();
    pWebDataReceived->ResetCurrentPos();
    EXPECT_TRUE(gl_pChinaStockMarket->IsInvalidTengxunRTData(pWebDataReceived));
    EXPECT_EQ(pWebDataReceived->GetCurrentPos(), 0);
    str = _T("v_pv_none_mtch=\"1\";\n");
    strcpy_s(pWebDataReceived->m_pDataBuffer, 30, (LPSTR)str.GetBuffer());
    pWebDataReceived->m_lBufferLength = str.GetLength();
    pWebDataReceived->ResetCurrentPos();
    EXPECT_FALSE(gl_pChinaStockMarket->IsInvalidTengxunRTData(pWebDataReceived));
    EXPECT_EQ(pWebDataReceived->GetCurrentPos(), 0);
  }

  TEST_F(CChinaMarketTest, TestStoreChoicedRTData) {
    EXPECT_EQ(gl_pChinaStockMarket->GetChoicedRTDataSize(), 0);
    CRTDataPtr pRTData = make_shared<CRTData>();
    gl_pChinaStockMarket->StoreChoiceRTData(pRTData);
    EXPECT_EQ(gl_pChinaStockMarket->GetChoicedRTDataSize(), 1);
    gl_pChinaStockMarket->ClearChoicedRTDataQueue();
    EXPECT_EQ(gl_pChinaStockMarket->GetChoicedRTDataSize(), 0);
  }

  TEST_F(CChinaMarketTest, TestSchedulingTaskPerHour) {
    EXPECT_TRUE(gl_pChinaStockMarket->SchedulingTaskPerHour(3600, 10100));
    EXPECT_FALSE(gl_pChinaStockMarket->SchedulingTaskPerHour(3599, 19000)) << _T("前面那个将计数器重置，此调用尚差一秒，故而返回假");
    EXPECT_TRUE(gl_pChinaStockMarket->SchedulingTaskPerHour(1, 10100));
  }

  TEST_F(CChinaMarketTest, TestGetRTDataReceived) {
    gl_pChinaStockMarket->SetRTDataReceived(101010);
    EXPECT_EQ(gl_pChinaStockMarket->GetRTDataReceived(), 101010);
    gl_pChinaStockMarket->SetRTDataReceived(1010101010);
    EXPECT_EQ(gl_pChinaStockMarket->GetRTDataReceived(), 1010101010);
  }

  TEST_F(CChinaMarketTest, TestCheckMarketReady) {
    gl_pChinaStockMarket->SetSystemReady(true);
    gl_pChinaStockMarket->SetRTDataReceived(0);
    EXPECT_TRUE(gl_pChinaStockMarket->CheckMarketReady());
    EXPECT_TRUE(gl_pChinaStockMarket->IsSystemReady());
    gl_pChinaStockMarket->SetSystemReady(false);
    gl_pChinaStockMarket->SetRTDataReceived(0);
    gl_pChinaStockMarket->SetSystemReady(false);
    EXPECT_FALSE(gl_pChinaStockMarket->CheckMarketReady());
    EXPECT_FALSE(gl_pChinaStockMarket->IsSystemReady());
    gl_pChinaStockMarket->SetSystemReady(false);
    gl_pChinaStockMarket->SetRTDataReceived(0);
    gl_pChinaStockMarket->SetSystemReady(false);
    EXPECT_FALSE(gl_pChinaStockMarket->CheckMarketReady());
    EXPECT_FALSE(gl_pChinaStockMarket->IsSystemReady());
    gl_pChinaStockMarket->SetRTDataReceived(gl_pChinaStockMarket->GetTotalStock() * 3);
    EXPECT_FALSE(gl_pChinaStockMarket->CheckMarketReady());
    EXPECT_FALSE(gl_pChinaStockMarket->IsSystemReady());
    gl_pChinaStockMarket->SetRTDataReceived(gl_pChinaStockMarket->GetTotalStock() * 3 + 1);
    EXPECT_TRUE(gl_pChinaStockMarket->CheckMarketReady());
    EXPECT_TRUE(gl_pChinaStockMarket->IsSystemReady());
    gl_pChinaStockMarket->SetSystemReady(false);
  }

  TEST_F(CChinaMarketTest, TestChangeCurrentStockToNextStock) {
    gl_pChinaStockMarket->SetCurrentStock(gl_pChinaStockMarket->GetStock(0));
    gl_pChinaStockMarket->ChangeCurrentStockToNextStock();
    EXPECT_EQ(gl_pChinaStockMarket->GetCurrentStock()->GetOffset(), 1);
    gl_pChinaStockMarket->SetCurrentStock(gl_pChinaStockMarket->GetStock(11998)); // 这个股票是中证煤炭
    gl_pChinaStockMarket->ChangeCurrentStockToNextStock();
    EXPECT_EQ(gl_pChinaStockMarket->GetCurrentStock()->GetOffset(), 0) << _T("中证煤炭后的为空，然后就转到最前面的浦发银行了");
  }

  TEST_F(CChinaMarketTest, TestChangeCurrentStockToPrevStock) {
    gl_pChinaStockMarket->SetCurrentStock(gl_pChinaStockMarket->GetStock(1)); // 选取退市的邯郸钢铁
    gl_pChinaStockMarket->ChangeCurrentStockToPrevStock();
    EXPECT_EQ(gl_pChinaStockMarket->GetCurrentStock()->GetOffset(), 0) << _T("上一个是浦发银行");
    gl_pChinaStockMarket->ChangeCurrentStockToPrevStock();
    EXPECT_EQ(gl_pChinaStockMarket->GetCurrentStock()->GetOffset(), 11998) << _T("浦发银行前的为空，然后就转到最后面的中证煤炭了");
  }
}