#include"pch.h"
#include"globedef.h"

#include"ChinaStock.h"
#include"ChinaMarket.h"
#include"WebRTData.h"

#include"MockChinaStock.h"

using namespace testing;

namespace StockAnalysisTest {
  static CSinaRTWebInquiry m_SinaRTWebInquiry; // 新浪实时数据采集
  static CTengxunRTWebInquiry m_TengxunRTWebData; // 腾讯实时数据采集
  static CNeteaseDayLineWebInquiry m_NeteaseDayLineWebInquiry; // 网易日线历史数据

  class CChinaStockTest : public ::testing::Test
  {
  protected:
    static void SetUpTestSuite(void) {
      EXPECT_EQ(gl_pChinaStockMarket->GetCurrentStock(), nullptr) << gl_pChinaStockMarket->GetCurrentStock()->GetSymbol();
      EXPECT_FALSE(gl_pChinaStockMarket->IsCurrentStockChanged());
      EXPECT_FALSE(gl_pChinaStockMarket->IsMarketOpened());
    }
    static void TearDownTestSuite(void) {
      EXPECT_EQ(gl_pChinaStockMarket->GetCurrentStock(), nullptr) << gl_pChinaStockMarket->GetCurrentStock()->GetSymbol();
      EXPECT_FALSE(gl_pChinaStockMarket->IsCurrentStockChanged());
      EXPECT_FALSE(gl_pChinaStockMarket->IsMarketOpened());
    }
    virtual void SetUp(void) override {
      EXPECT_FALSE(gl_pChinaStockMarket->IsMarketOpened());
      ASSERT_FALSE(gl_fNormalMode);
      pStock = nullptr;
      gl_pChinaStockMarket->CalculateTime();
      while (gl_systemMessage.GetInformationDequeSize() > 0) gl_systemMessage.PopInformationMessage();
      while (gl_systemMessage.GetDayLineInfoDequeSize() > 0) gl_systemMessage.PopDayLineInfoMessage();
      while (gl_systemMessage.GetInnerSystemInformationDequeSize() > 0) gl_systemMessage.PopInnerSystemInformationMessage();
    }

    virtual void TearDown(void) override {
      // clearup
      EXPECT_FALSE(gl_pChinaStockMarket->IsMarketOpened());
      gl_pChinaStockMarket->CalculateTime();
      gl_pChinaStockMarket->SetUpdateOptionDB(false);
      while (gl_systemMessage.GetInformationDequeSize() > 0) gl_systemMessage.PopInformationMessage();
      while (gl_systemMessage.GetDayLineInfoDequeSize() > 0) gl_systemMessage.PopDayLineInfoMessage();
      while (gl_systemMessage.GetInnerSystemInformationDequeSize() > 0) gl_systemMessage.PopInnerSystemInformationMessage();
      if (pStock != nullptr) {
        pStock->SetDayLineDBUpdated(false);
        if (pStock->IsDayLineNeedProcess()) pStock->SetDayLineNeedProcess(false);
        if (pStock->IsDayLineNeedSaving()) pStock->SetDayLineNeedSaving(false);
        pStock = nullptr;
      }
    }

  protected:
    CChinaStockPtr pStock;
  };

  TEST_F(CChinaStockTest, TestInitialize) {
    CChinaStock stock;
    EXPECT_STREQ(stock.GetSymbol(), _T(""));
    EXPECT_STREQ(stock.GetStockName(), _T(""));
    EXPECT_EQ(stock.GetOffset(), -1);
    EXPECT_EQ(stock.GetDayLineStartDate(), __CHINA_MARKET_BEGIN_DATE__);
    EXPECT_EQ(stock.GetDayLineEndDate(), __CHINA_MARKET_BEGIN_DATE__);
    EXPECT_TRUE(stock.IsNotChecked());
    EXPECT_EQ(stock.GetTransactionTime(), 0);
    EXPECT_EQ(stock.GetLastClose(), 0);
    EXPECT_EQ(stock.GetOpen(), 0);
    EXPECT_EQ(stock.GetHigh(), 0);
    EXPECT_EQ(stock.GetLow(), 0);
    EXPECT_EQ(stock.GetNew(), 0);
    EXPECT_DOUBLE_EQ(stock.GetUpDownRate(), 0);
    EXPECT_EQ(stock.GetVolume(), 0);
    EXPECT_EQ(stock.GetAmount(), 0);
    for (int i = 0; i < 5; i++) {
      EXPECT_EQ(stock.GetVBuy(i), 0);
      EXPECT_EQ(stock.GetPBuy(i), 0);
      EXPECT_EQ(stock.GetVSell(i), 0);
      EXPECT_EQ(stock.GetPSell(i), 0);
    }
    EXPECT_DOUBLE_EQ(stock.GetRS(), 0);
    EXPECT_DOUBLE_EQ(stock.GetChangeHandRate(), 0);
    EXPECT_EQ(stock.GetTotalValue(), 0);
    EXPECT_EQ(stock.GetCurrentValue(), 0);
    stock.SetSymbol(_T("abcde"));
    stock.SetStockName(_T("dcba"));
    stock.SetOffset(1);
    stock.SetDayLineEndDate(20020202);
    stock.SetIPOStatus(0);
    EXPECT_STREQ(stock.GetSymbol(), _T("abcde"));
    EXPECT_STREQ(stock.GetStockName(), _T("dcba"));
    EXPECT_EQ(stock.GetOffset(), 1);
    EXPECT_EQ(stock.GetDayLineEndDate(), 20020202);
    stock.Reset();
    EXPECT_STREQ(stock.GetSymbol(), _T(""));
    EXPECT_STREQ(stock.GetStockName(), _T(""));
    EXPECT_EQ(stock.GetOffset(), -1);
    EXPECT_EQ(stock.GetDayLineEndDate(), __CHINA_MARKET_BEGIN_DATE__);
    EXPECT_TRUE(stock.IsNotChecked());
  }

  TEST_F(CChinaStockTest, TestGetStockName) {
    CChinaStock stock;
    EXPECT_STREQ(stock.GetStockName(), _T(""));
    stock.SetStockName(_T("浦发银行"));
    EXPECT_STREQ(stock.GetStockName(), _T("浦发银行"));
  }
  TEST_F(CChinaStockTest, TestGetOffset) {
    CChinaStock stock;
    EXPECT_EQ(stock.GetOffset(), -1);
    stock.SetOffset(10101);
    EXPECT_EQ(stock.GetOffset(), 10101);
  }

  TEST_F(CChinaStockTest, TestGetDayLineEndDate) {
    CChinaStock stock;
    EXPECT_EQ(stock.GetDayLineEndDate(), __CHINA_MARKET_BEGIN_DATE__);
    stock.SetDayLineEndDate(19980101);
    EXPECT_EQ(stock.GetDayLineEndDate(), 19980101);
  }

  TEST_F(CChinaStockTest, TestGetDayLineStartDate) {
    CChinaStock stock;
    EXPECT_EQ(stock.GetDayLineStartDate(), __CHINA_MARKET_BEGIN_DATE__);
    stock.SetDayLineStartDate(19980101);
    EXPECT_EQ(stock.GetDayLineStartDate(), 19980101);
  }

  TEST_F(CChinaStockTest, TestGetIPOStatus) {
    CChinaStock stock;
    EXPECT_TRUE(stock.IsNotChecked());
    stock.SetIPOStatus(255);
    EXPECT_EQ(stock.GetIPOStatus(), 255);
  }

  TEST_F(CChinaStockTest, TestIsNullStock) {
    CChinaStock stock;
    stock.SetIPOStatus(__STAKE_NULL__);
    EXPECT_TRUE(stock.IsNullStock());
    stock.SetIPOStatus(__STAKE_NOT_CHECKED__);
    EXPECT_FALSE(stock.IsNullStock());
  }

  TEST_F(CChinaStockTest, TestIsIPOed) {
    CChinaStock stock;
    stock.SetIPOStatus(__STAKE_IPOED__);
    EXPECT_TRUE(stock.IsIPOed());
    stock.SetIPOStatus(__STAKE_NOT_CHECKED__);
    EXPECT_FALSE(stock.IsIPOed());
  }

  TEST_F(CChinaStockTest, TestIsNotChecked) {
    CChinaStock stock;
    stock.SetIPOStatus(__STAKE_NOT_CHECKED__);
    EXPECT_TRUE(stock.IsNotChecked());
    stock.SetIPOStatus(__STAKE_DELISTED__);
    EXPECT_FALSE(stock.IsNotChecked());
  }

  TEST_F(CChinaStockTest, TestIsDelisted) {
    CChinaStock stock;
    stock.SetIPOStatus(__STAKE_DELISTED__);
    EXPECT_TRUE(stock.IsDelisted());
    stock.SetIPOStatus(__STAKE_NOT_CHECKED__);
    EXPECT_FALSE(stock.IsDelisted());
  }

  TEST_F(CChinaStockTest, TestGetHighLimit) {
    CChinaStock stock;
    EXPECT_EQ(stock.GetHighLimit(), 0);
    stock.SetHighLimit(101010);
    EXPECT_EQ(stock.GetHighLimit(), 101010);
  }

  TEST_F(CChinaStockTest, TestGetLowLimit) {
    CChinaStock stock;
    EXPECT_EQ(stock.GetLowLimit(), 0);
    stock.SetLowLimit(101010);
    EXPECT_EQ(stock.GetLowLimit(), 101010);
  }

  TEST_F(CChinaStockTest, TestGetLastSavedVolume) {
    CChinaStock stock;
    EXPECT_EQ(stock.GetLastSavedVolume(), 0);
    stock.SetLastSavedVolume(101010101010);
    EXPECT_EQ(stock.GetLastSavedVolume(), 101010101010);
  }

  TEST_F(CChinaStockTest, TestGetPBuy) {
    CChinaStock stock;
    for (int i = 0; i < 5; i++) {
      EXPECT_EQ(stock.GetPBuy(i), 0);
      stock.SetPBuy(i, 10101010);
      EXPECT_EQ(stock.GetPBuy(i), 10101010);
    }
  }

  TEST_F(CChinaStockTest, TestGetVBuy) {
    CChinaStock stock;
    for (int i = 0; i < 5; i++) {
      EXPECT_EQ(stock.GetVBuy(i), 0);
      stock.SetVBuy(i, 10101010);
      EXPECT_EQ(stock.GetVBuy(i), 10101010);
    }
  }

  TEST_F(CChinaStockTest, TestGetPSell) {
    CChinaStock stock;
    for (int i = 0; i < 5; i++) {
      EXPECT_EQ(stock.GetPSell(i), 0);
      stock.SetPSell(i, 10101010);
      EXPECT_EQ(stock.GetPSell(i), 10101010);
    }
  }

  TEST_F(CChinaStockTest, TestGetVSell) {
    CChinaStock stock;
    for (int i = 0; i < 5; i++) {
      EXPECT_EQ(stock.GetVSell(i), 0);
      stock.SetVSell(i, 10101010);
      EXPECT_EQ(stock.GetVSell(i), 10101010);
    }
  }

  TEST_F(CChinaStockTest, TestGetRS) {
    CChinaStock stock;
    EXPECT_DOUBLE_EQ(stock.GetRS(), 0.0);
    stock.SetRS(10101010.0);
    EXPECT_DOUBLE_EQ(stock.GetRS(), 10101010.0);
  }

  TEST_F(CChinaStockTest, TestGetRSIndex) {
    CChinaStock stock;
    EXPECT_DOUBLE_EQ(stock.GetRSIndex(), 0.0);
    stock.SetRSIndex(10101010.0);
    EXPECT_DOUBLE_EQ(stock.GetRSIndex(), 10101010.0);
  }

  TEST_F(CChinaStockTest, TestReset) {
    ASSERT_FALSE(gl_fNormalMode);
    CWebRTDataPtr pRTData = make_shared<CWebRTData>();

    CChinaStock stock;
    EXPECT_STREQ(stock.GetSymbol(), _T(""));
    EXPECT_STREQ(stock.GetStockName(), _T(""));
    EXPECT_EQ(stock.GetOffset(), -1);
    EXPECT_EQ(stock.GetDayLineEndDate(), __CHINA_MARKET_BEGIN_DATE__);
    EXPECT_TRUE(stock.IsNotChecked());
    stock.SetSymbol(_T("abcde"));
    stock.SetStockName(_T("dcba"));
    stock.SetOffset(1);
    stock.SetDayLineEndDate(20020202);
    stock.SetIPOStatus(0);
    EXPECT_STREQ(stock.GetSymbol(), _T("abcde"));
    EXPECT_STREQ(stock.GetStockName(), _T("dcba"));
    EXPECT_EQ(stock.GetOffset(), 1);
    EXPECT_EQ(stock.GetDayLineEndDate(), 20020202);
    pRTData->SetTransactionTime(12345);
    pRTData->SetLastClose(10101010);
    pRTData->SetOpen(20202020);
    pRTData->SetHigh(30303030);
    pRTData->SetLow(40404040);
    pRTData->SetNew(50505050);
    pRTData->SetVolume(6060606060606060);
    pRTData->SetAmount(7070707070707070);
    pRTData->SetCurrentValue(1010);
    pRTData->SetTotalValue(2020);
    for (int i = 0; i < 5; i++) {
      pRTData->SetPBuy(i, i * 12345);
      pRTData->SetVBuy(i, i * 23456);
      pRTData->SetPSell(i, i * 34567);
      pRTData->SetVSell(i, i * 45678);
    }
    stock.UpdateStatus(pRTData);
    stock.Reset();
    EXPECT_TRUE(stock.IsNotChecked());
    EXPECT_STREQ(stock.GetSymbol(), _T(""));
    EXPECT_STREQ(stock.GetStockName(), _T(""));
    EXPECT_EQ(stock.GetOffset(), -1);
    EXPECT_EQ(stock.GetDayLineEndDate(), __CHINA_MARKET_BEGIN_DATE__);
    EXPECT_TRUE(stock.IsNotChecked());

    EXPECT_EQ(stock.GetLastClose(), 0);
    EXPECT_EQ(stock.GetOpen(), 0);
    EXPECT_EQ(stock.GetHigh(), 0);
    EXPECT_EQ(stock.GetLow(), 0);
    EXPECT_EQ(stock.GetNew(), 0);
    EXPECT_EQ(stock.GetVolume(), 0);
    EXPECT_EQ(stock.GetAmount(), 0);
    EXPECT_EQ(stock.GetUpDown(), 0);
    EXPECT_DOUBLE_EQ(stock.GetUpDownRate(), 0.0);
    EXPECT_DOUBLE_EQ(stock.GetChangeHandRate(), 0.0);
    EXPECT_EQ(stock.GetTotalValue(), 0);
    EXPECT_EQ(stock.GetCurrentValue(), 0);
    for (int i = 0; i < 5; i++) {
      EXPECT_EQ(stock.GetPBuy(i), 0);
      EXPECT_EQ(stock.GetVBuy(i), 0);
      EXPECT_EQ(stock.GetPSell(i), 0);
      EXPECT_EQ(stock.GetVSell(i), 0);
    }
  }

