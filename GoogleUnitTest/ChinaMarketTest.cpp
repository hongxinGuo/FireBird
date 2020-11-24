#include"stdafx.h"
#include"pch.h"

#include"globedef.h"

#include"ChinaMarket.h"
#include"ChinaStake.h"

#include"SetStakeCode.h"
#include"SetOption.h"
#include"SetWeekLineInfo.h"

#include"WebInquirer.h"

#include"MockNeteaseDayLineWebInquiry.h"
using namespace testing;

using namespace std;
#include<memory>

namespace StockAnalysisTest {
  class CChinaMarketTest : public ::testing::Test
  {
  protected:
    static void SetUpTestSuite(void) {
      gl_pChinaStakeMarket->Load10DaysRSStrongStockDB(); // 装入各十日强度股票集
      EXPECT_TRUE(gl_pChinaStakeMarket->GetChoicedStockSize(10) > 0);
      EXPECT_TRUE(gl_pChinaStakeMarket->GetChoicedStockSize(11) > 0);
      EXPECT_TRUE(gl_pChinaStakeMarket->GetChoicedStockSize(12) > 0);
      EXPECT_TRUE(gl_pChinaStakeMarket->GetChoicedStockSize(13) == 0);
      EXPECT_TRUE(gl_pChinaStakeMarket->GetChoicedStockSize(14) == 0);
      EXPECT_TRUE(gl_pChinaStakeMarket->GetChoicedStockSize(15) == 0);
      EXPECT_TRUE(gl_pChinaStakeMarket->GetChoicedStockSize(16) == 0);
      EXPECT_TRUE(gl_pChinaStakeMarket->GetChoicedStockSize(17) == 0);
      EXPECT_TRUE(gl_pChinaStakeMarket->GetChoicedStockSize(18) > 0);
      EXPECT_TRUE(gl_pChinaStakeMarket->GetChoicedStockSize(19) > 0);

      for (int i = 0; i < gl_pChinaStakeMarket->GetTotalStock(); i++) {
        CChinaStockPtr pStock = gl_pChinaStakeMarket->GetStock(i);
        EXPECT_TRUE(pStock->IsDayLineNeedUpdate()) << pStock->GetStakeCode();
      }
      EXPECT_EQ(gl_pChinaStakeMarket->GetDayLineNeedProcessNumber(), 0);
      ASSERT_FALSE(gl_fNormalMode);
      EXPECT_EQ(gl_pChinaStakeMarket->GetCurrentStock(), nullptr) << gl_pChinaStakeMarket->GetCurrentStock()->GetStakeCode();
      EXPECT_EQ(gl_pChinaStakeMarket->GetDayLineNeedUpdateNumber(), 12000);
      EXPECT_FALSE(gl_pChinaStakeMarket->IsCurrentStockChanged());
    }
    static void TearDownTestSuite(void) {
      EXPECT_EQ(gl_pChinaStakeMarket->GetDayLineNeedProcessNumber(), 0);
      EXPECT_EQ(gl_pChinaStakeMarket->GetCurrentStock(), nullptr) << gl_pChinaStakeMarket->GetCurrentStock()->GetStakeCode();
      EXPECT_FALSE(gl_pChinaStakeMarket->IsCurrentStockChanged());
      gl_pChinaStakeMarket->SetCurrentStockChanged(false);
      while (gl_WebInquirer.IsReadingWebThreadRunning()) Sleep(1);
      while (gl_ThreadStatus.IsWorkingThreadRunning()) Sleep(1);
      for (int i = 0; i < gl_pChinaStakeMarket->GetTotalStock(); i++) {
        CChinaStockPtr pStock = gl_pChinaStakeMarket->GetStock(i);
        EXPECT_TRUE(pStock->IsDayLineNeedUpdate());
      }
      EXPECT_EQ(gl_pChinaStakeMarket->GetDayLineNeedUpdateNumber(), 12000);
    }
    virtual void SetUp(void) override {
      ASSERT_FALSE(gl_fNormalMode);
      EXPECT_EQ(gl_pChinaStakeMarket->GetDayLineNeedUpdateNumber(), 12000);
      EXPECT_EQ(gl_pChinaStakeMarket->GetDayLineNeedProcessNumber(), 0);
      gl_pChinaStakeMarket->SetCurrentStockChanged(false);
      gl_pChinaStakeMarket->CalculateTime();
      gl_pChinaStakeMarket->ResetNeteaseRTDataInquiringIndex();
      gl_pChinaStakeMarket->ResetNeteaseDayLineDataInquiringIndex();
      gl_pChinaStakeMarket->ResetSinaRTDataInquiringIndex();
      gl_pChinaStakeMarket->ResetTengxunRTDataInquiringIndex();
      gl_pChinaStakeMarket->SetSystemReady(true); // 测试市场时，默认系统已经准备好
      gl_pChinaStakeMarket->SetPermitResetMarket(true);
      gl_pChinaStakeMarket->SetCheckActiveStock(true);
      EXPECT_TRUE(gl_pChinaStakeMarket->IsResetMarket());
      EXPECT_FALSE(gl_pChinaStakeMarket->IsMarketOpened());

      while (gl_systemMessage.GetInformationDequeSize() > 0) gl_systemMessage.PopInformationMessage();
      while (gl_systemMessage.GetDayLineInfoDequeSize() > 0) gl_systemMessage.PopDayLineInfoMessage();
      while (gl_systemMessage.GetInnerSystemInformationDequeSize() > 0) gl_systemMessage.PopInnerSystemInformationMessage();
    }

    virtual void TearDown(void) override {
      // clearup
      EXPECT_FALSE(gl_pChinaStakeMarket->IsMarketOpened());
      gl_ThreadStatus.SetSavingTempData(false);
      gl_pChinaStakeMarket->SetRTDataSetCleared(false);
      gl_pChinaStakeMarket->SetUpdateStockCodeDB(false);
      gl_pChinaStakeMarket->SetUpdateOptionDB(false);
      gl_pChinaStakeMarket->ClearChoicedRTDataQueue();
      gl_pChinaStakeMarket->SetResetMarket(true);
      gl_pChinaStakeMarket->ResetNeteaseRTDataInquiringIndex();
      gl_pChinaStakeMarket->ResetNeteaseDayLineDataInquiringIndex();
      gl_pChinaStakeMarket->ResetSinaRTDataInquiringIndex();
      gl_pChinaStakeMarket->ResetTengxunRTDataInquiringIndex();
      while (gl_systemMessage.GetInformationDequeSize() > 0) gl_systemMessage.PopInformationMessage();
      gl_pChinaStakeMarket->SetPermitResetMarket(true);
      gl_pChinaStakeMarket->SetCheckActiveStock(true);
      gl_pChinaStakeMarket->SetUpdateOptionDB(false);
      gl_pChinaStakeMarket->SetSystemReady(true); // 离开此测试时，默认系统已准备好。

      while (gl_systemMessage.GetInformationDequeSize() > 0) gl_systemMessage.PopInformationMessage();
      while (gl_systemMessage.GetDayLineInfoDequeSize() > 0) gl_systemMessage.PopDayLineInfoMessage();
      while (gl_systemMessage.GetInnerSystemInformationDequeSize() > 0) gl_systemMessage.PopInnerSystemInformationMessage();
      gl_pChinaStakeMarket->ResetCurrentStock();
      gl_pChinaStakeMarket->SetCurrentStockChanged(false);
      for (int i = 0; i < gl_pChinaStakeMarket->GetTotalStock(); i++) {
        CChinaStockPtr pStock = gl_pChinaStakeMarket->GetStock(i);
        if (!pStock->IsDayLineNeedUpdate()) pStock->SetDayLineNeedUpdate(true);
        if (pStock->IsDayLineNeedProcess()) pStock->SetDayLineNeedProcess(false);
        if (pStock->IsDayLineNeedSaving()) pStock->SetDayLineNeedSaving(false);
      }
      EXPECT_EQ(gl_pChinaStakeMarket->GetDayLineNeedUpdateNumber(), 12000);
      EXPECT_EQ(gl_pChinaStakeMarket->GetDayLineNeedProcessNumber(), 0);
      EXPECT_EQ(gl_pChinaStakeMarket->GetCurrentSelectedStockSet(), -1);
    }
  };

  TEST_F(CChinaMarketTest, TestInitialize) {
    CChinaStockPtr pStock = nullptr;
    for (int i = 0; i < 12000; i++) {
      pStock = gl_pChinaStakeMarket->GetStock(i);
      EXPECT_EQ(pStock->GetOffset(), i);
      EXPECT_TRUE(pStock->IsDayLineNeedUpdate()) << pStock->GetStakeCode();
      EXPECT_FALSE(pStock->IsDayLineNeedProcess());
      EXPECT_FALSE(pStock->IsDayLineNeedSaving());
      if ((pStock->GetStakeCode() >= _T("sh000000")) && (pStock->GetStakeCode() <= _T("sh000999"))) {
        EXPECT_FALSE(pStock->IsNeedProcessRTData());
      }
      else if ((pStock->GetStakeCode() >= _T("sz399000")) && (pStock->GetStakeCode() <= _T("sz399999"))) {
        EXPECT_FALSE(pStock->IsNeedProcessRTData());
      }
      else EXPECT_TRUE(pStock->IsNeedProcessRTData());
    }
    EXPECT_GT(gl_pChinaStakeMarket->GetTotalActiveStock(), 0);
    EXPECT_FALSE(gl_pChinaStakeMarket->IsLoadSelectedStock());
    EXPECT_TRUE(gl_pChinaStakeMarket->IsSystemReady());
    EXPECT_FALSE(gl_pChinaStakeMarket->IsCurrentEditStockChanged());
    EXPECT_FALSE(gl_pChinaStakeMarket->IsMarketOpened());
    EXPECT_FALSE(gl_pChinaStakeMarket->IsCurrentStockChanged());
    EXPECT_EQ(gl_pChinaStakeMarket->GetTotalAttackBuyAmount(), 0);
    EXPECT_EQ(gl_pChinaStakeMarket->GetTotalAttackSellAmount(), 0);
    EXPECT_TRUE(gl_pChinaStakeMarket->IsGetRTData());
    EXPECT_FALSE(gl_pChinaStakeMarket->IsTodayTempRTDataLoaded());
    EXPECT_FALSE(gl_pChinaStakeMarket->IsUpdatedStakeCode());
    EXPECT_EQ(gl_pChinaStakeMarket->GetCountDownSlowReadingRTData(), 3);
    EXPECT_EQ(gl_pChinaStakeMarket->GetTotalStock(), 12000);   // 在全局变量gl_ChinaStockMarket初始化时就生成了全部股票代码池
    EXPECT_EQ(gl_pChinaStakeMarket->GetTotalStockMapIndexSize(), 12000);
    pStock = gl_pChinaStakeMarket->GetStock(0);
    EXPECT_STREQ(pStock->GetStakeCode(), _T("sh600000"));
    EXPECT_EQ(pStock->GetMarket(), __SHANGHAI_MARKET__);
    pStock = gl_pChinaStakeMarket->GetStock(6000);
    EXPECT_STREQ(pStock->GetStakeCode(), _T("sz000000"));
    EXPECT_EQ(pStock->GetMarket(), __SHENZHEN_MARKET__);
    EXPECT_EQ(gl_pChinaStakeMarket->GetStockOffset(_T("sh600000")), 0);
    EXPECT_EQ(gl_pChinaStakeMarket->GetStockOffset(_T("sz000000")), 6000);

    EXPECT_TRUE(gl_pChinaStakeMarket->IsPermitResetMarket());
    EXPECT_EQ(gl_pChinaStakeMarket->GetDayLineNeedProcessNumber(), 0) << gl_pChinaStakeMarket->GetDayLineNeedProcessNumber();
    EXPECT_EQ(gl_pChinaStakeMarket->GetDayLineNeedSaveNumber(), 0);
    EXPECT_TRUE(gl_pChinaStakeMarket->IsUsingSinaRTDataReceiver());
    EXPECT_TRUE(gl_pChinaStakeMarket->IsUsingTengxunRTDataReceiver());
    EXPECT_TRUE(gl_pChinaStakeMarket->IsUsingNeteaseRTDataReceiver());

    EXPECT_EQ(gl_pChinaStakeMarket->GetNeteaseDayLineDataInquiringIndex(), 0);
    EXPECT_EQ(gl_pChinaStakeMarket->GetNeteaseRTDataInquiringIndex(), 0);
    EXPECT_EQ(gl_pChinaStakeMarket->GetSinaRTDataInquiringIndex(), 0);
    EXPECT_EQ(gl_pChinaStakeMarket->GetTengxunRTDataInquiringIndex(), 0);
  }

  TEST_F(CChinaMarketTest, TestClearUpdateStockCodeDBFlag) {
    EXPECT_FALSE(gl_pChinaStakeMarket->IsDayLineDBUpdated());
    for (int i = 0; i < gl_pChinaStakeMarket->GetTotalStock(); i++) {
      CChinaStockPtr pStock = gl_pChinaStakeMarket->GetStock(i);
      pStock->SetDayLineDBUpdated(true);
    }
    EXPECT_TRUE(gl_pChinaStakeMarket->IsDayLineDBUpdated());
    gl_pChinaStakeMarket->ClearDayLineDBUpdatedFlag();
    EXPECT_FALSE(gl_pChinaStakeMarket->IsDayLineDBUpdated());
  }

  TEST_F(CChinaMarketTest, TestGetTengxunInquiringStockStr) {
    CChinaStockPtr pStock = nullptr;
    gl_pChinaStakeMarket->SetSystemReady(true);
    gl_pChinaStakeMarket->ResetTengxunRTDataInquiringIndex();
    CString str = gl_pChinaStakeMarket->GetTengxunInquiringStockStr(900, false);
    EXPECT_EQ(gl_pChinaStakeMarket->GetTengxunRTDataInquiringIndex(), 899);
    str = gl_pChinaStakeMarket->GetTengxunInquiringStockStr(900, false);
    CString strCompare, str2;
    pStock = gl_pChinaStakeMarket->GetStock(899);
    strCompare = pStock->GetStakeCode();
    str2 = str.Left(8);
    EXPECT_EQ(str2.Compare(strCompare), 0);
    str = gl_pChinaStakeMarket->GetTengxunInquiringStockStr(900, false);
    EXPECT_EQ(gl_pChinaStakeMarket->GetTengxunRTDataInquiringIndex(), 2697);
    pStock = gl_pChinaStakeMarket->GetStock(1798);
    strCompare = pStock->GetStakeCode();
    str2 = str.Left(8);
    EXPECT_EQ(str2.Compare(strCompare), 0);
    str = gl_pChinaStakeMarket->GetTengxunInquiringStockStr(900, false);
    EXPECT_EQ(gl_pChinaStakeMarket->GetTengxunRTDataInquiringIndex(), 3596);
    pStock = gl_pChinaStakeMarket->GetStock(2697);
    strCompare = pStock->GetStakeCode();
    str2 = str.Left(8);
    EXPECT_EQ(str2.Compare(strCompare), 0);
    str = gl_pChinaStakeMarket->GetTengxunInquiringStockStr(900, false);
    EXPECT_EQ(gl_pChinaStakeMarket->GetTengxunRTDataInquiringIndex(), 4495);
    pStock = gl_pChinaStakeMarket->GetStock(3596);
    strCompare = pStock->GetStakeCode();
    str2 = str.Left(8);
    EXPECT_EQ(str2.Compare(strCompare), 0);
    str = gl_pChinaStakeMarket->GetTengxunInquiringStockStr(900, false);
    EXPECT_EQ(gl_pChinaStakeMarket->GetTengxunRTDataInquiringIndex(), 5394);
    pStock = gl_pChinaStakeMarket->GetStock(4495);
    strCompare = pStock->GetStakeCode();
    str2 = str.Left(8);
    EXPECT_EQ(str2.Compare(strCompare), 0);
    str = gl_pChinaStakeMarket->GetTengxunInquiringStockStr(900, false);
    EXPECT_EQ(gl_pChinaStakeMarket->GetTengxunRTDataInquiringIndex(), 6293);
    pStock = gl_pChinaStakeMarket->GetStock(5394);
    strCompare = pStock->GetStakeCode();
    str2 = str.Left(8);
    EXPECT_EQ(str2.Compare(strCompare), 0);
    str = gl_pChinaStakeMarket->GetTengxunInquiringStockStr(900, false);
    EXPECT_EQ(gl_pChinaStakeMarket->GetTengxunRTDataInquiringIndex(), 7192);
    pStock = gl_pChinaStakeMarket->GetStock(6293);
    strCompare = pStock->GetStakeCode();
    str2 = str.Left(8);
    EXPECT_EQ(str2.Compare(strCompare), 0);
    str = gl_pChinaStakeMarket->GetTengxunInquiringStockStr(900, false);
    EXPECT_EQ(gl_pChinaStakeMarket->GetTengxunRTDataInquiringIndex(), 8091);
    pStock = gl_pChinaStakeMarket->GetStock(7192);
    strCompare = pStock->GetStakeCode();
    str2 = str.Left(8);
    EXPECT_EQ(str2.Compare(strCompare), 0);
    str = gl_pChinaStakeMarket->GetTengxunInquiringStockStr(900, false);
    pStock = gl_pChinaStakeMarket->GetStock(8091);
    strCompare = pStock->GetStakeCode();
    str2 = str.Left(8);
    EXPECT_EQ(str2.Compare(strCompare), 0);
    str = gl_pChinaStakeMarket->GetTengxunInquiringStockStr(900, false);
    pStock = gl_pChinaStakeMarket->GetStock(8990);
    strCompare = pStock->GetStakeCode();
    str2 = str.Left(8);
    EXPECT_EQ(str2.Compare(strCompare), 0);
    str = gl_pChinaStakeMarket->GetTengxunInquiringStockStr(900, false);
    EXPECT_EQ(gl_pChinaStakeMarket->GetTengxunRTDataInquiringIndex(), 10788);
    pStock = gl_pChinaStakeMarket->GetStock(9889);
    strCompare = pStock->GetStakeCode();
    str2 = str.Left(8);
    EXPECT_EQ(str2.Compare(strCompare), 0);
    str = gl_pChinaStakeMarket->GetTengxunInquiringStockStr(900, false);
    EXPECT_EQ(gl_pChinaStakeMarket->GetTengxunRTDataInquiringIndex(), 11687);
    pStock = gl_pChinaStakeMarket->GetStock(10788);
    strCompare = pStock->GetStakeCode();
    str2 = str.Left(8);
    EXPECT_EQ(str2.Compare(strCompare), 0);
    str = gl_pChinaStakeMarket->GetTengxunInquiringStockStr(900, false);
    EXPECT_EQ(gl_pChinaStakeMarket->GetTengxunRTDataInquiringIndex(), 0);
    pStock = gl_pChinaStakeMarket->GetStock(11687);
    strCompare = pStock->GetStakeCode();
    str2 = str.Left(8);
    EXPECT_EQ(str2.Compare(strCompare), 0);
    str = gl_pChinaStakeMarket->GetTengxunInquiringStockStr(900, false);
    EXPECT_EQ(gl_pChinaStakeMarket->GetTengxunRTDataInquiringIndex(), 899);
    pStock = gl_pChinaStakeMarket->GetStock(0);
    strCompare = pStock->GetStakeCode();
    str2 = str.Left(8);
    EXPECT_EQ(str2.Compare(strCompare), 0);
    gl_pChinaStakeMarket->ResetTengxunRTDataInquiringIndex();
  }

