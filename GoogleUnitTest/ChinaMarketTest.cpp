#include"pch.h"

#include"globedef.h"

#include"ChinaMarket.h"
#include"ChinaStock.h"

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
      gl_pChinaStockMarket->Load10DaysRSStrongStockDB(); // 装入各十日强度股票集
      EXPECT_TRUE(gl_pChinaStockMarket->GetChoicedStockSize(10) > 0);
      EXPECT_TRUE(gl_pChinaStockMarket->GetChoicedStockSize(11) > 0);
      EXPECT_TRUE(gl_pChinaStockMarket->GetChoicedStockSize(12) > 0);
      EXPECT_TRUE(gl_pChinaStockMarket->GetChoicedStockSize(13) == 0);
      EXPECT_TRUE(gl_pChinaStockMarket->GetChoicedStockSize(14) == 0);
      EXPECT_TRUE(gl_pChinaStockMarket->GetChoicedStockSize(15) == 0);
      EXPECT_TRUE(gl_pChinaStockMarket->GetChoicedStockSize(16) == 0);
      EXPECT_TRUE(gl_pChinaStockMarket->GetChoicedStockSize(17) == 0);
      EXPECT_TRUE(gl_pChinaStockMarket->GetChoicedStockSize(18) > 0);
      EXPECT_TRUE(gl_pChinaStockMarket->GetChoicedStockSize(19) > 0);

      for (int i = 0; i < gl_pChinaStockMarket->GetTotalStock(); i++) {
        CChinaStockPtr pStock = gl_pChinaStockMarket->GetStock(i);
        //EXPECT_TRUE(pStock->IsDayLineNeedUpdate()) << pStock->GetSymbol();
      }
      EXPECT_EQ(gl_pChinaStockMarket->GetDayLineNeedProcessNumber(), 0);
      ASSERT_FALSE(gl_fNormalMode);
      EXPECT_EQ(gl_pChinaStockMarket->GetCurrentStock(), nullptr) << gl_pChinaStockMarket->GetCurrentStock()->GetSymbol();
      EXPECT_EQ(gl_pChinaStockMarket->GetDayLineNeedUpdateNumber(), gl_pChinaStockMarket->GetTotalStock());
      EXPECT_FALSE(gl_pChinaStockMarket->IsCurrentStockChanged());
    }
    static void TearDownTestSuite(void) {
      EXPECT_EQ(gl_pChinaStockMarket->GetDayLineNeedProcessNumber(), 0);
      EXPECT_EQ(gl_pChinaStockMarket->GetCurrentStock(), nullptr) << gl_pChinaStockMarket->GetCurrentStock()->GetSymbol();
      EXPECT_FALSE(gl_pChinaStockMarket->IsCurrentStockChanged());
      gl_pChinaStockMarket->SetCurrentStockChanged(false);
      for (int i = 0; i < gl_pChinaStockMarket->GetTotalStock(); i++) {
        CChinaStockPtr pStock = gl_pChinaStockMarket->GetStock(i);
        EXPECT_TRUE(pStock->IsDayLineNeedUpdate());
      }
      EXPECT_EQ(gl_pChinaStockMarket->GetDayLineNeedUpdateNumber(), gl_pChinaStockMarket->GetTotalStock());
    }
    virtual void SetUp(void) override {
      ASSERT_FALSE(gl_fNormalMode);
      EXPECT_LE(gl_pChinaStockMarket->GetDayLineNeedUpdateNumber(), gl_pChinaStockMarket->GetTotalStock());
      EXPECT_EQ(gl_pChinaStockMarket->GetDayLineNeedProcessNumber(), 0);
      gl_pChinaStockMarket->SetCurrentStockChanged(false);
      gl_pChinaStockMarket->CalculateTime();
      gl_pChinaStockMarket->ResetNeteaseRTDataInquiringIndex();
      gl_pChinaStockMarket->ResetNeteaseDayLineDataInquiringIndex();
      gl_pChinaStockMarket->ResetSinaStockRTDataInquiringIndex();
      gl_pChinaStockMarket->ResetTengxunRTDataInquiringIndex();
      gl_pChinaStockMarket->SetSystemReady(true); // 测试市场时，默认系统已经准备好
      gl_pChinaStockMarket->SetPermitResetMarket(true);
      EXPECT_TRUE(gl_pChinaStockMarket->IsResetMarket());
      EXPECT_FALSE(gl_pChinaStockMarket->IsMarketOpened());

      while (gl_systemMessage.GetInformationDequeSize() > 0) gl_systemMessage.PopInformationMessage();
      while (gl_systemMessage.GetDayLineInfoDequeSize() > 0) gl_systemMessage.PopDayLineInfoMessage();
      while (gl_systemMessage.GetInnerSystemInformationDequeSize() > 0) gl_systemMessage.PopInnerSystemInformationMessage();
    }

    virtual void TearDown(void) override {
      // clearup
      EXPECT_FALSE(gl_pChinaStockMarket->IsMarketOpened());
      gl_ThreadStatus.SetSavingTempData(false);
      gl_pChinaStockMarket->SetRTDataSetCleared(false);
      gl_pChinaStockMarket->SetUpdateOptionDB(false);
      gl_pChinaStockMarket->ClearChoicedRTDataQueue();
      gl_pChinaStockMarket->SetResetMarket(true);
      gl_pChinaStockMarket->ResetNeteaseRTDataInquiringIndex();
      gl_pChinaStockMarket->ResetNeteaseDayLineDataInquiringIndex();
      gl_pChinaStockMarket->ResetSinaStockRTDataInquiringIndex();
      gl_pChinaStockMarket->ResetTengxunRTDataInquiringIndex();
      while (gl_systemMessage.GetInformationDequeSize() > 0) gl_systemMessage.PopInformationMessage();
      gl_pChinaStockMarket->SetPermitResetMarket(true);
      gl_pChinaStockMarket->SetUpdateOptionDB(false);
      gl_pChinaStockMarket->SetSystemReady(true); // 离开此测试时，默认系统已准备好。
      EXPECT_TRUE(gl_pChinaStockMarket->IsCheckActiveStock());

      while (gl_systemMessage.GetInformationDequeSize() > 0) gl_systemMessage.PopInformationMessage();
      while (gl_systemMessage.GetDayLineInfoDequeSize() > 0) gl_systemMessage.PopDayLineInfoMessage();
      while (gl_systemMessage.GetInnerSystemInformationDequeSize() > 0) gl_systemMessage.PopInnerSystemInformationMessage();
      gl_pChinaStockMarket->ResetCurrentStock();
      gl_pChinaStockMarket->SetCurrentStockChanged(false);
      for (int i = 0; i < gl_pChinaStockMarket->GetTotalStock(); i++) {
        CChinaStockPtr pStock = gl_pChinaStockMarket->GetStock(i);
        if (!pStock->IsDayLineNeedUpdate()) pStock->SetDayLineNeedUpdate(true);
        if (pStock->IsDayLineNeedProcess()) pStock->SetDayLineNeedProcess(false);
        if (pStock->IsDayLineNeedSaving()) pStock->SetDayLineNeedSaving(false);
      }
      EXPECT_EQ(gl_pChinaStockMarket->GetDayLineNeedUpdateNumber(), gl_pChinaStockMarket->GetTotalStock());
      EXPECT_EQ(gl_pChinaStockMarket->GetDayLineNeedProcessNumber(), 0);
      EXPECT_EQ(gl_pChinaStockMarket->GetCurrentSelectedStockSet(), -1);
    }
  };

  TEST_F(CChinaMarketTest, TestInitialize) {
    CChinaStockPtr pStock = nullptr;
    for (int i = 0; i < gl_pChinaStockMarket->GetTotalStock(); i++) {
      pStock = gl_pChinaStockMarket->GetStock(i);
      EXPECT_EQ(pStock->GetOffset(), i);
      EXPECT_FALSE(pStock->IsDayLineNeedProcess());
      EXPECT_FALSE(pStock->IsDayLineNeedSaving());
      if (IsShanghaiExchange(pStock->GetSymbol())) {
        if ((pStock->GetSymbol().Left(6) >= _T("000000")) && (pStock->GetSymbol().Left(6) <= _T("000999"))) {
          EXPECT_FALSE(pStock->IsNeedProcessRTData());
        }
      }
      else if ((pStock->GetSymbol().Left(6) >= _T("399000")) && (pStock->GetSymbol().Left(6) <= _T("399999"))) {
        EXPECT_FALSE(pStock->IsNeedProcessRTData());
      }
      else {
        EXPECT_TRUE(pStock->IsNeedProcessRTData());
      }
    }
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
    EXPECT_GT(gl_pChinaStockMarket->GetTotalStock(), 0);   // 在全局变量gl_ChinaStockMarket初始化时就生成了全部股票代码池
    EXPECT_EQ(gl_pChinaStockMarket->GetTotalStockMapIndexSize(), gl_pChinaStockMarket->GetTotalStock());
    pStock = gl_pChinaStockMarket->GetStock(0);
    EXPECT_STREQ(pStock->GetSymbol(), _T("000001.SS"));
    EXPECT_EQ(gl_pChinaStockMarket->GetStockOffset(_T("000001.SS")), 0);

    EXPECT_TRUE(gl_pChinaStockMarket->IsPermitResetMarket());
    EXPECT_EQ(gl_pChinaStockMarket->GetDayLineNeedProcessNumber(), 0) << gl_pChinaStockMarket->GetDayLineNeedProcessNumber();
    EXPECT_EQ(gl_pChinaStockMarket->GetDayLineNeedSaveNumber(), 0);
    EXPECT_TRUE(gl_pChinaStockMarket->IsUsingSinaRTDataReceiver());
    EXPECT_TRUE(gl_pChinaStockMarket->IsUsingTengxunRTDataReceiver());
    EXPECT_TRUE(gl_pChinaStockMarket->IsUsingNeteaseRTDataReceiver());

    EXPECT_EQ(gl_pChinaStockMarket->GetNeteaseDayLineDataInquiringIndex(), 0);
    EXPECT_EQ(gl_pChinaStockMarket->GetNeteaseRTDataInquiringIndex(), 0);
    EXPECT_EQ(gl_pChinaStockMarket->GetSinaStockRTDataInquiringIndex(), 0);
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
    CString str = gl_pChinaStockMarket->GetTengxunInquiringStockStr(900, gl_pChinaStockMarket->GetTotalStock());
    str = gl_pChinaStockMarket->GetTengxunInquiringStockStr(900, gl_pChinaStockMarket->GetTotalStock());
    CString strCompare, str2;
    pStock = gl_pChinaStockMarket->GetStock(899);
    strCompare = pStock->GetSymbol();
    str2 = XferTengxunToStandred(str.Left(8));
    EXPECT_EQ(str2.Compare(strCompare), 0);
    str = gl_pChinaStockMarket->GetTengxunInquiringStockStr(900, gl_pChinaStockMarket->GetTotalStock());
    pStock = gl_pChinaStockMarket->GetStock(1798);
    strCompare = pStock->GetSymbol();
    str2 = XferTengxunToStandred(str.Left(8));
    EXPECT_EQ(str2.Compare(strCompare), 0);
    str = gl_pChinaStockMarket->GetTengxunInquiringStockStr(900, gl_pChinaStockMarket->GetTotalStock());
    pStock = gl_pChinaStockMarket->GetStock(2697);
    strCompare = pStock->GetSymbol();
    str2 = XferTengxunToStandred(str.Left(8));
    EXPECT_EQ(str2.Compare(strCompare), 0);
    str = gl_pChinaStockMarket->GetTengxunInquiringStockStr(900, gl_pChinaStockMarket->GetTotalStock());
    pStock = gl_pChinaStockMarket->GetStock(3596);
    strCompare = pStock->GetSymbol();
    str2 = XferTengxunToStandred(str.Left(8));
    EXPECT_EQ(str2.Compare(strCompare), 0);
    str = gl_pChinaStockMarket->GetTengxunInquiringStockStr(900, gl_pChinaStockMarket->GetTotalStock());
    pStock = gl_pChinaStockMarket->GetStock(4495);
    strCompare = pStock->GetSymbol();
    str2 = XferTengxunToStandred(str.Left(8));
    EXPECT_EQ(str2.Compare(strCompare), 0);
    str = gl_pChinaStockMarket->GetTengxunInquiringStockStr(900, gl_pChinaStockMarket->GetTotalStock());
    pStock = gl_pChinaStockMarket->GetStock(0);
    strCompare = pStock->GetSymbol();
    str2 = XferTengxunToStandred(str.Left(8));
    EXPECT_EQ(str2.Compare(strCompare), 1);
  }

  TEST_F(CChinaMarketTest, TestGetInquiringStockStr) {
    CString str2;

    gl_pChinaStockMarket->SetSystemReady(true);
    gl_pChinaStockMarket->ResetSinaStockRTDataInquiringIndex();
    CString str = gl_pChinaStockMarket->GetSinaStockInquiringStr(900, false);
    str2 = str.Left(2);
    EXPECT_STREQ(str2, _T("sh"));
    str = gl_pChinaStockMarket->GetSinaStockInquiringStr(900, false);
    str = gl_pChinaStockMarket->GetSinaStockInquiringStr(900, false);
    str = gl_pChinaStockMarket->GetSinaStockInquiringStr(900, false);
    str = gl_pChinaStockMarket->GetSinaStockInquiringStr(900, false);
    str = gl_pChinaStockMarket->GetSinaStockInquiringStr(900, false);
    EXPECT_TRUE(gl_pChinaStockMarket->GetSinaStockRTDataInquiringIndex() < 900);// 目前不到五千个活跃股票，故而六次即可遍历一次
  }

  TEST_F(CChinaMarketTest, TestGetNeteaseInquiringStockStr) {
    CChinaStockPtr pStock = nullptr;
    gl_pChinaStockMarket->SetSystemReady(true);
    gl_pChinaStockMarket->ResetNeteaseRTDataInquiringIndex();
    CString str = gl_pChinaStockMarket->GetNextNeteaseStockInquiringStr(900, gl_pChinaStockMarket->GetTotalStock());
    EXPECT_EQ(gl_pChinaStockMarket->GetNeteaseRTDataInquiringIndex(), 899);
    str = gl_pChinaStockMarket->GetNextNeteaseStockInquiringStr(900, gl_pChinaStockMarket->GetTotalStock());
    CString strCompare, str2;
    pStock = gl_pChinaStockMarket->GetStock(899);
    strCompare = pStock->GetSymbol();
    str2 = XferNeteaseToStandred(str.Left(7));
    EXPECT_STREQ(str2, strCompare);
    str = gl_pChinaStockMarket->GetNextNeteaseStockInquiringStr(900, gl_pChinaStockMarket->GetTotalStock());
    pStock = gl_pChinaStockMarket->GetStock(1798);
    strCompare = pStock->GetSymbol();
    str2 = XferNeteaseToStandred(str.Left(7));
    EXPECT_STREQ(str2, strCompare);
    str = gl_pChinaStockMarket->GetNextNeteaseStockInquiringStr(900, gl_pChinaStockMarket->GetTotalStock());
    pStock = gl_pChinaStockMarket->GetStock(2697);
    strCompare = pStock->GetSymbol();
    str2 = XferNeteaseToStandred(str.Left(7));
    EXPECT_STREQ(str2, strCompare);
    str = gl_pChinaStockMarket->GetNextNeteaseStockInquiringStr(900, gl_pChinaStockMarket->GetTotalStock());
    pStock = gl_pChinaStockMarket->GetStock(3596);
    strCompare = pStock->GetSymbol();
    str2 = XferNeteaseToStandred(str.Left(7));
    EXPECT_STREQ(str2, strCompare);
    str = gl_pChinaStockMarket->GetNextNeteaseStockInquiringStr(900, gl_pChinaStockMarket->GetTotalStock());
    pStock = gl_pChinaStockMarket->GetStock(4495);
    strCompare = pStock->GetSymbol();
    str2 = XferNeteaseToStandred(str.Left(7));
    EXPECT_STREQ(str2, strCompare);
    str = gl_pChinaStockMarket->GetNextNeteaseStockInquiringStr(900, gl_pChinaStockMarket->GetTotalStock());
    pStock = gl_pChinaStockMarket->GetStock(0);
    strCompare = pStock->GetSymbol();
    str2 = XferNeteaseToStandred(str.Left(7));
    EXPECT_STREQ(str2, strCompare);
    gl_pChinaStockMarket->ResetNeteaseRTDataInquiringIndex();
    gl_pChinaStockMarket->ResetSinaStockRTDataInquiringIndex();
  }

  TEST_F(CChinaMarketTest, TestGetNeteaseDayLineInquiringStr) {
    CString str;
    bool fStatus = false;
    CChinaStockPtr pStock = gl_pChinaStockMarket->GetStock(0);
    EXPECT_TRUE(pStock->IsDayLineNeedUpdate()) << _T("测试时使用teststock数据库，此数据库比较旧，最后更新时间不是昨日，故而活跃股票也需要更新日线");
    long lDate = pStock->GetDayLineEndDate();
    pStock->SetDayLineEndDate(gl_pChinaStockMarket->GetFormatedMarketDate());
    pStock->SetDayLineNeedUpdate(false);
    pStock->SetDayLineEndDate(lDate); // 恢复原状
    pStock = gl_pChinaStockMarket->GetStock(1);
    EXPECT_TRUE(pStock->IsDayLineNeedUpdate());
    pStock = gl_pChinaStockMarket->GetStock(2);
    EXPECT_TRUE(pStock->IsDayLineNeedUpdate());
    fStatus = gl_pChinaStockMarket->CreateNeteaseDayLineInquiringStr(str, gl_pChinaStockMarket->GetTotalStock());
    EXPECT_TRUE(fStatus);
    EXPECT_STREQ(str, _T("1000001"));
    pStock = gl_pChinaStockMarket->GetStock(1);
    EXPECT_FALSE(pStock->IsDayLineNeedUpdate());
    pStock = gl_pChinaStockMarket->GetStock(2);
    EXPECT_TRUE(pStock->IsDayLineNeedUpdate());
    long lIPOStatus = pStock->GetIPOStatus();
    pStock->SetIPOStatus(__STAKE_NULL__);
    fStatus = gl_pChinaStockMarket->CreateNeteaseDayLineInquiringStr(str, gl_pChinaStockMarket->GetTotalStock());
    EXPECT_TRUE(fStatus);
    EXPECT_STREQ(str, _T("0000002"));
    pStock->SetIPOStatus(lIPOStatus); // 恢复原状
    pStock = gl_pChinaStockMarket->GetStock(3);
    EXPECT_TRUE(pStock->IsDayLineNeedUpdate());
    pStock = gl_pChinaStockMarket->GetStock(4);
    lDate = pStock->GetDayLineEndDate();
    pStock->SetDayLineEndDate(gl_pChinaStockMarket->GetFormatedMarketDate());
    EXPECT_TRUE(pStock->IsDayLineNeedUpdate()) << _T("标识尚未更新");
    fStatus = gl_pChinaStockMarket->CreateNeteaseDayLineInquiringStr(str, gl_pChinaStockMarket->GetTotalStock());
    EXPECT_TRUE(fStatus);
    EXPECT_STREQ(str, _T("1000002"));
    pStock->SetDayLineEndDate(lDate); // 恢复原状。
    pStock = gl_pChinaStockMarket->GetStock(5);
    EXPECT_TRUE(pStock->IsDayLineNeedUpdate());

    // 恢复原状
    pStock = gl_pChinaStockMarket->GetStock(0);
    pStock->SetDayLineEndDate(lDate);
    for (int i = 0; i < gl_pChinaStockMarket->GetTotalStock(); i++) {
      pStock = gl_pChinaStockMarket->GetStock(i);
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
    long lOffset = gl_pChinaStockMarket->GetMinLineOffset(tMarket);
    EXPECT_EQ(lOffset, 0);
    tmCurrent.tm_hour = 10;
    tmCurrent.tm_min = 30;
    tmCurrent.tm_sec = 59;
    tMarket = mktime(&tmCurrent);
    lOffset = gl_pChinaStockMarket->GetMinLineOffset(tMarket);
    EXPECT_EQ(lOffset, 60);
    tmCurrent.tm_hour = 12;
    tmCurrent.tm_min = 30;
    tmCurrent.tm_sec = 59;
    tMarket = mktime(&tmCurrent);
    lOffset = gl_pChinaStockMarket->GetMinLineOffset(tMarket);
    EXPECT_EQ(lOffset, 119);
    tmCurrent.tm_hour = 14;
    tmCurrent.tm_min = 30;
    tmCurrent.tm_sec = 59;
    tMarket = mktime(&tmCurrent);
    lOffset = gl_pChinaStockMarket->GetMinLineOffset(tMarket);
    EXPECT_EQ(lOffset, 210);
    tmCurrent.tm_hour = 15;
    tmCurrent.tm_min = 30;
    tmCurrent.tm_sec = 59;
    tMarket = mktime(&tmCurrent);
    lOffset = gl_pChinaStockMarket->GetMinLineOffset(tMarket);
    EXPECT_EQ(lOffset, 239);
  }

  TEST_F(CChinaMarketTest, TestIsAStock) {
    CChinaStockPtr pstock = make_shared<CChinaStock>();
    pstock->SetSymbol(_T("600000.SS"));
    EXPECT_TRUE(gl_pChinaStockMarket->IsAStock(pstock));
    pstock->SetSymbol(_T("600000.SA"));
    EXPECT_FALSE(gl_pChinaStockMarket->IsAStock(pstock));
    pstock->SetSymbol(_T("000001.SZ"));
    EXPECT_TRUE(gl_pChinaStockMarket->IsAStock(pstock));
    pstock->SetSymbol(_T("10001.SZ"));
    EXPECT_FALSE(gl_pChinaStockMarket->IsAStock(pstock));
  }

  TEST_F(CChinaMarketTest, TestIsAStock2) {
    EXPECT_TRUE(gl_pChinaStockMarket->IsAStock(_T("600000.SS")));
    EXPECT_TRUE(gl_pChinaStockMarket->IsAStock(_T("601198.SS")));
    EXPECT_FALSE(gl_pChinaStockMarket->IsAStock(_T("602102.SS")));
    EXPECT_FALSE(gl_pChinaStockMarket->IsAStock(_T("603604.SS")));
    EXPECT_FALSE(gl_pChinaStockMarket->IsAStock(_T("604604.SS")));
    EXPECT_FALSE(gl_pChinaStockMarket->IsAStock(_T("605604.SS")));
    EXPECT_FALSE(gl_pChinaStockMarket->IsAStock(_T("606604.SS")));
    EXPECT_FALSE(gl_pChinaStockMarket->IsAStock(_T("607604.SS")));
    EXPECT_FALSE(gl_pChinaStockMarket->IsAStock(_T("608604.SS")));
    EXPECT_FALSE(gl_pChinaStockMarket->IsAStock(_T("609604.SS")));

    EXPECT_FALSE(gl_pChinaStockMarket->IsAStock(_T("600000.SZ")));
    EXPECT_FALSE(gl_pChinaStockMarket->IsAStock(_T("000001.SA")));
    EXPECT_FALSE(gl_pChinaStockMarket->IsAStock(_T("000001.AZ")));
    EXPECT_FALSE(gl_pChinaStockMarket->IsAStock(_T("200001.SZ")));
    EXPECT_TRUE(gl_pChinaStockMarket->IsAStock(_T("000001.SZ")));
    EXPECT_TRUE(gl_pChinaStockMarket->IsAStock(_T("002389.SZ")));
    EXPECT_FALSE(gl_pChinaStockMarket->IsAStock(_T("003389.SZ")));
    EXPECT_FALSE(gl_pChinaStockMarket->IsAStock(_T("004389.SZ")));
    EXPECT_FALSE(gl_pChinaStockMarket->IsAStock(_T("005389.SZ")));
    EXPECT_FALSE(gl_pChinaStockMarket->IsAStock(_T("006389.SZ")));
    EXPECT_FALSE(gl_pChinaStockMarket->IsAStock(_T("007389.SZ")));
    EXPECT_FALSE(gl_pChinaStockMarket->IsAStock(_T("008389.SZ")));
    EXPECT_FALSE(gl_pChinaStockMarket->IsAStock(_T("009389.SZ")));
    EXPECT_FALSE(gl_pChinaStockMarket->IsAStock(_T("001389.SZ")));
  }

  TEST_F(CChinaMarketTest, TestIsStock) {
    EXPECT_GT(gl_pChinaStockMarket->GetTotalStock(), 1);
    EXPECT_TRUE(gl_pChinaStockMarket->IsStock(_T("600000.SS")));
    EXPECT_FALSE(gl_pChinaStockMarket->IsStock(_T("60000.SS")));
  }

  TEST_F(CChinaMarketTest, TestGetStockName) {
    //未实现.由于stockName存储时使用的是UniCode制式，而本系统默认是Ansi制式，导致无法进行字符串对比。暂时不进行测试了。
    //EXPECT_STREQ(gl_pChinaStockMarket->GetStockName(_T("600000.SS")), _T("浦发银行"));
    EXPECT_STREQ(gl_pChinaStockMarket->GetStockName(_T("60000.SS")), _T("")); // 没找到返回空字符串
  }

  TEST_F(CChinaMarketTest, TestGetStockCode) {
    EXPECT_EQ(gl_pChinaStockMarket->GetStock(_T("66000.SZ")), nullptr);
    EXPECT_FALSE(gl_pChinaStockMarket->GetStock(_T("600001.SS")) == nullptr);

    EXPECT_EQ(gl_pChinaStockMarket->GetStock(-1), nullptr);
    EXPECT_EQ(gl_pChinaStockMarket->GetStock(gl_pChinaStockMarket->GetTotalStock()), nullptr);
    EXPECT_FALSE(gl_pChinaStockMarket->GetStock(0) == nullptr);
    EXPECT_FALSE(gl_pChinaStockMarket->GetStock(gl_pChinaStockMarket->GetTotalStock() - 1) == nullptr);
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
    gl_pChinaStockMarket->SetCurrentStock(_T("600000.SS"));
    pStock = gl_pChinaStockMarket->GetCurrentStock();
    EXPECT_TRUE(pStock->IsRecordRTData());
    EXPECT_STREQ(pStock->GetSymbol(), _T("600000.SS"));
    gl_pChinaStockMarket->ResetCurrentStock();
    EXPECT_FALSE(pStock->IsRecordRTData());
    EXPECT_EQ(gl_pChinaStockMarket->GetCurrentStock(), nullptr);

    gl_pChinaStockMarket->SetCurrentStockChanged(false);
  }

  TEST_F(CChinaMarketTest, TestUnloadDayLine) {
    EXPECT_TRUE(gl_pChinaStockMarket->UnloadDayLine());
  }

  TEST_F(CChinaMarketTest, TestMarketReady) {
    EXPECT_TRUE(gl_pChinaStockMarket->IsSystemReady()) << "中国股票市场默认为假, 但测试系统默认为真";
    gl_pChinaStockMarket->SetSystemReady(true);
    EXPECT_TRUE(gl_pChinaStockMarket->IsSystemReady());
    gl_pChinaStockMarket->SetSystemReady(false);
    EXPECT_FALSE(gl_pChinaStockMarket->IsSystemReady());

    gl_pChinaStockMarket->SetSystemReady(true); // 恢复原态
  }

  TEST_F(CChinaMarketTest, TestIsTodayStockProcessed) {
    gl_pChinaStockMarket->SetTodayStockProcessed(true);
    EXPECT_TRUE(gl_pChinaStockMarket->IsTodayStockProcessed());
    EXPECT_FALSE(gl_pChinaStockMarket->IsProcessingTodayStock());
    EXPECT_FALSE(gl_pChinaStockMarket->IsTodayStockNotProcessed());
    gl_pChinaStockMarket->SetTodayStockProcessed(false);
    EXPECT_TRUE(gl_pChinaStockMarket->IsTodayStockNotProcessed());
    EXPECT_FALSE(gl_pChinaStockMarket->IsTodayStockProcessed());
    EXPECT_FALSE(gl_pChinaStockMarket->IsProcessingTodayStock());
    gl_pChinaStockMarket->SetProcessingTodayStock();
    EXPECT_TRUE(gl_pChinaStockMarket->IsProcessingTodayStock());
  }

  TEST_F(CChinaMarketTest, TestGetCurrentSelectedPosition) {
    EXPECT_EQ(gl_pChinaStockMarket->GetCurrentSelectedPosition(), 0);
    gl_pChinaStockMarket->SetCurrentSelectedPosition(10101010);
    EXPECT_EQ(gl_pChinaStockMarket->GetCurrentSelectedPosition(), 10101010);

    gl_pChinaStockMarket->SetCurrentSelectedPosition(0);
  }

  TEST_F(CChinaMarketTest, TestGetCurrentSelectedStockSet) {
    EXPECT_EQ(gl_pChinaStockMarket->GetCurrentSelectedStockSet(), -1);
    gl_pChinaStockMarket->SetCurrentSelectedStockSet(10);
    EXPECT_EQ(gl_pChinaStockMarket->GetCurrentSelectedStockSet(), 10);

    gl_pChinaStockMarket->SetCurrentSelectedStockSet(-1);
  }

  TEST_F(CChinaMarketTest, TestIsChoiced10RSStrongStockSet) {
    gl_pChinaStockMarket->SetChoiced10RSStrongStockSet(true);
    EXPECT_TRUE(gl_pChinaStockMarket->IsChoiced10RSStrongStockSet());
    gl_pChinaStockMarket->SetChoiced10RSStrongStockSet(false);
    EXPECT_FALSE(gl_pChinaStockMarket->IsChoiced10RSStrongStockSet());
  }

  TEST_F(CChinaMarketTest, TestIsChoiced10RSStrong1StockSet) {
    gl_pChinaStockMarket->SetChoiced10RSStrong1StockSet(true);
    EXPECT_TRUE(gl_pChinaStockMarket->IsChoiced10RSStrong1StockSet());
    gl_pChinaStockMarket->SetChoiced10RSStrong1StockSet(false);
    EXPECT_FALSE(gl_pChinaStockMarket->IsChoiced10RSStrong1StockSet());
  }
  TEST_F(CChinaMarketTest, TestIsChoiced10RSStrong2StockSet) {
    gl_pChinaStockMarket->SetChoiced10RSStrong2StockSet(true);
    EXPECT_TRUE(gl_pChinaStockMarket->IsChoiced10RSStrong2StockSet());
    gl_pChinaStockMarket->SetChoiced10RSStrong2StockSet(false);
    EXPECT_FALSE(gl_pChinaStockMarket->IsChoiced10RSStrong2StockSet());
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
      if (!pStock->IsDayLineNeedUpdate()) pStock->SetDayLineNeedUpdate(true);
    }
  }

  TEST_F(CChinaMarketTest, TestTaskProcessDayLineGetFromNeeteaseServer) {
    CChinaStockPtr pStock = gl_pChinaStockMarket->GetStock(_T("600666.SS"));
    EXPECT_FALSE(pStock->IsDayLineNeedProcess());
    pStock->SetDayLineNeedProcess(true);
    EXPECT_TRUE(gl_pChinaStockMarket->TaskProcessDayLineGetFromNeeteaseServer());
    EXPECT_FALSE(pStock->IsDayLineNeedProcess());
    EXPECT_EQ(gl_pChinaStockMarket->GetDayLineNeedProcessNumber(), 0);
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

  TEST_F(CChinaMarketTest, TestSetUpdateOptionDB) {
    EXPECT_FALSE(gl_pChinaStockMarket->IsUpdateOptionDB());
    gl_pChinaStockMarket->SetUpdateOptionDB(true);
    EXPECT_TRUE(gl_pChinaStockMarket->IsUpdateOptionDB());
    gl_pChinaStockMarket->SetUpdateOptionDB(false);
    EXPECT_FALSE(gl_pChinaStockMarket->IsUpdateOptionDB());
  }

  TEST_F(CChinaMarketTest, TestSetUpdateChoicedStockDB) {
    EXPECT_FALSE(gl_pChinaStockMarket->IsUpdateChoicedStockDB());
    gl_pChinaStockMarket->SetUpdateChoicedStockDB(true);
    EXPECT_TRUE(gl_pChinaStockMarket->IsUpdateChoicedStockDB());
    gl_pChinaStockMarket->SetUpdateChoicedStockDB(false);
    EXPECT_FALSE(gl_pChinaStockMarket->IsUpdateChoicedStockDB());
  }

  TEST_F(CChinaMarketTest, TestSetRTDataSetCleared) {
    EXPECT_FALSE(gl_pChinaStockMarket->IsRTDataSetCleared());
    gl_pChinaStockMarket->SetRTDataSetCleared(true);
    EXPECT_TRUE(gl_pChinaStockMarket->IsRTDataSetCleared());
    gl_pChinaStockMarket->SetRTDataSetCleared(false);
    EXPECT_FALSE(gl_pChinaStockMarket->IsRTDataSetCleared());
  }

  TEST_F(CChinaMarketTest, TestSetSavingTempData) {
    EXPECT_TRUE(gl_pChinaStockMarket->IsSavingTempData());
    gl_pChinaStockMarket->SetSavingTempData(false);
    EXPECT_FALSE(gl_pChinaStockMarket->IsSavingTempData());
    gl_pChinaStockMarket->SetSavingTempData(true);
    EXPECT_TRUE(gl_pChinaStockMarket->IsSavingTempData());
  }

  TEST_F(CChinaMarketTest, TestTaskResetMarket) {
    tm tm_;
    tm_.tm_wday = 1; // 星期一
    gl_pChinaStockMarket->__TEST_SetMarketTM(tm_);
    EXPECT_TRUE(gl_pChinaStockMarket->IsPermitResetMarket());
    EXPECT_TRUE(gl_pChinaStockMarket->IsResetMarket());
    gl_pChinaStockMarket->SetSystemReady(true);
    gl_pChinaStockMarket->SetResetMarket(false);
    gl_pChinaStockMarket->TaskResetMarket(91259);
    EXPECT_FALSE(gl_pChinaStockMarket->IsResetMarket());
    gl_pChinaStockMarket->TaskResetMarket(91400);
    EXPECT_FALSE(gl_pChinaStockMarket->IsResetMarket()) << _T("第一次重启市场，其结束时间必须在9:14之前，这样才能保证只运行了一次（此函数必须每分钟调度一次");
    gl_pChinaStockMarket->SetSystemReady(true);
    gl_pChinaStockMarket->TaskResetMarket(91300);
    if (gl_pChinaStockMarket->TooManyStocksNeedUpdated()) {
      EXPECT_FALSE(gl_pChinaStockMarket->IsResetMarket());
      EXPECT_TRUE(gl_pChinaStockMarket->IsPermitResetMarket());
      EXPECT_TRUE(gl_pChinaStockMarket->IsSystemReady());
    }
    else {
      EXPECT_TRUE(gl_pChinaStockMarket->IsResetMarket());
      EXPECT_TRUE(gl_pChinaStockMarket->IsPermitResetMarket());
      EXPECT_FALSE(gl_pChinaStockMarket->IsSystemReady());
    }
  }

  TEST_F(CChinaMarketTest, TestTaskResetMarket2) {
    tm tm_;
    tm_.tm_wday = 0; // 星期日
    gl_pChinaStockMarket->__TEST_SetMarketTM(tm_);
    EXPECT_TRUE(gl_pChinaStockMarket->IsPermitResetMarket());
    gl_pChinaStockMarket->SetSystemReady(true);
    gl_pChinaStockMarket->SetResetMarket(false);
    gl_pChinaStockMarket->TaskResetMarket(91259);
    EXPECT_FALSE(gl_pChinaStockMarket->IsResetMarket());
    gl_pChinaStockMarket->TaskResetMarket(91401);
    EXPECT_FALSE(gl_pChinaStockMarket->IsResetMarket());
    gl_pChinaStockMarket->TaskResetMarket(91300);
    EXPECT_FALSE(gl_pChinaStockMarket->IsResetMarket());
    EXPECT_TRUE(gl_pChinaStockMarket->IsPermitResetMarket());
    EXPECT_TRUE(gl_pChinaStockMarket->IsSystemReady());
  }

  TEST_F(CChinaMarketTest, TestTaskResetMarket3) {
    tm tm_;
    tm_.tm_wday = 1; // 星期一
    gl_pChinaStockMarket->__TEST_SetMarketTM(tm_);
    gl_pChinaStockMarket->SetSystemReady(true);
    gl_pChinaStockMarket->SetStockNeedUpdated(1001);
    EXPECT_TRUE(gl_pChinaStockMarket->TooManyStocksNeedUpdated());
    EXPECT_TRUE(gl_pChinaStockMarket->IsPermitResetMarket());
    EXPECT_TRUE(gl_pChinaStockMarket->IsResetMarket());
    gl_pChinaStockMarket->SetResetMarket(false);
    gl_pChinaStockMarket->TaskResetMarket(91259);
    EXPECT_FALSE(gl_pChinaStockMarket->IsResetMarket());
    gl_pChinaStockMarket->TaskResetMarket(91400);
    EXPECT_FALSE(gl_pChinaStockMarket->IsResetMarket()) << _T("第一次重启市场，其结束时间必须在9:14之前，这样才能保证只运行了一次（此函数必须每分钟调度一次");
    gl_pChinaStockMarket->SetSystemReady(true);
    gl_pChinaStockMarket->TaskResetMarket(91300);
    EXPECT_FALSE(gl_pChinaStockMarket->IsResetMarket());
    EXPECT_TRUE(gl_pChinaStockMarket->IsPermitResetMarket());
    EXPECT_TRUE(gl_pChinaStockMarket->IsSystemReady());

    // 恢复系统原态
    gl_pChinaStockMarket->SetStockNeedUpdated(0);
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
    EXPECT_TRUE(gl_pChinaStockMarket->IsPermitResetMarket());
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

  TEST_F(CChinaMarketTest, TestAddChoicedStock) {
    auto pStock = gl_pChinaStockMarket->GetStock(1);
    gl_pChinaStockMarket->AddChoicedStock(pStock);
    EXPECT_EQ(gl_pChinaStockMarket->GetChoicedStockSize(), 1);
    pStock = gl_pChinaStockMarket->GetStock(2);
    EXPECT_TRUE(gl_pChinaStockMarket->AddChoicedStock(pStock));
    EXPECT_EQ(gl_pChinaStockMarket->GetChoicedStockSize(), 2);
    pStock = gl_pChinaStockMarket->GetStock(1);
    EXPECT_FALSE(gl_pChinaStockMarket->AddChoicedStock(pStock));
    EXPECT_EQ(gl_pChinaStockMarket->GetChoicedStockSize(), 2);

    EXPECT_TRUE(gl_pChinaStockMarket->DeleteChoicedStock(pStock));
    pStock = gl_pChinaStockMarket->GetStock(5);
    EXPECT_EQ(gl_pChinaStockMarket->GetChoicedStockSize(), 1);
    EXPECT_FALSE(gl_pChinaStockMarket->DeleteChoicedStock(pStock));
    gl_pChinaStockMarket->ClearChoiceStockContainer();
  }

  TEST_F(CChinaMarketTest, TestAddChoicedStock2) {
    EXPECT_EQ(gl_pChinaStockMarket->GetChoicedStockSize(), 0);
    CChinaStockPtr pStock = gl_pChinaStockMarket->GetStock(0);
    gl_pChinaStockMarket->AddChoicedStock(pStock);
    EXPECT_EQ(gl_pChinaStockMarket->GetChoicedStockSize(), 1);
    gl_pChinaStockMarket->ClearChoiceStockContainer();
    EXPECT_EQ(gl_pChinaStockMarket->GetChoicedStockSize(), 0);
  }

  TEST_F(CChinaMarketTest, TestCheckMarketOpen) {
    tm tm_;
    tm_.tm_wday = 1;
    gl_pChinaStockMarket->__TEST_SetMarketTM(tm_);
    EXPECT_FALSE(gl_pChinaStockMarket->TaskCheckMarketOpen(92800));
    EXPECT_FALSE(gl_pChinaStockMarket->IsMarketOpened());
    EXPECT_TRUE(gl_pChinaStockMarket->TaskCheckMarketOpen(92801));
    EXPECT_TRUE(gl_pChinaStockMarket->IsMarketOpened());
    EXPECT_TRUE(gl_pChinaStockMarket->TaskCheckMarketOpen(150559));
    EXPECT_TRUE(gl_pChinaStockMarket->IsMarketOpened());
    EXPECT_FALSE(gl_pChinaStockMarket->TaskCheckMarketOpen(150600));
    tm_.tm_wday = 0;
    gl_pChinaStockMarket->__TEST_SetMarketTM(tm_);
    EXPECT_FALSE(gl_pChinaStockMarket->TaskCheckMarketOpen(92859));
    EXPECT_FALSE(gl_pChinaStockMarket->IsMarketOpened());
    EXPECT_FALSE(gl_pChinaStockMarket->TaskCheckMarketOpen(92900));
    EXPECT_FALSE(gl_pChinaStockMarket->IsMarketOpened());
    EXPECT_FALSE(gl_pChinaStockMarket->TaskCheckMarketOpen(150559));
    EXPECT_FALSE(gl_pChinaStockMarket->IsMarketOpened());
    EXPECT_FALSE(gl_pChinaStockMarket->TaskCheckMarketOpen(150600));
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

  TEST_F(CChinaMarketTest, TestIncreaseNeteaseDayLineInquiringIndex) {
    long k = 0;
    int i = gl_pChinaStockMarket->IncreaseStockInquiringIndex(k, gl_pChinaStockMarket->GetTotalStock());
    EXPECT_EQ(i, 1);
    EXPECT_EQ(k, 1);
    i = gl_pChinaStockMarket->IncreaseStockInquiringIndex(k, gl_pChinaStockMarket->GetTotalStock());
    EXPECT_EQ(i, 2);
    EXPECT_EQ(k, 2);
    k = 11999;
    i = gl_pChinaStockMarket->IncreaseStockInquiringIndex(k, gl_pChinaStockMarket->GetTotalStock());
    EXPECT_EQ(k, 0);
    EXPECT_EQ(i, 0);
  }

  TEST_F(CChinaMarketTest, TestTaskProcessRTData) {
    gl_ThreadStatus.SetRTDataNeedCalculate(true);
    EXPECT_TRUE(gl_pChinaStockMarket->TaskProcessRTData());
    gl_ThreadStatus.SetRTDataNeedCalculate(false);
  }

  TEST_F(CChinaMarketTest, TestSetCheckActiveStockFlag1) {
    EXPECT_TRUE(gl_pChinaStockMarket->IsSystemReady()) << "单元测试时，系统状态永远为准备好了";
    gl_pChinaStockMarket->SetSystemReady(false);
    EXPECT_TRUE(gl_pChinaStockMarket->IsCheckActiveStock()) << "当系统尚未准备好时，检测活跃股票的标识永远为真";
    gl_pChinaStockMarket->SetSystemReady(true);
  }

  TEST_F(CChinaMarketTest, TestSetCheckActiveStockFlag2) {
    EXPECT_TRUE(gl_pChinaStockMarket->IsSystemReady());
    EXPECT_TRUE(gl_pChinaStockMarket->IsCheckActiveStock());
    gl_pChinaStockMarket->TaskSetCheckActiveStockFlag(91459);
    EXPECT_FALSE(gl_pChinaStockMarket->IsCheckActiveStock());
    gl_pChinaStockMarket->TaskSetCheckActiveStockFlag(91500);
    EXPECT_TRUE(gl_pChinaStockMarket->IsCheckActiveStock());
    gl_pChinaStockMarket->TaskSetCheckActiveStockFlag(92659);
    EXPECT_TRUE(gl_pChinaStockMarket->IsCheckActiveStock());
    gl_pChinaStockMarket->TaskSetCheckActiveStockFlag(92700);
    EXPECT_FALSE(gl_pChinaStockMarket->IsCheckActiveStock());
    gl_pChinaStockMarket->TaskSetCheckActiveStockFlag(113059);
    EXPECT_FALSE(gl_pChinaStockMarket->IsCheckActiveStock());
    gl_pChinaStockMarket->TaskSetCheckActiveStockFlag(113300);
    EXPECT_TRUE(gl_pChinaStockMarket->IsCheckActiveStock());
    gl_pChinaStockMarket->TaskSetCheckActiveStockFlag(125859);
    EXPECT_TRUE(gl_pChinaStockMarket->IsCheckActiveStock());
    gl_pChinaStockMarket->TaskSetCheckActiveStockFlag(125900);
    EXPECT_FALSE(gl_pChinaStockMarket->IsCheckActiveStock());
    gl_pChinaStockMarket->TaskSetCheckActiveStockFlag(150000);
    EXPECT_FALSE(gl_pChinaStockMarket->IsCheckActiveStock());

    gl_pChinaStockMarket->SetCheckActiveStock(true); // 恢复原态
  }

  TEST_F(CChinaMarketTest, TestLoadStockCodeDB) {
    // 股票代码数据库在全局环境设置时即已装入测试系统，故而直接测试即可。
    CChinaStockPtr pStock = nullptr;
    pStock = gl_pChinaStockMarket->GetStock(0);
    EXPECT_TRUE(pStock->IsIPOed());
    EXPECT_STREQ(pStock->GetSymbol(), _T("000001.SS"));
    EXPECT_EQ(pStock->GetDayLineStartDate(), 19901220);
    EXPECT_FALSE(pStock->IsActive()) << "装载股票代码时永远设置为假";
    pStock = gl_pChinaStockMarket->GetStock(_T("600002.SS"));
    EXPECT_TRUE(pStock->IsDelisted());
    EXPECT_EQ(pStock->GetDayLineStartDate(), 19980409);
    EXPECT_EQ(pStock->GetDayLineEndDate(), 20060406);
    EXPECT_FALSE(pStock->IsActive());
  }

  TEST_F(CChinaMarketTest, TestGetLastLoginDate) {
    gl_pChinaStockMarket->SetLastLoginDate(19900102);
    EXPECT_EQ(gl_pChinaStockMarket->GetLastLoginDate(), 19900102);
  }

  TEST_F(CChinaMarketTest, TestGetRSStartDate) {
    gl_pChinaStockMarket->SetRSStartDate(19900202);
    EXPECT_EQ(gl_pChinaStockMarket->GetRSStartDate(), 19900202);
  }

  TEST_F(CChinaMarketTest, TestGetRSEndDate) {
    gl_pChinaStockMarket->SetRSEndDate(19900302);
    EXPECT_EQ(gl_pChinaStockMarket->GetRSEndDate(), 19900302);
  }

  TEST_F(CChinaMarketTest, TestGetNewestTransactionTime) {
    gl_pChinaStockMarket->SetNewestTransactionTime(10101010);
    EXPECT_EQ(gl_pChinaStockMarket->GetNewestTransactionTime(), 10101010);
  }

  TEST_F(CChinaMarketTest, TestGetStockCodeForInquiringSinaRTData) {
    gl_pChinaStockMarket->SetStockCodeForInquiringRTData(_T("601919.SS"));
    EXPECT_EQ(gl_pChinaStockMarket->GetStockCodeForInquiringRTData(), _T("601919.SS"));
  }

  TEST_F(CChinaMarketTest, TestGetReadingNeteaseDayLineDataTime) {
    gl_pChinaStockMarket->SetStockCodeForInquiringNeteaseDayLine(_T("600000.SS"));
    EXPECT_EQ(gl_pChinaStockMarket->GetStockCodeForInquiringNeteaseDayLine(), _T("600000.SS"));
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

  TEST_F(CChinaMarketTest, TestGetStockPtr) {
    CChinaStockPtr pStock = nullptr;
    pStock = gl_pChinaStockMarket->GetStock(_T("600000.SS"));
    EXPECT_STREQ(pStock->GetSymbol(), _T("600000.SS"));
  }

  TEST_F(CChinaMarketTest, TestIsInvalidNeteaseRTData) {
    CWebDataPtr pWebDataReceived;
    pWebDataReceived = make_shared<CWebData>();
    CString str = _T("_ntes_quote_callback({ });");
    pWebDataReceived->SetData(str.GetBuffer(), str.GetLength(), 0);
    pWebDataReceived->SetBufferLength(str.GetLength());
    pWebDataReceived->ResetCurrentPos();
    EXPECT_TRUE(gl_pChinaStockMarket->IsInvalidNeteaseRTData(*pWebDataReceived));
    str = _T("_ntes_quote_callback({});");
    pWebDataReceived->SetData(str.GetBuffer(), str.GetLength(), 0);
    pWebDataReceived->SetBufferLength(str.GetLength());
    pWebDataReceived->ResetCurrentPos();
    EXPECT_FALSE(gl_pChinaStockMarket->IsInvalidNeteaseRTData(*pWebDataReceived));
  }

  TEST_F(CChinaMarketTest, TestIsValidNeteaseRTDataPrefix) {
    CWebDataPtr pWebDataReceived;
    pWebDataReceived = make_shared<CWebData>();
    CString str = _T("_ntes_quote_callback({\"");
    pWebDataReceived->SetData(str.GetBuffer(), str.GetLength(), 0);
    pWebDataReceived->SetBufferLength(str.GetLength());
    pWebDataReceived->ResetCurrentPos();
    EXPECT_TRUE(gl_pChinaStockMarket->IsValidNeteaseRTDataPrefix(*pWebDataReceived));
    str = _T("_ntes_quo_callback({\"");
    pWebDataReceived->SetData((LPSTR)str.GetBuffer(), str.GetLength(), 0);
    pWebDataReceived->SetBufferLength(str.GetLength());
    pWebDataReceived->ResetCurrentPos();
    EXPECT_FALSE(gl_pChinaStockMarket->IsValidNeteaseRTDataPrefix(*pWebDataReceived));
  }

  TEST_F(CChinaMarketTest, TestIsValidTengxunRTDataPrefix) {
    CWebDataPtr pWebDataReceived;
    pWebDataReceived = make_shared<CWebData>();
    CString str = _T("v_pv_none_match=\"1\";\n");
    pWebDataReceived->SetData(str.GetBuffer(), str.GetLength(), 0);
    pWebDataReceived->SetBufferLength(str.GetLength());
    pWebDataReceived->ResetCurrentPos();
    EXPECT_TRUE(gl_pChinaStockMarket->IsInvalidTengxunRTData(*pWebDataReceived));
    EXPECT_EQ(pWebDataReceived->GetCurrentPos(), 0);
    str = _T("v_pv_none_mtch=\"1\";\n");
    pWebDataReceived->SetData(str.GetBuffer(), str.GetLength(), 0);
    pWebDataReceived->SetBufferLength(str.GetLength());
    pWebDataReceived->ResetCurrentPos();
    EXPECT_FALSE(gl_pChinaStockMarket->IsInvalidTengxunRTData(*pWebDataReceived));
    EXPECT_EQ(pWebDataReceived->GetCurrentPos(), 0);
  }

  TEST_F(CChinaMarketTest, TestTaskProcessWebRTDataGetFromSinaServer) {
    CString strRTData = _T("var hq_str_sh600000=\"浦发银行,10.120,9.940,10.090,10.180,9.960,10.090,10.100,42609358,428744766.000,453445,10.090,312600,10.080,173519,10.070,242800,10.060,136700,10.050,3600,10.100,3200,10.120,5100,10.130,129900,10.150,476200,10.160,2020-03-20,15:00:00,00,\";\nvar hq_str_sh600004=\"白云机场,13.010,12.900,13.300,13.350,12.990,13.300,13.320,9689808,127814773.000,4810,13.300,11100,13.290,5800,13.280,5200,13.270,10800,13.260,1600,13.320,61100,13.330,46100,13.340,114467,13.350,35000,13.360,2020-03-20,15:00:09,00,\";\nvar hq_str_sh600008=\"首创股份,3.050,3.040,3.050,3.080,3.030,3.050,3.060,22087562,67408587.000,760200,3.050,625400,3.040,570900,3.030,329000,3.020,202000,3.010,9200,3.060,112100,3.070,491897,3.080,449600,3.090,664140,3.100,2020-03-20,15:00:09,00,\";\n");
    CWebDataPtr pData = make_shared<CWebData>();
    pData->__TEST_SetBuffer__(strRTData);
    gl_WebInquirer.PushSinaRTData(pData);
    gl_pChinaStockMarket->TaskProcessWebRTDataGetFromSinaServer();
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
    gl_pChinaStockMarket->TaskProcessWebRTDataGetFromNeteaseServer();
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
    gl_pChinaStockMarket->TaskProcessWebRTDataGetFromTengxunServer();
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
    gl_pChinaStockMarket->TaskDiscardNeteaseRTData();
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
    gl_pChinaStockMarket->TaskDiscardSinaRTData();
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
    gl_pChinaStockMarket->TaskDiscardTengxunRTData();
    EXPECT_EQ(gl_WebRTDataContainer.GetTengxunDataSize(), 0);
  }

  TEST_F(CChinaMarketTest, TestStoreChoicedRTData) {
    EXPECT_EQ(gl_pChinaStockMarket->GetChoicedRTDataSize(), 0);
    CWebRTDataPtr pRTData = make_shared<CWebRTData>();
    gl_pChinaStockMarket->StoreChoiceRTData(pRTData);
    EXPECT_EQ(gl_pChinaStockMarket->GetChoicedRTDataSize(), 1);
    gl_pChinaStockMarket->ClearChoicedRTDataQueue();
    EXPECT_EQ(gl_pChinaStockMarket->GetChoicedRTDataSize(), 0);
  }

  TEST_F(CChinaMarketTest, TestClearDayLineNeedUpdaeStatus) {
    /*
    for (int i = 0; i < gl_pChinaStockMarket->GetTotalStock(); i++) {
      if (!gl_pChinaStockMarket->GetStock(i)->IsDayLineNeedUpdate()) gl_pChinaStockMarket->GetStock(i)->SetDayLineNeedUpdate(true);
    }
    */
    gl_pChinaStockMarket->ClearDayLineNeedUpdaeStatus();

    for (int i = 0; i < gl_pChinaStockMarket->GetTotalStock(); i++) {
      EXPECT_FALSE(gl_pChinaStockMarket->GetStock(i)->IsDayLineNeedUpdate());
    }
  }

  TEST_F(CChinaMarketTest, TestTaskGetRTDataFromWeb1) {
    EXPECT_TRUE(gl_pChinaStockMarket->IsSystemReady());
    EXPECT_TRUE(gl_pChinaStockMarket->IsUsingSinaRTDataServer());
    gl_pChinaStockMarket->SetSystemReady(false);
    gl_pChinaStockMarket->SetCountDownTengxunNumber(5);
    EXPECT_TRUE(gl_pChinaStockMarket->IsUsingTengxunRTDataReceiver());
    EXPECT_TRUE(gl_pChinaStockMarket->IsUsingNeteaseRTDataReceiver());
    gl_pChinaStockMarket->SetUsingNeteaseRTDataReceiver(true);
    EXPECT_CALL(*gl_pSinaRTWebInquiry, StartReadingThread).Times(1);
    EXPECT_CALL(*gl_pNeteaseRTWebInquiry, StartReadingThread).Times(0);
    EXPECT_CALL(*gl_pTengxunRTWebInquiry, StartReadingThread).Times(0);
    EXPECT_FALSE(gl_pSinaRTWebInquiry->IsReadingWebData());
    gl_pChinaStockMarket->TaskGetRTDataFromWeb();
    EXPECT_TRUE(gl_pSinaRTWebInquiry->IsReadingWebData());

    gl_pSinaRTWebInquiry->SetReadingWebData(false);

    gl_pChinaStockMarket->SetUsingNeteaseRTDataReceiver(true);
    gl_pSinaRTWebInquiry->SetReadingWebData(false);
    gl_pTengxunRTWebInquiry->SetReadingWebData(false);
    gl_pNeteaseRTWebInquiry->SetReadingWebData(false);
    gl_pChinaStockMarket->SetSystemReady(true);
  }

  TEST_F(CChinaMarketTest, TestTaskGetRTDataFromWeb2) {
    EXPECT_TRUE(gl_pChinaStockMarket->IsSystemReady());
    EXPECT_TRUE(gl_pChinaStockMarket->IsUsingSinaRTDataServer());
    gl_pChinaStockMarket->SetUsingNeteaseRTDataServer();
    gl_pChinaStockMarket->SetCountDownTengxunNumber(5);
    EXPECT_TRUE(gl_pChinaStockMarket->IsUsingTengxunRTDataReceiver());
    EXPECT_TRUE(gl_pChinaStockMarket->IsUsingNeteaseRTDataReceiver());
    gl_pChinaStockMarket->SetUsingNeteaseRTDataReceiver(true);
    EXPECT_CALL(*gl_pSinaRTWebInquiry, StartReadingThread).Times(0);
    EXPECT_CALL(*gl_pNeteaseRTWebInquiry, StartReadingThread).Times(1);
    EXPECT_CALL(*gl_pTengxunRTWebInquiry, StartReadingThread).Times(0); // _T("腾讯实时数据采集需要计数器倒数至零");
    EXPECT_FALSE(gl_pNeteaseRTWebInquiry->IsReadingWebData());
    gl_pChinaStockMarket->TaskGetRTDataFromWeb();
    EXPECT_TRUE(gl_pNeteaseRTWebInquiry->IsReadingWebData());
    EXPECT_EQ(gl_pChinaStockMarket->GetCountDownTengxunNumber(), 4) << _T("调用TaskGetRTDataFromWeb后计数器已减一");
    gl_pNeteaseRTWebInquiry->SetReadingWebData(false);
    gl_pChinaStockMarket->SetUsingSinaRTDataServer();

    gl_pChinaStockMarket->SetCountDownTengxunNumber(5);
    gl_pChinaStockMarket->SetUsingNeteaseRTDataReceiver(true);
    gl_pSinaRTWebInquiry->SetReadingWebData(false);
    gl_pTengxunRTWebInquiry->SetReadingWebData(false);
    gl_pNeteaseRTWebInquiry->SetReadingWebData(false);
    gl_pChinaStockMarket->SetSystemReady(true);
  }

  TEST_F(CChinaMarketTest, TestTaskGetRTDataFromWeb3) {
    EXPECT_TRUE(gl_pChinaStockMarket->IsSystemReady());
    gl_pChinaStockMarket->SetSystemReady(true);
    gl_pChinaStockMarket->SetCountDownTengxunNumber(0);
    EXPECT_CALL(*gl_pSinaRTWebInquiry, StartReadingThread).Times(1);
    EXPECT_CALL(*gl_pTengxunRTWebInquiry, StartReadingThread).Times(1); // _T("腾讯实时数据采集计数器已倒数至零");
    EXPECT_FALSE(gl_pSinaRTWebInquiry->IsReadingWebData());
    EXPECT_FALSE(gl_pTengxunRTWebInquiry->IsReadingWebData());
    gl_pChinaStockMarket->TaskGetRTDataFromWeb();
    EXPECT_TRUE(gl_pSinaRTWebInquiry->IsReadingWebData());
    EXPECT_TRUE(gl_pTengxunRTWebInquiry->IsReadingWebData());

    gl_pChinaStockMarket->SetUsingNeteaseRTDataReceiver(true);
    gl_pSinaRTWebInquiry->SetReadingWebData(false);
    gl_pTengxunRTWebInquiry->SetReadingWebData(false);
    gl_pNeteaseRTWebInquiry->SetReadingWebData(false);
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
    gl_pChinaStockMarket->SetRTDataReceived(24000);
    EXPECT_FALSE(gl_pChinaStockMarket->CheckMarketReady());
    EXPECT_FALSE(gl_pChinaStockMarket->IsSystemReady());
    gl_pChinaStockMarket->SetRTDataReceived(24000 + 1);
    EXPECT_TRUE(gl_pChinaStockMarket->CheckMarketReady());
    EXPECT_TRUE(gl_pChinaStockMarket->IsSystemReady());
    gl_pChinaStockMarket->SetSystemReady(false);
  }

  TEST_F(CChinaMarketTest, TestChangeCurrentStockToNextStock1) {
    EXPECT_EQ(gl_pChinaStockMarket->GetCurrentSelectedStockSet(), -1);
    gl_pChinaStockMarket->SetCurrentStock(gl_pChinaStockMarket->GetStock(0));
    gl_pChinaStockMarket->ChangeToNextStock();
    EXPECT_EQ(gl_pChinaStockMarket->GetCurrentStock()->GetOffset(), 1);
    gl_pChinaStockMarket->SetCurrentStockChanged(false);

    gl_pChinaStockMarket->SetCurrentSelectedPosition(0);
    gl_pChinaStockMarket->SetCurrentSelectedStockSet(-1);
  }

  TEST_F(CChinaMarketTest, TestChangeCurrentStockToNextStock2) {
    EXPECT_EQ(gl_pChinaStockMarket->GetCurrentSelectedStockSet(), -1);
    gl_pChinaStockMarket->SetCurrentSelectedStockSet(10); // 10、11、12股票集中有股票
    EXPECT_TRUE(gl_pChinaStockMarket->GetCurrentSelectedStock() != nullptr);
    gl_pChinaStockMarket->SetCurrentStock(gl_pChinaStockMarket->GetCurrentSelectedStock());

    gl_pChinaStockMarket->ChangeToPrevStock();
    gl_pChinaStockMarket->ChangeToNextStock();
    EXPECT_EQ(gl_pChinaStockMarket->GetCurrentSelectedPosition(), 0);
    gl_pChinaStockMarket->ChangeToNextStock();
    EXPECT_EQ(gl_pChinaStockMarket->GetCurrentSelectedPosition(), 1);
    gl_pChinaStockMarket->ChangeToPrevStock();
    gl_pChinaStockMarket->ChangeToPrevStock();
    EXPECT_EQ(gl_pChinaStockMarket->GetCurrentStockSetSize(), gl_pChinaStockMarket->GetCurrentSelectedPosition() + 1);
    gl_pChinaStockMarket->SetCurrentStockChanged(false);

    gl_pChinaStockMarket->SetCurrentSelectedPosition(0);
    gl_pChinaStockMarket->SetCurrentSelectedStockSet(-1);
  }

  TEST_F(CChinaMarketTest, TestChangeCurrentStockToPrevStock1) {
    EXPECT_EQ(gl_pChinaStockMarket->GetCurrentSelectedStockSet(), -1);
    gl_pChinaStockMarket->SetCurrentStock(gl_pChinaStockMarket->GetStock(1)); // 选取A股指数
    gl_pChinaStockMarket->ChangeToPrevStock();
    EXPECT_EQ(gl_pChinaStockMarket->GetCurrentStock()->GetOffset(), 0) << _T("上一个是上证指数");
    gl_pChinaStockMarket->ChangeToPrevStock();
    EXPECT_EQ(gl_pChinaStockMarket->GetCurrentStock()->GetOffset(), gl_pChinaStockMarket->GetTotalStock() - 1) << _T("上证指数前的为空，然后就转到最后面的中证煤炭了");
    gl_pChinaStockMarket->SetCurrentStockChanged(false);
    gl_pChinaStockMarket->SetCurrentSelectedPosition(0);
  }

  TEST_F(CChinaMarketTest, TestChangeCurrentStockToPrevStock2) {
    EXPECT_EQ(gl_pChinaStockMarket->GetCurrentSelectedStockSet(), -1);
    gl_pChinaStockMarket->SetCurrentSelectedStockSet(10); // 10、11、12股票集中有股票
    EXPECT_TRUE(gl_pChinaStockMarket->GetCurrentSelectedStock() != nullptr);
    gl_pChinaStockMarket->SetCurrentStock(gl_pChinaStockMarket->GetCurrentSelectedStock());

    gl_pChinaStockMarket->ChangeToNextStock();
    EXPECT_EQ(gl_pChinaStockMarket->GetCurrentSelectedPosition(), 1);
    gl_pChinaStockMarket->ChangeToPrevStock();
    EXPECT_EQ(gl_pChinaStockMarket->GetCurrentSelectedPosition(), 0);
    gl_pChinaStockMarket->ChangeToPrevStock();
    EXPECT_EQ(gl_pChinaStockMarket->GetCurrentSelectedPosition() + 1, gl_pChinaStockMarket->GetCurrentStockSetSize());
    gl_pChinaStockMarket->SetCurrentStockChanged(false);

    gl_pChinaStockMarket->SetCurrentSelectedPosition(0);
    gl_pChinaStockMarket->SetCurrentSelectedStockSet(-1);
  }

  TEST_F(CChinaMarketTest, TestChangeToPrevStockSet) {
    EXPECT_EQ(gl_pChinaStockMarket->GetCurrentSelectedStockSet(), -1);
    gl_pChinaStockMarket->ChangeToPrevStockSet();
    EXPECT_EQ(gl_pChinaStockMarket->GetCurrentSelectedStockSet(), 19);
    gl_pChinaStockMarket->ChangeToPrevStockSet();
    EXPECT_EQ(gl_pChinaStockMarket->GetCurrentSelectedStockSet(), 18);
    gl_pChinaStockMarket->ChangeToPrevStockSet();
    EXPECT_EQ(gl_pChinaStockMarket->GetCurrentSelectedStockSet(), 12);
    gl_pChinaStockMarket->ChangeToPrevStockSet();
    EXPECT_EQ(gl_pChinaStockMarket->GetCurrentSelectedStockSet(), 11);
    gl_pChinaStockMarket->ChangeToPrevStockSet();
    EXPECT_EQ(gl_pChinaStockMarket->GetCurrentSelectedStockSet(), 10);

    gl_pChinaStockMarket->SetCurrentSelectedStockSet(-1);
  }

  TEST_F(CChinaMarketTest, TestChangeToNextStockSet) {
    EXPECT_EQ(gl_pChinaStockMarket->GetCurrentSelectedStockSet(), -1);
    gl_pChinaStockMarket->ChangeToNextStockSet();
    EXPECT_EQ(gl_pChinaStockMarket->GetCurrentSelectedStockSet(), 10);
    gl_pChinaStockMarket->ChangeToNextStockSet();
    EXPECT_EQ(gl_pChinaStockMarket->GetCurrentSelectedStockSet(), 11);
    gl_pChinaStockMarket->ChangeToNextStockSet();
    EXPECT_EQ(gl_pChinaStockMarket->GetCurrentSelectedStockSet(), 12);
    gl_pChinaStockMarket->ChangeToNextStockSet();
    EXPECT_EQ(gl_pChinaStockMarket->GetCurrentSelectedStockSet(), 18);
    gl_pChinaStockMarket->ChangeToNextStockSet();
    EXPECT_EQ(gl_pChinaStockMarket->GetCurrentSelectedStockSet(), 19);

    gl_pChinaStockMarket->SetCurrentSelectedStockSet(-1);
  }

  TEST_F(CChinaMarketTest, TestIsTotalStockSetSelected) {
    EXPECT_EQ(gl_pChinaStockMarket->GetCurrentSelectedStockSet(), -1);
    EXPECT_TRUE(gl_pChinaStockMarket->IsTotalStockSetSelected());
    gl_pChinaStockMarket->SetCurrentSelectedStockSet(0);
    EXPECT_FALSE(gl_pChinaStockMarket->IsTotalStockSetSelected());

    gl_pChinaStockMarket->SetCurrentSelectedStockSet(-1);
  }

  TEST_F(CChinaMarketTest, TestSetUsingSinaRTDataServer) {
    EXPECT_TRUE(gl_pChinaStockMarket->IsUsingSinaRTDataServer());
    EXPECT_FALSE(gl_pChinaStockMarket->IsUsingNeteaseRTDataServer());
    gl_pChinaStockMarket->SetUsingNeteaseRTDataServer();
    EXPECT_FALSE(gl_pChinaStockMarket->IsUsingSinaRTDataServer());
    EXPECT_TRUE(gl_pChinaStockMarket->IsUsingNeteaseRTDataServer());
    gl_pChinaStockMarket->SetUsingSinaRTDataServer();
    EXPECT_TRUE(gl_pChinaStockMarket->IsUsingSinaRTDataServer());
    EXPECT_FALSE(gl_pChinaStockMarket->IsUsingNeteaseRTDataServer());
  }

  TEST_F(CChinaMarketTest, TestIsUsingNeteaseRTDataReceiver) {
    EXPECT_TRUE(gl_pChinaStockMarket->IsUsingNeteaseRTDataReceiver());
    gl_pChinaStockMarket->SetUsingNeteaseRTDataReceiver(false);
    EXPECT_FALSE(gl_pChinaStockMarket->IsUsingNeteaseRTDataReceiver());
    gl_pChinaStockMarket->SetUsingNeteaseRTDataReceiver(true);
    EXPECT_TRUE(gl_pChinaStockMarket->IsUsingNeteaseRTDataReceiver());
  }

  TEST_F(CChinaMarketTest, TestIsUsingTengxunRTDataReceiver) {
    EXPECT_TRUE(gl_pChinaStockMarket->IsUsingTengxunRTDataReceiver());
    gl_pChinaStockMarket->SetUsingTengxunRTDataReceiver(false);
    EXPECT_FALSE(gl_pChinaStockMarket->IsUsingTengxunRTDataReceiver());
    gl_pChinaStockMarket->SetUsingTengxunRTDataReceiver(true);
    EXPECT_TRUE(gl_pChinaStockMarket->IsUsingTengxunRTDataReceiver());
  }

  TEST_F(CChinaMarketTest, TestGetCountDownTengxunNumber) {
    EXPECT_EQ(gl_pChinaStockMarket->GetCountDownTengxunNumber(), 5);
    gl_pChinaStockMarket->SetCountDownTengxunNumber(100);
    EXPECT_EQ(gl_pChinaStockMarket->GetCountDownTengxunNumber(), 100);
    gl_pChinaStockMarket->SetCountDownTengxunNumber(5);
  }

  TEST_F(CChinaMarketTest, TestGetUpDownRate) {
    EXPECT_DOUBLE_EQ(gl_pChinaStockMarket->GetUpDownRate(_T("10.0"), _T("0.0009")), 0.0) << _T("LastClose小于0.001时返回0");
    EXPECT_DOUBLE_EQ(gl_pChinaStockMarket->GetUpDownRate(_T("11.0"), _T("10.0")), 0.1);
    EXPECT_DOUBLE_EQ(gl_pChinaStockMarket->GetUpDownRate(_T("10.5"), _T("10.0")), 0.05);
    EXPECT_DOUBLE_EQ(gl_pChinaStockMarket->GetUpDownRate(_T("9.0"), _T("10.0")), -0.1);
    EXPECT_DOUBLE_EQ(gl_pChinaStockMarket->GetUpDownRate(_T("11.11"), _T("10.0")), 0.0) << _T("大于0.11时返回0");
    EXPECT_DOUBLE_EQ(gl_pChinaStockMarket->GetUpDownRate(_T("8.89"), _T("10.0")), 0.0) << _T("小于-0.11时返回0");
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

    gl_pChinaStockMarket->SetRSStartDate(20200101);
    gl_pChinaStockMarket->SetRSEndDate(20200202);
    gl_pChinaStockMarket->SetLastLoginDate(gl_pChinaStockMarket->GetFormatedMarketDate());
    gl_pChinaStockMarket->SetUpdatedDateFor10DaysRS1(19990101);
    gl_pChinaStockMarket->SetUpdatedDateFor10DaysRS2(19990202);

    gl_pChinaStockMarket->UpdateOptionDB();

    gl_pChinaStockMarket->SetRSStartDate(1);
    gl_pChinaStockMarket->SetRSEndDate(1);
    gl_pChinaStockMarket->SetLastLoginDate(1);
    gl_pChinaStockMarket->SetUpdatedDateFor10DaysRS1(1);
    gl_pChinaStockMarket->SetUpdatedDateFor10DaysRS2(1);

    gl_pChinaStockMarket->LoadOptionDB();

    EXPECT_EQ(gl_pChinaStockMarket->GetRSStartDate(), 20200101);
    EXPECT_EQ(gl_pChinaStockMarket->GetRSEndDate(), 20200202);
    EXPECT_EQ(gl_pChinaStockMarket->GetLastLoginDate(), gl_pChinaStockMarket->GetFormatedMarketDate());
    EXPECT_EQ(gl_pChinaStockMarket->GetUpdatedDateFor10DaysRS1(), 19990101);
    EXPECT_FALSE(gl_pChinaStockMarket->IsChoiced10RSStrong1StockSet());
    EXPECT_EQ(gl_pChinaStockMarket->GetUpdatedDateFor10DaysRS2(), 19990202);
    EXPECT_FALSE(gl_pChinaStockMarket->IsChoiced10RSStrong2StockSet());

    gl_pChinaStockMarket->SetRSStartDate(20100101);
    gl_pChinaStockMarket->SetRSEndDate(20100202);
    gl_pChinaStockMarket->SetLastLoginDate(20200303);
    gl_pChinaStockMarket->SetUpdatedDateFor10DaysRS1(19980101);
    gl_pChinaStockMarket->SetUpdatedDateFor10DaysRS2(19980202);

    gl_pChinaStockMarket->UpdateOptionDB();

    gl_pChinaStockMarket->SetRSStartDate(1);
    gl_pChinaStockMarket->SetRSEndDate(1);
    gl_pChinaStockMarket->SetLastLoginDate(1);
    gl_pChinaStockMarket->SetUpdatedDateFor10DaysRS1(1);
    gl_pChinaStockMarket->SetUpdatedDateFor10DaysRS2(1);

    gl_pChinaStockMarket->LoadOptionDB();

    EXPECT_EQ(gl_pChinaStockMarket->GetRSStartDate(), 20100101);
    EXPECT_EQ(gl_pChinaStockMarket->GetRSEndDate(), 20100202);
    EXPECT_EQ(gl_pChinaStockMarket->GetLastLoginDate(), gl_pChinaStockMarket->GetFormatedMarketDate()) << _T("永远是当前日期\n");
    EXPECT_EQ(gl_pChinaStockMarket->GetUpdatedDateFor10DaysRS1(), 19980101);
    EXPECT_FALSE(gl_pChinaStockMarket->IsChoiced10RSStrong1StockSet());
    EXPECT_EQ(gl_pChinaStockMarket->GetUpdatedDateFor10DaysRS2(), 19980202);
    EXPECT_FALSE(gl_pChinaStockMarket->IsChoiced10RSStrong2StockSet());

    setOption.Open();
    setOption.m_pDatabase->BeginTrans();
    while (!setOption.IsEOF()) {
      setOption.Delete();
      setOption.MoveNext();
    }
    setOption.m_pDatabase->CommitTrans();
    setOption.Close();
    gl_pChinaStockMarket->LoadOptionDB();

    EXPECT_EQ(gl_pChinaStockMarket->GetRSStartDate(), __CHINA_MARKET_BEGIN_DATE__);
    EXPECT_EQ(gl_pChinaStockMarket->GetRSEndDate(), __CHINA_MARKET_BEGIN_DATE__);
    EXPECT_EQ(gl_pChinaStockMarket->GetLastLoginDate(), __CHINA_MARKET_BEGIN_DATE__);
    EXPECT_EQ(gl_pChinaStockMarket->GetUpdatedDateFor10DaysRS1(), __CHINA_MARKET_BEGIN_DATE__);
    EXPECT_FALSE(gl_pChinaStockMarket->IsChoiced10RSStrong1StockSet());
    EXPECT_EQ(gl_pChinaStockMarket->GetUpdatedDateFor10DaysRS2(), __CHINA_MARKET_BEGIN_DATE__);
    EXPECT_FALSE(gl_pChinaStockMarket->IsChoiced10RSStrong2StockSet());
  }

  TEST_F(CChinaMarketTest, TestTooManyStockNeedUpdated) {
    EXPECT_FALSE(gl_pChinaStockMarket->TooManyStocksNeedUpdated());
    gl_pChinaStockMarket->SetStockNeedUpdated(1000);
    EXPECT_FALSE(gl_pChinaStockMarket->TooManyStocksNeedUpdated());
    gl_pChinaStockMarket->SetStockNeedUpdated(1001);
    EXPECT_TRUE(gl_pChinaStockMarket->TooManyStocksNeedUpdated());
    gl_pChinaStockMarket->SetStockNeedUpdated(0);
    EXPECT_FALSE(gl_pChinaStockMarket->TooManyStocksNeedUpdated());
  }

  TEST_F(CChinaMarketTest, TestDeleteDayLineBasicInfo) {
    char buffer[20];
    CString strDate;

    CSetDayLineBasicInfo setDayLine, setDayLine2;
    CDayLinePtr pDayLine = make_shared<CDayLine>();

    pDayLine->SetSymbol(_T("600000.SS"));
    pDayLine->SetDate(19900101);

    _ltoa_s(19900101, buffer, 10);
    strDate = buffer;
    setDayLine.m_strFilter = _T("[Date] =");
    setDayLine.m_strFilter += strDate;
    setDayLine.Open();
    setDayLine.m_pDatabase->BeginTrans();
    pDayLine->AppendChinaMarketData(&setDayLine);
    setDayLine.m_pDatabase->CommitTrans();
    setDayLine.Close();

    setDayLine.m_strFilter = _T("[Date] =");
    setDayLine.m_strFilter += strDate;
    setDayLine.Open();
    EXPECT_FALSE(setDayLine.IsEOF());
    setDayLine.Close();

    gl_pChinaStockMarket->DeleteDayLineBasicInfo(19900101);

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

    pDayLine->SetSymbol(_T("600000.SS"));
    pDayLine->SetDate(19900101);

    _ltoa_s(19900101, buffer, 10);
    strDate = buffer;
    setDayLine.m_strFilter = _T("[ID] = 1");
    setDayLine.Open();
    setDayLine.m_pDatabase->BeginTrans();
    setDayLine.AddNew();
    setDayLine.m_Symbol = _T("600000.SS");
    setDayLine.m_Date = 19900101;
    setDayLine.Update();
    setDayLine.m_pDatabase->CommitTrans();
    setDayLine.Close();

    setDayLine.m_strFilter = _T("[Date] =");
    setDayLine.m_strFilter += strDate;
    setDayLine.Open();
    EXPECT_FALSE(setDayLine.IsEOF());
    setDayLine.Close();

    gl_pChinaStockMarket->DeleteDayLineExtendInfo(19900101);

    setDayLine2.m_strFilter = _T("[Date] =");
    setDayLine2.m_strFilter += strDate;
    setDayLine2.Open();
    EXPECT_TRUE(setDayLine2.IsEOF());
    setDayLine2.Close();
  }

  TEST_F(CChinaMarketTest, TestDeleteCurrentWeekLine) {
    CSetWeekLineInfo setCurrentWeekLine, setCurrentWeekLine2;
    CWeekLinePtr pWeekLine = make_shared<CWeekLine>();

    pWeekLine->SetSymbol(_T("600000.SS"));
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

    gl_pChinaStockMarket->DeleteCurrentWeekWeekLine();

    setCurrentWeekLine2.Open();
    EXPECT_TRUE(setCurrentWeekLine2.IsEOF());
    setCurrentWeekLine2.Close();
  }

  TEST_F(CChinaMarketTest, TestSaveLoadCurrentWeekLine) {
    CSetWeekLineInfo setCurrentWeekLine, setCurrentWeekLine2;
    CWeekLinePtr pWeekLine = make_shared<CWeekLine>();
    CWeekLineContainer weekLineContainer, weekLineContainer2;

    pWeekLine->SetSymbol(_T("600000.SS"));
    pWeekLine->SetDate(GetCurrentMonday(20200101));
    weekLineContainer.StoreData(pWeekLine);

    gl_pChinaStockMarket->DeleteCurrentWeekWeekLine();
    gl_pChinaStockMarket->SaveCurrentWeekLine(weekLineContainer);

    gl_pChinaStockMarket->LoadCurrentWeekLine(weekLineContainer2);
    pWeekLine = weekLineContainer2.GetData(0);
    EXPECT_STREQ(pWeekLine->GetSymbol(), _T("600000.SS"));
    EXPECT_EQ(pWeekLine->GetFormatedMarketDate(), 20191230) << "20200101之前的星期一";

    gl_pChinaStockMarket->DeleteCurrentWeekWeekLine();
  }

  TEST_F(CChinaMarketTest, TestLoadDayLine) {
    CDayLineContainer dayLineContainer;
    long lDate = GetCurrentMonday(20200101);

    gl_pChinaStockMarket->LoadDayLine(dayLineContainer, lDate);

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
    setDayLineBasicInfo.m_strSort = _T("[Symbol]");
    setDayLineBasicInfo.m_strFilter = _T("[Date] =");
    setDayLineBasicInfo.m_strFilter += strDate;
    setDayLineBasicInfo.Open();
    while (!setDayLineBasicInfo.IsEOF()) {
      pDayLine = dayLineContainer.GetData(i++);
      EXPECT_STREQ(setDayLineBasicInfo.m_Symbol, pDayLine->GetSymbol());
      setDayLineBasicInfo.MoveNext();
    }
    EXPECT_EQ(i, dayLineContainer.GetDataSize());
    setDayLineBasicInfo.Close();
  }

  TEST_F(CChinaMarketTest, TestCreateStockCodeSet) {
    vector<CChinaStockHistoryDataPtr> vData;
    CChinaStockHistoryDataPtr pData = make_shared<CChinaStockHistoryData>();
    pData->SetSymbol(_T("600000.SS"));
    vData.resize(2);
    vData[0] = pData;
    pData = make_shared<CChinaStockHistoryData>();
    pData->SetSymbol(_T("sh600004"));
    vData[1] = pData;

    set<CString> setStockCode;
    gl_pChinaStockMarket->CreateStockCodeSet(setStockCode, &vData);

    EXPECT_TRUE(setStockCode.find(_T("600000.SS")) != setStockCode.end());
    EXPECT_TRUE(setStockCode.find(_T("sh600004")) != setStockCode.end());
    EXPECT_FALSE(setStockCode.find(_T("sh600001")) != setStockCode.end());
  }
}