  TEST_F(CChinaStockTest, TestUpdateStatus) {
    ASSERT_FALSE(gl_fNormalMode);
    CChinaStock stock;
    EXPECT_STREQ(stock.GetSymbol(), _T(""));
    EXPECT_STREQ(stock.GetStockName(), _T(""));
    EXPECT_EQ(stock.GetOffset(), -1);
    EXPECT_EQ(stock.GetDayLineEndDate(), __CHINA_MARKET_BEGIN_DATE__);
    EXPECT_TRUE(stock.IsNotChecked());
    stock.SetSymbol(_T("abcde"));
    stock.SetStockName(_T("dcba"));
    stock.SetOffset(1);
    stock.SetDayLineEndDate(20020202);
    stock.SetIPOStatus(0);
    EXPECT_STREQ(stock.GetSymbol(), _T("abcde"));
    EXPECT_STREQ(stock.GetStockName(), _T("dcba"));
    EXPECT_EQ(stock.GetOffset(), 1);
    EXPECT_EQ(stock.GetDayLineEndDate(), 20020202);
    stock.Reset();
    EXPECT_STREQ(stock.GetSymbol(), _T(""));
    EXPECT_STREQ(stock.GetStockName(), _T(""));
    EXPECT_EQ(stock.GetOffset(), -1);
    EXPECT_EQ(stock.GetDayLineEndDate(), __CHINA_MARKET_BEGIN_DATE__);
    EXPECT_TRUE(stock.IsNotChecked());

    CWebRTDataPtr  pRTData = make_shared<CWebRTData>();
    time_t tt;
    time(&tt);
    long lDateSource = FormatToDate(tt);

    pRTData->SetTransactionTime(tt);
    pRTData->SetLastClose(10101010);
    pRTData->SetOpen(20202020);
    pRTData->SetHigh(30303030);
    pRTData->SetLow(40404040);
    pRTData->SetNew(50505050);
    pRTData->SetVolume(6060606060606060);
    pRTData->SetAmount(7070707070707070);
    pRTData->SetTotalValue(3);
    pRTData->SetCurrentValue(4);
    for (int i = 0; i < 5; i++) {
      pRTData->SetPBuy(i, i * 12345);
      pRTData->SetVBuy(i, i * 23456);
      pRTData->SetPSell(i, i * 34567);
      pRTData->SetVSell(i, i * 45678);
    }
    stock.UpdateStatus(pRTData);
    long lDateDest = FormatToDate(stock.GetTransactionTime());
    EXPECT_EQ(lDateSource, lDateDest);
    EXPECT_STREQ(stock.GetSymbol(), _T(""));
    EXPECT_STREQ(stock.GetStockName(), _T(""));
    EXPECT_EQ(stock.GetOffset(), -1);
    EXPECT_EQ(stock.GetDayLineEndDate(), __CHINA_MARKET_BEGIN_DATE__);
    EXPECT_TRUE(stock.IsNotChecked());

    EXPECT_EQ(stock.GetLastClose(), 10101010);
    EXPECT_EQ(stock.GetOpen(), 20202020);
    EXPECT_EQ(stock.GetHigh(), 30303030);
    EXPECT_EQ(stock.GetLow(), 40404040);
    EXPECT_EQ(stock.GetNew(), 50505050);
    EXPECT_EQ(stock.GetVolume(), 6060606060606060);
    EXPECT_EQ(stock.GetAmount(), 7070707070707070);
    EXPECT_EQ(stock.GetUpDown(), pRTData->GetNew() - pRTData->GetLastClose());
    EXPECT_DOUBLE_EQ(stock.GetUpDownRate(), (double)(pRTData->GetNew() - pRTData->GetLastClose()) * 100 / pRTData->GetLastClose());
    if (pRTData->GetTotalValue() != 0) {
      EXPECT_DOUBLE_EQ(stock.GetChangeHandRate(), (double)(pRTData->GetAmount()) * 100 / pRTData->GetTotalValue());
    }
    else {
      EXPECT_DOUBLE_EQ(stock.GetChangeHandRate(), 0.0);
    }
    EXPECT_EQ(stock.GetTotalValue(), pRTData->GetTotalValue());
    EXPECT_EQ(stock.GetCurrentValue(), pRTData->GetCurrentValue());
    for (int i = 0; i < 5; i++) {
      EXPECT_EQ(stock.GetPBuy(i), i * 12345);
      EXPECT_EQ(stock.GetVBuy(i), i * 23456);
      EXPECT_EQ(stock.GetPSell(i), i * 34567);
      EXPECT_EQ(stock.GetVSell(i), i * 45678);
    }
    pRTData->SetTotalValue(0);
    pRTData->SetCurrentValue(0);
    pRTData->SetHigh(0);
    stock.UpdateStatus(pRTData);
    EXPECT_EQ(stock.GetCurrentValue(), 4);
    EXPECT_EQ(stock.GetTotalValue(), 3);
    EXPECT_EQ(stock.GetHigh(), 0);
    pRTData->SetTotalValue(1);
    pRTData->SetCurrentValue(2);
    stock.UpdateStatus(pRTData);
    EXPECT_EQ(stock.GetCurrentValue(), 2);
    EXPECT_EQ(stock.GetTotalValue(), 1);
  }

  TEST_F(CChinaStockTest, TestSaveTodayInfo) {
    CSetDayLineBasicInfo setDayLineBasicInfo;
    CChinaStock stock;
    long lDate = 21091101;
    time_t tt = FormatToTTime(lDate);
    CString strDate = _T("21091101"); // 最好设置此日期为未来，以防止误操作实际数据库

    stock.SetSymbol(_T("600000.SS"));
    stock.SetStockName("浦东发展");
    stock.SetTransactionTime(tt); // 此处设置固定的日期，而存储时使用的是当前日期，故而需要与gl_systemTime.FormatToDate()作比较
    stock.SetLastClose(101010);
    stock.SetOpen(202020);
    stock.SetHigh(303030);
    stock.SetLow(404040);
    stock.SetNew(505050);
    stock.SetVolume(6060606060);
    stock.SetAmount(70707070707);
    stock.SetUpDown(stock.GetOpen() - stock.GetNew());
    stock.SetUpDownRate((double)stock.GetUpDown() / stock.GetLastClose() * 100);
    stock.SetCurrentValue(8080808080808);
    stock.SetTotalValue(9090909090909);

    ASSERT(!gl_fNormalMode);
    ASSERT(gl_fTestMode);

    setDayLineBasicInfo.m_strFilter = _T("[Date] =");
    setDayLineBasicInfo.m_strFilter += strDate;
    setDayLineBasicInfo.Open();
    setDayLineBasicInfo.m_pDatabase->BeginTrans();
    while (!setDayLineBasicInfo.IsEOF()) {
      setDayLineBasicInfo.Delete();
      setDayLineBasicInfo.MoveNext();
    }
    setDayLineBasicInfo.m_pDatabase->CommitTrans();
    setDayLineBasicInfo.Close();
    setDayLineBasicInfo.m_strFilter = _T("[ID] = 1"); // 存储新数据时无需查询旧数据，故而使用最简单的主索引ID
    setDayLineBasicInfo.Open();
    setDayLineBasicInfo.m_pDatabase->BeginTrans();
    setDayLineBasicInfo.AddNew();
    stock.SaveTodayBasicInfo(&setDayLineBasicInfo);
    setDayLineBasicInfo.Update();
    setDayLineBasicInfo.m_pDatabase->CommitTrans();
    setDayLineBasicInfo.Close();
  }

  TEST_F(CChinaStockTest, TestGetAttackBuyAmount) {
    CChinaStock stock;
    EXPECT_EQ(stock.GetAttackBuyAmount(), 0);
    stock.SetAttackBuyAmount(10101010);
    EXPECT_EQ(stock.GetAttackBuyAmount(), 10101010);
  }

  TEST_F(CChinaStockTest, TestGetAttackSellAmount) {
    CChinaStock stock;
    EXPECT_EQ(stock.GetAttackSellAmount(), 0);
    stock.SetAttackSellAmount(10101010);
    EXPECT_EQ(stock.GetAttackSellAmount(), 10101010);
  }

  TEST_F(CChinaStockTest, TestGetOrdinaryBuyVolume) {
    CChinaStock stock;
    EXPECT_EQ(stock.GetOrdinaryBuyVolume(), 0);
    stock.SetOrdinaryBuyVolume(10101010);
    EXPECT_EQ(stock.GetOrdinaryBuyVolume(), 10101010);
  }

  TEST_F(CChinaStockTest, TestGetOrdinarySellVolume) {
    CChinaStock stock;
    EXPECT_EQ(stock.GetOrdinarySellVolume(), 0);
    stock.SetOrdinarySellVolume(10101010);
    EXPECT_EQ(stock.GetOrdinarySellVolume(), 10101010);
  }

  TEST_F(CChinaStockTest, TestGetAttackBuyVolume) {
    CChinaStock stock;
    EXPECT_EQ(stock.GetAttackBuyVolume(), 0);
    stock.SetAttackBuyVolume(10101010);
    EXPECT_EQ(stock.GetAttackBuyVolume(), 10101010);
  }

  TEST_F(CChinaStockTest, TestGetStrongBuyVolume) {
    CChinaStock stock;
    EXPECT_EQ(stock.GetStrongBuyVolume(), 0);
    stock.SetStrongBuyVolume(10101010);
    EXPECT_EQ(stock.GetStrongBuyVolume(), 10101010);
  }

  TEST_F(CChinaStockTest, TestGetCurrentAttackBuy) {
    CChinaStock stock;
    EXPECT_EQ(stock.GetCurrentAttackBuy(), 0);
    stock.SetCurrentAttackBuy(10101010);
    EXPECT_EQ(stock.GetCurrentAttackBuy(), 10101010);
  }

  TEST_F(CChinaStockTest, TestGetCurrentStrongBuy) {
    CChinaStock stock;
    EXPECT_EQ(stock.GetCurrentStrongBuy(), 0);
    stock.SetCurrentStrongBuy(10101010);
    EXPECT_EQ(stock.GetCurrentStrongBuy(), 10101010);
  }

  TEST_F(CChinaStockTest, TestGetAttackSellVolume) {
    CChinaStock stock;
    EXPECT_EQ(stock.GetAttackSellVolume(), 0);
    stock.SetAttackSellVolume(10101010);
    EXPECT_EQ(stock.GetAttackSellVolume(), 10101010);
  }

  TEST_F(CChinaStockTest, TestGetStrongSellVolume) {
    CChinaStock stock;
    EXPECT_EQ(stock.GetStrongSellVolume(), 0);
    stock.SetStrongSellVolume(10101010);
    EXPECT_EQ(stock.GetStrongSellVolume(), 10101010);
  }

  TEST_F(CChinaStockTest, TestGetCurrentAttackSell) {
    CChinaStock stock;
    EXPECT_EQ(stock.GetCurrentAttackSell(), 0);
    stock.SetCurrentAttackSell(10101010);
    EXPECT_EQ(stock.GetCurrentAttackSell(), 10101010);
  }

  TEST_F(CChinaStockTest, TestGetCurrentStrongSell) {
    CChinaStock stock;
    EXPECT_EQ(stock.GetCurrentStrongSell(), 0);
    stock.SetCurrentStrongSell(10101010);
    EXPECT_EQ(stock.GetCurrentStrongSell(), 10101010);
  }

  TEST_F(CChinaStockTest, TestGetUnknownVolume) {
    CChinaStock stock;
    EXPECT_EQ(stock.GetUnknownVolume(), 0);
    stock.SetUnknownVolume(10101010);
    EXPECT_EQ(stock.GetUnknownVolume(), 10101010);
  }

  TEST_F(CChinaStockTest, TestGetCurrentUnknown) {
    CChinaStock stock;
    EXPECT_EQ(stock.GetCurrentUnknown(), 0);
    stock.SetCurrentUnknown(10101010);
    EXPECT_EQ(stock.GetCurrentUnknown(), 10101010);
  }

  TEST_F(CChinaStockTest, TestGetCanceledBuyVolume) {
    CChinaStock stock;
    EXPECT_EQ(stock.GetCanceledBuyVolume(), 0);
    stock.SetCanceledBuyVolume(10101010);
    EXPECT_EQ(stock.GetCanceledBuyVolume(), 10101010);
  }
  TEST_F(CChinaStockTest, TestGetCanceledSellVolume) {
    CChinaStock stock;
    EXPECT_EQ(stock.GetCanceledSellVolume(), 0);
    stock.SetCanceledSellVolume(10101010);
    EXPECT_EQ(stock.GetCanceledSellVolume(), 10101010);
  }

  TEST_F(CChinaStockTest, TestReadOnlyFunctions) {
  }

  TEST_F(CChinaStockTest, TestIsSaveToChoicedStockDB) {
    CChinaStock stock;
    EXPECT_FALSE(stock.IsSaveToChoicedStockDB());
    stock.SetSaveToChoicedStockDB(true);
    EXPECT_TRUE(stock.IsSaveToChoicedStockDB());
    stock.SetSaveToChoicedStockDB(false);
    EXPECT_FALSE(stock.IsSaveToChoicedStockDB());
  }

  TEST_F(CChinaStockTest, TestIsChoiced) {
    CChinaStock stock;
    EXPECT_FALSE(stock.IsChoiced());
    stock.SetChoiced(true);
    EXPECT_TRUE(stock.IsChoiced());
    stock.SetChoiced(false);
    EXPECT_FALSE(stock.IsChoiced());
  }

  TEST_F(CChinaStockTest, TestIsDayLineDBUpdated) {
    CChinaStock stock;
    EXPECT_FALSE(stock.IsDayLineDBUpdated());
    stock.SetDayLineDBUpdated(true);
    EXPECT_TRUE(stock.IsDayLineDBUpdated());
    stock.SetDayLineDBUpdated(false);
    EXPECT_FALSE(stock.IsDayLineDBUpdated());
  }

  TEST_F(CChinaStockTest, TestIsDayLineLoaded) {
    CChinaStock stock;
    EXPECT_FALSE(stock.IsDayLineLoaded());
    stock.SetDayLineLoaded(true);
    EXPECT_TRUE(stock.IsDayLineLoaded());
    stock.SetDayLineLoaded(false);
    EXPECT_FALSE(stock.IsDayLineLoaded());
  }

  TEST_F(CChinaStockTest, TestHaveFirstRTData) {
    CChinaStock stock;
    EXPECT_FALSE(stock.HaveFirstRTData());
    stock.SetHavingFirstRTData(true);
    EXPECT_TRUE(stock.HaveFirstRTData());
    stock.SetHavingFirstRTData(false);
    EXPECT_TRUE(stock.HaveFirstRTData()); // 此标识设置后就永远为真了。
  }

  TEST_F(CChinaStockTest, TestIsRTDataCalculated) {
    CChinaStock stock;
    EXPECT_FALSE(stock.IsRTDataCalculated());
    stock.SetRTDataCalculated(true);
    EXPECT_TRUE(stock.IsRTDataCalculated());
    stock.SetRTDataCalculated(false);
    EXPECT_FALSE(stock.IsRTDataCalculated());
  }

  TEST_F(CChinaStockTest, TestIsRecordRTData) {
    CChinaStock stock;
    EXPECT_FALSE(stock.IsRecordRTData());
    stock.SetRecordRTData(true);
    EXPECT_TRUE(stock.IsRecordRTData());
    stock.SetRecordRTData(false);
    EXPECT_FALSE(stock.IsRecordRTData());
  }

  TEST_F(CChinaStockTest, TestIsStartCalculating) {
    CChinaStock stock;
    EXPECT_FALSE(stock.HaveFirstRTData());
    EXPECT_FALSE(stock.SetHavingFirstRTData(false)); // 不允许外部设置停止计算标识（内部可以）
    EXPECT_FALSE(stock.HaveFirstRTData());
    EXPECT_TRUE(stock.SetHavingFirstRTData(true));
    EXPECT_TRUE(stock.HaveFirstRTData());
    EXPECT_FALSE(stock.SetHavingFirstRTData(true));  // 不允许再次设置开始计算标识
    EXPECT_TRUE(stock.HaveFirstRTData());
  }

