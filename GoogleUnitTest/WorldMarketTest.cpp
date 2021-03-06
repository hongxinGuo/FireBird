#include"pch.h"
#include"globedef.h"
#include"WebInquirer.h"

#include"WorldStock.h"
#include"WorldMarket.h"

using namespace testing;

namespace StockAnalysisTest {
  class CWorldMarketTest : public ::testing::Test
  {
  protected:
    static void SetUpTestSuite(void) {
    }
    static void TearDownTestSuite(void) {
    }
    virtual void SetUp(void) override {
    }

    virtual void TearDown(void) override {
      // clearup
      gl_pWorldMarket->SetFinnhubInquiring(false);
    }

  protected:
  };

  TEST_F(CWorldMarketTest, TestTaskResetMarket) {
    gl_pWorldMarket->SetSystemReady(true);
    gl_pWorldMarket->SetPermitResetMarket(false);
    EXPECT_FALSE(gl_pWorldMarket->TaskResetMarket(101010)) << "不允许重置系统时，永远返回假";

    gl_pWorldMarket->SetPermitResetMarket(true);
    EXPECT_FALSE(gl_pWorldMarket->TaskResetMarket(170000));
    gl_pWorldMarket->SetPermitResetMarket(true);
    EXPECT_FALSE(gl_pWorldMarket->TaskResetMarket(170101));
    gl_pWorldMarket->SetPermitResetMarket(true);
    EXPECT_TRUE(gl_pWorldMarket->TaskResetMarket(170001));
    gl_pWorldMarket->SetPermitResetMarket(true);
    EXPECT_TRUE(gl_pWorldMarket->TaskResetMarket(170100));
    EXPECT_FALSE(gl_pWorldMarket->IsSystemReady());
    EXPECT_FALSE(gl_pWorldMarket->IsPermitResetMarket());
    EXPECT_TRUE(gl_pWorldMarket->IsResetMarket());

    gl_pWorldMarket->SetPermitResetMarket(true);
    gl_pWorldMarket->SetSystemReady(true);
    gl_pWorldMarket->SetResetMarket(false);
  }

  TEST_F(CWorldMarketTest, TestIsFinnhubInquiring) {
    EXPECT_FALSE(gl_pWorldMarket->IsFinnhubInquiring());
    gl_pWorldMarket->SetFinnhubInquiring(true);
    EXPECT_TRUE(gl_pWorldMarket->IsFinnhubInquiring());
    gl_pWorldMarket->SetFinnhubInquiring(false);
    EXPECT_FALSE(gl_pWorldMarket->IsFinnhubInquiring());
  }

  TEST_F(CWorldMarketTest, TestGetTotalStock) {
    EXPECT_EQ(gl_pWorldMarket->GetTotalStock(), 2462) << "默认状态下数据库总数为2462";
  }

  TEST_F(CWorldMarketTest, TestIsStock) {
    EXPECT_FALSE(gl_pWorldMarket->IsStock(_T("000000.SS")));
    EXPECT_TRUE(gl_pWorldMarket->IsStock(_T("000001.SS")));
    EXPECT_TRUE(gl_pWorldMarket->IsStock(_T("600601.SS")));

    CWorldStockPtr pStock = make_shared<CWorldStock>();
    pStock->SetSymbol(_T("000000.SS"));
    EXPECT_FALSE(gl_pWorldMarket->IsStock(pStock));
    pStock->SetSymbol(_T("000001.SS"));
    EXPECT_TRUE(gl_pWorldMarket->IsStock(pStock));
    pStock->SetSymbol(_T("600601.SS"));
    EXPECT_TRUE(gl_pWorldMarket->IsStock(pStock));
  }

  TEST_F(CWorldMarketTest, TestIsForexExchange) {
    EXPECT_FALSE(gl_pWorldMarket->IsForexExchange(_T("ABC")));
    EXPECT_TRUE(gl_pWorldMarket->IsForexExchange(_T("forex.com")));
    EXPECT_TRUE(gl_pWorldMarket->IsForexExchange(_T("ic markets")));
  }

  TEST_F(CWorldMarketTest, TestIsForexSymbol) {
    EXPECT_FALSE(gl_pWorldMarket->IsForexSymbol(_T("ABC")));
    EXPECT_TRUE(gl_pWorldMarket->IsForexSymbol(_T("OANDA:XAU_SGD")));
    EXPECT_TRUE(gl_pWorldMarket->IsForexSymbol(_T("FXCM:EUR/CHF")));
  }