  TEST_F(CChinaMarketTest, TestGetNeteaseDayLineFromWeb) {
    gl_pChinaStakeMarket->SetSystemReady(true);
    gl_pChinaStakeMarket->__TEST_SetFormatedMarketTime(92559); // 更新网易历史数据开始于09:25:00
    gl_pChinaStakeMarket->SetDayLineNeedUpdateNumber(0);
    EXPECT_CALL(*gl_pNeteaseDayLineWebInquiry, StartReadingThread())
      .Times(0);
    EXPECT_CALL(*gl_pNeteaseDayLineWebInquirySecond, StartReadingThread())
      .Times(0);
    EXPECT_CALL(*gl_pNeteaseDayLineWebInquiryThird, StartReadingThread())
      .Times(0);
    EXPECT_CALL(*gl_pNeteaseDayLineWebInquiryFourth, StartReadingThread())
      .Times(0);
    EXPECT_FALSE(gl_pChinaStakeMarket->TaskGetNeteaseDayLineFromWeb());
    gl_pChinaStakeMarket->SetDayLineNeedUpdateNumber(10);
    EXPECT_FALSE(gl_pChinaStakeMarket->TaskGetNeteaseDayLineFromWeb());

    for (int i = 0; i < gl_pChinaStakeMarket->GetTotalStock(); i++) {
      CChinaStockPtr pStock = gl_pChinaStakeMarket->GetStock(i);
      if (!pStock->IsDayLineNeedUpdate()) pStock->SetDayLineNeedUpdate(true);
    }
    gl_pChinaStakeMarket->SetDayLineNeedUpdateNumber(12000);
  }

  TEST_F(CChinaMarketTest, TestGetNeteaseDayLineFromWeb2) {
    gl_pChinaStakeMarket->SetSystemReady(true);
    gl_pChinaStakeMarket->__TEST_SetFormatedMarketTime(92600); // 更新网易历史数据开始于09:25:00
    gl_pChinaStakeMarket->SetDayLineNeedUpdateNumber(0);
    EXPECT_CALL(*gl_pNeteaseDayLineWebInquiry, StartReadingThread())
      .Times(1);
    EXPECT_CALL(*gl_pNeteaseDayLineWebInquirySecond, StartReadingThread())
      .Times(1);
    EXPECT_CALL(*gl_pNeteaseDayLineWebInquiryThird, StartReadingThread())
      .Times(1);
    EXPECT_CALL(*gl_pNeteaseDayLineWebInquiryFourth, StartReadingThread())
      .Times(1);
    EXPECT_FALSE(gl_pChinaStakeMarket->TaskGetNeteaseDayLineFromWeb());
    gl_pChinaStakeMarket->SetDayLineNeedUpdateNumber(10);
    EXPECT_TRUE(gl_pChinaStakeMarket->TaskGetNeteaseDayLineFromWeb());

    for (int i = 0; i < gl_pChinaStakeMarket->GetTotalStock(); i++) {
      CChinaStockPtr pStock = gl_pChinaStakeMarket->GetStock(i);
      if (!pStock->IsDayLineNeedUpdate()) pStock->SetDayLineNeedUpdate(true);
    }
    gl_pChinaStakeMarket->SetDayLineNeedUpdateNumber(12000);
  }

  TEST_F(CChinaMarketTest, TestGetSinaInquiringStockStr2) {
    CChinaStockPtr pStock = nullptr;
    gl_pChinaStakeMarket->ResetSinaRTDataInquiringIndex();
    CString str = gl_pChinaStakeMarket->GetSinaInquiringStockStr(900, false);
    EXPECT_EQ(gl_pChinaStakeMarket->GetSinaRTDataInquiringIndex(), 899);
    str = gl_pChinaStakeMarket->GetSinaInquiringStockStr(900, false);
    EXPECT_EQ(gl_pChinaStakeMarket->GetSinaRTDataInquiringIndex(), 1798);
    CString strCompare, str2;
    pStock = gl_pChinaStakeMarket->GetStock(899);
    strCompare = pStock->GetStakeCode();
    str2 = str.Left(8);
    EXPECT_EQ(str2.Compare(strCompare), 0);
    str = gl_pChinaStakeMarket->GetSinaInquiringStockStr(900, false);
    pStock = gl_pChinaStakeMarket->GetStock(1798);
    strCompare = pStock->GetStakeCode();
    str2 = str.Left(8);
    EXPECT_EQ(str2.Compare(strCompare), 0);
    str = gl_pChinaStakeMarket->GetSinaInquiringStockStr(900, false);
    pStock = gl_pChinaStakeMarket->GetStock(2697);
    strCompare = pStock->GetStakeCode();
    str2 = str.Left(8);
    EXPECT_EQ(str2.Compare(strCompare), 0);
    str = gl_pChinaStakeMarket->GetSinaInquiringStockStr(900, false);
    pStock = gl_pChinaStakeMarket->GetStock(3596);
    strCompare = pStock->GetStakeCode();
    str2 = str.Left(8);
    EXPECT_EQ(str2.Compare(strCompare), 0);
    str = gl_pChinaStakeMarket->GetSinaInquiringStockStr(900, false);
    pStock = gl_pChinaStakeMarket->GetStock(4495);
    strCompare = pStock->GetStakeCode();
    str2 = str.Left(8);
    EXPECT_EQ(str2.Compare(strCompare), 0);
    str = gl_pChinaStakeMarket->GetSinaInquiringStockStr(900, false);
    pStock = gl_pChinaStakeMarket->GetStock(5394);
    strCompare = pStock->GetStakeCode();
    str2 = str.Left(8);
    EXPECT_EQ(str2.Compare(strCompare), 0);
    str = gl_pChinaStakeMarket->GetSinaInquiringStockStr(900, false);
    pStock = gl_pChinaStakeMarket->GetStock(6293);
    strCompare = pStock->GetStakeCode();
    str2 = str.Left(8);
    EXPECT_EQ(str2.Compare(strCompare), 0);
    str = gl_pChinaStakeMarket->GetSinaInquiringStockStr(900, false);
    pStock = gl_pChinaStakeMarket->GetStock(7192);
    strCompare = pStock->GetStakeCode();
    str2 = str.Left(8);
    EXPECT_EQ(str2.Compare(strCompare), 0);
    str = gl_pChinaStakeMarket->GetSinaInquiringStockStr(900, false);
    pStock = gl_pChinaStakeMarket->GetStock(8091);
    strCompare = pStock->GetStakeCode();
    str2 = str.Left(8);
    EXPECT_EQ(str2.Compare(strCompare), 0);
    str = gl_pChinaStakeMarket->GetSinaInquiringStockStr(900, false);
    pStock = gl_pChinaStakeMarket->GetStock(8990);
    strCompare = pStock->GetStakeCode();
    str2 = str.Left(8);
    EXPECT_EQ(str2.Compare(strCompare), 0);
    str = gl_pChinaStakeMarket->GetSinaInquiringStockStr(900, false);
    pStock = gl_pChinaStakeMarket->GetStock(9889);
    strCompare = pStock->GetStakeCode();
    str2 = str.Left(8);
    EXPECT_EQ(str2.Compare(strCompare), 0);
    str = gl_pChinaStakeMarket->GetSinaInquiringStockStr(900, false);
    pStock = gl_pChinaStakeMarket->GetStock(10788);
    strCompare = pStock->GetStakeCode();
    str2 = str.Left(8);
    EXPECT_EQ(str2.Compare(strCompare), 0);
    str = gl_pChinaStakeMarket->GetSinaInquiringStockStr(900, false);
    EXPECT_EQ(gl_pChinaStakeMarket->GetSinaRTDataInquiringIndex(), 0);
    pStock = gl_pChinaStakeMarket->GetStock(11687);
    strCompare = pStock->GetStakeCode();
    str2 = str.Left(8);
    EXPECT_EQ(str2.Compare(strCompare), 0);
    str = gl_pChinaStakeMarket->GetSinaInquiringStockStr(900, false);
    pStock = gl_pChinaStakeMarket->GetStock(0);
    strCompare = pStock->GetStakeCode();
    str2 = str.Left(8);
    EXPECT_EQ(str2.Compare(strCompare), 0);
  }

  TEST_F(CChinaMarketTest, TestGetInquiringStockStr) {
    CString str2;

    gl_pChinaStakeMarket->SetSystemReady(true);
    gl_pChinaStakeMarket->ResetSinaRTDataInquiringIndex();
    CString str = gl_pChinaStakeMarket->GetSinaInquiringStockStr(900);
    str = gl_pChinaStakeMarket->GetSinaInquiringStockStr(900);
    str = gl_pChinaStakeMarket->GetSinaInquiringStockStr(900);
    str = gl_pChinaStakeMarket->GetSinaInquiringStockStr(900);
    str = gl_pChinaStakeMarket->GetSinaInquiringStockStr(900);
    if (gl_pChinaStakeMarket->GetSinaRTDataInquiringIndex() == 0) {
      str = gl_pChinaStakeMarket->GetSinaInquiringStockStr(900); // 目前不到五千个活跃股票，故而六次即可遍历一次
      str2 = str.Left(9);
      EXPECT_EQ(str2, _T("sh600000,"));
    }
    else {
      str = gl_pChinaStakeMarket->GetSinaInquiringStockStr(900);
      str = gl_pChinaStakeMarket->GetSinaInquiringStockStr(900);
      str2 = str.Left(9);
      EXPECT_STREQ(str2, _T("sh600000,"));
    }
  }

  TEST_F(CChinaMarketTest, TestGetNeteaseInquiringStockStr) {
    CChinaStockPtr pStock = nullptr;
    gl_pChinaStakeMarket->SetSystemReady(true);
    gl_pChinaStakeMarket->ResetNeteaseRTDataInquiringIndex();
    CString str = gl_pChinaStakeMarket->GetNeteaseInquiringStockStr(900, false);
    EXPECT_EQ(gl_pChinaStakeMarket->GetNeteaseRTDataInquiringIndex(), 899);
    str = gl_pChinaStakeMarket->GetNeteaseInquiringStockStr(900, false);
    CString strCompare, str2;
    pStock = gl_pChinaStakeMarket->GetStock(899);
    strCompare = pStock->GetStakeCode();
    str2 = str.Left(7);
    EXPECT_STREQ(str2.Right(6), strCompare.Right(6));
    str = gl_pChinaStakeMarket->GetNeteaseInquiringStockStr(900, false);
    pStock = gl_pChinaStakeMarket->GetStock(1798);
    strCompare = pStock->GetStakeCode();
    str2 = str.Left(7);
    EXPECT_STREQ(str2.Right(6), strCompare.Right(6));
    str = gl_pChinaStakeMarket->GetNeteaseInquiringStockStr(900, false);
    pStock = gl_pChinaStakeMarket->GetStock(2697);
    strCompare = pStock->GetStakeCode();
    str2 = str.Left(7);
    EXPECT_STREQ(str2.Right(6), strCompare.Right(6));
    str = gl_pChinaStakeMarket->GetNeteaseInquiringStockStr(900, false);
    pStock = gl_pChinaStakeMarket->GetStock(3596);
    strCompare = pStock->GetStakeCode();
    str2 = str.Left(7);
    EXPECT_STREQ(str2.Right(6), strCompare.Right(6));
    str = gl_pChinaStakeMarket->GetNeteaseInquiringStockStr(900, false);
    pStock = gl_pChinaStakeMarket->GetStock(4495);
    strCompare = pStock->GetStakeCode();
    str2 = str.Left(7);
    EXPECT_STREQ(str2.Right(6), strCompare.Right(6));
    str = gl_pChinaStakeMarket->GetNeteaseInquiringStockStr(900, false);
    pStock = gl_pChinaStakeMarket->GetStock(5394);
    strCompare = pStock->GetStakeCode();
    str2 = str.Left(7);
    EXPECT_STREQ(str2.Right(6), strCompare.Right(6));
    str = gl_pChinaStakeMarket->GetNeteaseInquiringStockStr(900, false);
    pStock = gl_pChinaStakeMarket->GetStock(6293);
    strCompare = pStock->GetStakeCode();
    str2 = str.Left(7);
    EXPECT_STREQ(str2.Right(6), strCompare.Right(6));
    str = gl_pChinaStakeMarket->GetNeteaseInquiringStockStr(900, false);
    pStock = gl_pChinaStakeMarket->GetStock(7192);
    strCompare = pStock->GetStakeCode();
    str2 = str.Left(7);
    EXPECT_STREQ(str2.Right(6), strCompare.Right(6));
    str = gl_pChinaStakeMarket->GetNeteaseInquiringStockStr(900, false);
    pStock = gl_pChinaStakeMarket->GetStock(8091);
    strCompare = pStock->GetStakeCode();
    str2 = str.Left(7);
    EXPECT_STREQ(str2.Right(6), strCompare.Right(6));
    str = gl_pChinaStakeMarket->GetNeteaseInquiringStockStr(900, false);
    pStock = gl_pChinaStakeMarket->GetStock(8990);
    strCompare = pStock->GetStakeCode();
    str2 = str.Left(7);
    EXPECT_STREQ(str2.Right(6), strCompare.Right(6));
    str = gl_pChinaStakeMarket->GetNeteaseInquiringStockStr(900, false);
    pStock = gl_pChinaStakeMarket->GetStock(9889);
    strCompare = pStock->GetStakeCode();
    str2 = str.Left(7);
    EXPECT_STREQ(str2.Right(6), strCompare.Right(6));
    str = gl_pChinaStakeMarket->GetNeteaseInquiringStockStr(900, false);
    pStock = gl_pChinaStakeMarket->GetStock(10788);
    strCompare = pStock->GetStakeCode();
    str2 = str.Left(7);
    EXPECT_STREQ(str2.Right(6), strCompare.Right(6));
    str = gl_pChinaStakeMarket->GetNeteaseInquiringStockStr(900, false);
    EXPECT_EQ(gl_pChinaStakeMarket->GetNeteaseRTDataInquiringIndex(), 0);
    pStock = gl_pChinaStakeMarket->GetStock(11687);
    strCompare = pStock->GetStakeCode();
    str2 = str.Left(7);
    EXPECT_STREQ(str2.Right(6), strCompare.Right(6));
    str = gl_pChinaStakeMarket->GetNeteaseInquiringStockStr(900, false);
    pStock = gl_pChinaStakeMarket->GetStock(0);
    strCompare = pStock->GetStakeCode();
    str2 = str.Left(7);
    EXPECT_STREQ(str2.Right(6), strCompare.Right(6));
    gl_pChinaStakeMarket->ResetNeteaseRTDataInquiringIndex();
    gl_pChinaStakeMarket->ResetSinaRTDataInquiringIndex();
  }