  TEST_F(CChinaStockTest, TestHaveNewDayLineData) {
    CChinaStock stock;
    EXPECT_FALSE(stock.HaveNewDayLineData());
    EXPECT_EQ(stock.GetDayLineSize(), 0);
    CDayLinePtr pDayLine = make_shared<CDayLine>();
    pDayLine->SetDate(20200101);
    stock.StoreDayLine(pDayLine);
    EXPECT_EQ(stock.GetDayLineSize(), 1);
    stock.SetDayLineEndDate(20200101);
    EXPECT_FALSE(stock.HaveNewDayLineData());
    stock.SetDayLineEndDate(20191231);
    EXPECT_TRUE(stock.HaveNewDayLineData());
    stock.UnloadDayLine();
    EXPECT_EQ(stock.GetDayLineSize(), 0);
  }

  TEST_F(CChinaStockTest, TestTransferNeteaseDayLineWebDataToBuffer) {
    CString str = _T("abcedfg\r\n");
    m_NeteaseDayLineWebInquiry.__TESTSetBuffer(str);
    CChinaStock stock;
    EXPECT_EQ(stock.GetDayLineBufferLength(), 0);
    stock.TransferNeteaseDayLineWebDataToBuffer(&m_NeteaseDayLineWebInquiry);
    EXPECT_EQ(stock.GetDayLineBufferLength(), str.GetLength());
    EXPECT_EQ(gl_pChinaStockMarket->GetDayLineNeedProcessNumber(), 0);
  }

  TEST_F(CChinaStockTest, TestTodayDataIsActive) {
    CChinaStock stock;
    EXPECT_FALSE(stock.IsTodayDataActive());
    stock.SetActive(true);
    EXPECT_FALSE(stock.IsTodayDataActive());
    stock.SetHigh(1010);
    EXPECT_TRUE(stock.IsTodayDataActive());
    stock.SetHigh(0);
    stock.SetLow(1);
    EXPECT_TRUE(stock.IsTodayDataActive());
    stock.SetLow(0);
    stock.SetAmount(10000);
    EXPECT_TRUE(stock.IsTodayDataActive());
    stock.SetAmount(0);
    stock.SetVolume(100);
    EXPECT_TRUE(stock.IsTodayDataActive());
    stock.SetActive(false);
    EXPECT_FALSE(stock.IsTodayDataActive());
  }

  TEST_F(CChinaStockTest, TestIsSaveStock) {
    pStock = gl_pChinaStockMarket->GetStock(1);
    CChinaStockPtr pStock2 = nullptr;
    EXPECT_FALSE(pStock->IsSameStock(pStock2));
    pStock2 = gl_pChinaStockMarket->GetStock(2);
    EXPECT_FALSE(pStock->IsSameStock(pStock2));
    pStock2 = gl_pChinaStockMarket->GetStock(1);
    EXPECT_TRUE(pStock->IsSameStock(pStock2));
  }

  TEST_F(CChinaStockTest, TestIsTodayDataChanged) {
    CChinaStock stock;
    EXPECT_FALSE(stock.IsTodayDataChanged());
    stock.SetHigh(10);
    EXPECT_TRUE(stock.IsTodayDataChanged());
    stock.SetHigh(0);
    stock.SetLow(10);
    EXPECT_TRUE(stock.IsTodayDataChanged());
    stock.SetLow(0);
    stock.SetVolume(10);
    EXPECT_TRUE(stock.IsTodayDataChanged());
    stock.SetVolume(0);
    stock.SetAmount(10);
    EXPECT_TRUE(stock.IsTodayDataChanged());
  }

  TEST_F(CChinaStockTest, TestGetCurrentGuadanTransactionPrice) {
    CChinaStock stock;

    EXPECT_DOUBLE_EQ(stock.GetCurrentGuadanTransactionPrice(), 0);
    stock.SetCurrentGuadanTransactionPrice(10.01);
    EXPECT_DOUBLE_EQ(stock.GetCurrentGuadanTransactionPrice(), 10.01);
  }

  TEST_F(CChinaStockTest, TestSaveStockCodeDB) {
    CSetStockCode setStockCode;
    CChinaStock stock, stock2;
    stock.SetSymbol(_T("400000.SS")); // 这个必须用未曾使用过的股票代码，已利于随后删除
    stock.SetOffset(13000);
    stock.SetStockName(_T("未使用过"));
    stock.SetIPOStatus(__STAKE_IPOED__);
    stock.SetDayLineEndDate(gl_pChinaStockMarket->GetFormatedMarketDate());
    stock.SetDayLineStartDate(19900101);
    setStockCode.m_strFilter = _T("[ID] = 1");
    setStockCode.Open();
    stock.AppendStockCodeDB(setStockCode);
    setStockCode.Close();

    setStockCode.m_strFilter = _T("[Symbol] = '400000.SS'");
    setStockCode.Open();
    EXPECT_EQ(setStockCode.m_IPOStatus, __STAKE_IPOED__);
    EXPECT_EQ(setStockCode.m_Symbol, _T("400000.SS"));
    setStockCode.Delete();
    setStockCode.Close();
  }

  TEST_F(CChinaStockTest, TestUpdateStockCodeDB) {
    CSetStockCode setStockCode;
    CChinaStock stock;

    setStockCode.Open();
    stock.LoadStockCodeDB(setStockCode);
    EXPECT_STREQ(stock.GetSymbol(), _T("000001.SS"));
    EXPECT_EQ(stock.GetIPOStatus(), __STAKE_IPOED__);
    stock.SetIPOStatus(__STAKE_NULL__);
    stock.UpdateStockCodeDB(setStockCode);
    setStockCode.Close();

    setStockCode.Open();
    EXPECT_EQ(setStockCode.m_IPOStatus, __STAKE_NULL__) << "此时状态已变为NULL";
    setStockCode.Edit();
    setStockCode.m_IPOStatus = __STAKE_IPOED__;
    setStockCode.Update();
    setStockCode.Close();
  }

  TEST_F(CChinaStockTest, TestLoadStockCodeDB1) {
    CSetStockCode setStockCode;
    CChinaStock stock;
    EXPECT_TRUE(stock.IsDayLineNeedUpdate());
    setStockCode.m_strSort = _T("[ID]");
    setStockCode.Open();
    stock.LoadStockCodeDB(setStockCode);
    EXPECT_STREQ(stock.GetSymbol(), _T("000001.SS"));
    EXPECT_EQ(stock.GetIPOStatus(), setStockCode.m_IPOStatus);
    EXPECT_EQ(stock.GetDayLineStartDate(), setStockCode.m_DayLineStartDate);
    EXPECT_EQ(stock.GetDayLineEndDate(), setStockCode.m_DayLineEndDate);
    setStockCode.Close();
  }

  TEST_F(CChinaStockTest, TestLoadStockCodeDB2) {
    CSetStockCode setStockCode;
    CChinaStock stock;

    gl_pChinaStockMarket->CalculateTime();
    stock.SetDayLineEndDate(gl_pChinaStockMarket->GetFormatedMarketDate());
    long lCurrentDate = gl_pChinaStockMarket->GetFormatedMarketDate();
    EXPECT_TRUE(stock.IsDayLineNeedUpdate());
    setStockCode.Open();
    stock.LoadStockCodeDB(setStockCode);
    EXPECT_STREQ(stock.GetSymbol(), _T("000001.SS"));
    EXPECT_EQ(stock.GetIPOStatus(), setStockCode.m_IPOStatus);
    EXPECT_EQ(stock.GetDayLineStartDate(), setStockCode.m_DayLineStartDate);
    EXPECT_EQ(stock.GetDayLineEndDate(), lCurrentDate);
    setStockCode.Close();
  }

  TEST_F(CChinaStockTest, TestSetCheckingDayLineStatus) {
    CChinaStock stock;
    EXPECT_TRUE(stock.IsDayLineNeedUpdate());
    stock.SetDayLineEndDate(gl_pChinaStockMarket->GetFormatedMarketDate());
    stock.SetCheckingDayLineStatus();
    EXPECT_FALSE(stock.IsDayLineNeedUpdate()) << stock.GetDayLineEndDate() << gl_pChinaStockMarket->GetFormatedMarketDate();
    stock.SetDayLineNeedUpdate(true);
    stock.SetDayLineEndDate(gl_pChinaStockMarket->GetLastTradeDate());
    stock.SetCheckingDayLineStatus();
    EXPECT_FALSE(stock.IsDayLineNeedUpdate());
    stock.SetDayLineNeedUpdate(true);
    stock.SetIPOStatus(__STAKE_IPOED__);
    stock.SetDayLineEndDate(__CHINA_MARKET_BEGIN_DATE__);
    stock.SetCheckingDayLineStatus();
    EXPECT_TRUE(stock.IsDayLineNeedUpdate());
    stock.SetIPOStatus(__STAKE_DELISTED__);
    stock.SetDayLineEndDate(__CHINA_MARKET_BEGIN_DATE__ + 1);
    stock.SetCheckingDayLineStatus();
    if (gl_pChinaStockMarket->GetDayOfWeek() == 1) EXPECT_TRUE(stock.IsDayLineNeedUpdate());
    else EXPECT_FALSE(stock.IsDayLineNeedUpdate());
  }

  TEST_F(CChinaStockTest, TestRTDataDeque) {    // 此三个函数是具备同步机制的，这里没有进行测试
    CWebRTDataPtr pData = make_shared<CWebRTData>();
    pData->SetSymbol(_T("600008.SS"));
    CChinaStock stock;
    EXPECT_EQ(stock.GetRTDataQueueSize(), 0);
    stock.PushRTData(pData);
    EXPECT_EQ(stock.GetRTDataQueueSize(), 1);
    pData = stock.GetRTDataAtHead();
    EXPECT_EQ(stock.GetRTDataQueueSize(), 1);
    EXPECT_STREQ(pData->GetSymbol(), _T("600008.SS"));
    CWebRTDataPtr pData2 = stock.PopRTData();
    EXPECT_EQ(stock.GetRTDataQueueSize(), 0);
  }

  TEST_F(CChinaStockTest, TestGetGuadan) {
    CChinaStock stock;
    EXPECT_FALSE(stock.HaveGuadan(10000));
    stock.SetGuadan(10000, 10000);
    EXPECT_TRUE(stock.HaveGuadan(10000));
    EXPECT_EQ(stock.GetGuadan(10000), 10000);
  }

  TEST_F(CChinaStockTest, TestCheckCurrentRTData) {
    CChinaStock stock;
    EXPECT_TRUE(stock.CheckCurrentRTData());
    stock.SetOrdinaryBuyVolume(-1);
    EXPECT_FALSE(stock.CheckCurrentRTData());
    stock.SetOrdinaryBuyVolume(0);
    stock.SetOrdinarySellVolume(-1);
    EXPECT_FALSE(stock.CheckCurrentRTData());
    stock.SetAttackBuyVolume(-1);
    stock.SetOrdinarySellVolume(0);
    EXPECT_FALSE(stock.CheckCurrentRTData());
    stock.SetAttackBuyVolume(0);
    stock.SetAttackSellVolume(-1);
    EXPECT_FALSE(stock.CheckCurrentRTData());
    stock.SetStrongBuyVolume(-1);
    stock.SetAttackSellVolume(0);
    EXPECT_FALSE(stock.CheckCurrentRTData());
    stock.SetStrongBuyVolume(0);
    stock.SetStrongSellVolume(-1);
    EXPECT_FALSE(stock.CheckCurrentRTData());
  }

  TEST_F(CChinaStockTest, TestClearRTDataDeque) {
    CChinaStock stock;
    CWebRTDataPtr pRTData;
    for (int i = 0; i < 10; i++) {
      pRTData = make_shared<CWebRTData>();
      pRTData->SetLastClose(i);
      stock.PushRTData(pRTData);
    }
    EXPECT_EQ(stock.GetRTDataQueueSize(), 10);
    stock.ClearRTDataDeque();
    EXPECT_EQ(stock.GetRTDataQueueSize(), 0);
  }