  TEST_F(CWorldMarketTest, TestIsCountry) {
    CCountryPtr pCountry = make_shared<CCountry>();

    EXPECT_FALSE(gl_pWorldMarket->IsCountry(_T("ABC")));
    EXPECT_TRUE(gl_pWorldMarket->IsCountry(_T("American Samoa")));

    pCountry->m_strCountry = _T("ABC");
    EXPECT_FALSE(gl_pWorldMarket->IsCountry(pCountry));
    pCountry->m_strCountry = _T("American Samoa");
    EXPECT_TRUE(gl_pWorldMarket->IsCountry(pCountry));
  }

  TEST_F(CWorldMarketTest, TestGetStock) {
    CWorldStockPtr pStock = gl_pWorldMarket->GetStock(0); // 000001.SS
    EXPECT_STREQ(pStock->GetSymbol(), _T("000001.SS")) << "第一个股票代码为000001.SS";
    pStock = gl_pWorldMarket->GetStock(_T("000001.SS"));
    EXPECT_FALSE(pStock == nullptr);
    EXPECT_STREQ(pStock->GetDescription(), _T("SSE Composite Index"));
  }

  TEST_F(CWorldMarketTest, TestLoadOption) {
    EXPECT_STREQ(gl_pFinnhubWebInquiry->GetInquiringStringSuffix(), _T("&token=bv4ac1n48v6tcp17l5cg"));

    gl_pFinnhubWebInquiry->SetInquiryingStringSuffix(_T(""));
    gl_pTiingoWebInquiry->SetInquiryingStringSuffix(_T(""));
    gl_pQuandlWebInquiry->SetInquiryingStringSuffix(_T(""));
    gl_pWorldMarket->LoadOption();
    EXPECT_STREQ(gl_pFinnhubWebInquiry->GetInquiringStringSuffix(), _T("&token=bv4ac1n48v6tcp17l5cg"));
    EXPECT_STREQ(gl_pTiingoWebInquiry->GetInquiringStringSuffix(), _T("&token=859bd66ca24b2a81a2b5f4de6616e2c408b2a769"));
    EXPECT_STREQ(gl_pQuandlWebInquiry->GetInquiringStringSuffix(), _T("&api_key=zBMXMyoTyiy_N3pMb3ex"));

    gl_pFinnhubWebInquiry->SetInquiryingStringSuffix(_T("&token=bv4ac1n48v6tcp17l5cg"));
    gl_pTiingoWebInquiry->SetInquiryingStringSuffix(_T("&token=859bd66ca24b2a81a2b5f4de6616e2c408b2a769"));
    gl_pQuandlWebInquiry->SetInquiryingStringSuffix(_T("&api_key=zBMXMyoTyiy_N3pMb3ex"));
  }

  TEST_F(CWorldMarketTest, TestLoadExchangeCode) {
    // 暂缓
  }

  TEST_F(CWorldMarketTest, TestUpdateCountryDB) {
    CCountryPtr pCountry;
    size_t lTotal = gl_pWorldMarket->GetTotalCountry();

    pCountry = make_shared<CCountry>();
    pCountry->m_strCode2 = _T("AB");
    pCountry->m_strCountry = _T("NoName");
    EXPECT_FALSE(gl_pWorldMarket->IsCountry(pCountry));
    gl_pWorldMarket->AddCountry(pCountry);
    EXPECT_EQ(gl_pWorldMarket->GetTotalCountry(), lTotal + 1);
    gl_pWorldMarket->UpdateCountryListDB(); // 此测试函数执行完后，新增了一个Country没有删除（数据库中的删除了）。

    CSetCountry setCountry;
    setCountry.m_strFilter = _T("[Country] = 'NoName'");
    setCountry.Open();
    EXPECT_FALSE(setCountry.IsEOF());
    setCountry.m_pDatabase->BeginTrans();
    while (!setCountry.IsEOF()) {
      setCountry.Delete();
      setCountry.MoveNext();
    }
    setCountry.m_pDatabase->CommitTrans();
    setCountry.Close();
  }