  TEST_F(CChinaMarketTest, TestGetNeteaseDayLineInquiringStr) {
    CString str;
    bool fStatus = false;
    CChinaStockPtr pStock = gl_pChinaStakeMarket->GetStock(0);
    EXPECT_TRUE(pStock->IsDayLineNeedUpdate()) << _T("测试时使用teststock数据库，此数据库比较旧，最后更新时间不是昨日，故而活跃股票也需要更新日线");
    long lDate = pStock->GetDayLineEndDate();
    pStock->SetDayLineEndDate(gl_pChinaStakeMarket->GetFormatedMarketDate());
    pStock->SetDayLineNeedUpdate(false);
    pStock->SetDayLineEndDate(lDate); // 恢复原状
    pStock = gl_pChinaStakeMarket->GetStock(1);
    EXPECT_TRUE(pStock->IsDayLineNeedUpdate());
    pStock = gl_pChinaStakeMarket->GetStock(2);
    EXPECT_TRUE(pStock->IsDayLineNeedUpdate());
    fStatus = gl_pChinaStakeMarket->CreateNeteaseDayLineInquiringStr(str);
    EXPECT_TRUE(fStatus);
    EXPECT_STREQ(str, _T("0600001")) << _T("第一个股票已设置为无需查询日线历史数据");
    pStock = gl_pChinaStakeMarket->GetStock(1);
    EXPECT_FALSE(pStock->IsDayLineNeedUpdate());
    pStock = gl_pChinaStakeMarket->GetStock(2);
    EXPECT_TRUE(pStock->IsDayLineNeedUpdate());
    long lIPOStatus = pStock->GetIPOStatus();
    pStock->SetIPOStatus(__STOCK_NULL__);
    fStatus = gl_pChinaStakeMarket->CreateNeteaseDayLineInquiringStr(str);
    EXPECT_TRUE(fStatus);
    EXPECT_STREQ(str, _T("0600003")) << _T("第三个股票设置为无效股票");
    pStock->SetIPOStatus(lIPOStatus); // 恢复原状
    pStock = gl_pChinaStakeMarket->GetStock(3);
    EXPECT_FALSE(pStock->IsDayLineNeedUpdate());
    pStock = gl_pChinaStakeMarket->GetStock(4);
    lDate = pStock->GetDayLineEndDate();
    pStock->SetDayLineEndDate(gl_pChinaStakeMarket->GetFormatedMarketDate());
    EXPECT_TRUE(pStock->IsDayLineNeedUpdate()) << _T("标识尚未更新");
    fStatus = gl_pChinaStakeMarket->CreateNeteaseDayLineInquiringStr(str);
    EXPECT_TRUE(fStatus);
    EXPECT_STREQ(str, _T("0600005")) << _T("0600004的日线结束日已设置为最新，故而无需再更新日线");
    pStock->SetDayLineEndDate(lDate); // 恢复原状。
    pStock = gl_pChinaStakeMarket->GetStock(5);
    EXPECT_FALSE(pStock->IsDayLineNeedUpdate());
    pStock = gl_pChinaStakeMarket->GetStock(4);
    EXPECT_FALSE(pStock->IsDayLineNeedUpdate()) << _T("标识在查询下载股票时更新了");

    // 恢复原状
    pStock = gl_pChinaStakeMarket->GetStock(0);
    pStock->SetDayLineEndDate(lDate);
    for (int i = 0; i < 12000; i++) {
      pStock = gl_pChinaStakeMarket->GetStock(i);
      if (!pStock->IsDayLineNeedUpdate()) pStock->SetDayLineNeedUpdate(true);
    }
  }

  TEST_F(CChinaMarketTest, TestGetMinLineOffset) {
    tm tmCurrent;
    tmCurrent.tm_year = 2020 - 1900;
    tmCurrent.tm_mon = 1;
    tmCurrent.tm_mday = 1;
    tmCurrent.tm_hour = 9;
    tmCurrent.tm_min = 0;
    tmCurrent.tm_sec = 0;
    time_t tMarket = mktime(&tmCurrent);
    long lOffset = gl_pChinaStakeMarket->GetMinLineOffset(tMarket);
    EXPECT_EQ(lOffset, 0);
    tmCurrent.tm_hour = 10;
    tmCurrent.tm_min = 30;
    tmCurrent.tm_sec = 59;
    tMarket = mktime(&tmCurrent);
    lOffset = gl_pChinaStakeMarket->GetMinLineOffset(tMarket);
    EXPECT_EQ(lOffset, 60);
    tmCurrent.tm_hour = 12;
    tmCurrent.tm_min = 30;
    tmCurrent.tm_sec = 59;
    tMarket = mktime(&tmCurrent);
    lOffset = gl_pChinaStakeMarket->GetMinLineOffset(tMarket);
    EXPECT_EQ(lOffset, 119);
    tmCurrent.tm_hour = 14;
    tmCurrent.tm_min = 30;
    tmCurrent.tm_sec = 59;
    tMarket = mktime(&tmCurrent);
    lOffset = gl_pChinaStakeMarket->GetMinLineOffset(tMarket);
    EXPECT_EQ(lOffset, 210);
    tmCurrent.tm_hour = 15;
    tmCurrent.tm_min = 30;
    tmCurrent.tm_sec = 59;
    tMarket = mktime(&tmCurrent);
    lOffset = gl_pChinaStakeMarket->GetMinLineOffset(tMarket);
    EXPECT_EQ(lOffset, 239);
  }

  TEST_F(CChinaMarketTest, TestIsAStock) {
    CChinaStockPtr pstock = make_shared<CChinaStake>();
    pstock->SetStakeCode(_T("sh600000"));
    EXPECT_TRUE(gl_pChinaStakeMarket->IsAStock(pstock));
    pstock->SetStakeCode(_T("ss600000"));
    EXPECT_FALSE(gl_pChinaStakeMarket->IsAStock(pstock));
    pstock->SetStakeCode(_T("sz000001"));
    EXPECT_TRUE(gl_pChinaStakeMarket->IsAStock(pstock));
    pstock->SetStakeCode(_T("sz10001"));
    EXPECT_FALSE(gl_pChinaStakeMarket->IsAStock(pstock));
  }

  TEST_F(CChinaMarketTest, TestIsAStock2) {
    EXPECT_TRUE(gl_pChinaStakeMarket->IsAStock(_T("sh600000")));
    EXPECT_TRUE(gl_pChinaStakeMarket->IsAStock(_T("sh601198")));
    EXPECT_FALSE(gl_pChinaStakeMarket->IsAStock(_T("sh602102")));
    EXPECT_FALSE(gl_pChinaStakeMarket->IsAStock(_T("sh603604")));
    EXPECT_FALSE(gl_pChinaStakeMarket->IsAStock(_T("sh604604")));
    EXPECT_FALSE(gl_pChinaStakeMarket->IsAStock(_T("sh605604")));
    EXPECT_FALSE(gl_pChinaStakeMarket->IsAStock(_T("sh606604")));
    EXPECT_FALSE(gl_pChinaStakeMarket->IsAStock(_T("sh607604")));
    EXPECT_FALSE(gl_pChinaStakeMarket->IsAStock(_T("sh608604")));
    EXPECT_FALSE(gl_pChinaStakeMarket->IsAStock(_T("sh609604")));

    EXPECT_FALSE(gl_pChinaStakeMarket->IsAStock(_T("sz600000")));
    EXPECT_FALSE(gl_pChinaStakeMarket->IsAStock(_T("ss000001")));
    EXPECT_FALSE(gl_pChinaStakeMarket->IsAStock(_T("az000001")));
    EXPECT_FALSE(gl_pChinaStakeMarket->IsAStock(_T("sz200001")));
    EXPECT_TRUE(gl_pChinaStakeMarket->IsAStock(_T("sz000001")));
    EXPECT_TRUE(gl_pChinaStakeMarket->IsAStock(_T("sz002389")));
    EXPECT_FALSE(gl_pChinaStakeMarket->IsAStock(_T("sz003389")));
    EXPECT_FALSE(gl_pChinaStakeMarket->IsAStock(_T("sz004389")));
    EXPECT_FALSE(gl_pChinaStakeMarket->IsAStock(_T("sz005389")));
    EXPECT_FALSE(gl_pChinaStakeMarket->IsAStock(_T("sz006389")));
    EXPECT_FALSE(gl_pChinaStakeMarket->IsAStock(_T("sz007389")));
    EXPECT_FALSE(gl_pChinaStakeMarket->IsAStock(_T("sz008389")));
    EXPECT_FALSE(gl_pChinaStakeMarket->IsAStock(_T("sz009389")));
    EXPECT_FALSE(gl_pChinaStakeMarket->IsAStock(_T("sz001389")));
  }

  TEST_F(CChinaMarketTest, TestIsStock) {
    EXPECT_GT(gl_pChinaStakeMarket->GetTotalActiveStock(), 1);
    EXPECT_TRUE(gl_pChinaStakeMarket->IsStock(_T("sh600000")));
    EXPECT_FALSE(gl_pChinaStakeMarket->IsStock(_T("sh60000")));
  }

  TEST_F(CChinaMarketTest, TestGetStockName) {
    //未实现.由于stockName存储时使用的是UniCode制式，而本系统默认是Ansi制式，导致无法进行字符串对比。暂时不进行测试了。
    //EXPECT_STREQ(gl_pChinaStakeMarket->GetStakeName(_T("sh600000")), _T("浦发银行"));
    EXPECT_STREQ(gl_pChinaStakeMarket->GetStakeName(_T("sh60000")), _T("")); // 没找到返回空字符串
  }

  TEST_F(CChinaMarketTest, TestGetStockIndex) {
    long lIndex = -2;
    EXPECT_TRUE(gl_pChinaStakeMarket->GetStockIndex(_T("sh600000"), lIndex));
    EXPECT_STREQ(gl_pChinaStakeMarket->GetStock(lIndex)->GetStakeCode(), _T("sh600000"));
    EXPECT_FALSE(gl_pChinaStakeMarket->GetStockIndex(_T("sh60000"), lIndex));
    EXPECT_EQ(lIndex, -1);
  }

  TEST_F(CChinaMarketTest, TestIncreaseTotalActiveStock) {
    long l = gl_pChinaStakeMarket->GetTotalActiveStock();
    gl_pChinaStakeMarket->IncreaseActiveStockNumber();
    EXPECT_EQ(gl_pChinaStakeMarket->GetTotalActiveStock(), l + 1);
  }

  TEST_F(CChinaMarketTest, TestGetStockCode) {
    EXPECT_EQ(gl_pChinaStakeMarket->GetStock(_T("sh66000")), nullptr);
    EXPECT_FALSE(gl_pChinaStakeMarket->GetStock(_T("sh600001")) == nullptr);

    EXPECT_EQ(gl_pChinaStakeMarket->GetStock(-1), nullptr);
    EXPECT_EQ(gl_pChinaStakeMarket->GetStock(12000), nullptr);
    EXPECT_FALSE(gl_pChinaStakeMarket->GetStock(0) == nullptr);
    EXPECT_FALSE(gl_pChinaStakeMarket->GetStock(11999) == nullptr);
  }

  TEST_F(CChinaMarketTest, TestGetCurrentStock) {
    CChinaStockPtr pStock = gl_pChinaStakeMarket->GetStock(7);
    CChinaStockPtr pStock2 = gl_pChinaStakeMarket->GetStock(4);

    gl_pChinaStakeMarket->SetCurrentStock(pStock);
    EXPECT_EQ(gl_pChinaStakeMarket->GetCurrentStock(), pStock);
    EXPECT_TRUE(pStock->IsRecordRTData());
    EXPECT_TRUE(gl_pChinaStakeMarket->IsCurrentStockChanged());
    gl_pChinaStakeMarket->SetCurrentStockChanged(false);
    gl_pChinaStakeMarket->SetCurrentStock(pStock);
    EXPECT_FALSE(gl_pChinaStakeMarket->IsCurrentStockChanged());
    EXPECT_FALSE(pStock2->IsRecordRTData());
    gl_pChinaStakeMarket->SetCurrentStock(pStock2);
    EXPECT_TRUE(gl_pChinaStakeMarket->IsCurrentStockChanged());
    EXPECT_TRUE(pStock2->IsRecordRTData());
    gl_pChinaStakeMarket->SetCurrentStock(_T("sh600000"));
    pStock = gl_pChinaStakeMarket->GetCurrentStock();
    EXPECT_TRUE(pStock->IsRecordRTData());
    EXPECT_STREQ(pStock->GetStakeCode(), _T("sh600000"));
    gl_pChinaStakeMarket->ResetCurrentStock();
    EXPECT_FALSE(pStock->IsRecordRTData());
    EXPECT_EQ(gl_pChinaStakeMarket->GetCurrentStock(), nullptr);

    gl_pChinaStakeMarket->SetCurrentStockChanged(false);
  }

  TEST_F(CChinaMarketTest, TestUnloadDayLine) {
    EXPECT_TRUE(gl_pChinaStakeMarket->UnloadDayLine());
  }

  TEST_F(CChinaMarketTest, TestMarketReady) {
    EXPECT_TRUE(gl_pChinaStakeMarket->IsSystemReady()) << "中国股票市场默认为假, 但测试系统默认为真";
    gl_pChinaStakeMarket->SetSystemReady(true);
    EXPECT_TRUE(gl_pChinaStakeMarket->IsSystemReady());
    gl_pChinaStakeMarket->SetSystemReady(false);
    EXPECT_FALSE(gl_pChinaStakeMarket->IsSystemReady());

    gl_pChinaStakeMarket->SetSystemReady(true); // 恢复原态
  }

  TEST_F(CChinaMarketTest, TestIsTodayStockProcessed) {
    gl_pChinaStakeMarket->SetTodayStockProcessed(true);
    EXPECT_TRUE(gl_pChinaStakeMarket->IsTodayStockProcessed());
    EXPECT_FALSE(gl_pChinaStakeMarket->IsProcessingTodayStock());
    EXPECT_FALSE(gl_pChinaStakeMarket->IsTodayStockNotProcessed());
    gl_pChinaStakeMarket->SetTodayStockProcessed(false);
    EXPECT_TRUE(gl_pChinaStakeMarket->IsTodayStockNotProcessed());
    EXPECT_FALSE(gl_pChinaStakeMarket->IsTodayStockProcessed());
    EXPECT_FALSE(gl_pChinaStakeMarket->IsProcessingTodayStock());
    gl_pChinaStakeMarket->SetProcessingTodayStock();
    EXPECT_TRUE(gl_pChinaStakeMarket->IsProcessingTodayStock());
  }

  TEST_F(CChinaMarketTest, TestGetCurrentSelectedPosition) {
    EXPECT_EQ(gl_pChinaStakeMarket->GetCurrentSelectedPosition(), 0);
    gl_pChinaStakeMarket->SetCurrentSelectedPosition(10101010);
    EXPECT_EQ(gl_pChinaStakeMarket->GetCurrentSelectedPosition(), 10101010);

    gl_pChinaStakeMarket->SetCurrentSelectedPosition(0);
  }

  TEST_F(CChinaMarketTest, TestGetCurrentSelectedStockSet) {
    EXPECT_EQ(gl_pChinaStakeMarket->GetCurrentSelectedStockSet(), -1);
    gl_pChinaStakeMarket->SetCurrentSelectedStockSet(10);
    EXPECT_EQ(gl_pChinaStakeMarket->GetCurrentSelectedStockSet(), 10);

    gl_pChinaStakeMarket->SetCurrentSelectedStockSet(-1);
  }

  TEST_F(CChinaMarketTest, TestIsChoiced10RSStrongStockSet) {
    gl_pChinaStakeMarket->SetChoiced10RSStrongStockSet(true);
    EXPECT_TRUE(gl_pChinaStakeMarket->IsChoiced10RSStrongStockSet());
    gl_pChinaStakeMarket->SetChoiced10RSStrongStockSet(false);
    EXPECT_FALSE(gl_pChinaStakeMarket->IsChoiced10RSStrongStockSet());
  }