  TEST_F(CChinaStockTest, TestSaveTempInfo) {
    CSetDayLineToday setDayLineToday;
    pStock = make_shared<CChinaStock>();
    CChinaStock stock;

    pStock->SetHavingFirstRTData(true);
    pStock->SetSymbol(_T("600000.SS"));
    pStock->SetTransactionTime(FormatToTTime(20191101));
    pStock->SetLastClose(101010);
    pStock->SetOpen(202020);
    pStock->SetHigh(303030);
    pStock->SetLow(404040);
    pStock->SetNew(505050);
    pStock->SetVolume(60606060606060);
    pStock->SetAmount(707070707070707);
    pStock->SetUpDown(pStock->GetOpen() - pStock->GetNew());
    pStock->SetUpDownRate((double)pStock->GetUpDown() / pStock->GetLastClose() * 100);
    pStock->SetCurrentValue(808080808080808);
    pStock->SetTotalValue(909090909090909);

    pStock->SetTransactionNumber(123435);
    pStock->SetTransactionNumberBelow5000(45346456);
    pStock->SetTransactionNumberBelow50000(5698);
    pStock->SetTransactionNumberBelow200000(67687);
    pStock->SetTransactionNumberAbove200000(6876);
    pStock->SetOrdinaryBuyVolume(435);
    pStock->SetOrdinarySellVolume(560985);
    pStock->SetAttackBuyBelow50000(54509);
    pStock->SetAttackBuyBelow200000(45049);
    pStock->SetAttackBuyAbove200000(34508);
    pStock->SetAttackSellBelow50000(45896);
    pStock->SetAttackSellBelow200000(56457);
    pStock->SetAttackSellAbove200000(8767);
    pStock->SetAttackBuyAmount(123456654);
    pStock->SetAttackBuyVolume(23423534);
    pStock->SetAttackSellAmount(4353454);
    pStock->SetAttackSellVolume(94589489);
    pStock->SetCanceledBuyVolume(435245);
    pStock->SetCanceledSellVolume(45648698);
    pStock->SetUnknownVolume(4895747);
    pStock->SetStrongBuyVolume(453456);
    pStock->SetStrongSellVolume(98976);

    pStock->SetOrdinaryBuyVolumeBelow5000(30);
    pStock->SetOrdinaryBuyVolumeBelow10000(31);
    pStock->SetOrdinaryBuyVolumeBelow20000(32);
    pStock->SetOrdinaryBuyVolumeBelow50000(33);
    pStock->SetOrdinaryBuyVolumeBelow100000(34);
    pStock->SetOrdinaryBuyVolumeBelow200000(35);
    pStock->SetOrdinaryBuyVolumeAbove200000(36);
    pStock->SetOrdinarySellVolumeBelow5000(37);
    pStock->SetOrdinarySellVolumeBelow10000(38);
    pStock->SetOrdinarySellVolumeBelow20000(39);
    pStock->SetOrdinarySellVolumeBelow50000(40);
    pStock->SetOrdinarySellVolumeBelow100000(41);
    pStock->SetOrdinarySellVolumeBelow200000(42);
    pStock->SetOrdinarySellVolumeAbove200000(43);
    pStock->SetOrdinaryBuyNumberBelow5000(44);
    pStock->SetOrdinaryBuyNumberBelow10000(45);
    pStock->SetOrdinaryBuyNumberBelow20000(46);
    pStock->SetOrdinaryBuyNumberBelow50000(47);
    pStock->SetOrdinaryBuyNumberBelow100000(48);
    pStock->SetOrdinaryBuyNumberBelow200000(49);
    pStock->SetOrdinaryBuyNumberAbove200000(50);
    pStock->SetOrdinarySellNumberBelow5000(51);
    pStock->SetOrdinarySellNumberBelow10000(52);
    pStock->SetOrdinarySellNumberBelow20000(53);
    pStock->SetOrdinarySellNumberBelow50000(54);
    pStock->SetOrdinarySellNumberBelow100000(55);
    pStock->SetOrdinarySellNumberBelow200000(56);
    pStock->SetOrdinarySellNumberAbove200000(57);

    pStock->SetCanceledBuyVolumeBelow5000(64);
    pStock->SetCanceledBuyVolumeBelow10000(65);
    pStock->SetCanceledBuyVolumeBelow20000(66);
    pStock->SetCanceledBuyVolumeBelow50000(67);
    pStock->SetCanceledBuyVolumeBelow100000(68);
    pStock->SetCanceledBuyVolumeBelow200000(69);
    pStock->SetCanceledBuyVolumeAbove200000(70);
    pStock->SetCanceledSellVolumeBelow5000(71);
    pStock->SetCanceledSellVolumeBelow10000(72);
    pStock->SetCanceledSellVolumeBelow20000(73);
    pStock->SetCanceledSellVolumeBelow50000(74);
    pStock->SetCanceledSellVolumeBelow100000(75);
    pStock->SetCanceledSellVolumeBelow200000(76);
    pStock->SetCanceledSellVolumeAbove200000(77);

    ASSERT(!gl_fNormalMode);
    ASSERT(gl_fTestMode);
    setDayLineToday.Open();
    setDayLineToday.m_pDatabase->BeginTrans();
    while (!setDayLineToday.IsEOF()) {
      setDayLineToday.Delete();
      setDayLineToday.MoveNext();
    }
    setDayLineToday.m_pDatabase->CommitTrans();
    setDayLineToday.m_pDatabase->BeginTrans();
    setDayLineToday.AddNew();

    pStock->SaveTempInfo(setDayLineToday);
    setDayLineToday.Update();
    setDayLineToday.m_pDatabase->CommitTrans();
    setDayLineToday.Close();

    setDayLineToday.Open();
    EXPECT_EQ(setDayLineToday.m_Date, 20191101);
    EXPECT_STREQ(setDayLineToday.m_Symbol, pStock->GetSymbol());
    EXPECT_DOUBLE_EQ(atof(setDayLineToday.m_LastClose), (double)pStock->GetLastClose() / 1000);
    EXPECT_DOUBLE_EQ(atof(setDayLineToday.m_Open), (double)pStock->GetOpen() / 1000);
    EXPECT_DOUBLE_EQ(atof(setDayLineToday.m_High), (double)pStock->GetHigh() / 1000);
    EXPECT_DOUBLE_EQ(atof(setDayLineToday.m_Low), (double)pStock->GetLow() / 1000);
    EXPECT_DOUBLE_EQ(atof(setDayLineToday.m_Close), (double)pStock->GetNew() / 1000);
    EXPECT_EQ(atoll(setDayLineToday.m_Volume), pStock->GetVolume());
    EXPECT_EQ(atoll(setDayLineToday.m_Amount), pStock->GetAmount());
    EXPECT_EQ(atof(setDayLineToday.m_UpAndDown), (double)pStock->GetUpDown() / 1000);
    EXPECT_DOUBLE_EQ(atof(setDayLineToday.m_UpDownRate), pStock->GetUpDownRate());
    EXPECT_EQ(atoll(setDayLineToday.m_CurrentValue), pStock->GetCurrentValue());
    EXPECT_EQ(atoll(setDayLineToday.m_TotalValue), pStock->GetTotalValue());

    EXPECT_EQ(atol(setDayLineToday.m_AttackBuyAbove200000), pStock->GetAttackBuyAbove200000());
    EXPECT_EQ(atol(setDayLineToday.m_AttackBuyBelow200000), pStock->GetAttackBuyBelow200000());
    EXPECT_EQ(atol(setDayLineToday.m_AttackBuyBelow50000), pStock->GetAttackBuyBelow50000());
    EXPECT_EQ(atol(setDayLineToday.m_AttackBuyVolume), pStock->GetAttackBuyVolume());
    EXPECT_EQ(atol(setDayLineToday.m_AttackSellAbove200000), pStock->GetAttackSellAbove200000());
    EXPECT_EQ(atol(setDayLineToday.m_AttackSellBelow200000), pStock->GetAttackSellBelow200000());
    EXPECT_EQ(atol(setDayLineToday.m_AttackSellBelow50000), pStock->GetAttackSellBelow50000());
    EXPECT_EQ(atol(setDayLineToday.m_AttackSellVolume), pStock->GetAttackSellVolume());

    EXPECT_EQ(atol(setDayLineToday.m_OrdinaryBuyVolume), pStock->GetOrdinaryBuyVolume());
    EXPECT_EQ(atol(setDayLineToday.m_OrdinarySellVolume), pStock->GetOrdinarySellVolume());
    EXPECT_EQ(atol(setDayLineToday.m_CanceledBuyVolume), pStock->GetCanceledBuyVolume());
    EXPECT_EQ(atol(setDayLineToday.m_CanceledSellVolume), pStock->GetCanceledSellVolume());
    EXPECT_EQ(atol(setDayLineToday.m_StrongBuyVolume), pStock->GetStrongBuyVolume());
    EXPECT_EQ(atol(setDayLineToday.m_StrongSellVolume), pStock->GetStrongSellVolume());
    EXPECT_EQ(atol(setDayLineToday.m_UnknownVolume), pStock->GetUnknownVolume());

    EXPECT_EQ(atol(setDayLineToday.m_TransactionNumber), pStock->GetTransactionNumber());
    EXPECT_EQ(atol(setDayLineToday.m_TransactionNumberAbove200000), pStock->GetTransactionNumberAbove200000());
    EXPECT_EQ(atol(setDayLineToday.m_TransactionNumberBelow200000), pStock->GetTransactionNumberBelow200000());
    EXPECT_EQ(atol(setDayLineToday.m_TransactionNumberBelow50000), pStock->GetTransactionNumberBelow50000());
    EXPECT_EQ(atol(setDayLineToday.m_TransactionNumberBelow5000), pStock->GetTransactionNumberBelow5000());

    EXPECT_EQ(atoll(setDayLineToday.m_OrdinaryBuyVolumeBelow5000), pStock->GetOrdinaryBuyVolumeBelow5000());
    EXPECT_EQ(atoll(setDayLineToday.m_OrdinaryBuyVolumeBelow10000), pStock->GetOrdinaryBuyVolumeBelow10000());
    EXPECT_EQ(atoll(setDayLineToday.m_OrdinaryBuyVolumeBelow20000), pStock->GetOrdinaryBuyVolumeBelow20000());
    EXPECT_EQ(atoll(setDayLineToday.m_OrdinaryBuyVolumeBelow50000), pStock->GetOrdinaryBuyVolumeBelow50000());
    EXPECT_EQ(atoll(setDayLineToday.m_OrdinaryBuyVolumeBelow100000), pStock->GetOrdinaryBuyVolumeBelow100000());
    EXPECT_EQ(atoll(setDayLineToday.m_OrdinaryBuyVolumeBelow200000), pStock->GetOrdinaryBuyVolumeBelow200000());
    EXPECT_EQ(atoll(setDayLineToday.m_OrdinaryBuyVolumeAbove200000), pStock->GetOrdinaryBuyVolumeAbove200000());
    EXPECT_EQ(atoll(setDayLineToday.m_OrdinarySellVolumeBelow5000), pStock->GetOrdinarySellVolumeBelow5000());
    EXPECT_EQ(atoll(setDayLineToday.m_OrdinarySellVolumeBelow10000), pStock->GetOrdinarySellVolumeBelow10000());
    EXPECT_EQ(atoll(setDayLineToday.m_OrdinarySellVolumeBelow20000), pStock->GetOrdinarySellVolumeBelow20000());
    EXPECT_EQ(atoll(setDayLineToday.m_OrdinarySellVolumeBelow50000), pStock->GetOrdinarySellVolumeBelow50000());
    EXPECT_EQ(atoll(setDayLineToday.m_OrdinarySellVolumeBelow100000), pStock->GetOrdinarySellVolumeBelow100000());
    EXPECT_EQ(atoll(setDayLineToday.m_OrdinarySellVolumeBelow200000), pStock->GetOrdinarySellVolumeBelow200000());
    EXPECT_EQ(atoll(setDayLineToday.m_OrdinarySellVolumeAbove200000), pStock->GetOrdinarySellVolumeAbove200000());
    EXPECT_EQ(atoll(setDayLineToday.m_OrdinaryBuyNumberBelow5000), pStock->GetOrdinaryBuyNumberBelow5000());
    EXPECT_EQ(atoll(setDayLineToday.m_OrdinaryBuyNumberBelow10000), pStock->GetOrdinaryBuyNumberBelow10000());
    EXPECT_EQ(atoll(setDayLineToday.m_OrdinaryBuyNumberBelow20000), pStock->GetOrdinaryBuyNumberBelow20000());
    EXPECT_EQ(atoll(setDayLineToday.m_OrdinaryBuyNumberBelow50000), pStock->GetOrdinaryBuyNumberBelow50000());
    EXPECT_EQ(atoll(setDayLineToday.m_OrdinaryBuyNumberBelow100000), pStock->GetOrdinaryBuyNumberBelow100000());
    EXPECT_EQ(atoll(setDayLineToday.m_OrdinaryBuyNumberBelow200000), pStock->GetOrdinaryBuyNumberBelow200000());
    EXPECT_EQ(atoll(setDayLineToday.m_OrdinaryBuyNumberAbove200000), pStock->GetOrdinaryBuyNumberAbove200000());
    EXPECT_EQ(atoll(setDayLineToday.m_OrdinarySellNumberBelow5000), pStock->GetOrdinarySellNumberBelow5000());
    EXPECT_EQ(atoll(setDayLineToday.m_OrdinarySellNumberBelow10000), pStock->GetOrdinarySellNumberBelow10000());
    EXPECT_EQ(atoll(setDayLineToday.m_OrdinarySellNumberBelow20000), pStock->GetOrdinarySellNumberBelow20000());
    EXPECT_EQ(atoll(setDayLineToday.m_OrdinarySellNumberBelow50000), pStock->GetOrdinarySellNumberBelow50000());
    EXPECT_EQ(atoll(setDayLineToday.m_OrdinarySellNumberBelow100000), pStock->GetOrdinarySellNumberBelow100000());
    EXPECT_EQ(atoll(setDayLineToday.m_OrdinarySellNumberBelow200000), pStock->GetOrdinarySellNumberBelow200000());
    EXPECT_EQ(atoll(setDayLineToday.m_OrdinarySellNumberAbove200000), pStock->GetOrdinarySellNumberAbove200000());

    EXPECT_EQ(atoll(setDayLineToday.m_CanceledBuyVolumeBelow5000), pStock->GetCanceledBuyVolumeBelow5000());
    EXPECT_EQ(atoll(setDayLineToday.m_CanceledBuyVolumeBelow10000), pStock->GetCanceledBuyVolumeBelow10000());
    EXPECT_EQ(atoll(setDayLineToday.m_CanceledBuyVolumeBelow20000), pStock->GetCanceledBuyVolumeBelow20000());
    EXPECT_EQ(atoll(setDayLineToday.m_CanceledBuyVolumeBelow50000), pStock->GetCanceledBuyVolumeBelow50000());
    EXPECT_EQ(atoll(setDayLineToday.m_CanceledBuyVolumeBelow100000), pStock->GetCanceledBuyVolumeBelow100000());
    EXPECT_EQ(atoll(setDayLineToday.m_CanceledBuyVolumeBelow200000), pStock->GetCanceledBuyVolumeBelow200000());
    EXPECT_EQ(atoll(setDayLineToday.m_CanceledBuyVolumeAbove200000), pStock->GetCanceledBuyVolumeAbove200000());
    EXPECT_EQ(atoll(setDayLineToday.m_CanceledSellVolumeBelow5000), pStock->GetCanceledSellVolumeBelow5000());
    EXPECT_EQ(atoll(setDayLineToday.m_CanceledSellVolumeBelow10000), pStock->GetCanceledSellVolumeBelow10000());
    EXPECT_EQ(atoll(setDayLineToday.m_CanceledSellVolumeBelow20000), pStock->GetCanceledSellVolumeBelow20000());
    EXPECT_EQ(atoll(setDayLineToday.m_CanceledSellVolumeBelow50000), pStock->GetCanceledSellVolumeBelow50000());
    EXPECT_EQ(atoll(setDayLineToday.m_CanceledSellVolumeBelow100000), pStock->GetCanceledSellVolumeBelow100000());
    EXPECT_EQ(atoll(setDayLineToday.m_CanceledSellVolumeBelow200000), pStock->GetCanceledSellVolumeBelow200000());
    EXPECT_EQ(atoll(setDayLineToday.m_CanceledSellVolumeAbove200000), pStock->GetCanceledSellVolumeAbove200000());

    setDayLineToday.Close();

    setDayLineToday.Open();
    stock.LoadTempInfo(setDayLineToday);
    setDayLineToday.Close();

    EXPECT_EQ(stock.GetTransactionTime(), 0);
    EXPECT_STREQ(stock.GetSymbol(), _T(""));
    EXPECT_EQ(stock.GetLastClose(), 0);
    EXPECT_EQ(stock.GetOpen(), 0);
    EXPECT_EQ(stock.GetHigh(), 0);
    EXPECT_EQ(stock.GetLow(), 0);
    EXPECT_EQ(stock.GetNew(), 0);
    EXPECT_EQ(stock.GetVolume(), pStock->GetVolume());
    EXPECT_EQ(stock.GetAmount(), 0);
    EXPECT_EQ(stock.GetUpDown(), 0);
    EXPECT_DOUBLE_EQ(stock.GetUpDownRate(), 0);
    EXPECT_EQ(stock.GetCurrentValue(), 0);
    EXPECT_EQ(stock.GetTotalValue(), 0);

    EXPECT_EQ(stock.GetLastSavedVolume(), pStock->GetVolume());

    EXPECT_EQ(stock.GetAttackBuyAbove200000(), pStock->GetAttackBuyAbove200000());
    EXPECT_EQ(stock.GetAttackBuyBelow200000(), pStock->GetAttackBuyBelow200000());
    EXPECT_EQ(stock.GetAttackBuyBelow50000(), pStock->GetAttackBuyBelow50000());
    EXPECT_EQ(stock.GetAttackBuyVolume(), pStock->GetAttackBuyVolume());
    EXPECT_EQ(stock.GetAttackSellAbove200000(), pStock->GetAttackSellAbove200000());
    EXPECT_EQ(stock.GetAttackSellBelow200000(), pStock->GetAttackSellBelow200000());
    EXPECT_EQ(stock.GetAttackSellBelow50000(), pStock->GetAttackSellBelow50000());
    EXPECT_EQ(stock.GetAttackSellVolume(), pStock->GetAttackSellVolume());

    EXPECT_EQ(stock.GetOrdinaryBuyVolume(), pStock->GetOrdinaryBuyVolume());
    EXPECT_EQ(stock.GetOrdinarySellVolume(), pStock->GetOrdinarySellVolume());
    EXPECT_EQ(stock.GetCanceledBuyVolume(), pStock->GetCanceledBuyVolume());
    EXPECT_EQ(stock.GetCanceledSellVolume(), pStock->GetCanceledSellVolume());
    EXPECT_EQ(stock.GetStrongBuyVolume(), pStock->GetStrongBuyVolume());
    EXPECT_EQ(stock.GetStrongSellVolume(), pStock->GetStrongSellVolume());
    EXPECT_EQ(stock.GetUnknownVolume(), pStock->GetUnknownVolume());

    EXPECT_EQ(stock.GetTransactionNumber(), pStock->GetTransactionNumber());
    EXPECT_EQ(stock.GetTransactionNumberAbove200000(), pStock->GetTransactionNumberAbove200000());
    EXPECT_EQ(stock.GetTransactionNumberBelow200000(), pStock->GetTransactionNumberBelow200000());
    EXPECT_EQ(stock.GetTransactionNumberBelow50000(), pStock->GetTransactionNumberBelow50000());
    EXPECT_EQ(stock.GetTransactionNumberBelow5000(), pStock->GetTransactionNumberBelow5000());

    EXPECT_EQ(stock.GetOrdinaryBuyVolumeBelow5000(), pStock->GetOrdinaryBuyVolumeBelow5000());
    EXPECT_EQ(stock.GetOrdinaryBuyVolumeBelow10000(), pStock->GetOrdinaryBuyVolumeBelow10000());
    EXPECT_EQ(stock.GetOrdinaryBuyVolumeBelow20000(), pStock->GetOrdinaryBuyVolumeBelow20000());
    EXPECT_EQ(stock.GetOrdinaryBuyVolumeBelow50000(), pStock->GetOrdinaryBuyVolumeBelow50000());
    EXPECT_EQ(stock.GetOrdinaryBuyVolumeBelow100000(), pStock->GetOrdinaryBuyVolumeBelow100000());
    EXPECT_EQ(stock.GetOrdinaryBuyVolumeBelow200000(), pStock->GetOrdinaryBuyVolumeBelow200000());
    EXPECT_EQ(stock.GetOrdinaryBuyVolumeAbove200000(), pStock->GetOrdinaryBuyVolumeAbove200000());
    EXPECT_EQ(stock.GetOrdinarySellVolumeBelow5000(), pStock->GetOrdinarySellVolumeBelow5000());
    EXPECT_EQ(stock.GetOrdinarySellVolumeBelow10000(), pStock->GetOrdinarySellVolumeBelow10000());
    EXPECT_EQ(stock.GetOrdinarySellVolumeBelow20000(), pStock->GetOrdinarySellVolumeBelow20000());
    EXPECT_EQ(stock.GetOrdinarySellVolumeBelow50000(), pStock->GetOrdinarySellVolumeBelow50000());
    EXPECT_EQ(stock.GetOrdinarySellVolumeBelow100000(), pStock->GetOrdinarySellVolumeBelow100000());
    EXPECT_EQ(stock.GetOrdinarySellVolumeBelow200000(), pStock->GetOrdinarySellVolumeBelow200000());
    EXPECT_EQ(stock.GetOrdinarySellVolumeAbove200000(), pStock->GetOrdinarySellVolumeAbove200000());
    EXPECT_EQ(stock.GetOrdinaryBuyNumberBelow5000(), pStock->GetOrdinaryBuyNumberBelow5000());
    EXPECT_EQ(stock.GetOrdinaryBuyNumberBelow10000(), pStock->GetOrdinaryBuyNumberBelow10000());
    EXPECT_EQ(stock.GetOrdinaryBuyNumberBelow20000(), pStock->GetOrdinaryBuyNumberBelow20000());
    EXPECT_EQ(stock.GetOrdinaryBuyNumberBelow50000(), pStock->GetOrdinaryBuyNumberBelow50000());
    EXPECT_EQ(stock.GetOrdinaryBuyNumberBelow100000(), pStock->GetOrdinaryBuyNumberBelow100000());
    EXPECT_EQ(stock.GetOrdinaryBuyNumberBelow200000(), pStock->GetOrdinaryBuyNumberBelow200000());
    EXPECT_EQ(stock.GetOrdinaryBuyNumberAbove200000(), pStock->GetOrdinaryBuyNumberAbove200000());
    EXPECT_EQ(stock.GetOrdinarySellNumberBelow5000(), pStock->GetOrdinarySellNumberBelow5000());
    EXPECT_EQ(stock.GetOrdinarySellNumberBelow10000(), pStock->GetOrdinarySellNumberBelow10000());
    EXPECT_EQ(stock.GetOrdinarySellNumberBelow20000(), pStock->GetOrdinarySellNumberBelow20000());
    EXPECT_EQ(stock.GetOrdinarySellNumberBelow50000(), pStock->GetOrdinarySellNumberBelow50000());
    EXPECT_EQ(stock.GetOrdinarySellNumberBelow100000(), pStock->GetOrdinarySellNumberBelow100000());
    EXPECT_EQ(stock.GetOrdinarySellNumberBelow200000(), pStock->GetOrdinarySellNumberBelow200000());
    EXPECT_EQ(stock.GetOrdinarySellNumberAbove200000(), pStock->GetOrdinarySellNumberAbove200000());

    EXPECT_EQ(stock.GetCanceledBuyVolumeBelow5000(), pStock->GetCanceledBuyVolumeBelow5000());
    EXPECT_EQ(stock.GetCanceledBuyVolumeBelow10000(), pStock->GetCanceledBuyVolumeBelow10000());
    EXPECT_EQ(stock.GetCanceledBuyVolumeBelow20000(), pStock->GetCanceledBuyVolumeBelow20000());
    EXPECT_EQ(stock.GetCanceledBuyVolumeBelow50000(), pStock->GetCanceledBuyVolumeBelow50000());
    EXPECT_EQ(stock.GetCanceledBuyVolumeBelow100000(), pStock->GetCanceledBuyVolumeBelow100000());
    EXPECT_EQ(stock.GetCanceledBuyVolumeBelow200000(), pStock->GetCanceledBuyVolumeBelow200000());
    EXPECT_EQ(stock.GetCanceledBuyVolumeAbove200000(), pStock->GetCanceledBuyVolumeAbove200000());
    EXPECT_EQ(stock.GetCanceledSellVolumeBelow5000(), pStock->GetCanceledSellVolumeBelow5000());
    EXPECT_EQ(stock.GetCanceledSellVolumeBelow10000(), pStock->GetCanceledSellVolumeBelow10000());
    EXPECT_EQ(stock.GetCanceledSellVolumeBelow20000(), pStock->GetCanceledSellVolumeBelow20000());
    EXPECT_EQ(stock.GetCanceledSellVolumeBelow50000(), pStock->GetCanceledSellVolumeBelow50000());
    EXPECT_EQ(stock.GetCanceledSellVolumeBelow100000(), pStock->GetCanceledSellVolumeBelow100000());
    EXPECT_EQ(stock.GetCanceledSellVolumeBelow200000(), pStock->GetCanceledSellVolumeBelow200000());
    EXPECT_EQ(stock.GetCanceledSellVolumeAbove200000(), pStock->GetCanceledSellVolumeAbove200000());
  }