  TEST_F(CWorldMarketTest, TestUpdateStockDB) {
    CWorldStockPtr pStock = make_shared<CWorldStock>();
    pStock->SetSymbol(_T("SS.SS.US"));
    EXPECT_FALSE(gl_pWorldMarket->IsStock(pStock)); // 确保是一个新股票代码
    gl_pWorldMarket->CreateNewStock(pStock);
    pStock = gl_pWorldMarket->GetStock(_T("000001.SS"));
    EXPECT_STREQ(pStock->GetCurrency(), _T(""));
    pStock->SetUpdateStockProfileDB(true);
    pStock->SetCurrency(_T("No Currency"));
    gl_pWorldMarket->UpdateStockDB();

    CSetWorldStock setWorldStock;
    setWorldStock.m_strFilter = _T("[Symbol] = '000001.SS'");
    setWorldStock.Open();
    EXPECT_STREQ(setWorldStock.m_Currency, _T("No Currency"));
    setWorldStock.m_pDatabase->BeginTrans();
    setWorldStock.Edit();
    setWorldStock.m_Currency = _T("");
    setWorldStock.Update();
    setWorldStock.m_pDatabase->CommitTrans();
    setWorldStock.Close();

    setWorldStock.m_strFilter = _T("[Symbol] = 'SS.SS.US'");
    setWorldStock.Open();
    EXPECT_FALSE(setWorldStock.IsEOF());
    setWorldStock.m_pDatabase->BeginTrans();
    while (!setWorldStock.IsEOF()) {
      setWorldStock.Delete();
      setWorldStock.MoveNext();
    }
    setWorldStock.m_pDatabase->CommitTrans();
    setWorldStock.Close();

    pStock = gl_pWorldMarket->GetStock(_T("SS.SS.US"));
    gl_pWorldMarket->DeleteStock(pStock); // 恢复原状
  }

  TEST_F(CWorldMarketTest, TestGetFinnhubInquiry) {
    WebInquiry inquiry, inquiry2;

    EXPECT_EQ(gl_pWorldMarket->GetFinnhubInquiryQueueSize(), 0);
    inquiry.m_lInquiryIndex = 0;
    inquiry.m_iPriority = 10;
    inquiry.m_lStockIndex = 0;
    gl_pWorldMarket->PushFinnhubInquiry(inquiry);
    EXPECT_EQ(gl_pWorldMarket->GetFinnhubInquiryQueueSize(), 1);
    inquiry2 = gl_pWorldMarket->GetFinnhubInquiry();
    EXPECT_EQ(inquiry.m_iPriority, inquiry2.m_iPriority);
    EXPECT_EQ(inquiry.m_lInquiryIndex, inquiry2.m_lInquiryIndex);
    EXPECT_EQ(inquiry.m_lStockIndex, inquiry2.m_lStockIndex);
    EXPECT_EQ(gl_pWorldMarket->GetFinnhubInquiryQueueSize(), 0);
  }

  TEST_F(CWorldMarketTest, TestIsCountryListUpdated) {
    EXPECT_FALSE(gl_pWorldMarket->IsCountryListUpdated());
    gl_pWorldMarket->SetCountryListUpdated(true);
    EXPECT_TRUE(gl_pWorldMarket->IsCountryListUpdated());
    gl_pWorldMarket->SetCountryListUpdated(false);
    EXPECT_FALSE(gl_pWorldMarket->IsCountryListUpdated());
  }

  TEST_F(CWorldMarketTest, TestIsFinnhubSymbolUpdated) {
    EXPECT_FALSE(gl_pWorldMarket->IsFinnhubSymbolUpdated());
    gl_pWorldMarket->SetFinnhubSymbolUpdated(true);
    EXPECT_TRUE(gl_pWorldMarket->IsFinnhubSymbolUpdated());
    gl_pWorldMarket->SetFinnhubSymbolUpdated(false);
    EXPECT_FALSE(gl_pWorldMarket->IsFinnhubSymbolUpdated());
  }

  TEST_F(CWorldMarketTest, TestIsWorldStockUpdated) {
    EXPECT_FALSE(gl_pWorldMarket->IsWorldStockUpdated());
    gl_pWorldMarket->SetWorldStockUpdated(true);
    EXPECT_TRUE(gl_pWorldMarket->IsWorldStockUpdated());
    gl_pWorldMarket->SetWorldStockUpdated(false);
    EXPECT_FALSE(gl_pWorldMarket->IsWorldStockUpdated());
  }

  TEST_F(CWorldMarketTest, TestIsFinnhubDayLineUpdated) {
    EXPECT_FALSE(gl_pWorldMarket->IsFinnhubDayLineUpdated());
    gl_pWorldMarket->SetFinnhubDayLineUpdated(true);
    EXPECT_TRUE(gl_pWorldMarket->IsFinnhubDayLineUpdated());
    gl_pWorldMarket->SetFinnhubDayLineUpdated(false);
    EXPECT_FALSE(gl_pWorldMarket->IsFinnhubDayLineUpdated());
  }