  TEST_F(CChinaMarketTest, TestIsChoiced10RSStrong1StockSet) {
    gl_pChinaStakeMarket->SetChoiced10RSStrong1StockSet(true);
    EXPECT_TRUE(gl_pChinaStakeMarket->IsChoiced10RSStrong1StockSet());
    gl_pChinaStakeMarket->SetChoiced10RSStrong1StockSet(false);
    EXPECT_FALSE(gl_pChinaStakeMarket->IsChoiced10RSStrong1StockSet());
  }
  TEST_F(CChinaMarketTest, TestIsChoiced10RSStrong2StockSet) {
    gl_pChinaStakeMarket->SetChoiced10RSStrong2StockSet(true);
    EXPECT_TRUE(gl_pChinaStakeMarket->IsChoiced10RSStrong2StockSet());
    gl_pChinaStakeMarket->SetChoiced10RSStrong2StockSet(false);
    EXPECT_FALSE(gl_pChinaStakeMarket->IsChoiced10RSStrong2StockSet());
  }

  TEST_F(CChinaMarketTest, TestIsDayLineNeedSaving) {
    EXPECT_FALSE(gl_pChinaStakeMarket->IsDayLineNeedSaving());
    CChinaStockPtr pStock = gl_pChinaStakeMarket->GetStock(0);
    pStock->SetDayLineNeedSaving(true);
    EXPECT_TRUE(gl_pChinaStakeMarket->IsDayLineNeedSaving());
    pStock->SetDayLineNeedSaving(false);
  }

  TEST_F(CChinaMarketTest, TestIsDayLineNeedUpdate) {
    CChinaStockPtr pStock = nullptr;
    ASSERT(gl_pChinaStakeMarket->GetTotalStock() == 12000);
    EXPECT_TRUE(gl_pChinaStakeMarket->IsDayLineNeedUpdate());
    for (int i = 0; i < gl_pChinaStakeMarket->GetTotalStock(); i++) {
      pStock = gl_pChinaStakeMarket->GetStock(i);
      if (pStock->IsDayLineNeedUpdate()) pStock->SetDayLineNeedUpdate(false);
    }
    EXPECT_FALSE(gl_pChinaStakeMarket->IsDayLineNeedUpdate());
    for (int i = 0; i < gl_pChinaStakeMarket->GetTotalStock(); i++) {
      pStock = gl_pChinaStakeMarket->GetStock(i);
      if (!pStock->IsDayLineNeedUpdate()) pStock->SetDayLineNeedUpdate(true);
    }
  }

  TEST_F(CChinaMarketTest, TestTaskProcessDayLineGetFromNeeteaseServer) {
    CChinaStockPtr pStock = gl_pChinaStakeMarket->GetStock(_T("sh600666"));
    EXPECT_FALSE(pStock->IsDayLineNeedProcess());
    pStock->SetDayLineNeedProcess(true);
    EXPECT_TRUE(gl_pChinaStakeMarket->TaskProcessDayLineGetFromNeeteaseServer());
    EXPECT_FALSE(pStock->IsDayLineNeedProcess());
    EXPECT_EQ(gl_pChinaStakeMarket->GetDayLineNeedProcessNumber(), 0);
  }

  TEST_F(CChinaMarketTest, TestIsLoadSelectedStock) {
    EXPECT_FALSE(gl_pChinaStakeMarket->IsLoadSelectedStock());
    gl_pChinaStakeMarket->SetLoadSelectedStock(true);
    EXPECT_TRUE(gl_pChinaStakeMarket->IsLoadSelectedStock());
    gl_pChinaStakeMarket->SetLoadSelectedStock(false);
    EXPECT_FALSE(gl_pChinaStakeMarket->IsLoadSelectedStock());
  }

  TEST_F(CChinaMarketTest, TestIsRecordingRTData) {
    EXPECT_FALSE(gl_pChinaStakeMarket->IsRecordingRTData());
    gl_pChinaStakeMarket->SetRecordRTData(true);
    EXPECT_TRUE(gl_pChinaStakeMarket->IsRecordingRTData());
    gl_pChinaStakeMarket->SetRecordRTData(false);
    EXPECT_FALSE(gl_pChinaStakeMarket->IsRecordingRTData());
  }

  TEST_F(CChinaMarketTest, TestSetUpdateStockCodeDB) {
    EXPECT_FALSE(gl_pChinaStakeMarket->IsUpdateStockCodeDB());
    gl_pChinaStakeMarket->SetUpdateStockCodeDB(true);
    EXPECT_TRUE(gl_pChinaStakeMarket->IsUpdateStockCodeDB());
    gl_pChinaStakeMarket->SetUpdateStockCodeDB(false);
    EXPECT_FALSE(gl_pChinaStakeMarket->IsUpdateStockCodeDB());
  }
  TEST_F(CChinaMarketTest, TestSetUpdateOptionDB) {
    EXPECT_FALSE(gl_pChinaStakeMarket->IsUpdateOptionDB());
    gl_pChinaStakeMarket->SetUpdateOptionDB(true);
    EXPECT_TRUE(gl_pChinaStakeMarket->IsUpdateOptionDB());
    gl_pChinaStakeMarket->SetUpdateOptionDB(false);
    EXPECT_FALSE(gl_pChinaStakeMarket->IsUpdateOptionDB());
  }

  TEST_F(CChinaMarketTest, TestSetUpdateChoicedStockDB) {
    EXPECT_FALSE(gl_pChinaStakeMarket->IsUpdateChoicedStockDB());
    gl_pChinaStakeMarket->SetUpdateChoicedStockDB(true);
    EXPECT_TRUE(gl_pChinaStakeMarket->IsUpdateChoicedStockDB());
    gl_pChinaStakeMarket->SetUpdateChoicedStockDB(false);
    EXPECT_FALSE(gl_pChinaStakeMarket->IsUpdateChoicedStockDB());
  }

  TEST_F(CChinaMarketTest, TestSetRTDataSetCleared) {
    EXPECT_FALSE(gl_pChinaStakeMarket->IsRTDataSetCleared());
    gl_pChinaStakeMarket->SetRTDataSetCleared(true);
    EXPECT_TRUE(gl_pChinaStakeMarket->IsRTDataSetCleared());
    gl_pChinaStakeMarket->SetRTDataSetCleared(false);
    EXPECT_FALSE(gl_pChinaStakeMarket->IsRTDataSetCleared());
  }

  TEST_F(CChinaMarketTest, TestSetSavingTempData) {
    EXPECT_TRUE(gl_pChinaStakeMarket->IsSavingTempData());
    gl_pChinaStakeMarket->SetSavingTempData(false);
    EXPECT_FALSE(gl_pChinaStakeMarket->IsSavingTempData());
    gl_pChinaStakeMarket->SetSavingTempData(true);
    EXPECT_TRUE(gl_pChinaStakeMarket->IsSavingTempData());
  }

  TEST_F(CChinaMarketTest, TestTaskResetMarket) {
    tm tm_;
    tm_.tm_wday = 1; // 星期一
    gl_pChinaStakeMarket->__TEST_SetMarketTM(tm_);
    EXPECT_TRUE(gl_pChinaStakeMarket->IsPermitResetMarket());
    EXPECT_TRUE(gl_pChinaStakeMarket->IsResetMarket());
    gl_pChinaStakeMarket->SetSystemReady(true);
    gl_pChinaStakeMarket->SetResetMarket(false);
    gl_pChinaStakeMarket->TaskResetMarket(91259);
    EXPECT_FALSE(gl_pChinaStakeMarket->IsResetMarket());
    gl_pChinaStakeMarket->TaskResetMarket(91400);
    EXPECT_FALSE(gl_pChinaStakeMarket->IsResetMarket()) << _T("第一次重启市场，其结束时间必须在9:14之前，这样才能保证只运行了一次（此函数必须每分钟调度一次");
    gl_pChinaStakeMarket->SetSystemReady(true);
    gl_pChinaStakeMarket->TaskResetMarket(91300);
    EXPECT_TRUE(gl_pChinaStakeMarket->IsResetMarket());
    EXPECT_TRUE(gl_pChinaStakeMarket->IsPermitResetMarket());
    EXPECT_FALSE(gl_pChinaStakeMarket->IsSystemReady());
  }

  TEST_F(CChinaMarketTest, TestTaskResetMarket2) {
    tm tm_;
    tm_.tm_wday = 0; // 星期日
    gl_pChinaStakeMarket->__TEST_SetMarketTM(tm_);
    EXPECT_TRUE(gl_pChinaStakeMarket->IsPermitResetMarket());
    gl_pChinaStakeMarket->SetSystemReady(true);
    gl_pChinaStakeMarket->SetResetMarket(false);
    gl_pChinaStakeMarket->TaskResetMarket(91259);
    EXPECT_FALSE(gl_pChinaStakeMarket->IsResetMarket());
    gl_pChinaStakeMarket->TaskResetMarket(91401);
    EXPECT_FALSE(gl_pChinaStakeMarket->IsResetMarket());
    gl_pChinaStakeMarket->TaskResetMarket(91300);
    EXPECT_FALSE(gl_pChinaStakeMarket->IsResetMarket());
    EXPECT_TRUE(gl_pChinaStakeMarket->IsPermitResetMarket());
    EXPECT_TRUE(gl_pChinaStakeMarket->IsSystemReady());
  }

  TEST_F(CChinaMarketTest, TestTaskResetMarket3) {
    tm tm_;
    tm_.tm_wday = 1; // 星期一
    gl_pChinaStakeMarket->__TEST_SetMarketTM(tm_);
    gl_pChinaStakeMarket->SetSystemReady(true);
    gl_pChinaStakeMarket->SetStockNeedUpdated(1001);
    EXPECT_TRUE(gl_pChinaStakeMarket->TooManyStocksNeedUpdated());
    EXPECT_TRUE(gl_pChinaStakeMarket->IsPermitResetMarket());
    EXPECT_TRUE(gl_pChinaStakeMarket->IsResetMarket());
    gl_pChinaStakeMarket->SetResetMarket(false);
    gl_pChinaStakeMarket->TaskResetMarket(91259);
    EXPECT_FALSE(gl_pChinaStakeMarket->IsResetMarket());
    gl_pChinaStakeMarket->TaskResetMarket(91400);
    EXPECT_FALSE(gl_pChinaStakeMarket->IsResetMarket()) << _T("第一次重启市场，其结束时间必须在9:14之前，这样才能保证只运行了一次（此函数必须每分钟调度一次");
    gl_pChinaStakeMarket->SetSystemReady(true);
    gl_pChinaStakeMarket->TaskResetMarket(91300);
    EXPECT_FALSE(gl_pChinaStakeMarket->IsResetMarket());
    EXPECT_TRUE(gl_pChinaStakeMarket->IsPermitResetMarket());
    EXPECT_TRUE(gl_pChinaStakeMarket->IsSystemReady());

    // 恢复系统原态
    gl_pChinaStakeMarket->SetStockNeedUpdated(0);
  }

  TEST_F(CChinaMarketTest, TestTaskResetMarketAgain) {
    tm tm_;
    tm_.tm_wday = 1;
    gl_pChinaStakeMarket->SetSystemReady(false);
    gl_pChinaStakeMarket->__TEST_SetMarketTM(tm_);
    EXPECT_TRUE(gl_pChinaStakeMarket->IsPermitResetMarket());
    EXPECT_FALSE(gl_pChinaStakeMarket->IsSystemReady());
    EXPECT_TRUE(gl_pChinaStakeMarket->IsResetMarket());
    EXPECT_TRUE(gl_pChinaStakeMarket->TaskResetMarketAgain(92459));
    EXPECT_FALSE(gl_pChinaStakeMarket->IsSystemReady());
    EXPECT_TRUE(gl_pChinaStakeMarket->IsResetMarket());
    EXPECT_TRUE(gl_pChinaStakeMarket->IsPermitResetMarket());
    EXPECT_TRUE(gl_pChinaStakeMarket->TaskResetMarketAgain(93001));
    EXPECT_FALSE(gl_pChinaStakeMarket->IsSystemReady());
    EXPECT_FALSE(gl_pChinaStakeMarket->IsPermitResetMarket());
    EXPECT_TRUE(gl_pChinaStakeMarket->IsResetMarket());
    gl_pChinaStakeMarket->SetResetMarket(false);
    gl_pChinaStakeMarket->SetSystemReady(false);
    gl_pChinaStakeMarket->SetPermitResetMarket(true);
    EXPECT_TRUE(gl_pChinaStakeMarket->TaskResetMarketAgain(92500));
    EXPECT_FALSE(gl_pChinaStakeMarket->IsSystemReady());
    EXPECT_FALSE(gl_pChinaStakeMarket->IsPermitResetMarket());
    EXPECT_TRUE(gl_pChinaStakeMarket->IsResetMarket());
  }

  TEST_F(CChinaMarketTest, TestTaskClearChoicedRTDataSet) {
    EXPECT_FALSE(gl_pChinaStakeMarket->IsRTDataSetCleared());
    gl_pChinaStakeMarket->TaskClearChoicedRTDataSet(92900);
    EXPECT_FALSE(gl_pChinaStakeMarket->IsRTDataSetCleared());
    gl_pChinaStakeMarket->TaskClearChoicedRTDataSet(92901);
    EXPECT_TRUE(gl_pChinaStakeMarket->IsRTDataSetCleared());
  }

  TEST_F(CChinaMarketTest, TestIsCurrentEditStockChanged) {
    EXPECT_FALSE(gl_pChinaStakeMarket->IsCurrentEditStockChanged());
    gl_pChinaStakeMarket->SetCurrentEditStockChanged(true);
    EXPECT_TRUE(gl_pChinaStakeMarket->IsCurrentEditStockChanged());
    gl_pChinaStakeMarket->SetCurrentEditStockChanged(false);
    EXPECT_FALSE(gl_pChinaStakeMarket->IsCurrentEditStockChanged());
  }

  TEST_F(CChinaMarketTest, TestAddChoicedStock) {
    auto pStock = gl_pChinaStakeMarket->GetStock(1);
    gl_pChinaStakeMarket->AddChoicedStock(pStock);
    EXPECT_EQ(gl_pChinaStakeMarket->GetChoicedStockSize(), 1);
    pStock = gl_pChinaStakeMarket->GetStock(2);
    EXPECT_TRUE(gl_pChinaStakeMarket->AddChoicedStock(pStock));
    EXPECT_EQ(gl_pChinaStakeMarket->GetChoicedStockSize(), 2);
    pStock = gl_pChinaStakeMarket->GetStock(1);
    EXPECT_FALSE(gl_pChinaStakeMarket->AddChoicedStock(pStock));
    EXPECT_EQ(gl_pChinaStakeMarket->GetChoicedStockSize(), 2);

    EXPECT_TRUE(gl_pChinaStakeMarket->DeleteChoicedStock(pStock));
    pStock = gl_pChinaStakeMarket->GetStock(5);
    EXPECT_EQ(gl_pChinaStakeMarket->GetChoicedStockSize(), 1);
    EXPECT_FALSE(gl_pChinaStakeMarket->DeleteChoicedStock(pStock));
    gl_pChinaStakeMarket->ClearChoiceStockContainer();
  }

  TEST_F(CChinaMarketTest, TestAddChoicedStock2) {
    EXPECT_EQ(gl_pChinaStakeMarket->GetChoicedStockSize(), 0);
    CChinaStockPtr pStock = gl_pChinaStakeMarket->GetStock(0);
    gl_pChinaStakeMarket->AddChoicedStock(pStock);
    EXPECT_EQ(gl_pChinaStakeMarket->GetChoicedStockSize(), 1);
    gl_pChinaStakeMarket->ClearChoiceStockContainer();
    EXPECT_EQ(gl_pChinaStakeMarket->GetChoicedStockSize(), 0);
  }

  TEST_F(CChinaMarketTest, TestCheckMarketOpen) {
    tm tm_;
    tm_.tm_wday = 1;
    gl_pChinaStakeMarket->__TEST_SetMarketTM(tm_);
    EXPECT_FALSE(gl_pChinaStakeMarket->TaskCheckMarketOpen(92800));
    EXPECT_FALSE(gl_pChinaStakeMarket->IsMarketOpened());
    EXPECT_TRUE(gl_pChinaStakeMarket->TaskCheckMarketOpen(92801));
    EXPECT_TRUE(gl_pChinaStakeMarket->IsMarketOpened());
    EXPECT_TRUE(gl_pChinaStakeMarket->TaskCheckMarketOpen(150559));
    EXPECT_TRUE(gl_pChinaStakeMarket->IsMarketOpened());
    EXPECT_FALSE(gl_pChinaStakeMarket->TaskCheckMarketOpen(150600));
    tm_.tm_wday = 0;
    gl_pChinaStakeMarket->__TEST_SetMarketTM(tm_);
    EXPECT_FALSE(gl_pChinaStakeMarket->TaskCheckMarketOpen(92859));
    EXPECT_FALSE(gl_pChinaStakeMarket->IsMarketOpened());
    EXPECT_FALSE(gl_pChinaStakeMarket->TaskCheckMarketOpen(92900));
    EXPECT_FALSE(gl_pChinaStakeMarket->IsMarketOpened());
    EXPECT_FALSE(gl_pChinaStakeMarket->TaskCheckMarketOpen(150559));
    EXPECT_FALSE(gl_pChinaStakeMarket->IsMarketOpened());
    EXPECT_FALSE(gl_pChinaStakeMarket->TaskCheckMarketOpen(150600));
    EXPECT_FALSE(gl_pChinaStakeMarket->IsMarketOpened());
  }