  TEST_F(CChinaStockTest, TestLoadDayLineAndDayLineInfo) {
    CSetDayLineBasicInfo setDayLineBasicInfo;
    CDayLinePtr pid;
    CDayLine stock;
    pStock = gl_pChinaStockMarket->GetStock(_T("600011.SS"));
    EXPECT_FALSE(gl_pChinaStockMarket->IsDayLineDBUpdated());
    gl_pChinaStockMarket->__TEST_SetFormatedMarketDate(21900101);

    pid = make_shared<CDayLine>();
    pid->SetDate(21900101);
    pid->SetSymbol(_T("600011.SS"));
    pid->SetStockName(_T("首创股份"));
    pid->SetLastClose(34235345);
    pid->SetOpen(1000000);
    pid->SetHigh(45234543);
    pid->SetLow(3452345);
    pid->SetClose(452435);
    pid->SetVolume(34523454);
    pid->SetAmount(3245235345);
    pid->SetUpDown(((double)pid->GetClose() - pid->GetLastClose()) / 1000);
    pid->SetUpDownRate(123.45);
    pid->SetTotalValue(234523452345);
    pid->SetCurrentValue(234145345245);
    pid->SetChangeHandRate(54.321);
    pid->SetRS(14.5);
    pStock->StoreDayLine(pid);

    pStock->SetDayLineEndDate(21890101);
    pStock->SetSymbol(_T("600011.SS"));
    ASSERT(!gl_fNormalMode);
    pStock->SaveDayLineBasicInfo();
    EXPECT_FALSE(gl_pChinaStockMarket->IsDayLineDBUpdated()) << "存储数据时不修改数据库状态，需要单独执行修改标识的函数";

    pStock->SetTransactionTime(FormatToTTime(21900101));
    pStock->SetTransactionNumber(1);
    pStock->SetTransactionNumberBelow5000(2);
    pStock->SetTransactionNumberBelow50000(3);
    pStock->SetTransactionNumberBelow200000(4);
    pStock->SetTransactionNumberAbove200000(5);

    pStock->SetCanceledBuyVolume(6);
    pStock->SetCanceledSellVolume(7);
    pStock->SetAttackBuyVolume(8);
    pStock->SetAttackSellVolume(9);
    pStock->SetStrongBuyVolume(10);
    pStock->SetStrongSellVolume(11);
    pStock->SetUnknownVolume(12);
    pStock->SetOrdinaryBuyVolume(13);
    pStock->SetOrdinarySellVolume(14);
    pStock->SetAttackBuyBelow50000(15);
    pStock->SetAttackBuyBelow200000(16);
    pStock->SetAttackBuyAbove200000(17);
    pStock->SetAttackSellBelow50000(18);
    pStock->SetAttackBuyBelow200000(19);
    pStock->SetAttackBuyAbove200000(20);
    pStock->SetAttackSellBelow200000(21);
    pStock->SetAttackSellAbove200000(22);

    pStock->SetOrdinaryBuyVolumeBelow5000(30);
    pStock->SetOrdinaryBuyVolumeBelow10000(31);
    pStock->SetOrdinaryBuyVolumeBelow20000(32);
    pStock->SetOrdinaryBuyVolumeBelow50000(33);
    pStock->SetOrdinaryBuyVolumeBelow100000(34);
    pStock->SetOrdinaryBuyVolumeBelow200000(35);
    pStock->SetOrdinaryBuyVolumeAbove200000(36);
    pStock->SetOrdinarySellVolumeBelow5000(37);
    pStock->SetOrdinarySellVolumeBelow10000(38);
    pStock->SetOrdinarySellVolumeBelow20000(39);
    pStock->SetOrdinarySellVolumeBelow50000(40);
    pStock->SetOrdinarySellVolumeBelow100000(41);
    pStock->SetOrdinarySellVolumeBelow200000(42);
    pStock->SetOrdinarySellVolumeAbove200000(43);
    pStock->SetOrdinaryBuyNumberBelow5000(44);
    pStock->SetOrdinaryBuyNumberBelow10000(45);
    pStock->SetOrdinaryBuyNumberBelow20000(46);
    pStock->SetOrdinaryBuyNumberBelow50000(47);
    pStock->SetOrdinaryBuyNumberBelow100000(48);
    pStock->SetOrdinaryBuyNumberBelow200000(49);
    pStock->SetOrdinaryBuyNumberAbove200000(50);
    pStock->SetOrdinarySellNumberBelow5000(51);
    pStock->SetOrdinarySellNumberBelow10000(52);
    pStock->SetOrdinarySellNumberBelow20000(53);
    pStock->SetOrdinarySellNumberBelow50000(54);
    pStock->SetOrdinarySellNumberBelow100000(55);
    pStock->SetOrdinarySellNumberBelow200000(56);
    pStock->SetOrdinarySellNumberAbove200000(57);

    pStock->SetCanceledBuyVolumeBelow5000(64);
    pStock->SetCanceledBuyVolumeBelow10000(65);
    pStock->SetCanceledBuyVolumeBelow20000(66);
    pStock->SetCanceledBuyVolumeBelow50000(67);
    pStock->SetCanceledBuyVolumeBelow100000(68);
    pStock->SetCanceledBuyVolumeBelow200000(69);
    pStock->SetCanceledBuyVolumeAbove200000(70);
    pStock->SetCanceledSellVolumeBelow5000(71);
    pStock->SetCanceledSellVolumeBelow10000(72);
    pStock->SetCanceledSellVolumeBelow20000(73);
    pStock->SetCanceledSellVolumeBelow50000(74);
    pStock->SetCanceledSellVolumeBelow100000(75);
    pStock->SetCanceledSellVolumeBelow200000(76);
    pStock->SetCanceledSellVolumeAbove200000(77);

    CSetDayLineExtendInfo setDayLineExtendInfo;
    setDayLineExtendInfo.m_strFilter = _T("[ID] = 1");
    setDayLineExtendInfo.Open();
    setDayLineExtendInfo.m_pDatabase->BeginTrans();
    setDayLineExtendInfo.AddNew();
    pStock->SaveTodayExtendInfo(&setDayLineExtendInfo);
    setDayLineExtendInfo.Update();
    setDayLineExtendInfo.m_pDatabase->CommitTrans();
    setDayLineExtendInfo.Close();

    EXPECT_FALSE(pStock->IsDayLineLoaded());
    pStock->LoadDayLine(pStock->GetSymbol());
    EXPECT_TRUE(pStock->IsDayLineLoaded());

    CDayLinePtr pDayLine;
    pDayLine = pStock->GetDayLine(pStock->GetDayLineSize() - 1);

    EXPECT_EQ(pDayLine->GetFormatedMarketTime(), 0);
    EXPECT_STREQ(pDayLine->GetSymbol(), _T("600011.SS"));
    EXPECT_EQ(pDayLine->GetLastClose(), pid->GetLastClose());
    EXPECT_EQ(pDayLine->GetOpen(), pid->GetOpen());
    EXPECT_EQ(pDayLine->GetHigh(), pid->GetHigh());
    EXPECT_EQ(pDayLine->GetLow(), pid->GetLow());
    EXPECT_EQ(pDayLine->GetClose(), pid->GetClose());
    EXPECT_EQ(pDayLine->GetVolume(), pid->GetVolume());
    EXPECT_EQ(pDayLine->GetAmount(), pid->GetAmount());
    EXPECT_EQ(pDayLine->GetUpDown(), pid->GetUpDown());
    EXPECT_EQ(pDayLine->GetUpDownRate(), pid->GetUpDownRate());
    EXPECT_EQ(pDayLine->GetTotalValue(), pid->GetTotalValue());
    EXPECT_EQ(pDayLine->GetCurrentValue(), pid->GetCurrentValue());
    EXPECT_EQ(pDayLine->GetChangeHandRate(), pid->GetChangeHandRate());
    EXPECT_EQ(pDayLine->GetRS(), pid->GetRS());

    EXPECT_EQ(pDayLine->GetTransactionNumber(), pStock->GetTransactionNumber());
    EXPECT_EQ(pDayLine->GetTransactionNumberBelow5000(), pStock->GetTransactionNumberBelow5000());
    EXPECT_EQ(pDayLine->GetTransactionNumberBelow50000(), pStock->GetTransactionNumberBelow50000());
    EXPECT_EQ(pDayLine->GetTransactionNumberBelow200000(), pStock->GetTransactionNumberBelow200000());
    EXPECT_EQ(pDayLine->GetTransactionNumberAbove200000(), pStock->GetTransactionNumberAbove200000());
    EXPECT_EQ(pDayLine->GetCanceledBuyVolume(), pStock->GetCanceledBuyVolume());
    EXPECT_EQ(pDayLine->GetCanceledSellVolume(), pStock->GetCanceledSellVolume());
    EXPECT_EQ(pDayLine->GetAttackBuyVolume(), pStock->GetAttackBuyVolume());
    EXPECT_EQ(pDayLine->GetAttackSellVolume(), pStock->GetAttackSellVolume());
    EXPECT_EQ(pDayLine->GetStrongBuyVolume(), pStock->GetStrongBuyVolume());
    EXPECT_EQ(pDayLine->GetStrongSellVolume(), pStock->GetStrongSellVolume());
    EXPECT_EQ(pDayLine->GetUnknownVolume(), pStock->GetUnknownVolume());
    EXPECT_EQ(pDayLine->GetOrdinaryBuyVolume(), pStock->GetOrdinaryBuyVolume());
    EXPECT_EQ(pDayLine->GetOrdinarySellVolume(), pStock->GetOrdinarySellVolume());
    EXPECT_EQ(pDayLine->GetAttackBuyBelow50000(), pStock->GetAttackBuyBelow50000());
    EXPECT_EQ(pDayLine->GetAttackBuyBelow200000(), pStock->GetAttackBuyBelow200000());
    EXPECT_EQ(pDayLine->GetAttackBuyAbove200000(), pStock->GetAttackBuyAbove200000());
    EXPECT_EQ(pDayLine->GetAttackSellBelow50000(), pStock->GetAttackSellBelow50000());
    EXPECT_EQ(pDayLine->GetAttackSellBelow200000(), pStock->GetAttackSellBelow200000());
    EXPECT_EQ(pDayLine->GetAttackSellAbove200000(), pStock->GetAttackSellAbove200000());

    EXPECT_EQ(pDayLine->GetOrdinaryBuyVolumeBelow5000(), pStock->GetOrdinaryBuyVolumeBelow5000());
    EXPECT_EQ(pDayLine->GetOrdinaryBuyVolumeBelow10000(), pStock->GetOrdinaryBuyVolumeBelow10000());
    EXPECT_EQ(pDayLine->GetOrdinaryBuyVolumeBelow20000(), pStock->GetOrdinaryBuyVolumeBelow20000());
    EXPECT_EQ(pDayLine->GetOrdinaryBuyVolumeBelow50000(), pStock->GetOrdinaryBuyVolumeBelow50000());
    EXPECT_EQ(pDayLine->GetOrdinaryBuyVolumeBelow100000(), pStock->GetOrdinaryBuyVolumeBelow100000());
    EXPECT_EQ(pDayLine->GetOrdinaryBuyVolumeBelow200000(), pStock->GetOrdinaryBuyVolumeBelow200000());
    EXPECT_EQ(pDayLine->GetOrdinaryBuyVolumeAbove200000(), pStock->GetOrdinaryBuyVolumeAbove200000());
    EXPECT_EQ(pDayLine->GetOrdinarySellVolumeBelow5000(), pStock->GetOrdinarySellVolumeBelow5000());
    EXPECT_EQ(pDayLine->GetOrdinarySellVolumeBelow10000(), pStock->GetOrdinarySellVolumeBelow10000());
    EXPECT_EQ(pDayLine->GetOrdinarySellVolumeBelow20000(), pStock->GetOrdinarySellVolumeBelow20000());
    EXPECT_EQ(pDayLine->GetOrdinarySellVolumeBelow50000(), pStock->GetOrdinarySellVolumeBelow50000());
    EXPECT_EQ(pDayLine->GetOrdinarySellVolumeBelow100000(), pStock->GetOrdinarySellVolumeBelow100000());
    EXPECT_EQ(pDayLine->GetOrdinarySellVolumeBelow200000(), pStock->GetOrdinarySellVolumeBelow200000());
    EXPECT_EQ(pDayLine->GetOrdinarySellVolumeAbove200000(), pStock->GetOrdinarySellVolumeAbove200000());
    EXPECT_EQ(pDayLine->GetOrdinaryBuyNumberBelow5000(), pStock->GetOrdinaryBuyNumberBelow5000());
    EXPECT_EQ(pDayLine->GetOrdinaryBuyNumberBelow10000(), pStock->GetOrdinaryBuyNumberBelow10000());
    EXPECT_EQ(pDayLine->GetOrdinaryBuyNumberBelow20000(), pStock->GetOrdinaryBuyNumberBelow20000());
    EXPECT_EQ(pDayLine->GetOrdinaryBuyNumberBelow50000(), pStock->GetOrdinaryBuyNumberBelow50000());
    EXPECT_EQ(pDayLine->GetOrdinaryBuyNumberBelow100000(), pStock->GetOrdinaryBuyNumberBelow100000());
    EXPECT_EQ(pDayLine->GetOrdinaryBuyNumberBelow200000(), pStock->GetOrdinaryBuyNumberBelow200000());
    EXPECT_EQ(pDayLine->GetOrdinaryBuyNumberAbove200000(), pStock->GetOrdinaryBuyNumberAbove200000());
    EXPECT_EQ(pDayLine->GetOrdinarySellNumberBelow5000(), pStock->GetOrdinarySellNumberBelow5000());
    EXPECT_EQ(pDayLine->GetOrdinarySellNumberBelow10000(), pStock->GetOrdinarySellNumberBelow10000());
    EXPECT_EQ(pDayLine->GetOrdinarySellNumberBelow20000(), pStock->GetOrdinarySellNumberBelow20000());
    EXPECT_EQ(pDayLine->GetOrdinarySellNumberBelow50000(), pStock->GetOrdinarySellNumberBelow50000());
    EXPECT_EQ(pDayLine->GetOrdinarySellNumberBelow100000(), pStock->GetOrdinarySellNumberBelow100000());
    EXPECT_EQ(pDayLine->GetOrdinarySellNumberBelow200000(), pStock->GetOrdinarySellNumberBelow200000());
    EXPECT_EQ(pDayLine->GetOrdinarySellNumberAbove200000(), pStock->GetOrdinarySellNumberAbove200000());

    EXPECT_EQ(pStock->GetCanceledBuyVolumeBelow5000(), pStock->GetCanceledBuyVolumeBelow5000());
    EXPECT_EQ(pStock->GetCanceledBuyVolumeBelow10000(), pStock->GetCanceledBuyVolumeBelow10000());
    EXPECT_EQ(pStock->GetCanceledBuyVolumeBelow20000(), pStock->GetCanceledBuyVolumeBelow20000());
    EXPECT_EQ(pStock->GetCanceledBuyVolumeBelow50000(), pStock->GetCanceledBuyVolumeBelow50000());
    EXPECT_EQ(pStock->GetCanceledBuyVolumeBelow100000(), pStock->GetCanceledBuyVolumeBelow100000());
    EXPECT_EQ(pStock->GetCanceledBuyVolumeBelow200000(), pStock->GetCanceledBuyVolumeBelow200000());
    EXPECT_EQ(pStock->GetCanceledBuyVolumeAbove200000(), pStock->GetCanceledBuyVolumeAbove200000());
    EXPECT_EQ(pStock->GetCanceledSellVolumeBelow5000(), pStock->GetCanceledSellVolumeBelow5000());
    EXPECT_EQ(pStock->GetCanceledSellVolumeBelow10000(), pStock->GetCanceledSellVolumeBelow10000());
    EXPECT_EQ(pStock->GetCanceledSellVolumeBelow20000(), pStock->GetCanceledSellVolumeBelow20000());
    EXPECT_EQ(pStock->GetCanceledSellVolumeBelow50000(), pStock->GetCanceledSellVolumeBelow50000());
    EXPECT_EQ(pStock->GetCanceledSellVolumeBelow100000(), pStock->GetCanceledSellVolumeBelow100000());
    EXPECT_EQ(pStock->GetCanceledSellVolumeBelow200000(), pStock->GetCanceledSellVolumeBelow200000());
    EXPECT_EQ(pStock->GetCanceledSellVolumeAbove200000(), pStock->GetCanceledSellVolumeAbove200000());

    setDayLineBasicInfo.m_strFilter = _T("[Date] = 21900101");
    setDayLineBasicInfo.Open();
    setDayLineBasicInfo.m_pDatabase->BeginTrans();
    while (!setDayLineBasicInfo.IsEOF()) {
      setDayLineBasicInfo.Delete();
      setDayLineBasicInfo.MoveNext();
    }
    setDayLineBasicInfo.m_pDatabase->CommitTrans();
    setDayLineBasicInfo.Close();

    setDayLineExtendInfo.m_strFilter = _T("[Date] = 21900101");
    setDayLineExtendInfo.Open();
    setDayLineExtendInfo.m_pDatabase->BeginTrans();
    while (!setDayLineExtendInfo.IsEOF()) {
      setDayLineExtendInfo.Delete();
      setDayLineExtendInfo.MoveNext();
    }
    setDayLineExtendInfo.m_pDatabase->CommitTrans();
    setDayLineExtendInfo.Close();
  }