  TEST_F(CWorldMarketTest, TestIsFinnhubForexExhangeUpdated) {
    EXPECT_FALSE(gl_pWorldMarket->IsFinnhubForexExchangeUpdated());
    gl_pWorldMarket->SetFinnhubForexExchangeUpdated(true);
    EXPECT_TRUE(gl_pWorldMarket->IsFinnhubForexExchangeUpdated());
    gl_pWorldMarket->SetFinnhubForexExchangeUpdated(false);
    EXPECT_FALSE(gl_pWorldMarket->IsFinnhubForexExchangeUpdated());
  }

  TEST_F(CWorldMarketTest, TestIsFinnhubForexSymbolUpdated) {
    EXPECT_FALSE(gl_pWorldMarket->IsFinnhubForexSymbolUpdated());
    gl_pWorldMarket->SetFinnhubForexSymbolUpdated(true);
    EXPECT_TRUE(gl_pWorldMarket->IsFinnhubForexSymbolUpdated());
    gl_pWorldMarket->SetFinnhubForexSymbolUpdated(false);
    EXPECT_FALSE(gl_pWorldMarket->IsFinnhubForexSymbolUpdated());
  }

  TEST_F(CWorldMarketTest, TestIsFinnhubEconomicCalendarUpdated) {
    EXPECT_FALSE(gl_pWorldMarket->IsFinnhubEconomicCalendarUpdated());
    gl_pWorldMarket->SetFinnhubEconomicCalendarUpdated(true);
    EXPECT_TRUE(gl_pWorldMarket->IsFinnhubEconomicCalendarUpdated());
    gl_pWorldMarket->SetFinnhubEconomicCalendarUpdated(false);
    EXPECT_FALSE(gl_pWorldMarket->IsFinnhubEconomicCalendarUpdated());
  }

  TEST_F(CWorldMarketTest, TestIsFinnhubEPSSurpriseUpdated) {
    EXPECT_FALSE(gl_pWorldMarket->IsFinnhubEPSSurpriseUpdated());
    gl_pWorldMarket->SetFinnhubEPSSurpriseUpdated(true);
    EXPECT_TRUE(gl_pWorldMarket->IsFinnhubEPSSurpriseUpdated());
    gl_pWorldMarket->SetFinnhubEPSSurpriseUpdated(false);
    EXPECT_FALSE(gl_pWorldMarket->IsFinnhubEPSSurpriseUpdated());
  }

  TEST_F(CWorldMarketTest, TestTaskInquiryFinnhubCountryList) {
    EXPECT_FALSE(gl_pWorldMarket->IsCountryListUpdated());
    gl_pWorldMarket->SetFinnhubInquiring(true);
    EXPECT_FALSE(gl_pWorldMarket->TaskInquiryFinnhubCountryList());
    EXPECT_EQ(gl_pWorldMarket->GetFinnhubInquiryQueueSize(), 0);
    gl_pWorldMarket->SetCountryListUpdated(true);
    gl_pWorldMarket->SetFinnhubInquiring(false);
    EXPECT_FALSE(gl_pWorldMarket->TaskInquiryFinnhubCountryList());
    EXPECT_EQ(gl_pWorldMarket->GetFinnhubInquiryQueueSize(), 0);
    gl_pWorldMarket->SetCountryListUpdated(false);
    gl_pWorldMarket->SetFinnhubInquiring(false);
    EXPECT_TRUE(gl_pWorldMarket->TaskInquiryFinnhubCountryList());
    EXPECT_EQ(gl_pWorldMarket->GetFinnhubInquiryQueueSize(), 1);
    EXPECT_TRUE(gl_pWorldMarket->IsFinnhubInquiring());
    WebInquiry inquiry = gl_pWorldMarket->GetFinnhubInquiry();
    EXPECT_EQ(inquiry.m_iPriority, 10);
    EXPECT_EQ(inquiry.m_lInquiryIndex, __ECONOMIC_COUNTRY_LIST__);
    EXPECT_EQ(inquiry.m_lStockIndex, 0);
    EXPECT_EQ(gl_pWorldMarket->GetFinnhubInquiryQueueSize(), 0);

    gl_pWorldMarket->SetFinnhubInquiring(false);
  }
}