  TEST_F(CChinaMarketTest, TestCheckStartReceivingData) {
    tm tm_;
    tm_.tm_wday = 1;
    gl_pChinaStakeMarket->__TEST_SetMarketTM(tm_);
    EXPECT_FALSE(gl_pChinaStakeMarket->TaskCheckStartReceivingData(91159));
    EXPECT_FALSE(gl_pChinaStakeMarket->IsStartReceivingData());
    EXPECT_TRUE(gl_pChinaStakeMarket->TaskCheckStartReceivingData(91200));
    EXPECT_TRUE(gl_pChinaStakeMarket->IsStartReceivingData());
    EXPECT_TRUE(gl_pChinaStakeMarket->TaskCheckStartReceivingData(113500));
    EXPECT_TRUE(gl_pChinaStakeMarket->IsStartReceivingData());
    EXPECT_FALSE(gl_pChinaStakeMarket->TaskCheckStartReceivingData(113501));
    EXPECT_FALSE(gl_pChinaStakeMarket->IsStartReceivingData());
    EXPECT_FALSE(gl_pChinaStakeMarket->TaskCheckStartReceivingData(125459));
    EXPECT_FALSE(gl_pChinaStakeMarket->IsStartReceivingData());
    EXPECT_TRUE(gl_pChinaStakeMarket->TaskCheckStartReceivingData(125500));
    EXPECT_TRUE(gl_pChinaStakeMarket->IsStartReceivingData());
    EXPECT_TRUE(gl_pChinaStakeMarket->TaskCheckStartReceivingData(150630));
    EXPECT_TRUE(gl_pChinaStakeMarket->IsStartReceivingData());
    EXPECT_FALSE(gl_pChinaStakeMarket->TaskCheckStartReceivingData(150631));
    tm_.tm_wday = 0;
    gl_pChinaStakeMarket->__TEST_SetMarketTM(tm_);
    EXPECT_FALSE(gl_pChinaStakeMarket->TaskCheckStartReceivingData(91459));
    EXPECT_FALSE(gl_pChinaStakeMarket->IsStartReceivingData());
    EXPECT_FALSE(gl_pChinaStakeMarket->TaskCheckStartReceivingData(91500));
    EXPECT_FALSE(gl_pChinaStakeMarket->IsStartReceivingData());
    EXPECT_FALSE(gl_pChinaStakeMarket->TaskCheckStartReceivingData(113459));
    EXPECT_FALSE(gl_pChinaStakeMarket->IsStartReceivingData());
    EXPECT_FALSE(gl_pChinaStakeMarket->TaskCheckStartReceivingData(113500));
    EXPECT_FALSE(gl_pChinaStakeMarket->IsStartReceivingData());
    EXPECT_FALSE(gl_pChinaStakeMarket->TaskCheckStartReceivingData(125459));
    EXPECT_FALSE(gl_pChinaStakeMarket->IsStartReceivingData());
    EXPECT_FALSE(gl_pChinaStakeMarket->TaskCheckStartReceivingData(125500));
    EXPECT_FALSE(gl_pChinaStakeMarket->IsStartReceivingData());
    EXPECT_FALSE(gl_pChinaStakeMarket->TaskCheckStartReceivingData(150630));
    EXPECT_FALSE(gl_pChinaStakeMarket->IsStartReceivingData());
    EXPECT_FALSE(gl_pChinaStakeMarket->TaskCheckStartReceivingData(150631));
    EXPECT_FALSE(gl_pChinaStakeMarket->IsStartReceivingData());
  }

  TEST_F(CChinaMarketTest, TestTaskCheckDayLineDB) {
    EXPECT_EQ(gl_pChinaStakeMarket->GetDayLineNeedUpdateNumber(), 12000);
    gl_pChinaStakeMarket->SetSaveDayLine(true);
    gl_pChinaStakeMarket->SetDayLineNeedSaveNumber(1);
    gl_pChinaStakeMarket->SetDayLineNeedProcessNumber(1);
    gl_pChinaStakeMarket->SetDayLineNeedUpdateNumber(1);
    gl_pChinaStakeMarket->TaskCheckDayLineDB();
    EXPECT_TRUE(gl_pChinaStakeMarket->IsSaveDayLine());
    gl_pChinaStakeMarket->SetDayLineNeedProcessNumber(0);
    gl_pChinaStakeMarket->TaskCheckDayLineDB();
    EXPECT_TRUE(gl_pChinaStakeMarket->IsSaveDayLine());
    gl_pChinaStakeMarket->SetDayLineNeedSaveNumber(0);
    gl_pChinaStakeMarket->TaskCheckDayLineDB();
    EXPECT_TRUE(gl_pChinaStakeMarket->IsSaveDayLine());
    gl_pChinaStakeMarket->SetDayLineNeedUpdateNumber(0);
    gl_pChinaStakeMarket->TaskCheckDayLineDB();
    EXPECT_FALSE(gl_pChinaStakeMarket->IsSaveDayLine());
    gl_pChinaStakeMarket->SetSaveDayLine(true);
    gl_pChinaStakeMarket->GetStock(_T("sh600000"))->SetDayLineDBUpdated(true);
    EXPECT_TRUE(gl_pChinaStakeMarket->IsDayLineDBUpdated());
    gl_pChinaStakeMarket->TaskCheckDayLineDB();
    EXPECT_FALSE(gl_pChinaStakeMarket->IsDayLineDBUpdated());
    EXPECT_TRUE(gl_pChinaStakeMarket->IsUpdateStockCodeDB());

    gl_pChinaStakeMarket->SetUpdateStockCodeDB(false);

    gl_pChinaStakeMarket->SetDayLineNeedUpdateNumber(12000);
  }

  TEST_F(CChinaMarketTest, TestIncreaseNeteaseDayLineInquiringIndex) {
    long k = 0;
    EXPECT_EQ(gl_pChinaStakeMarket->GetTotalStock(), 12000);
    int i = gl_pChinaStakeMarket->IncreaseStockInquiringIndex(k);
    EXPECT_EQ(i, 1);
    EXPECT_EQ(k, 1);
    i = gl_pChinaStakeMarket->IncreaseStockInquiringIndex(k);
    EXPECT_EQ(i, 2);
    EXPECT_EQ(k, 2);
    k = 11999;
    i = gl_pChinaStakeMarket->IncreaseStockInquiringIndex(k);
    EXPECT_EQ(k, 0);
    EXPECT_EQ(i, 0);
  }

  TEST_F(CChinaMarketTest, TestStepToActiveStockIndex) {
    long i = 0; //
    gl_pChinaStakeMarket->StepToActiveStockIndex(i);
    EXPECT_EQ(i, 0); // sh600000为活跃股票
    i++;
    gl_pChinaStakeMarket->StepToActiveStockIndex(i);
    EXPECT_EQ(i, 4); // sh600004为活跃股票，之间三个是退市股
    for (int j = 1; j < 4; j++) {
      EXPECT_FALSE(gl_pChinaStakeMarket->GetStock(j)->IsActive());
    }
  }

  TEST_F(CChinaMarketTest, TestTaskProcessRTData) {
    gl_ThreadStatus.SetRTDataNeedCalculate(true);
    EXPECT_TRUE(gl_pChinaStakeMarket->TaskProcessRTData());
    gl_ThreadStatus.SetRTDataNeedCalculate(false);
  }

  TEST_F(CChinaMarketTest, TestSetCheckActiveStockFlag) {
    EXPECT_TRUE(gl_pChinaStakeMarket->IsCheckActiveStock());
    gl_pChinaStakeMarket->TaskSetCheckActiveStockFlag(91459);
    EXPECT_FALSE(gl_pChinaStakeMarket->IsCheckActiveStock());
    gl_pChinaStakeMarket->TaskSetCheckActiveStockFlag(91500);
    EXPECT_TRUE(gl_pChinaStakeMarket->IsCheckActiveStock());
    gl_pChinaStakeMarket->TaskSetCheckActiveStockFlag(92859);
    EXPECT_TRUE(gl_pChinaStakeMarket->IsCheckActiveStock());
    gl_pChinaStakeMarket->TaskSetCheckActiveStockFlag(92900);
    EXPECT_FALSE(gl_pChinaStakeMarket->IsCheckActiveStock());
    gl_pChinaStakeMarket->TaskSetCheckActiveStockFlag(113059);
    EXPECT_FALSE(gl_pChinaStakeMarket->IsCheckActiveStock());
    gl_pChinaStakeMarket->TaskSetCheckActiveStockFlag(113100);
    EXPECT_TRUE(gl_pChinaStakeMarket->IsCheckActiveStock());
    gl_pChinaStakeMarket->TaskSetCheckActiveStockFlag(125859);
    EXPECT_TRUE(gl_pChinaStakeMarket->IsCheckActiveStock());
    gl_pChinaStakeMarket->TaskSetCheckActiveStockFlag(125900);
    EXPECT_FALSE(gl_pChinaStakeMarket->IsCheckActiveStock());
    gl_pChinaStakeMarket->TaskSetCheckActiveStockFlag(150000);
    EXPECT_FALSE(gl_pChinaStakeMarket->IsCheckActiveStock());
  }

  TEST_F(CChinaMarketTest, TestLoadStockCodeDB) {
    // 股票代码数据库在全局环境设置时即已装入测试系统，故而直接测试即可。
    CChinaStockPtr pStock = nullptr;
    pStock = gl_pChinaStakeMarket->GetStock(0);
    EXPECT_TRUE(pStock->IsIPOed());
    EXPECT_STREQ(pStock->GetStakeCode(), _T("sh600000"));
    EXPECT_EQ(pStock->GetDayLineStartDate(), 19991110);
    EXPECT_TRUE(pStock->IsActive());
    pStock = gl_pChinaStakeMarket->GetStock(1);
    EXPECT_TRUE(pStock->IsDelisted());
    EXPECT_STREQ(pStock->GetStakeCode(), _T("sh600001"));
    EXPECT_EQ(pStock->GetDayLineStartDate(), 19980122);
    EXPECT_EQ(pStock->GetDayLineEndDate(), 20091215);
    EXPECT_FALSE(pStock->IsActive());
  }

  TEST_F(CChinaMarketTest, TestGetLastLoginDate) {
    gl_pChinaStakeMarket->SetLastLoginDate(19900102);
    EXPECT_EQ(gl_pChinaStakeMarket->GetLastLoginDate(), 19900102);
  }

  TEST_F(CChinaMarketTest, TestGetRSStartDate) {
    gl_pChinaStakeMarket->SetRSStartDate(19900202);
    EXPECT_EQ(gl_pChinaStakeMarket->GetRSStartDate(), 19900202);
  }

  TEST_F(CChinaMarketTest, TestGetRSEndDate) {
    gl_pChinaStakeMarket->SetRSEndDate(19900302);
    EXPECT_EQ(gl_pChinaStakeMarket->GetRSEndDate(), 19900302);
  }

  TEST_F(CChinaMarketTest, TestGetNewestTransactionTime) {
    gl_pChinaStakeMarket->SetNewestTransactionTime(10101010);
    EXPECT_EQ(gl_pChinaStakeMarket->GetNewestTransactionTime(), 10101010);
  }

  TEST_F(CChinaMarketTest, TestGetStockCodeForInquiringSinaRTData) {
    gl_pChinaStakeMarket->SetStockCodeForInquiringRTData(_T("sh601919"));
    EXPECT_EQ(gl_pChinaStakeMarket->GetStockCodeForInquiringRTData(), _T("sh601919"));
  }

  TEST_F(CChinaMarketTest, TestGetReadingNeteaseDayLineDataTime) {
    gl_pChinaStakeMarket->SetStockCodeForInquiringNeteaseDayLine(_T("0600000"));
    EXPECT_EQ(gl_pChinaStakeMarket->GetStockCodeForInquiringNeteaseDayLine(), _T("0600000"));
  }

  TEST_F(CChinaMarketTest, TestGetReadingTengxunRTDataTime) {
    time_t tt = 1010101010;
    gl_pChinaStakeMarket->SetReadingTengxunRTDataTime(tt);
    EXPECT_EQ(gl_pChinaStakeMarket->GetReadingTengxunRTDataTime(), tt);
  }

  TEST_F(CChinaMarketTest, TesstIsTodayTempRTDataLoaded) {
    EXPECT_FALSE(gl_pChinaStakeMarket->IsTodayTempRTDataLoaded());
    gl_pChinaStakeMarket->SetTodayTempRTDataLoaded(true);
    EXPECT_TRUE(gl_pChinaStakeMarket->IsTodayTempRTDataLoaded());
    gl_pChinaStakeMarket->SetTodayTempRTDataLoaded(false);
    EXPECT_FALSE(gl_pChinaStakeMarket->IsTodayTempRTDataLoaded());
  }

  TEST_F(CChinaMarketTest, TesstIsUpdatedStakeCode) {
    EXPECT_FALSE(gl_pChinaStakeMarket->IsUpdatedStakeCode());
    gl_pChinaStakeMarket->SetUpdatedStakeCode(true);
    EXPECT_TRUE(gl_pChinaStakeMarket->IsUpdatedStakeCode());
    gl_pChinaStakeMarket->SetUpdatedStakeCode(false);
    EXPECT_FALSE(gl_pChinaStakeMarket->IsUpdatedStakeCode());
  }

  TEST_F(CChinaMarketTest, TesstIsCheckActiveStock) {
    EXPECT_TRUE(gl_pChinaStakeMarket->IsCheckActiveStock());
    gl_pChinaStakeMarket->SetCheckActiveStock(false);
    EXPECT_FALSE(gl_pChinaStakeMarket->IsCheckActiveStock());
    gl_pChinaStakeMarket->SetCheckActiveStock(true);
    EXPECT_TRUE(gl_pChinaStakeMarket->IsCheckActiveStock());
  }

  TEST_F(CChinaMarketTest, TestGetTotalActiveStock) {
    long l = gl_pChinaStakeMarket->GetTotalActiveStock();
    gl_pChinaStakeMarket->SetTotalActiveStock(4000);
    EXPECT_EQ(gl_pChinaStakeMarket->GetTotalActiveStock(), 4000);
    gl_pChinaStakeMarket->SetTotalActiveStock(l);
  }

  TEST_F(CChinaMarketTest, TestGetStockPtr) {
    CChinaStockPtr pStock = nullptr;
    long lIndex = -1;
    pStock = gl_pChinaStakeMarket->GetStock(_T("sh600000"));
    EXPECT_TRUE(gl_pChinaStakeMarket->GetStockIndex(_T("sh600000"), lIndex));
    EXPECT_EQ(lIndex, 0);
    EXPECT_STREQ(pStock->GetStakeCode(), _T("sh600000"));
  }

  TEST_F(CChinaMarketTest, TestIsInvalidNeteaseRTData) {
    CWebDataPtr pWebDataReceived;
    pWebDataReceived = make_shared<CWebData>();
    CString str = _T("_ntes_quote_callback({ });");
    pWebDataReceived->SetData(str.GetBuffer(), str.GetLength(), 0);
    pWebDataReceived->SetBufferLength(str.GetLength());
    pWebDataReceived->ResetCurrentPos();
    EXPECT_TRUE(gl_pChinaStakeMarket->IsInvalidNeteaseRTData(*pWebDataReceived));
    str = _T("_ntes_quote_callback({});");
    pWebDataReceived->SetData(str.GetBuffer(), str.GetLength(), 0);
    pWebDataReceived->SetBufferLength(str.GetLength());
    pWebDataReceived->ResetCurrentPos();
    EXPECT_FALSE(gl_pChinaStakeMarket->IsInvalidNeteaseRTData(*pWebDataReceived));
  }

  TEST_F(CChinaMarketTest, TestIsValidNeteaseRTDataPrefix) {
    CWebDataPtr pWebDataReceived;
    pWebDataReceived = make_shared<CWebData>();
    CString str = _T("_ntes_quote_callback({\"");
    pWebDataReceived->SetData(str.GetBuffer(), str.GetLength(), 0);
    pWebDataReceived->SetBufferLength(str.GetLength());
    pWebDataReceived->ResetCurrentPos();
    EXPECT_TRUE(gl_pChinaStakeMarket->IsValidNeteaseRTDataPrefix(*pWebDataReceived));
    str = _T("_ntes_quo_callback({\"");
    pWebDataReceived->SetData((LPSTR)str.GetBuffer(), str.GetLength(), 0);
    pWebDataReceived->SetBufferLength(str.GetLength());
    pWebDataReceived->ResetCurrentPos();
    EXPECT_FALSE(gl_pChinaStakeMarket->IsValidNeteaseRTDataPrefix(*pWebDataReceived));
  }