  TEST_F(CChinaStockTest, TestSaveDayLine) {
    CSetDayLineBasicInfo setDayLineBasicInfo;
    CDayLinePtr pid;
    CDayLine dayLine;
    pStock = gl_pChinaStockMarket->GetStock(_T("600016.SS"));
    EXPECT_FALSE(gl_pChinaStockMarket->IsDayLineDBUpdated());
    gl_pChinaStockMarket->__TEST_SetFormatedMarketDate(20190101);

    for (int i = 0; i < 10; i++) {
      pid = make_shared<CDayLine>();
      pid->SetDate(21101201);
      pid->SetSymbol(_T("600016.SS"));
      pid->SetStockName(_T("首创股份"));
      pid->SetLastClose(34235345);
      pid->SetOpen(1000000 + i);
      pid->SetHigh(45234543);
      pid->SetLow(3452345);
      pid->SetClose(452435);
      pid->SetVolume(34523454);
      pid->SetAmount(3245235345);
      pid->SetUpDown(((double)pid->GetClose() - pid->GetLastClose()) / 1000);
      pid->SetUpDownRate(123.45);
      pid->SetTotalValue(234523452345);
      pid->SetCurrentValue(234145345245);
      pid->SetChangeHandRate(54.321);
      pid->SetRS(14.5);
      pStock->StoreDayLine(pid);
    }
    pStock->SetDayLineEndDate(10190101);
    pStock->SetSymbol(_T("600016.SS"));
    ASSERT(!gl_fNormalMode);
    pStock->SaveDayLineBasicInfo();
    EXPECT_FALSE(gl_pChinaStockMarket->IsDayLineDBUpdated()) << "存储数据时不修改数据库状态，需要单独执行修改标识的函数";

    setDayLineBasicInfo.m_strFilter = _T("[Date] = 21101201");
    setDayLineBasicInfo.Open();
    for (int i = 0; i < 10; i++) {
      dayLine.LoadChinaMarketBasicData(&setDayLineBasicInfo);
      pid = pStock->GetDayLine(i);
      EXPECT_EQ(setDayLineBasicInfo.m_Date, pid->GetFormatedMarketDate());
      EXPECT_STREQ(setDayLineBasicInfo.m_Symbol, pid->GetSymbol());
      EXPECT_DOUBLE_EQ(atof(setDayLineBasicInfo.m_LastClose) * 1000, pid->GetLastClose());
      EXPECT_DOUBLE_EQ(atof(setDayLineBasicInfo.m_Open) * 1000, pid->GetOpen());
      EXPECT_DOUBLE_EQ(atof(setDayLineBasicInfo.m_High) * 1000, pid->GetHigh());
      EXPECT_DOUBLE_EQ(atof(setDayLineBasicInfo.m_Low) * 1000, pid->GetLow());
      EXPECT_DOUBLE_EQ(atof(setDayLineBasicInfo.m_Close) * 1000, pid->GetClose());
      EXPECT_EQ(atoll(setDayLineBasicInfo.m_Volume), pid->GetVolume());
      EXPECT_EQ(atoll(setDayLineBasicInfo.m_Amount), pid->GetAmount());
      EXPECT_DOUBLE_EQ(atof(setDayLineBasicInfo.m_UpAndDown), pid->GetUpDown());
      EXPECT_DOUBLE_EQ(atof(setDayLineBasicInfo.m_UpDownRate), pid->GetUpDownRate());
      EXPECT_EQ(atoll(setDayLineBasicInfo.m_TotalValue), pid->GetTotalValue());
      EXPECT_EQ(atoll(setDayLineBasicInfo.m_CurrentValue), pid->GetCurrentValue());
      EXPECT_DOUBLE_EQ(atof(setDayLineBasicInfo.m_ChangeHandRate), pid->GetChangeHandRate());
      EXPECT_DOUBLE_EQ(atof(setDayLineBasicInfo.m_RS), pid->GetRS());
      setDayLineBasicInfo.MoveNext();
    }
    setDayLineBasicInfo.Close();

    setDayLineBasicInfo.m_strFilter = _T("[Date] = 21101201");
    setDayLineBasicInfo.Open();
    setDayLineBasicInfo.m_pDatabase->BeginTrans();
    while (!setDayLineBasicInfo.IsEOF()) {
      setDayLineBasicInfo.Delete();
      setDayLineBasicInfo.MoveNext();
    }
    setDayLineBasicInfo.m_pDatabase->CommitTrans();
    setDayLineBasicInfo.Close();
  }

  TEST_F(CChinaStockTest, TestLoadDayLine) {
    CSetDayLineBasicInfo setDayLineBasicInfo;
    CDayLinePtr pid;
    CDayLinePtr pDayLine = nullptr;
    CChinaStock stock;

    pStock = gl_pChinaStockMarket->GetStock(_T("600010.SS"));

    for (int i = 0; i < 10; i++) {
      pid = make_shared<CDayLine>();
      pid->SetDate(21101201);
      pid->SetSymbol(_T("600010.SS"));
      pid->SetStockName(_T("首创股份"));
      pid->SetLastClose(34235345);
      pid->SetOpen(1000000 + i);
      pid->SetHigh(45234543);
      pid->SetLow(3452345);
      pid->SetClose(452435);
      pid->SetVolume(34523454);
      pid->SetAmount(3245235345);
      pid->SetUpDown(((double)pid->GetClose() - pid->GetLastClose()) / 1000);
      pid->SetUpDownRate(123.45);
      pid->SetTotalValue(234523452345);
      pid->SetCurrentValue(234145345245);
      pid->SetChangeHandRate(54.321);
      pid->SetRS(14.5);
      pStock->StoreDayLine(pid);
    }
    pStock->SetSymbol(_T("600010.SS"));
    pStock->SetDayLineEndDate(10190101);
    ASSERT(!gl_fNormalMode);
    pStock->SaveDayLineBasicInfo();

    setDayLineBasicInfo.m_strFilter = _T("[Date] = 21101201");
    setDayLineBasicInfo.Open();
    stock.LoadDayLineBasicInfo(&setDayLineBasicInfo);
    for (int i = 0; i < 10; i++) {
      pid = stock.GetDayLine(i);
      pDayLine = pStock->GetDayLine(i);
      EXPECT_EQ(pDayLine->GetFormatedMarketDate(), pid->GetFormatedMarketDate());
      EXPECT_STREQ(pDayLine->GetSymbol(), pid->GetSymbol());
      EXPECT_EQ(pDayLine->GetLastClose(), pid->GetLastClose());
      EXPECT_EQ(pDayLine->GetOpen(), pid->GetOpen());
      EXPECT_EQ(pDayLine->GetHigh(), pid->GetHigh());
      EXPECT_EQ(pDayLine->GetLow(), pid->GetLow());
      EXPECT_EQ(pDayLine->GetClose(), pid->GetClose());
      EXPECT_EQ(pDayLine->GetVolume(), pid->GetVolume());
      EXPECT_EQ(pDayLine->GetAmount(), pid->GetAmount());
      EXPECT_DOUBLE_EQ(pDayLine->GetUpDown(), pid->GetUpDown());
      EXPECT_DOUBLE_EQ(pDayLine->GetUpDownRate(), pid->GetUpDownRate());
      EXPECT_EQ(pDayLine->GetTotalValue(), pid->GetTotalValue());
      EXPECT_EQ(pDayLine->GetCurrentValue(), pid->GetCurrentValue());
      EXPECT_DOUBLE_EQ(pDayLine->GetChangeHandRate(), pid->GetChangeHandRate());
      EXPECT_DOUBLE_EQ(pDayLine->GetRS(), pid->GetRS());
    }
    setDayLineBasicInfo.Close();

    setDayLineBasicInfo.m_strFilter = _T("[Date] = 21101201");
    setDayLineBasicInfo.Open();
    setDayLineBasicInfo.m_pDatabase->BeginTrans();
    while (!setDayLineBasicInfo.IsEOF()) {
      setDayLineBasicInfo.Delete();
      setDayLineBasicInfo.MoveNext();
    }
    setDayLineBasicInfo.m_pDatabase->CommitTrans();
    setDayLineBasicInfo.Close();
  }