  TEST_F(CChinaMarketTest, TestIsValidTengxunRTDataPrefix) {
    CWebDataPtr pWebDataReceived;
    pWebDataReceived = make_shared<CWebData>();
    CString str = _T("v_pv_none_match=\"1\";\n");
    pWebDataReceived->SetData(str.GetBuffer(), str.GetLength(), 0);
    pWebDataReceived->SetBufferLength(str.GetLength());
    pWebDataReceived->ResetCurrentPos();
    EXPECT_TRUE(gl_pChinaStakeMarket->IsInvalidTengxunRTData(*pWebDataReceived));
    EXPECT_EQ(pWebDataReceived->GetCurrentPos(), 0);
    str = _T("v_pv_none_mtch=\"1\";\n");
    pWebDataReceived->SetData(str.GetBuffer(), str.GetLength(), 0);
    pWebDataReceived->SetBufferLength(str.GetLength());
    pWebDataReceived->ResetCurrentPos();
    EXPECT_FALSE(gl_pChinaStakeMarket->IsInvalidTengxunRTData(*pWebDataReceived));
    EXPECT_EQ(pWebDataReceived->GetCurrentPos(), 0);
  }

  TEST_F(CChinaMarketTest, TestTaskProcessWebRTDataGetFromSinaServer) {
    CString strRTData = _T("var hq_str_sh600000=\"浦发银行,10.120,9.940,10.090,10.180,9.960,10.090,10.100,42609358,428744766.000,453445,10.090,312600,10.080,173519,10.070,242800,10.060,136700,10.050,3600,10.100,3200,10.120,5100,10.130,129900,10.150,476200,10.160,2020-03-20,15:00:00,00,\";\nvar hq_str_sh600004=\"白云机场,13.010,12.900,13.300,13.350,12.990,13.300,13.320,9689808,127814773.000,4810,13.300,11100,13.290,5800,13.280,5200,13.270,10800,13.260,1600,13.320,61100,13.330,46100,13.340,114467,13.350,35000,13.360,2020-03-20,15:00:09,00,\";\nvar hq_str_sh600008=\"首创股份,3.050,3.040,3.050,3.080,3.030,3.050,3.060,22087562,67408587.000,760200,3.050,625400,3.040,570900,3.030,329000,3.020,202000,3.010,9200,3.060,112100,3.070,491897,3.080,449600,3.090,664140,3.100,2020-03-20,15:00:09,00,\";\n");
    CWebDataPtr pData = make_shared<CWebData>();
    pData->__TEST_SetBuffer__(strRTData);
    gl_WebInquirer.PushSinaRTData(pData);
    gl_pChinaStakeMarket->TaskProcessWebRTDataGetFromSinaServer();
    EXPECT_EQ(gl_WebRTDataContainer.GetSinaDataSize(), 3);
    EXPECT_EQ(gl_WebInquirer.GetSinaRTDataSize(), 0);
    for (int i = 0; i < 3; i++) {
      CWebRTDataPtr pRTData = gl_WebRTDataContainer.PopSinaData();
    }
  }

  TEST_F(CChinaMarketTest, TestTaskProcessWebRTDataGetFromNeteaseServer) {
    CString strRTData = _T("_ntes_quote_callback({\"0601872\":{\"code\": \"0601872\",\"percent\": 0.038251, \"high\": 5.72, \"askvol3\": 311970, \"askvol2\": 257996,\"askvol5\": 399200, \"askvol4\": 201000, \"price\": 5.7, \"open\": 5.53, \"bid5\": 5.65, \"bid4\": 5.66, \"bid3\": 5.67,\"bid2\": 5.68, \"bid1\": 5.69, \"low\": 5.51, \"updown\": 0.21, \"type\": \"SH\", \"symbol\": \"601872\", \"status\": 0,\"ask4\": 5.73, \"bidvol3\": 234700, \"bidvol2\": 166300, \"bidvol1\": 641291, \"update\": \"2019/11/04 15:59:54\",\"bidvol5\": 134500, \"bidvol4\": 96600, \"yestclose\": 5.49, \"askvol1\": 396789, \"ask5\": 5.74, \"volume\": 78750304,\"ask1\": 5.7, \"name\": \"\u62db\u5546\u8f6e\u8239\", \"ask3\": 5.72, \"ask2\": 5.71, \"arrow\": \"\u2191\",\"time\": \"2019/11/04 15:59:52\", \"turnover\": 443978974} });");
    CWebDataPtr pData = make_shared<CWebData>();
    pData->__TEST_SetBuffer__(strRTData);
    gl_WebInquirer.PushNeteaseRTData(pData);
    gl_pChinaStakeMarket->TaskProcessWebRTDataGetFromNeteaseServer();
    EXPECT_EQ(gl_WebRTDataContainer.GetNeteaseDataSize(), 1);
    EXPECT_EQ(gl_WebInquirer.GetNeteaseRTDataSize(), 0);
    for (int i = 0; i < 1; i++) {
      CWebRTDataPtr pRTData = gl_WebRTDataContainer.PopNeteaseData();
    }
  }

  TEST_F(CChinaMarketTest, TestTaskProcessWebRTDataGetFromTengxunServer) {
    CString strRTData = _T("v_sz000001=\"51~平安银行~000001~15.59~15.90~15.75~1046363~518391~527971~15.58~2365~15.57~802~15.56~1855~15.55~2316~15.54~320~15.59~661~15.60~15381~15.61~3266~15.62~450~15.63~520~~20190930154003~-0.31~-1.95~15.89~15.57~15.59/1046363/1645828527~1046363~164583~0.54~11.27~~15.89~15.57~2.01~3025.36~3025.38~1.15~17.49~14.31~0.73~-12617~15.73~9.82~12.19~~~1.24~164582.85~0.00~0~~GP-A~68.91~~0.82\";\n");
    CWebDataPtr pData = make_shared<CWebData>();
    pData->__TEST_SetBuffer__(strRTData);
    gl_WebInquirer.PushTengxunRTData(pData);
    gl_pChinaStakeMarket->TaskProcessWebRTDataGetFromTengxunServer();
    EXPECT_EQ(gl_WebRTDataContainer.GetTengxunDataSize(), 1);
    EXPECT_EQ(gl_WebInquirer.GetTengxunRTDataSize(), 0);
    for (int i = 0; i < 1; i++) {
      CWebRTDataPtr pRTData = gl_WebRTDataContainer.PopTengxunData();
    }
  }

  TEST_F(CChinaMarketTest, TestTaskDiscardNeteaseRTData) {
    CWebRTDataPtr prtData1, prtData2;
    prtData1 = make_shared<CWebRTData>();
    prtData2 = make_shared<CWebRTData>();
    EXPECT_EQ(gl_WebRTDataContainer.GetNeteaseDataSize(), 0);
    gl_WebRTDataContainer.PushNeteaseData(prtData1);
    gl_WebRTDataContainer.PushNeteaseData(prtData2);
    EXPECT_EQ(gl_WebRTDataContainer.GetNeteaseDataSize(), 2);
    gl_pChinaStakeMarket->TaskDiscardNeteaseRTData();
    EXPECT_EQ(gl_WebRTDataContainer.GetNeteaseDataSize(), 0);
  }

  TEST_F(CChinaMarketTest, TestTaskDiscardSinaRTData) {
    CWebRTDataPtr prtData1, prtData2;
    prtData1 = make_shared<CWebRTData>();
    prtData2 = make_shared<CWebRTData>();
    EXPECT_EQ(gl_WebRTDataContainer.GetSinaDataSize(), 0);
    gl_WebRTDataContainer.PushSinaData(prtData1);
    gl_WebRTDataContainer.PushSinaData(prtData2);
    EXPECT_EQ(gl_WebRTDataContainer.GetSinaDataSize(), 2);
    gl_pChinaStakeMarket->TaskDiscardSinaRTData();
    EXPECT_EQ(gl_WebRTDataContainer.GetSinaDataSize(), 0);
  }

  TEST_F(CChinaMarketTest, TestTaskDiscardTengxunRTData) {
    CWebRTDataPtr prtData1, prtData2;
    prtData1 = make_shared<CWebRTData>();
    prtData2 = make_shared<CWebRTData>();
    EXPECT_EQ(gl_WebRTDataContainer.GetTengxunDataSize(), 0);
    gl_WebRTDataContainer.PushTengxunData(prtData1);
    gl_WebRTDataContainer.PushTengxunData(prtData2);
    EXPECT_EQ(gl_WebRTDataContainer.GetTengxunDataSize(), 2);
    gl_pChinaStakeMarket->TaskDiscardTengxunRTData();
    EXPECT_EQ(gl_WebRTDataContainer.GetTengxunDataSize(), 0);
  }

  TEST_F(CChinaMarketTest, TestStoreChoicedRTData) {
    EXPECT_EQ(gl_pChinaStakeMarket->GetChoicedRTDataSize(), 0);
    CWebRTDataPtr pRTData = make_shared<CWebRTData>();
    gl_pChinaStakeMarket->StoreChoiceRTData(pRTData);
    EXPECT_EQ(gl_pChinaStakeMarket->GetChoicedRTDataSize(), 1);
    gl_pChinaStakeMarket->ClearChoicedRTDataQueue();
    EXPECT_EQ(gl_pChinaStakeMarket->GetChoicedRTDataSize(), 0);
  }

  TEST_F(CChinaMarketTest, TestClearDayLineNeedUpdaeStatus) {
    /*
    for (int i = 0; i < gl_pChinaStakeMarket->GetTotalStock(); i++) {
      if (!gl_pChinaStakeMarket->GetStock(i)->IsDayLineNeedUpdate()) gl_pChinaStakeMarket->GetStock(i)->SetDayLineNeedUpdate(true);
    }
    */
    gl_pChinaStakeMarket->ClearDayLineNeedUpdaeStatus();

    for (int i = 0; i < gl_pChinaStakeMarket->GetTotalStock(); i++) {
      EXPECT_FALSE(gl_pChinaStakeMarket->GetStock(i)->IsDayLineNeedUpdate());
    }
  }

  TEST_F(CChinaMarketTest, TestGetDayLineNeedUpdateNumber) {
    EXPECT_EQ(gl_pChinaStakeMarket->GetDayLineNeedUpdateNumber(), 12000);
    gl_pChinaStakeMarket->SetDayLineNeedUpdateNumber(6);
    EXPECT_EQ(gl_pChinaStakeMarket->GetDayLineNeedUpdateNumber(), 6);
    gl_pChinaStakeMarket->IncreaseNeteaseDayLineNeedUpdateNumber();
    EXPECT_EQ(gl_pChinaStakeMarket->GetDayLineNeedUpdateNumber(), 7);
    gl_pChinaStakeMarket->IncreaseNeteaseDayLineNeedUpdateNumber(10);
    EXPECT_EQ(gl_pChinaStakeMarket->GetDayLineNeedUpdateNumber(), 17);
    gl_pChinaStakeMarket->DecreaseNeteaseDayLineNeedUpdateNumber();
    EXPECT_EQ(gl_pChinaStakeMarket->GetDayLineNeedUpdateNumber(), 16);
    gl_pChinaStakeMarket->DecreaseNeteaseDayLineNeedUpdateNumber(10);
    EXPECT_EQ(gl_pChinaStakeMarket->GetDayLineNeedUpdateNumber(), 6);
    gl_pChinaStakeMarket->DecreaseNeteaseDayLineNeedUpdateNumber(10);
    EXPECT_EQ(gl_pChinaStakeMarket->GetDayLineNeedUpdateNumber(), 6);
    gl_pChinaStakeMarket->DecreaseNeteaseDayLineNeedUpdateNumber(6);
    EXPECT_EQ(gl_pChinaStakeMarket->GetDayLineNeedUpdateNumber(), 0);

    gl_pChinaStakeMarket->SetDayLineNeedUpdateNumber(12000);
  }

  TEST_F(CChinaMarketTest, TestGetDayLineNeedProcessNumber) {
    EXPECT_EQ(gl_pChinaStakeMarket->GetDayLineNeedProcessNumber(), 0);
    gl_pChinaStakeMarket->SetDayLineNeedProcessNumber(6);
    EXPECT_EQ(gl_pChinaStakeMarket->GetDayLineNeedProcessNumber(), 6);
    gl_pChinaStakeMarket->IncreaseNeteaseDayLineNeedProcessNumber();
    EXPECT_EQ(gl_pChinaStakeMarket->GetDayLineNeedProcessNumber(), 7);
    gl_pChinaStakeMarket->IncreaseNeteaseDayLineNeedProcessNumber(10);
    EXPECT_EQ(gl_pChinaStakeMarket->GetDayLineNeedProcessNumber(), 17);
    gl_pChinaStakeMarket->DecreaseNeteaseDayLineNeedProcessNumber();
    EXPECT_EQ(gl_pChinaStakeMarket->GetDayLineNeedProcessNumber(), 16);
    gl_pChinaStakeMarket->DecreaseNeteaseDayLineNeedProcessNumber(10);
    EXPECT_EQ(gl_pChinaStakeMarket->GetDayLineNeedProcessNumber(), 6);
    gl_pChinaStakeMarket->DecreaseNeteaseDayLineNeedProcessNumber(10);
    EXPECT_EQ(gl_pChinaStakeMarket->GetDayLineNeedProcessNumber(), 6);
    gl_pChinaStakeMarket->DecreaseNeteaseDayLineNeedProcessNumber(6);
    EXPECT_EQ(gl_pChinaStakeMarket->GetDayLineNeedProcessNumber(), 0);
  }

  TEST_F(CChinaMarketTest, TestGetDayLineNeedSaveNumber) {
    EXPECT_EQ(gl_pChinaStakeMarket->GetDayLineNeedSaveNumber(), 0);
    gl_pChinaStakeMarket->SetDayLineNeedSaveNumber(6);
    EXPECT_EQ(gl_pChinaStakeMarket->GetDayLineNeedSaveNumber(), 6);
    gl_pChinaStakeMarket->IncreaseNeteaseDayLineNeedSaveNumber();
    EXPECT_EQ(gl_pChinaStakeMarket->GetDayLineNeedSaveNumber(), 7);
    gl_pChinaStakeMarket->IncreaseNeteaseDayLineNeedSaveNumber(10);
    EXPECT_EQ(gl_pChinaStakeMarket->GetDayLineNeedSaveNumber(), 17);
    gl_pChinaStakeMarket->DecreaseNeteaseDayLineNeedSaveNumber();
    EXPECT_EQ(gl_pChinaStakeMarket->GetDayLineNeedSaveNumber(), 16);
    gl_pChinaStakeMarket->DecreaseNeteaseDayLineNeedSaveNumber(10);
    EXPECT_EQ(gl_pChinaStakeMarket->GetDayLineNeedSaveNumber(), 6);
    gl_pChinaStakeMarket->DecreaseNeteaseDayLineNeedSaveNumber(10);
    EXPECT_EQ(gl_pChinaStakeMarket->GetDayLineNeedSaveNumber(), 6);
    gl_pChinaStakeMarket->DecreaseNeteaseDayLineNeedSaveNumber(6);
    EXPECT_EQ(gl_pChinaStakeMarket->GetDayLineNeedSaveNumber(), 0);
  }

  TEST_F(CChinaMarketTest, TestTaskGetRTDataFromWeb1) {
    EXPECT_TRUE(gl_pChinaStakeMarket->IsSystemReady());
    EXPECT_TRUE(gl_pChinaStakeMarket->IsUsingSinaRTDataServer());
    gl_pChinaStakeMarket->SetSystemReady(false);
    gl_pChinaStakeMarket->SetCountDownTengxunNumber(5);
    EXPECT_TRUE(gl_pChinaStakeMarket->IsUsingTengxunRTDataReceiver());
    EXPECT_TRUE(gl_pChinaStakeMarket->IsUsingNeteaseRTDataReceiver());
    gl_pChinaStakeMarket->SetUsingNeteaseRTDataReceiver(true);
    EXPECT_CALL(*gl_pSinaRTWebInquiry, StartReadingThread).Times(1);
    EXPECT_CALL(*gl_pNeteaseRTWebInquiry, StartReadingThread).Times(0);
    EXPECT_CALL(*gl_pTengxunRTWebInquiry, StartReadingThread).Times(0);
    EXPECT_FALSE(gl_pSinaRTWebInquiry->IsReadingWebData());
    gl_pChinaStakeMarket->TaskGetRTDataFromWeb();
    EXPECT_TRUE(gl_pSinaRTWebInquiry->IsReadingWebData());

    gl_pSinaRTWebInquiry->SetReadingWebData(false);

    gl_pChinaStakeMarket->SetUsingNeteaseRTDataReceiver(true);
    gl_pSinaRTWebInquiry->SetReadingWebData(false);
    gl_pTengxunRTWebInquiry->SetReadingWebData(false);
    gl_pNeteaseRTWebInquiry->SetReadingWebData(false);
    gl_pChinaStakeMarket->SetSystemReady(true);
  }

  TEST_F(CChinaMarketTest, TestTaskGetRTDataFromWeb2) {
    EXPECT_TRUE(gl_pChinaStakeMarket->IsSystemReady());
    EXPECT_TRUE(gl_pChinaStakeMarket->IsUsingSinaRTDataServer());
    gl_pChinaStakeMarket->SetUsingNeteaseRTDataServer();
    gl_pChinaStakeMarket->SetCountDownTengxunNumber(5);
    EXPECT_TRUE(gl_pChinaStakeMarket->IsUsingTengxunRTDataReceiver());
    EXPECT_TRUE(gl_pChinaStakeMarket->IsUsingNeteaseRTDataReceiver());
    gl_pChinaStakeMarket->SetUsingNeteaseRTDataReceiver(true);
    EXPECT_CALL(*gl_pSinaRTWebInquiry, StartReadingThread).Times(0);
    EXPECT_CALL(*gl_pNeteaseRTWebInquiry, StartReadingThread).Times(1);
    EXPECT_CALL(*gl_pTengxunRTWebInquiry, StartReadingThread).Times(0); // _T("腾讯实时数据采集需要计数器倒数至零");
    EXPECT_FALSE(gl_pNeteaseRTWebInquiry->IsReadingWebData());
    gl_pChinaStakeMarket->TaskGetRTDataFromWeb();
    EXPECT_TRUE(gl_pNeteaseRTWebInquiry->IsReadingWebData());
    EXPECT_EQ(gl_pChinaStakeMarket->GetCountDownTengxunNumber(), 4) << _T("调用TaskGetRTDataFromWeb后计数器已减一");
    gl_pNeteaseRTWebInquiry->SetReadingWebData(false);
    gl_pChinaStakeMarket->SetUsingSinaRTDataServer();

    gl_pChinaStakeMarket->SetCountDownTengxunNumber(5);
    gl_pChinaStakeMarket->SetUsingNeteaseRTDataReceiver(true);
    gl_pSinaRTWebInquiry->SetReadingWebData(false);
    gl_pTengxunRTWebInquiry->SetReadingWebData(false);
    gl_pNeteaseRTWebInquiry->SetReadingWebData(false);
    gl_pChinaStakeMarket->SetSystemReady(true);
  }

  TEST_F(CChinaMarketTest, TestTaskGetRTDataFromWeb3) {
    EXPECT_TRUE(gl_pChinaStakeMarket->IsSystemReady());
    gl_pChinaStakeMarket->SetSystemReady(true);
    gl_pChinaStakeMarket->SetCountDownTengxunNumber(0);
    EXPECT_CALL(*gl_pSinaRTWebInquiry, StartReadingThread).Times(1);
    EXPECT_CALL(*gl_pTengxunRTWebInquiry, StartReadingThread).Times(1); // _T("腾讯实时数据采集计数器已倒数至零");
    EXPECT_FALSE(gl_pSinaRTWebInquiry->IsReadingWebData());
    EXPECT_FALSE(gl_pTengxunRTWebInquiry->IsReadingWebData());
    gl_pChinaStakeMarket->TaskGetRTDataFromWeb();
    EXPECT_TRUE(gl_pSinaRTWebInquiry->IsReadingWebData());
    EXPECT_TRUE(gl_pTengxunRTWebInquiry->IsReadingWebData());

    gl_pChinaStakeMarket->SetUsingNeteaseRTDataReceiver(true);
    gl_pSinaRTWebInquiry->SetReadingWebData(false);
    gl_pTengxunRTWebInquiry->SetReadingWebData(false);
    gl_pNeteaseRTWebInquiry->SetReadingWebData(false);
  }

  TEST_F(CChinaMarketTest, TestSchedulingTaskPerHour) {
    EXPECT_TRUE(gl_pChinaStakeMarket->SchedulingTaskPerHour(3600, 10100));
    EXPECT_FALSE(gl_pChinaStakeMarket->SchedulingTaskPerHour(3599, 19000)) << _T("前面那个将计数器重置，此调用尚差一秒，故而返回假");
    EXPECT_TRUE(gl_pChinaStakeMarket->SchedulingTaskPerHour(1, 10100));
  }

  TEST_F(CChinaMarketTest, TestGetRTDataReceived) {
    gl_pChinaStakeMarket->SetRTDataReceived(101010);
    EXPECT_EQ(gl_pChinaStakeMarket->GetRTDataReceived(), 101010);
    gl_pChinaStakeMarket->SetRTDataReceived(1010101010);
    EXPECT_EQ(gl_pChinaStakeMarket->GetRTDataReceived(), 1010101010);
  }

  TEST_F(CChinaMarketTest, TestCheckMarketReady) {
    gl_pChinaStakeMarket->SetSystemReady(true);
    gl_pChinaStakeMarket->SetRTDataReceived(0);
    EXPECT_TRUE(gl_pChinaStakeMarket->CheckMarketReady());
    EXPECT_TRUE(gl_pChinaStakeMarket->IsSystemReady());
    gl_pChinaStakeMarket->SetSystemReady(false);
    gl_pChinaStakeMarket->SetRTDataReceived(0);
    gl_pChinaStakeMarket->SetSystemReady(false);
    EXPECT_FALSE(gl_pChinaStakeMarket->CheckMarketReady());
    EXPECT_FALSE(gl_pChinaStakeMarket->IsSystemReady());
    gl_pChinaStakeMarket->SetSystemReady(false);
    gl_pChinaStakeMarket->SetRTDataReceived(0);
    gl_pChinaStakeMarket->SetSystemReady(false);
    EXPECT_FALSE(gl_pChinaStakeMarket->CheckMarketReady());
    EXPECT_FALSE(gl_pChinaStakeMarket->IsSystemReady());
    gl_pChinaStakeMarket->SetRTDataReceived(gl_pChinaStakeMarket->GetTotalStock() * 2);
    EXPECT_FALSE(gl_pChinaStakeMarket->CheckMarketReady());
    EXPECT_FALSE(gl_pChinaStakeMarket->IsSystemReady());
    gl_pChinaStakeMarket->SetRTDataReceived(gl_pChinaStakeMarket->GetTotalStock() * 2 + 1);
    EXPECT_TRUE(gl_pChinaStakeMarket->CheckMarketReady());
    EXPECT_TRUE(gl_pChinaStakeMarket->IsSystemReady());
    gl_pChinaStakeMarket->SetSystemReady(false);
  }

  TEST_F(CChinaMarketTest, TestChangeCurrentStockToNextStock1) {
    EXPECT_EQ(gl_pChinaStakeMarket->GetCurrentSelectedStockSet(), -1);
    gl_pChinaStakeMarket->SetCurrentStock(gl_pChinaStakeMarket->GetStock(0));
    gl_pChinaStakeMarket->ChangeToNextStock();
    EXPECT_EQ(gl_pChinaStakeMarket->GetCurrentStock()->GetOffset(), 1);
    gl_pChinaStakeMarket->SetCurrentStock(gl_pChinaStakeMarket->GetStock(11998)); // 这个股票是中证煤炭
    gl_pChinaStakeMarket->ChangeToNextStock();
    EXPECT_EQ(gl_pChinaStakeMarket->GetCurrentStock()->GetOffset(), 0) << _T("中证煤炭后的为空，然后就转到最前面的浦发银行了");
    gl_pChinaStakeMarket->SetCurrentStockChanged(false);

    gl_pChinaStakeMarket->SetCurrentSelectedPosition(0);
    gl_pChinaStakeMarket->SetCurrentSelectedStockSet(-1);
  }

  TEST_F(CChinaMarketTest, TestChangeCurrentStockToNextStock2) {
    EXPECT_EQ(gl_pChinaStakeMarket->GetCurrentSelectedStockSet(), -1);
    gl_pChinaStakeMarket->SetCurrentSelectedStockSet(10); // 10、11、12股票集中有股票
    EXPECT_TRUE(gl_pChinaStakeMarket->GetCurrentSelectedStock() != nullptr);
    gl_pChinaStakeMarket->SetCurrentStock(gl_pChinaStakeMarket->GetCurrentSelectedStock());

    gl_pChinaStakeMarket->ChangeToPrevStock();
    gl_pChinaStakeMarket->ChangeToNextStock();
    EXPECT_EQ(gl_pChinaStakeMarket->GetCurrentSelectedPosition(), 0);
    gl_pChinaStakeMarket->ChangeToNextStock();
    EXPECT_EQ(gl_pChinaStakeMarket->GetCurrentSelectedPosition(), 1);
    gl_pChinaStakeMarket->ChangeToPrevStock();
    gl_pChinaStakeMarket->ChangeToPrevStock();
    EXPECT_EQ(gl_pChinaStakeMarket->GetCurrentStockSetSize(), gl_pChinaStakeMarket->GetCurrentSelectedPosition() + 1);
    gl_pChinaStakeMarket->SetCurrentStockChanged(false);

    gl_pChinaStakeMarket->SetCurrentSelectedPosition(0);
    gl_pChinaStakeMarket->SetCurrentSelectedStockSet(-1);
  }

  TEST_F(CChinaMarketTest, TestChangeCurrentStockToPrevStock1) {
    EXPECT_EQ(gl_pChinaStakeMarket->GetCurrentSelectedStockSet(), -1);
    gl_pChinaStakeMarket->SetCurrentStock(gl_pChinaStakeMarket->GetStock(1)); // 选取退市的邯郸钢铁
    gl_pChinaStakeMarket->ChangeToPrevStock();
    EXPECT_EQ(gl_pChinaStakeMarket->GetCurrentStock()->GetOffset(), 0) << _T("上一个是浦发银行");
    gl_pChinaStakeMarket->ChangeToPrevStock();
    EXPECT_EQ(gl_pChinaStakeMarket->GetCurrentStock()->GetOffset(), 11998) << _T("浦发银行前的为空，然后就转到最后面的中证煤炭了");
    gl_pChinaStakeMarket->SetCurrentStockChanged(false);
    gl_pChinaStakeMarket->SetCurrentSelectedPosition(0);
  }

  TEST_F(CChinaMarketTest, TestChangeCurrentStockToPrevStock2) {
    EXPECT_EQ(gl_pChinaStakeMarket->GetCurrentSelectedStockSet(), -1);
    gl_pChinaStakeMarket->SetCurrentSelectedStockSet(10); // 10、11、12股票集中有股票
    EXPECT_TRUE(gl_pChinaStakeMarket->GetCurrentSelectedStock() != nullptr);
    gl_pChinaStakeMarket->SetCurrentStock(gl_pChinaStakeMarket->GetCurrentSelectedStock());

    gl_pChinaStakeMarket->ChangeToNextStock();
    EXPECT_EQ(gl_pChinaStakeMarket->GetCurrentSelectedPosition(), 1);
    gl_pChinaStakeMarket->ChangeToPrevStock();
    EXPECT_EQ(gl_pChinaStakeMarket->GetCurrentSelectedPosition(), 0);
    gl_pChinaStakeMarket->ChangeToPrevStock();
    EXPECT_EQ(gl_pChinaStakeMarket->GetCurrentSelectedPosition() + 1, gl_pChinaStakeMarket->GetCurrentStockSetSize());
    gl_pChinaStakeMarket->SetCurrentStockChanged(false);

    gl_pChinaStakeMarket->SetCurrentSelectedPosition(0);
    gl_pChinaStakeMarket->SetCurrentSelectedStockSet(-1);
  }

  TEST_F(CChinaMarketTest, TestChangeToPrevStockSet) {
    EXPECT_EQ(gl_pChinaStakeMarket->GetCurrentSelectedStockSet(), -1);
    gl_pChinaStakeMarket->ChangeToPrevStockSet();
    EXPECT_EQ(gl_pChinaStakeMarket->GetCurrentSelectedStockSet(), 19);
    gl_pChinaStakeMarket->ChangeToPrevStockSet();
    EXPECT_EQ(gl_pChinaStakeMarket->GetCurrentSelectedStockSet(), 18);
    gl_pChinaStakeMarket->ChangeToPrevStockSet();
    EXPECT_EQ(gl_pChinaStakeMarket->GetCurrentSelectedStockSet(), 12);
    gl_pChinaStakeMarket->ChangeToPrevStockSet();
    EXPECT_EQ(gl_pChinaStakeMarket->GetCurrentSelectedStockSet(), 11);
    gl_pChinaStakeMarket->ChangeToPrevStockSet();
    EXPECT_EQ(gl_pChinaStakeMarket->GetCurrentSelectedStockSet(), 10);

    gl_pChinaStakeMarket->SetCurrentSelectedStockSet(-1);
  }

  TEST_F(CChinaMarketTest, TestChangeToNextStockSet) {
    EXPECT_EQ(gl_pChinaStakeMarket->GetCurrentSelectedStockSet(), -1);
    gl_pChinaStakeMarket->ChangeToNextStockSet();
    EXPECT_EQ(gl_pChinaStakeMarket->GetCurrentSelectedStockSet(), 10);
    gl_pChinaStakeMarket->ChangeToNextStockSet();
    EXPECT_EQ(gl_pChinaStakeMarket->GetCurrentSelectedStockSet(), 11);
    gl_pChinaStakeMarket->ChangeToNextStockSet();
    EXPECT_EQ(gl_pChinaStakeMarket->GetCurrentSelectedStockSet(), 12);
    gl_pChinaStakeMarket->ChangeToNextStockSet();
    EXPECT_EQ(gl_pChinaStakeMarket->GetCurrentSelectedStockSet(), 18);
    gl_pChinaStakeMarket->ChangeToNextStockSet();
    EXPECT_EQ(gl_pChinaStakeMarket->GetCurrentSelectedStockSet(), 19);

    gl_pChinaStakeMarket->SetCurrentSelectedStockSet(-1);
  }

  TEST_F(CChinaMarketTest, TestIsTotalStockSetSelected) {
    EXPECT_EQ(gl_pChinaStakeMarket->GetCurrentSelectedStockSet(), -1);
    EXPECT_TRUE(gl_pChinaStakeMarket->IsTotalStockSetSelected());
    gl_pChinaStakeMarket->SetCurrentSelectedStockSet(0);
    EXPECT_FALSE(gl_pChinaStakeMarket->IsTotalStockSetSelected());

    gl_pChinaStakeMarket->SetCurrentSelectedStockSet(-1);
  }

  TEST_F(CChinaMarketTest, TestSetUsingSinaRTDataServer) {
    EXPECT_TRUE(gl_pChinaStakeMarket->IsUsingSinaRTDataServer());
    EXPECT_FALSE(gl_pChinaStakeMarket->IsUsingNeteaseRTDataServer());
    gl_pChinaStakeMarket->SetUsingNeteaseRTDataServer();
    EXPECT_FALSE(gl_pChinaStakeMarket->IsUsingSinaRTDataServer());
    EXPECT_TRUE(gl_pChinaStakeMarket->IsUsingNeteaseRTDataServer());
    gl_pChinaStakeMarket->SetUsingSinaRTDataServer();
    EXPECT_TRUE(gl_pChinaStakeMarket->IsUsingSinaRTDataServer());
    EXPECT_FALSE(gl_pChinaStakeMarket->IsUsingNeteaseRTDataServer());
  }

  TEST_F(CChinaMarketTest, TestIsUsingNeteaseRTDataReceiver) {
    EXPECT_TRUE(gl_pChinaStakeMarket->IsUsingNeteaseRTDataReceiver());
    gl_pChinaStakeMarket->SetUsingNeteaseRTDataReceiver(false);
    EXPECT_FALSE(gl_pChinaStakeMarket->IsUsingNeteaseRTDataReceiver());
    gl_pChinaStakeMarket->SetUsingNeteaseRTDataReceiver(true);
    EXPECT_TRUE(gl_pChinaStakeMarket->IsUsingNeteaseRTDataReceiver());
  }

  TEST_F(CChinaMarketTest, TestIsUsingTengxunRTDataReceiver) {
    EXPECT_TRUE(gl_pChinaStakeMarket->IsUsingTengxunRTDataReceiver());
    gl_pChinaStakeMarket->SetUsingTengxunRTDataReceiver(false);
    EXPECT_FALSE(gl_pChinaStakeMarket->IsUsingTengxunRTDataReceiver());
    gl_pChinaStakeMarket->SetUsingTengxunRTDataReceiver(true);
    EXPECT_TRUE(gl_pChinaStakeMarket->IsUsingTengxunRTDataReceiver());
  }