  TEST_F(CChinaStockTest, TestUpdateDayLineStartEndDate) {
    CDayLinePtr pid;
    CChinaStock stock;

    pStock = gl_pChinaStockMarket->GetStock(_T("600004.SS"));

    for (int i = 0; i < 10; i++) {
      pid = make_shared<CDayLine>();
      pid->SetDate(__CHINA_MARKET_BEGIN_DATE__ + i * 100000 + 2);
      pid->SetSymbol(_T("600004.SS"));
      pid->SetStockName(_T("白云机场"));
      pid->SetLastClose(34235345);
      pid->SetOpen(1000000 + i);
      pid->SetHigh(45234543);
      pid->SetLow(3452345);
      pid->SetClose(452435);
      pid->SetVolume(34523454);
      pid->SetAmount(3245235345);
      pid->SetUpDown(((double)pid->GetClose() - pid->GetLastClose()) / 1000);
      pid->SetUpDownRate(123.45);
      pid->SetTotalValue(234523452345);
      pid->SetCurrentValue(234145345245);
      pid->SetChangeHandRate(54.321);
      pid->SetRS(14.5);
      pStock->StoreDayLine(pid);
    }
    pStock->SetSymbol(_T("600004.SS"));
    pStock->SetDayLineStartDate(19920102);
    pStock->SetDayLineEndDate(20800100);
    ASSERT(!gl_fNormalMode);
    pStock->UpdateDayLineStartEndDate();
    EXPECT_EQ(pStock->GetDayLineEndDate(), __CHINA_MARKET_BEGIN_DATE__ + 9 * 100000 + 2) << "日线最新日期已更新";
    EXPECT_EQ(pStock->GetDayLineStartDate(), __CHINA_MARKET_BEGIN_DATE__ + 2) << "日线最初日期已更新";
    EXPECT_TRUE(gl_pChinaStockMarket->IsDayLineDBUpdated());
  }

  TEST_F(CChinaStockTest, TestUpdateDayLineStartEndDate3) {
    CDayLinePtr pid;
    CChinaStock stock;

    pStock = gl_pChinaStockMarket->GetStock(_T("600008.SS"));

    for (int i = 1; i < 10; i++) {
      pid = make_shared<CDayLine>();
      pid->SetDate(__CHINA_MARKET_BEGIN_DATE__ + i * 100000);
      pid->SetSymbol(_T("600008.SS"));
      pid->SetStockName(_T("首创股份"));
      pid->SetLastClose(34235345);
      pid->SetOpen(1000000 + i);
      pid->SetHigh(45234543);
      pid->SetLow(3452345);
      pid->SetClose(452435);
      pid->SetVolume(34523454);
      pid->SetAmount(3245235345);
      pid->SetUpDown(((double)pid->GetClose() - pid->GetLastClose()) / 1000);
      pid->SetUpDownRate(123.45);
      pid->SetTotalValue(234523452345);
      pid->SetCurrentValue(234145345245);
      pid->SetChangeHandRate(54.321);
      pid->SetRS(14.5);
      pStock->StoreDayLine(pid);
    }
    pStock->SetSymbol(_T("600008.SS"));
    pStock->SetDayLineStartDate(19900101);
    pStock->SetDayLineEndDate(20800102);
    ASSERT(!gl_fNormalMode);
    pStock->UpdateDayLineStartEndDate();
    EXPECT_EQ(pStock->GetDayLineEndDate(), 20800102);
    EXPECT_EQ(pStock->GetDayLineStartDate(), 19900101 + 100000) << _T("当起始日期为19900101时，需要更新之");
    EXPECT_TRUE(gl_pChinaStockMarket->IsDayLineDBUpdated());
  }

  TEST_F(CChinaStockTest, TestUpdateDayLineStartEndDate2) {
    CDayLinePtr pid;
    CChinaStock stock;

    pStock = gl_pChinaStockMarket->GetStock(_T("600008.SS"));

    for (int i = 1; i < 10; i++) {
      pid = make_shared<CDayLine>();
      pid->SetDate(__CHINA_MARKET_BEGIN_DATE__ + i * 100000);
      pid->SetSymbol(_T("600008.SS"));
      pid->SetStockName(_T("首创股份"));
      pid->SetLastClose(34235345);
      pid->SetOpen(1000000 + i);
      pid->SetHigh(45234543);
      pid->SetLow(3452345);
      pid->SetClose(452435);
      pid->SetVolume(34523454);
      pid->SetAmount(3245235345);
      pid->SetUpDown(((double)pid->GetClose() - pid->GetLastClose()) / 1000);
      pid->SetUpDownRate(123.45);
      pid->SetTotalValue(234523452345);
      pid->SetCurrentValue(234145345245);
      pid->SetChangeHandRate(54.321);
      pid->SetRS(14.5);
      pStock->StoreDayLine(pid);
    }
    pStock->SetSymbol(_T("600008.SS"));
    pStock->SetDayLineStartDate(19900102);
    pStock->SetDayLineEndDate(20800102);
    ASSERT(!gl_fNormalMode);
    pStock->UpdateDayLineStartEndDate();
    EXPECT_EQ(pStock->GetDayLineEndDate(), 20800102);
    EXPECT_EQ(pStock->GetDayLineStartDate(), 19900102);
    EXPECT_FALSE(gl_pChinaStockMarket->IsDayLineDBUpdated());
  }

  TEST_F(CChinaStockTest, TestSetTodayActive) {
    pStock = gl_pChinaStockMarket->GetStock(_T("600001.SS")); // 这个股票退市了，故而可以作为测试对象
    EXPECT_FALSE(pStock->IsActive());
    CString strStockName = pStock->GetStockName();
    pStock->SetTodayActive(_T("600002.SZ"), _T("梨园"));
    EXPECT_TRUE(pStock->IsActive());
    EXPECT_FALSE(pStock->IsDayLineLoaded());
    EXPECT_STREQ(pStock->GetSymbol(), _T("600002.SZ"));
    EXPECT_STREQ(pStock->GetStockName(), _T("梨园"));

    // clearup
    pStock->SetActive(false);
    pStock->SetSymbol(_T("600001.SS"));
    pStock->SetStockName(strStockName);
  }

  TEST_F(CChinaStockTest, TestSkipNeteaseDayLineFirstInformationLine) {
    CChinaStock stock;
    INT64 lCurrentPos = 0;
    CString str = _T("日期,股票代码,名称,收盘价,最高价,最低价,开盘价,前收盘,涨跌额,换手率,成交量,成交金额,总市值,流通市值\r\n");
    stock.__TestSetDayLineBuffer(str.GetLength(), str.GetBuffer());
    EXPECT_TRUE(stock.SkipNeteaseDayLineInformationHeader(lCurrentPos));
    EXPECT_EQ(lCurrentPos, str.GetLength());
    str = _T("日期,股票代码,名称,收盘价,最高价,最低价,开盘价,前收盘,涨跌额,换手率,成交量,成交金额,总市值,流通市值\n"); // 缺少\r
    stock.__TestSetDayLineBuffer(str.GetLength(), str.GetBuffer());
    lCurrentPos = 0;
    EXPECT_FALSE(stock.SkipNeteaseDayLineInformationHeader(lCurrentPos));
    EXPECT_EQ(lCurrentPos, str.GetLength());
    str = _T("日期,股票代码,名称,收盘价,最高价,最低价,开盘价,前收盘,涨跌额,换手率,成交量,成交金额,总市值,流通市值\r"); // 缺少\n
    stock.__TestSetDayLineBuffer(str.GetLength(), str.GetBuffer());
    lCurrentPos = 0;
    EXPECT_FALSE(stock.SkipNeteaseDayLineInformationHeader(lCurrentPos));
    EXPECT_EQ(lCurrentPos, str.GetLength());
  }

  TEST_F(CChinaStockTest, TestIsVolumeConsisitence) {
    CChinaStock stock;
    EXPECT_TRUE(stock.IsVolumeConsistence());
    stock.SetVolume(10000);
    EXPECT_FALSE(stock.IsVolumeConsistence());
    stock.SetUnknownVolume(10000);
    EXPECT_TRUE(stock.IsVolumeConsistence());
  }

  TEST_F(CChinaStockTest, TestReportDayLineDownLoaded) {
    CChinaStock stock;
    stock.SetSymbol(_T("600008.SS"));
    stock.ReportDayLineDownLoaded();
    //EXPECT_EQ(gl_systemMessage.GetDayLineInfoDequeSize(), 1);
    //CString str = gl_systemMessage.PopDayLineInfoMessage();
    //EXPECT_STREQ(str, _T("sh600008日线下载完成."));
  }

  TEST_F(CChinaStockTest, TestStoreDayLine) {
    vector<CDayLinePtr> vDayLine;
    CDayLinePtr pDayLine;
    for (int i = 0; i < 10; i++) {
      pDayLine = make_shared<CDayLine>();
      pDayLine->SetDate(19900101 + i);
      pDayLine->SetClose(10);
      pDayLine->SetLastClose(10);
      vDayLine.push_back(pDayLine);
    }
    EXPECT_EQ(vDayLine.size(), 10);
    CChinaStock stock;
    EXPECT_FALSE(stock.IsDayLineLoaded());
    stock.UpdateDayLine(vDayLine);
    EXPECT_EQ(stock.GetDayLineSize(), 10);
    for (int i = 0; i < 10; i++) {
      EXPECT_EQ(stock.GetDayLine(i)->GetFormatedMarketDate(), 19900101 + i);
    }
    EXPECT_TRUE(stock.IsDayLineLoaded());
  }

  TEST_F(CChinaStockTest, TestSaveCalculatedInfo) {
    CChinaStock stock;

    stock.SetTransactionTime(FormatToTTime(19900101));
    stock.SetSymbol(_T("600601.SS"));
    stock.SetTransactionNumber(1);
    stock.SetTransactionNumberBelow5000(2);
    stock.SetTransactionNumberBelow50000(3);
    stock.SetTransactionNumberBelow200000(4);
    stock.SetTransactionNumberAbove200000(5);

    stock.SetCanceledBuyVolume(6);
    stock.SetCanceledSellVolume(7);
    stock.SetAttackBuyVolume(8);
    stock.SetAttackSellVolume(9);
    stock.SetStrongBuyVolume(10);
    stock.SetStrongSellVolume(11);
    stock.SetUnknownVolume(12);
    stock.SetOrdinaryBuyVolume(13);
    stock.SetOrdinarySellVolume(14);
    stock.SetAttackBuyBelow50000(15);
    stock.SetAttackBuyBelow200000(16);
    stock.SetAttackBuyAbove200000(17);
    stock.SetAttackSellBelow50000(18);
    stock.SetAttackBuyBelow200000(19);
    stock.SetAttackBuyAbove200000(20);

    stock.SetOrdinaryBuyVolumeBelow5000(30);
    stock.SetOrdinaryBuyVolumeBelow10000(31);
    stock.SetOrdinaryBuyVolumeBelow20000(32);
    stock.SetOrdinaryBuyVolumeBelow50000(33);
    stock.SetOrdinaryBuyVolumeBelow100000(34);
    stock.SetOrdinaryBuyVolumeBelow200000(35);
    stock.SetOrdinaryBuyVolumeAbove200000(36);
    stock.SetOrdinarySellVolumeBelow5000(37);
    stock.SetOrdinarySellVolumeBelow10000(38);
    stock.SetOrdinarySellVolumeBelow20000(39);
    stock.SetOrdinarySellVolumeBelow50000(40);
    stock.SetOrdinarySellVolumeBelow100000(41);
    stock.SetOrdinarySellVolumeBelow200000(42);
    stock.SetOrdinarySellVolumeAbove200000(43);
    stock.SetOrdinaryBuyNumberBelow5000(44);
    stock.SetOrdinaryBuyNumberBelow10000(45);
    stock.SetOrdinaryBuyNumberBelow20000(46);
    stock.SetOrdinaryBuyNumberBelow50000(47);
    stock.SetOrdinaryBuyNumberBelow100000(48);
    stock.SetOrdinaryBuyNumberBelow200000(49);
    stock.SetOrdinaryBuyNumberAbove200000(50);
    stock.SetOrdinarySellNumberBelow5000(51);
    stock.SetOrdinarySellNumberBelow10000(52);
    stock.SetOrdinarySellNumberBelow20000(53);
    stock.SetOrdinarySellNumberBelow50000(54);
    stock.SetOrdinarySellNumberBelow100000(55);
    stock.SetOrdinarySellNumberBelow200000(56);
    stock.SetOrdinarySellNumberAbove200000(57);

    stock.SetCanceledBuyVolumeBelow5000(64);
    stock.SetCanceledBuyVolumeBelow10000(65);
    stock.SetCanceledBuyVolumeBelow20000(66);
    stock.SetCanceledBuyVolumeBelow50000(67);
    stock.SetCanceledBuyVolumeBelow100000(68);
    stock.SetCanceledBuyVolumeBelow200000(69);
    stock.SetCanceledBuyVolumeAbove200000(70);
    stock.SetCanceledSellVolumeBelow5000(71);
    stock.SetCanceledSellVolumeBelow10000(72);
    stock.SetCanceledSellVolumeBelow20000(73);
    stock.SetCanceledSellVolumeBelow50000(74);
    stock.SetCanceledSellVolumeBelow100000(75);
    stock.SetCanceledSellVolumeBelow200000(76);
    stock.SetCanceledSellVolumeAbove200000(77);

    CSetDayLineExtendInfo setDayLineExtendInfo;
    setDayLineExtendInfo.m_strFilter = _T("[ID] = 1");
    setDayLineExtendInfo.Open();
    setDayLineExtendInfo.AddNew();
    stock.SaveTodayExtendInfo(&setDayLineExtendInfo);
    setDayLineExtendInfo.Update();
    setDayLineExtendInfo.Close();

    CDayLine dayLine;
    setDayLineExtendInfo.m_strFilter = _T("[Symbol] = '600601.SS'");
    setDayLineExtendInfo.Open();
    dayLine.LoadChinaMarketExtendData(&setDayLineExtendInfo);
    setDayLineExtendInfo.Close();
    EXPECT_EQ(dayLine.GetFormatedMarketTime(), 0);
    EXPECT_STREQ(dayLine.GetSymbol(), _T(""));
    EXPECT_EQ(dayLine.GetTransactionNumber(), stock.GetTransactionNumber());
    EXPECT_EQ(dayLine.GetTransactionNumberBelow5000(), stock.GetTransactionNumberBelow5000());
    EXPECT_EQ(dayLine.GetTransactionNumberBelow50000(), stock.GetTransactionNumberBelow50000());
    EXPECT_EQ(dayLine.GetTransactionNumberBelow200000(), stock.GetTransactionNumberBelow200000());
    EXPECT_EQ(dayLine.GetTransactionNumberAbove200000(), stock.GetTransactionNumberAbove200000());
    EXPECT_EQ(dayLine.GetCanceledBuyVolume(), stock.GetCanceledBuyVolume());
    EXPECT_EQ(dayLine.GetCanceledSellVolume(), stock.GetCanceledSellVolume());
    EXPECT_EQ(dayLine.GetAttackBuyVolume(), stock.GetAttackBuyVolume());
    EXPECT_EQ(dayLine.GetAttackSellVolume(), stock.GetAttackSellVolume());
    EXPECT_EQ(dayLine.GetStrongBuyVolume(), stock.GetStrongBuyVolume());
    EXPECT_EQ(dayLine.GetStrongSellVolume(), stock.GetStrongSellVolume());
    EXPECT_EQ(dayLine.GetUnknownVolume(), stock.GetUnknownVolume());
    EXPECT_EQ(dayLine.GetOrdinaryBuyVolume(), stock.GetOrdinaryBuyVolume());
    EXPECT_EQ(dayLine.GetOrdinarySellVolume(), stock.GetOrdinarySellVolume());
    EXPECT_EQ(dayLine.GetAttackBuyBelow50000(), stock.GetAttackBuyBelow50000());
    EXPECT_EQ(dayLine.GetAttackBuyBelow200000(), stock.GetAttackBuyBelow200000());
    EXPECT_EQ(dayLine.GetAttackBuyAbove200000(), stock.GetAttackBuyAbove200000());
    EXPECT_EQ(dayLine.GetAttackSellBelow50000(), stock.GetAttackSellBelow50000());
    EXPECT_EQ(dayLine.GetAttackSellBelow200000(), stock.GetAttackSellBelow200000());
    EXPECT_EQ(dayLine.GetAttackSellAbove200000(), stock.GetAttackSellAbove200000());

    EXPECT_EQ(dayLine.GetOrdinaryBuyVolumeBelow5000(), stock.GetOrdinaryBuyVolumeBelow5000());
    EXPECT_EQ(dayLine.GetOrdinaryBuyVolumeBelow10000(), stock.GetOrdinaryBuyVolumeBelow10000());
    EXPECT_EQ(dayLine.GetOrdinaryBuyVolumeBelow20000(), stock.GetOrdinaryBuyVolumeBelow20000());
    EXPECT_EQ(dayLine.GetOrdinaryBuyVolumeBelow50000(), stock.GetOrdinaryBuyVolumeBelow50000());
    EXPECT_EQ(dayLine.GetOrdinaryBuyVolumeBelow100000(), stock.GetOrdinaryBuyVolumeBelow100000());
    EXPECT_EQ(dayLine.GetOrdinaryBuyVolumeBelow200000(), stock.GetOrdinaryBuyVolumeBelow200000());
    EXPECT_EQ(dayLine.GetOrdinaryBuyVolumeAbove200000(), stock.GetOrdinaryBuyVolumeAbove200000());
    EXPECT_EQ(dayLine.GetOrdinarySellVolumeBelow5000(), stock.GetOrdinarySellVolumeBelow5000());
    EXPECT_EQ(dayLine.GetOrdinarySellVolumeBelow10000(), stock.GetOrdinarySellVolumeBelow10000());
    EXPECT_EQ(dayLine.GetOrdinarySellVolumeBelow20000(), stock.GetOrdinarySellVolumeBelow20000());
    EXPECT_EQ(dayLine.GetOrdinarySellVolumeBelow50000(), stock.GetOrdinarySellVolumeBelow50000());
    EXPECT_EQ(dayLine.GetOrdinarySellVolumeBelow100000(), stock.GetOrdinarySellVolumeBelow100000());
    EXPECT_EQ(dayLine.GetOrdinarySellVolumeBelow200000(), stock.GetOrdinarySellVolumeBelow200000());
    EXPECT_EQ(dayLine.GetOrdinarySellVolumeAbove200000(), stock.GetOrdinarySellVolumeAbove200000());
    EXPECT_EQ(dayLine.GetOrdinaryBuyNumberBelow5000(), stock.GetOrdinaryBuyNumberBelow5000());
    EXPECT_EQ(dayLine.GetOrdinaryBuyNumberBelow10000(), stock.GetOrdinaryBuyNumberBelow10000());
    EXPECT_EQ(dayLine.GetOrdinaryBuyNumberBelow20000(), stock.GetOrdinaryBuyNumberBelow20000());
    EXPECT_EQ(dayLine.GetOrdinaryBuyNumberBelow50000(), stock.GetOrdinaryBuyNumberBelow50000());
    EXPECT_EQ(dayLine.GetOrdinaryBuyNumberBelow100000(), stock.GetOrdinaryBuyNumberBelow100000());
    EXPECT_EQ(dayLine.GetOrdinaryBuyNumberBelow200000(), stock.GetOrdinaryBuyNumberBelow200000());
    EXPECT_EQ(dayLine.GetOrdinaryBuyNumberAbove200000(), stock.GetOrdinaryBuyNumberAbove200000());
    EXPECT_EQ(dayLine.GetOrdinarySellNumberBelow5000(), stock.GetOrdinarySellNumberBelow5000());
    EXPECT_EQ(dayLine.GetOrdinarySellNumberBelow10000(), stock.GetOrdinarySellNumberBelow10000());
    EXPECT_EQ(dayLine.GetOrdinarySellNumberBelow20000(), stock.GetOrdinarySellNumberBelow20000());
    EXPECT_EQ(dayLine.GetOrdinarySellNumberBelow50000(), stock.GetOrdinarySellNumberBelow50000());
    EXPECT_EQ(dayLine.GetOrdinarySellNumberBelow100000(), stock.GetOrdinarySellNumberBelow100000());
    EXPECT_EQ(dayLine.GetOrdinarySellNumberBelow200000(), stock.GetOrdinarySellNumberBelow200000());
    EXPECT_EQ(dayLine.GetOrdinarySellNumberAbove200000(), stock.GetOrdinarySellNumberAbove200000());

    EXPECT_EQ(dayLine.GetCanceledBuyVolumeBelow5000(), stock.GetCanceledBuyVolumeBelow5000());
    EXPECT_EQ(dayLine.GetCanceledBuyVolumeBelow10000(), stock.GetCanceledBuyVolumeBelow10000());
    EXPECT_EQ(dayLine.GetCanceledBuyVolumeBelow20000(), stock.GetCanceledBuyVolumeBelow20000());
    EXPECT_EQ(dayLine.GetCanceledBuyVolumeBelow50000(), stock.GetCanceledBuyVolumeBelow50000());
    EXPECT_EQ(dayLine.GetCanceledBuyVolumeBelow100000(), stock.GetCanceledBuyVolumeBelow100000());
    EXPECT_EQ(dayLine.GetCanceledBuyVolumeBelow200000(), stock.GetCanceledBuyVolumeBelow200000());
    EXPECT_EQ(dayLine.GetCanceledBuyVolumeAbove200000(), stock.GetCanceledBuyVolumeAbove200000());
    EXPECT_EQ(dayLine.GetCanceledSellVolumeBelow5000(), stock.GetCanceledSellVolumeBelow5000());
    EXPECT_EQ(dayLine.GetCanceledSellVolumeBelow10000(), stock.GetCanceledSellVolumeBelow10000());
    EXPECT_EQ(dayLine.GetCanceledSellVolumeBelow20000(), stock.GetCanceledSellVolumeBelow20000());
    EXPECT_EQ(dayLine.GetCanceledSellVolumeBelow50000(), stock.GetCanceledSellVolumeBelow50000());
    EXPECT_EQ(dayLine.GetCanceledSellVolumeBelow100000(), stock.GetCanceledSellVolumeBelow100000());
    EXPECT_EQ(dayLine.GetCanceledSellVolumeBelow200000(), stock.GetCanceledSellVolumeBelow200000());
    EXPECT_EQ(dayLine.GetCanceledSellVolumeAbove200000(), stock.GetCanceledSellVolumeAbove200000());
  }

  TEST_F(CChinaStockTest, TestProcessRTData1) {
    CChinaStock stock;
    EXPECT_FALSE(stock.ProcessRTData()) << _T("实时队列为空，故而并未执行任何计算工作，只是调用一下函数而已");
  }

  TEST_F(CChinaStockTest, TestProcessRTData2) {
    CChinaStock stock;
    CWebRTDataPtr prtData;
    prtData = make_shared<CWebRTData>();
    prtData->SetTransactionTime(10101010);
    prtData->SetActive(true);
    stock.PushRTData(prtData);

    gl_pChinaStockMarket->SetMarketOpened(true);
    stock.SetNeedProcessRTData(true);
    EXPECT_TRUE(stock.ProcessRTData());
    EXPECT_EQ(stock.GetTransactionTime(), 10101010);
    EXPECT_TRUE(stock.IsRTDataCalculated());

    gl_pChinaStockMarket->SetMarketOpened(false);
  }

  TEST_F(CChinaStockTest, TestSaveWeekLine) {
    CSetWeekLineBasicInfo setWeekLineBasicInfo;
    CWeekLinePtr pid;
    CWeekLine stock;
    pStock = gl_pChinaStockMarket->GetStock(_T("600016.SS"));
    gl_pChinaStockMarket->__TEST_SetFormatedMarketDate(20190101);

    for (int i = 0; i < 10; i++) {
      pid = make_shared<CWeekLine>();
      pid->SetDate(21101201);
      pid->SetSymbol(_T("600016.SS"));
      pid->SetStockName(_T("首创股份"));
      pid->SetLastClose(34235345);
      pid->SetOpen(1000000 + i);
      pid->SetHigh(45234543);
      pid->SetLow(3452345);
      pid->SetClose(452435);
      pid->SetVolume(34523454);
      pid->SetAmount(3245235345);
      pid->SetUpDown(((double)pid->GetClose() - pid->GetLastClose()) / 1000);
      pid->SetUpDownRate(123.45);
      pid->SetTotalValue(234523452345);
      pid->SetCurrentValue(234145345245);
      pid->SetChangeHandRate(54.321);
      pid->SetRS(14.5);
      pStock->StoreWeekLine(pid);
    }
    pStock->SetSymbol(_T("600016.SS"));
    ASSERT(!gl_fNormalMode);
    pStock->SaveWeekLine();

    setWeekLineBasicInfo.m_strFilter = _T("[Date] = 21101201");
    setWeekLineBasicInfo.Open();
    setWeekLineBasicInfo.m_pDatabase->BeginTrans();
    for (int i = 0; i < 10; i++) {
      stock.LoadBasicData(&setWeekLineBasicInfo);
      pid = pStock->GetWeekLine(i);
      EXPECT_EQ(setWeekLineBasicInfo.m_Date, pid->GetFormatedMarketDate());
      EXPECT_STREQ(setWeekLineBasicInfo.m_Symbol, pid->GetSymbol());
      EXPECT_DOUBLE_EQ(atof(setWeekLineBasicInfo.m_LastClose) * 1000, pid->GetLastClose());
      EXPECT_DOUBLE_EQ(atof(setWeekLineBasicInfo.m_Open) * 1000, pid->GetOpen());
      EXPECT_DOUBLE_EQ(atof(setWeekLineBasicInfo.m_High) * 1000, pid->GetHigh());
      EXPECT_DOUBLE_EQ(atof(setWeekLineBasicInfo.m_Low) * 1000, pid->GetLow());
      EXPECT_DOUBLE_EQ(atof(setWeekLineBasicInfo.m_Close) * 1000, pid->GetClose());
      EXPECT_EQ(atoll(setWeekLineBasicInfo.m_Volume), pid->GetVolume());
      EXPECT_EQ(atoll(setWeekLineBasicInfo.m_Amount), pid->GetAmount());
      EXPECT_DOUBLE_EQ(atof(setWeekLineBasicInfo.m_UpAndDown), pid->GetUpDown());
      EXPECT_DOUBLE_EQ(atof(setWeekLineBasicInfo.m_UpDownRate), pid->GetUpDownRate());
      EXPECT_EQ(atoll(setWeekLineBasicInfo.m_TotalValue), pid->GetTotalValue());
      EXPECT_EQ(atoll(setWeekLineBasicInfo.m_CurrentValue), pid->GetCurrentValue());
      EXPECT_DOUBLE_EQ(atof(setWeekLineBasicInfo.m_ChangeHandRate), pid->GetChangeHandRate());
      EXPECT_DOUBLE_EQ(atof(setWeekLineBasicInfo.m_RS), pid->GetRS());
      setWeekLineBasicInfo.MoveNext();
    }
    setWeekLineBasicInfo.m_pDatabase->CommitTrans();
    setWeekLineBasicInfo.Close();

    setWeekLineBasicInfo.m_strFilter = _T("[Date] = 21101201");
    setWeekLineBasicInfo.Open();
    setWeekLineBasicInfo.m_pDatabase->BeginTrans();
    while (!setWeekLineBasicInfo.IsEOF()) {
      setWeekLineBasicInfo.Delete();
      setWeekLineBasicInfo.MoveNext();
    }
    setWeekLineBasicInfo.m_pDatabase->CommitTrans();
    setWeekLineBasicInfo.Close();
  }

  TEST_F(CChinaStockTest, TestLoadWeekLine) {
    CSetWeekLineBasicInfo setWeekLineBasicInfo;
    CWeekLinePtr pid;
    CWeekLinePtr pWeekLine = nullptr;
    CChinaStock stock;

    pStock = gl_pChinaStockMarket->GetStock(_T("600010.SS"));

    for (int i = 0; i < 10; i++) {
      pid = make_shared<CWeekLine>();
      pid->SetDate(21101201);
      pid->SetSymbol(_T("600010.SS"));
      pid->SetStockName(_T("首创股份"));
      pid->SetLastClose(34235345);
      pid->SetOpen(1000000 + i);
      pid->SetHigh(45234543);
      pid->SetLow(3452345);
      pid->SetClose(452435);
      pid->SetVolume(34523454);
      pid->SetAmount(3245235345);
      pid->SetUpDown(((double)pid->GetClose() - pid->GetLastClose()) / 1000);
      pid->SetUpDownRate(123.45);
      pid->SetTotalValue(234523452345);
      pid->SetCurrentValue(234145345245);
      pid->SetChangeHandRate(54.321);
      pid->SetRS(14.5);
      pStock->StoreWeekLine(pid);
    }
    pStock->SetSymbol(_T("600010.SS"));
    ASSERT(!gl_fNormalMode);
    pStock->SaveWeekLine();

    setWeekLineBasicInfo.m_strFilter = _T("[Date] = 21101201");
    setWeekLineBasicInfo.Open();
    setWeekLineBasicInfo.m_pDatabase->BeginTrans();
    stock.LoadWeekLineBasicInfo(&setWeekLineBasicInfo);
    for (int i = 0; i < 10; i++) {
      pid = stock.GetWeekLine(i);
      pWeekLine = pStock->GetWeekLine(i);
      EXPECT_EQ(pWeekLine->GetFormatedMarketDate(), pid->GetFormatedMarketDate());
      EXPECT_STREQ(pWeekLine->GetSymbol(), pid->GetSymbol());
      EXPECT_EQ(pWeekLine->GetLastClose(), pid->GetLastClose());
      EXPECT_EQ(pWeekLine->GetOpen(), pid->GetOpen());
      EXPECT_EQ(pWeekLine->GetHigh(), pid->GetHigh());
      EXPECT_EQ(pWeekLine->GetLow(), pid->GetLow());
      EXPECT_EQ(pWeekLine->GetClose(), pid->GetClose());
      EXPECT_EQ(pWeekLine->GetVolume(), pid->GetVolume());
      EXPECT_EQ(pWeekLine->GetAmount(), pid->GetAmount());
      EXPECT_DOUBLE_EQ(pWeekLine->GetUpDown(), pid->GetUpDown());
      EXPECT_DOUBLE_EQ(pWeekLine->GetUpDownRate(), pid->GetUpDownRate());
      EXPECT_EQ(pWeekLine->GetTotalValue(), pid->GetTotalValue());
      EXPECT_EQ(pWeekLine->GetCurrentValue(), pid->GetCurrentValue());
      EXPECT_DOUBLE_EQ(pWeekLine->GetChangeHandRate(), pid->GetChangeHandRate());
      EXPECT_DOUBLE_EQ(pWeekLine->GetRS(), pid->GetRS());
    }
    setWeekLineBasicInfo.m_pDatabase->CommitTrans();
    setWeekLineBasicInfo.Close();

    setWeekLineBasicInfo.m_strFilter = _T("[Date] = 21101201");
    setWeekLineBasicInfo.Open();
    setWeekLineBasicInfo.m_pDatabase->BeginTrans();
    while (!setWeekLineBasicInfo.IsEOF()) {
      setWeekLineBasicInfo.Delete();
      setWeekLineBasicInfo.MoveNext();
    }
    setWeekLineBasicInfo.m_pDatabase->CommitTrans();
    setWeekLineBasicInfo.Close();
  }

  //TEST_F_TRAITS();
}