  TEST_F(CChinaMarketTest, TestGetCountDownTengxunNumber) {
    EXPECT_EQ(gl_pChinaStakeMarket->GetCountDownTengxunNumber(), 5);
    gl_pChinaStakeMarket->SetCountDownTengxunNumber(100);
    EXPECT_EQ(gl_pChinaStakeMarket->GetCountDownTengxunNumber(), 100);
    gl_pChinaStakeMarket->SetCountDownTengxunNumber(5);
  }

  TEST_F(CChinaMarketTest, TestGetUpDownRate) {
    EXPECT_DOUBLE_EQ(gl_pChinaStakeMarket->GetUpDownRate(_T("10.0"), _T("0.0009")), 0.0) << _T("LastClose小于0.001时返回0");
    EXPECT_DOUBLE_EQ(gl_pChinaStakeMarket->GetUpDownRate(_T("11.0"), _T("10.0")), 0.1);
    EXPECT_DOUBLE_EQ(gl_pChinaStakeMarket->GetUpDownRate(_T("10.5"), _T("10.0")), 0.05);
    EXPECT_DOUBLE_EQ(gl_pChinaStakeMarket->GetUpDownRate(_T("9.0"), _T("10.0")), -0.1);
    EXPECT_DOUBLE_EQ(gl_pChinaStakeMarket->GetUpDownRate(_T("11.11"), _T("10.0")), 0.0) << _T("大于0.11时返回0");
    EXPECT_DOUBLE_EQ(gl_pChinaStakeMarket->GetUpDownRate(_T("8.89"), _T("10.0")), 0.0) << _T("小于-0.11时返回0");
  }

  TEST_F(CChinaMarketTest, TestLoadOptionDB) {
    CSetOption setOption;

    setOption.Open();
    setOption.m_pDatabase->BeginTrans();
    while (!setOption.IsEOF()) {
      setOption.Delete();
      setOption.MoveNext();
    }
    setOption.m_pDatabase->CommitTrans();
    setOption.Close();

    gl_pChinaStakeMarket->SetRSStartDate(20200101);
    gl_pChinaStakeMarket->SetRSEndDate(20200202);
    gl_pChinaStakeMarket->SetLastLoginDate(gl_pChinaStakeMarket->GetFormatedMarketDate());
    gl_pChinaStakeMarket->SetUpdatedDateFor10DaysRS1(19990101);
    gl_pChinaStakeMarket->SetUpdatedDateFor10DaysRS2(19990202);

    gl_pChinaStakeMarket->UpdateOptionDB();

    gl_pChinaStakeMarket->SetRSStartDate(1);
    gl_pChinaStakeMarket->SetRSEndDate(1);
    gl_pChinaStakeMarket->SetLastLoginDate(1);
    gl_pChinaStakeMarket->SetUpdatedDateFor10DaysRS1(1);
    gl_pChinaStakeMarket->SetUpdatedDateFor10DaysRS2(1);

    gl_pChinaStakeMarket->LoadOptionDB();

    EXPECT_EQ(gl_pChinaStakeMarket->GetRSStartDate(), 20200101);
    EXPECT_EQ(gl_pChinaStakeMarket->GetRSEndDate(), 20200202);
    EXPECT_EQ(gl_pChinaStakeMarket->GetLastLoginDate(), gl_pChinaStakeMarket->GetFormatedMarketDate());
    EXPECT_EQ(gl_pChinaStakeMarket->GetUpdatedDateFor10DaysRS1(), 19990101);
    EXPECT_FALSE(gl_pChinaStakeMarket->IsChoiced10RSStrong1StockSet());
    EXPECT_EQ(gl_pChinaStakeMarket->GetUpdatedDateFor10DaysRS2(), 19990202);
    EXPECT_FALSE(gl_pChinaStakeMarket->IsChoiced10RSStrong2StockSet());

    gl_pChinaStakeMarket->SetRSStartDate(20100101);
    gl_pChinaStakeMarket->SetRSEndDate(20100202);
    gl_pChinaStakeMarket->SetLastLoginDate(20200303);
    gl_pChinaStakeMarket->SetUpdatedDateFor10DaysRS1(19980101);
    gl_pChinaStakeMarket->SetUpdatedDateFor10DaysRS2(19980202);

    gl_pChinaStakeMarket->UpdateOptionDB();

    gl_pChinaStakeMarket->SetRSStartDate(1);
    gl_pChinaStakeMarket->SetRSEndDate(1);
    gl_pChinaStakeMarket->SetLastLoginDate(1);
    gl_pChinaStakeMarket->SetUpdatedDateFor10DaysRS1(1);
    gl_pChinaStakeMarket->SetUpdatedDateFor10DaysRS2(1);

    gl_pChinaStakeMarket->LoadOptionDB();

    EXPECT_EQ(gl_pChinaStakeMarket->GetRSStartDate(), 20100101);
    EXPECT_EQ(gl_pChinaStakeMarket->GetRSEndDate(), 20100202);
    EXPECT_EQ(gl_pChinaStakeMarket->GetLastLoginDate(), gl_pChinaStakeMarket->GetFormatedMarketDate()) << _T("永远是当前日期\n");
    EXPECT_EQ(gl_pChinaStakeMarket->GetUpdatedDateFor10DaysRS1(), 19980101);
    EXPECT_FALSE(gl_pChinaStakeMarket->IsChoiced10RSStrong1StockSet());
    EXPECT_EQ(gl_pChinaStakeMarket->GetUpdatedDateFor10DaysRS2(), 19980202);
    EXPECT_FALSE(gl_pChinaStakeMarket->IsChoiced10RSStrong2StockSet());

    setOption.Open();
    setOption.m_pDatabase->BeginTrans();
    while (!setOption.IsEOF()) {
      setOption.Delete();
      setOption.MoveNext();
    }
    setOption.m_pDatabase->CommitTrans();
    setOption.Close();
    gl_pChinaStakeMarket->LoadOptionDB();

    EXPECT_EQ(gl_pChinaStakeMarket->GetRSStartDate(), __CHINA_MARKET_BEGIN_DATE__);
    EXPECT_EQ(gl_pChinaStakeMarket->GetRSEndDate(), __CHINA_MARKET_BEGIN_DATE__);
    EXPECT_EQ(gl_pChinaStakeMarket->GetLastLoginDate(), __CHINA_MARKET_BEGIN_DATE__);
    EXPECT_EQ(gl_pChinaStakeMarket->GetUpdatedDateFor10DaysRS1(), __CHINA_MARKET_BEGIN_DATE__);
    EXPECT_FALSE(gl_pChinaStakeMarket->IsChoiced10RSStrong1StockSet());
    EXPECT_EQ(gl_pChinaStakeMarket->GetUpdatedDateFor10DaysRS2(), __CHINA_MARKET_BEGIN_DATE__);
    EXPECT_FALSE(gl_pChinaStakeMarket->IsChoiced10RSStrong2StockSet());
  }

  TEST_F(CChinaMarketTest, TestTooManyStockNeedUpdated) {
    EXPECT_FALSE(gl_pChinaStakeMarket->TooManyStocksNeedUpdated());
    gl_pChinaStakeMarket->SetStockNeedUpdated(1000);
    EXPECT_FALSE(gl_pChinaStakeMarket->TooManyStocksNeedUpdated());
    gl_pChinaStakeMarket->SetStockNeedUpdated(1001);
    EXPECT_TRUE(gl_pChinaStakeMarket->TooManyStocksNeedUpdated());
    gl_pChinaStakeMarket->SetStockNeedUpdated(0);
    EXPECT_FALSE(gl_pChinaStakeMarket->TooManyStocksNeedUpdated());
  }

  TEST_F(CChinaMarketTest, TestDeleteDayLineBasicInfo) {
    char buffer[20];
    CString strDate;

    CSetDayLineBasicInfo setDayLine, setDayLine2;
    CDayLinePtr pDayLine = make_shared<CDayLine>();

    pDayLine->SetStakeCode(_T("sh600000"));
    pDayLine->SetDate(19900101);

    _ltoa_s(19900101, buffer, 10);
    strDate = buffer;
    setDayLine.m_strFilter = _T("[Date] =");
    setDayLine.m_strFilter += strDate;
    setDayLine.Open();
    setDayLine.m_pDatabase->BeginTrans();
    pDayLine->AppendData(&setDayLine);
    setDayLine.m_pDatabase->CommitTrans();
    setDayLine.Close();

    setDayLine.m_strFilter = _T("[Date] =");
    setDayLine.m_strFilter += strDate;
    setDayLine.Open();
    EXPECT_FALSE(setDayLine.IsEOF());
    setDayLine.Close();

    gl_pChinaStakeMarket->DeleteDayLineBasicInfo(19900101);

    setDayLine2.m_strFilter = _T("[Date] =");
    setDayLine2.m_strFilter += strDate;
    setDayLine2.Open();
    EXPECT_TRUE(setDayLine2.IsEOF());
    setDayLine2.Close();
  }

  TEST_F(CChinaMarketTest, TestDeleteDayLineExtendInfo) {
    char buffer[20];
    CString strDate;

    CSetDayLineExtendInfo setDayLine, setDayLine2;
    CDayLinePtr pDayLine = make_shared<CDayLine>();

    pDayLine->SetStakeCode(_T("sh600000"));
    pDayLine->SetDate(19900101);

    _ltoa_s(19900101, buffer, 10);
    strDate = buffer;
    setDayLine.m_strFilter = _T("[ID] = 1");
    setDayLine.Open();
    setDayLine.m_pDatabase->BeginTrans();
    setDayLine.AddNew();
    setDayLine.m_StockCode = _T("sh600000");
    setDayLine.m_Date = 19900101;
    setDayLine.Update();
    setDayLine.m_pDatabase->CommitTrans();
    setDayLine.Close();

    setDayLine.m_strFilter = _T("[Date] =");
    setDayLine.m_strFilter += strDate;
    setDayLine.Open();
    EXPECT_FALSE(setDayLine.IsEOF());
    setDayLine.Close();

    gl_pChinaStakeMarket->DeleteDayLineExtendInfo(19900101);

    setDayLine2.m_strFilter = _T("[Date] =");
    setDayLine2.m_strFilter += strDate;
    setDayLine2.Open();
    EXPECT_TRUE(setDayLine2.IsEOF());
    setDayLine2.Close();
  }

  TEST_F(CChinaMarketTest, TestDeleteCurrentWeekLine) {
    CSetWeekLineInfo setCurrentWeekLine, setCurrentWeekLine2;
    CWeekLinePtr pWeekLine = make_shared<CWeekLine>();

    pWeekLine->SetStakeCode(_T("sh600000"));
    pWeekLine->SetDate(GetCurrentMonday(20200101));
    setCurrentWeekLine.m_strFilter = _T("[ID] = 1");
    setCurrentWeekLine.Open();
    setCurrentWeekLine.m_pDatabase->BeginTrans();
    pWeekLine->AppendData(&setCurrentWeekLine);
    setCurrentWeekLine.m_pDatabase->CommitTrans();
    setCurrentWeekLine.Close();

    setCurrentWeekLine.m_strFilter = _T("");
    setCurrentWeekLine.Open();
    EXPECT_FALSE(setCurrentWeekLine.IsEOF());
    setCurrentWeekLine.Close();

    gl_pChinaStakeMarket->DeleteCurrentWeekWeekLine();

    setCurrentWeekLine2.Open();
    EXPECT_TRUE(setCurrentWeekLine2.IsEOF());
    setCurrentWeekLine2.Close();
  }

  TEST_F(CChinaMarketTest, TestSaveLoadCurrentWeekLine) {
    CSetWeekLineInfo setCurrentWeekLine, setCurrentWeekLine2;
    CWeekLinePtr pWeekLine = make_shared<CWeekLine>();
    CWeekLineContainer weekLineContainer, weekLineContainer2;

    pWeekLine->SetStakeCode(_T("sh600000"));
    pWeekLine->SetDate(GetCurrentMonday(20200101));
    weekLineContainer.StoreData(pWeekLine);

    gl_pChinaStakeMarket->DeleteCurrentWeekWeekLine();
    gl_pChinaStakeMarket->SaveCurrentWeekLine(weekLineContainer);

    gl_pChinaStakeMarket->LoadCurrentWeekLine(weekLineContainer2);
    pWeekLine = weekLineContainer2.GetData(0);
    EXPECT_STREQ(pWeekLine->GetStakeCode(), _T("sh600000"));
    EXPECT_EQ(pWeekLine->GetFormatedMarketDate(), 20191230) << "20200101之前的星期一";

    gl_pChinaStakeMarket->DeleteCurrentWeekWeekLine();
  }

  TEST_F(CChinaMarketTest, TestDeleteTodayTempDB) {
    CSetDayLineToday setDayLineToday, setDayLineToday2;
    CString strName;

    setDayLineToday.m_strFilter = _T("[ID] = 1");
    setDayLineToday.Open();
    setDayLineToday.m_pDatabase->BeginTrans();
    setDayLineToday.AddNew();
    setDayLineToday.m_StockCode = _T("sh600000");
    setDayLineToday.m_Date = 20201212;
    setDayLineToday.Update();
    setDayLineToday.m_pDatabase->CommitTrans();
    setDayLineToday.Close();

    setDayLineToday.m_strFilter = _T("");
    setDayLineToday.Open();
    EXPECT_FALSE(setDayLineToday.IsEOF());
    setDayLineToday.Close();

    gl_pChinaStakeMarket->DeleteTodayTempDB();

    setDayLineToday2.Open();
    EXPECT_TRUE(setDayLineToday2.IsEOF());
    setDayLineToday2.Close();
  }

  TEST_F(CChinaMarketTest, TestLoadDayLine) {
    CDayLineContainer dayLineContainer;
    long lDate = GetCurrentMonday(20200101);

    gl_pChinaStakeMarket->LoadDayLine(dayLineContainer, lDate);

    CString strSQL;
    CString strDate;
    char  pch[30];
    CTime ctTime;
    CSetDayLineBasicInfo setDayLineBasicInfo;
    CSetDayLineExtendInfo setDayLineExtendInfo;
    CDayLinePtr pDayLine;
    long i = 0;

    sprintf_s(pch, _T("%08d"), lDate);
    strDate = pch;
    setDayLineBasicInfo.m_strSort = _T("[StockCode]");
    setDayLineBasicInfo.m_strFilter = _T("[Date] =");
    setDayLineBasicInfo.m_strFilter += strDate;
    setDayLineBasicInfo.Open();
    while (!setDayLineBasicInfo.IsEOF()) {
      pDayLine = dayLineContainer.GetData(i++);
      EXPECT_STREQ(setDayLineBasicInfo.m_StockCode, pDayLine->GetStakeCode());
      setDayLineBasicInfo.MoveNext();
    }
    EXPECT_EQ(i, dayLineContainer.GetDataSize());
    setDayLineBasicInfo.Close();
  }

  TEST_F(CChinaMarketTest, TestCreateStockCodeSet) {
    vector<CChinaStockHistoryDataPtr> vData;
    CChinaStockHistoryDataPtr pData = make_shared<CChinaStockHistoryData>();
    pData->SetStakeCode(_T("sh600000"));
    vData.resize(2);
    vData[0] = pData;
    pData = make_shared<CChinaStockHistoryData>();
    pData->SetStakeCode(_T("sh600004"));
    vData[1] = pData;

    set<CString> setStockCode;
    gl_pChinaStakeMarket->CreateStockCodeSet(setStockCode, &vData);

    EXPECT_TRUE(setStockCode.find(_T("sh600000")) != setStockCode.end());
    EXPECT_TRUE(setStockCode.find(_T("sh600004")) != setStockCode.end());
    EXPECT_FALSE(setStockCode.find(_T("sh600001")) != setStockCode.end());
  }

  TEST_F(CChinaMarketTest, TestCreateStakeCode) {
    EXPECT_STREQ(gl_pChinaStakeMarket->CreateStakeCode(true, 0), _T("sh000000"));
    EXPECT_STREQ(gl_pChinaStakeMarket->CreateStakeCode(true, 999999), _T("sh999999"));
    EXPECT_STREQ(gl_pChinaStakeMarket->CreateStakeCode(false, 0), _T("sz000000"));
    EXPECT_STREQ(gl_pChinaStakeMarket->CreateStakeCode(false, 999999), _T("sz999999"));
  }

  TEST_F(CChinaMarketTest, TestGetNextInquiringStakeMiddleStr) {
    long l = 0;
    EXPECT_STREQ(gl_pChinaStakeMarket->GetNextStakeInquiringMiddleStr(l, _T(","), 2), _T("sh000000,sh000001"));
    l = 1000000;
    EXPECT_STREQ(gl_pChinaStakeMarket->GetNextStakeInquiringMiddleStr(l, _T(","), 2), _T("sz000000,sz000001"));
  }
}