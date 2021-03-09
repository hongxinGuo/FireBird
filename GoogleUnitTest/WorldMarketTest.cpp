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
      gl_pWorldMarket->SetFinnhubInquiring(false);
      gl_pWorldMarket->SetCountryListUpdated(false);
      gl_pWorldMarket->SetFinnhubSymbolUpdated(false);
      gl_pWorldMarket->SetFinnhubStockProfileUpdated(false);
      gl_pWorldMarket->SetFinnhubDayLineUpdated(false);
      gl_pWorldMarket->SetFinnhubPeerUpdated(false);
      gl_pWorldMarket->SetFinnhubEconomicCalendarUpdated(false);
      gl_pWorldMarket->SetFinnhubEPSSurpriseUpdated(true);
      gl_pWorldMarket->SetFinnhubForexExchangeUpdated(false);
      gl_pWorldMarket->SetFinnhubForexSymbolUpdated(false);
    }
    static void TearDownTestSuite(void) {
    }
    virtual void SetUp(void) override {
    }

    virtual void TearDown(void) override {
      // clearup
      gl_pWorldMarket->SetFinnhubInquiring(false);
      gl_pWorldMarket->SetCountryListUpdated(false);
      gl_pWorldMarket->SetFinnhubSymbolUpdated(false);
      gl_pWorldMarket->SetFinnhubStockProfileUpdated(false);
      gl_pWorldMarket->SetFinnhubDayLineUpdated(false);
      gl_pWorldMarket->SetFinnhubPeerUpdated(false);
      gl_pWorldMarket->SetFinnhubEconomicCalendarUpdated(false);
      gl_pWorldMarket->SetFinnhubEPSSurpriseUpdated(true);
      gl_pWorldMarket->SetFinnhubForexExchangeUpdated(false);
      gl_pWorldMarket->SetFinnhubForexSymbolUpdated(false);

      gl_pWorldMarket->SetTiingoInquiring(false);
      gl_pWorldMarket->SetTiingoDayLineUpdated(false);

      while (gl_systemMessage.GetInformationDequeSize() > 0) gl_systemMessage.PopInformationMessage();
      while (gl_systemMessage.GetDayLineInfoDequeSize() > 0) gl_systemMessage.PopDayLineInfoMessage();
      while (gl_systemMessage.GetInnerSystemInformationDequeSize() > 0) gl_systemMessage.PopInnerSystemInformationMessage();
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

  TEST_F(CWorldMarketTest, TestIsTiingoInquiring) {
    EXPECT_FALSE(gl_pWorldMarket->IsTiingoInquiring());
    gl_pWorldMarket->SetTiingoInquiring(true);
    EXPECT_TRUE(gl_pWorldMarket->IsTiingoInquiring());
    gl_pWorldMarket->SetTiingoInquiring(false);
    EXPECT_FALSE(gl_pWorldMarket->IsTiingoInquiring());
  }

  TEST_F(CWorldMarketTest, TestGetTotalStock) {
    EXPECT_EQ(gl_pWorldMarket->GetTotalStock(), 28876) << "默认状态下数据库总数为28876";
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

    CForexSymbolPtr pForexSymbol = make_shared<CFinnhubForexSymbol>();
    pForexSymbol->m_strSymbol = _T("ABC");
    EXPECT_FALSE(gl_pWorldMarket->IsForexSymbol(pForexSymbol));
    pForexSymbol->m_strSymbol = _T("OANDA:XAU_SGD");
    EXPECT_TRUE(gl_pWorldMarket->IsForexSymbol(pForexSymbol));
    pForexSymbol->m_strSymbol = _T("FXCM:EUR/CHF");
    EXPECT_TRUE(gl_pWorldMarket->IsForexSymbol(pForexSymbol));
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
    gl_pWorldMarket->AddStock(pStock);
    pStock = gl_pWorldMarket->GetStock(_T("000001.SS"));
    EXPECT_STREQ(pStock->GetCurrency(), _T(""));
    pStock->SetUpdateProfileDB(true);
    pStock->SetCurrency(_T("No Currency"));
    gl_pWorldMarket->UpdateStockProfileDB();

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

  TEST_F(CWorldMarketTest, TestGetTiingoInquiry) {
    WebInquiry inquiry, inquiry2;

    EXPECT_EQ(gl_pWorldMarket->GetTiingoInquiryQueueSize(), 0);
    inquiry.m_lInquiryIndex = 0;
    inquiry.m_iPriority = 10;
    inquiry.m_lStockIndex = 0;
    gl_pWorldMarket->PushTiingoInquiry(inquiry);
    EXPECT_EQ(gl_pWorldMarket->GetTiingoInquiryQueueSize(), 1);
    inquiry2 = gl_pWorldMarket->GetTiingoInquiry();
    EXPECT_EQ(inquiry.m_iPriority, inquiry2.m_iPriority);
    EXPECT_EQ(inquiry.m_lInquiryIndex, inquiry2.m_lInquiryIndex);
    EXPECT_EQ(inquiry.m_lStockIndex, inquiry2.m_lStockIndex);
    EXPECT_EQ(gl_pWorldMarket->GetTiingoInquiryQueueSize(), 0);
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

  TEST_F(CWorldMarketTest, TestIsFinnhubStockProfileUpdated) {
    EXPECT_FALSE(gl_pWorldMarket->IsFinnhubStockProfileUpdated());
    gl_pWorldMarket->SetFinnhubStockProfileUpdated(true);
    EXPECT_TRUE(gl_pWorldMarket->IsFinnhubStockProfileUpdated());
    gl_pWorldMarket->SetFinnhubStockProfileUpdated(false);
    EXPECT_FALSE(gl_pWorldMarket->IsFinnhubStockProfileUpdated());
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

  TEST_F(CWorldMarketTest, TestIsFinnhubForexDayLineUpdated) {
    EXPECT_FALSE(gl_pWorldMarket->IsFinnhubForexDayLineUpdated());
    gl_pWorldMarket->SetFinnhubForexDayLineUpdated(true);
    EXPECT_TRUE(gl_pWorldMarket->IsFinnhubForexDayLineUpdated());
    gl_pWorldMarket->SetFinnhubForexDayLineUpdated(false);
    EXPECT_FALSE(gl_pWorldMarket->IsFinnhubForexDayLineUpdated());
  }
  TEST_F(CWorldMarketTest, TestIsFinnhubPeerUpdated) {
    EXPECT_FALSE(gl_pWorldMarket->IsFinnhubPeerUpdated());
    gl_pWorldMarket->SetFinnhubPeerUpdated(true);
    EXPECT_TRUE(gl_pWorldMarket->IsFinnhubPeerUpdated());
    gl_pWorldMarket->SetFinnhubPeerUpdated(false);
    EXPECT_FALSE(gl_pWorldMarket->IsFinnhubPeerUpdated());
  }

  TEST_F(CWorldMarketTest, TestIsFinnhubEconomicCalendarUpdated) {
    EXPECT_FALSE(gl_pWorldMarket->IsFinnhubEconomicCalendarUpdated());
    gl_pWorldMarket->SetFinnhubEconomicCalendarUpdated(true);
    EXPECT_TRUE(gl_pWorldMarket->IsFinnhubEconomicCalendarUpdated());
    gl_pWorldMarket->SetFinnhubEconomicCalendarUpdated(false);
    EXPECT_FALSE(gl_pWorldMarket->IsFinnhubEconomicCalendarUpdated());
  }

  TEST_F(CWorldMarketTest, TestIsFinnhubEPSSurpriseUpdated) {
    EXPECT_TRUE(gl_pWorldMarket->IsFinnhubEPSSurpriseUpdated());
    gl_pWorldMarket->SetFinnhubEPSSurpriseUpdated(false);
    EXPECT_FALSE(gl_pWorldMarket->IsFinnhubEPSSurpriseUpdated());
    gl_pWorldMarket->SetFinnhubEPSSurpriseUpdated(true);
    EXPECT_TRUE(gl_pWorldMarket->IsFinnhubEPSSurpriseUpdated());
  }

  TEST_F(CWorldMarketTest, TestIsTiingoSymbolUpdated) {
    EXPECT_FALSE(gl_pWorldMarket->IsTiingoSymbolUpdated());
    gl_pWorldMarket->SetTiingoSymbolUpdated(true);
    EXPECT_TRUE(gl_pWorldMarket->IsTiingoSymbolUpdated());
    gl_pWorldMarket->SetTiingoSymbolUpdated(false);
    EXPECT_FALSE(gl_pWorldMarket->IsTiingoSymbolUpdated());
  }\
    TEST_F(CWorldMarketTest, TestIsTiingoDayLineUpdated) {
    EXPECT_FALSE(gl_pWorldMarket->IsTiingoDayLineUpdated());
    gl_pWorldMarket->SetTiingoDayLineUpdated(true);
    EXPECT_TRUE(gl_pWorldMarket->IsTiingoDayLineUpdated());
    gl_pWorldMarket->SetTiingoDayLineUpdated(false);
    EXPECT_FALSE(gl_pWorldMarket->IsTiingoDayLineUpdated());
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
  }

  TEST_F(CWorldMarketTest, TestTaskInquiryFinnhubCompanySymbol) {
    CFinnhubExchangePtr pExchange;
    WebInquiry inquiry;

    EXPECT_EQ(gl_pWorldMarket->GetExchangeSize(), 68);
    for (int i = 0; i < gl_pWorldMarket->GetExchangeSize(); i++) {
      pExchange = gl_pWorldMarket->GetExchange(i);
      pExchange->m_fUpdated = true;
    }
    gl_pWorldMarket->GetExchange(1)->m_fUpdated = false;
    gl_pWorldMarket->GetExchange(10)->m_fUpdated = false;
    gl_pWorldMarket->SetFinnhubSymbolUpdated(true);
    EXPECT_FALSE(gl_pWorldMarket->TaskInquiryFinnhubCompanySymbol()) << "Symbol Updated";

    gl_pWorldMarket->SetFinnhubSymbolUpdated(false);
    gl_pWorldMarket->SetFinnhubInquiring(true);
    EXPECT_FALSE(gl_pWorldMarket->TaskInquiryFinnhubCompanySymbol()) << "其他FinnhubInquiry正在进行";

    gl_pWorldMarket->SetFinnhubInquiring(false);
    EXPECT_TRUE(gl_pWorldMarket->TaskInquiryFinnhubCompanySymbol());
    EXPECT_TRUE(gl_pWorldMarket->IsFinnhubInquiring());
    inquiry = gl_pWorldMarket->GetFinnhubInquiry();
    EXPECT_EQ(inquiry.m_lInquiryIndex, __COMPANY_SYMBOLS__);
    EXPECT_EQ(inquiry.m_lStockIndex, 1) << "第一个待查询交易所索引";
    EXPECT_TRUE(gl_pWorldMarket->GetExchange(1)->m_fUpdated);
    EXPECT_FALSE(gl_pWorldMarket->GetExchange(10)->m_fUpdated);

    gl_pWorldMarket->SetFinnhubInquiring(false);
    EXPECT_TRUE(gl_pWorldMarket->TaskInquiryFinnhubCompanySymbol());
    inquiry = gl_pWorldMarket->GetFinnhubInquiry();
    EXPECT_EQ(inquiry.m_lInquiryIndex, __COMPANY_SYMBOLS__);
    EXPECT_EQ(inquiry.m_lStockIndex, 10) << "第二个待查询交易所索引";
    EXPECT_TRUE(gl_pWorldMarket->GetExchange(1)->m_fUpdated);
    EXPECT_TRUE(gl_pWorldMarket->GetExchange(10)->m_fUpdated);

    gl_pWorldMarket->SetFinnhubInquiring(false);
    EXPECT_FALSE(gl_pWorldMarket->TaskInquiryFinnhubCompanySymbol()) << "第三次查询时没有找到待查询的交易所";
    EXPECT_TRUE(gl_pWorldMarket->IsFinnhubSymbolUpdated()) << "交易所都查询完了";
    CString str = gl_systemMessage.PopInformationMessage();
    EXPECT_STREQ(str, _T("交易所代码数据查询完毕"));
  }

  TEST_F(CWorldMarketTest, TestTaskInquiryFinnhubCompanyProfileConcise) {
    CWorldStockPtr pStock;
    WebInquiry inquiry;

    gl_pWorldMarket->SetSystemReady(true);
    for (int i = 0; i < gl_pWorldMarket->GetTotalStock(); i++) {
      pStock = gl_pWorldMarket->GetStock(i);
      pStock->SetProfileUpdated(true);
    }
    gl_pWorldMarket->GetStock(1)->SetProfileUpdated(false);
    gl_pWorldMarket->GetStock(10)->SetProfileUpdated(false);
    gl_pWorldMarket->SetFinnhubStockProfileUpdated(true);
    EXPECT_FALSE(gl_pWorldMarket->TaskInquiryFinnhubCompanyProfileConcise()) << "Stock Profile Updated";

    gl_pWorldMarket->SetFinnhubStockProfileUpdated(false);
    gl_pWorldMarket->SetFinnhubInquiring(true);
    EXPECT_FALSE(gl_pWorldMarket->TaskInquiryFinnhubCompanyProfileConcise()) << "其他FinnhubInquiry正在进行";

    gl_pWorldMarket->SetFinnhubInquiring(false);
    EXPECT_TRUE(gl_pWorldMarket->TaskInquiryFinnhubCompanyProfileConcise());
    EXPECT_TRUE(gl_pWorldMarket->IsFinnhubInquiring());
    inquiry = gl_pWorldMarket->GetFinnhubInquiry();
    EXPECT_EQ(inquiry.m_lInquiryIndex, __COMPANY_PROFILE_CONCISE__);
    EXPECT_EQ(inquiry.m_lStockIndex, 1) << "第一个待查询股票位置";
    EXPECT_FALSE(gl_pWorldMarket->GetStock(1)->IsProfileUpdated()) << "此更新标识需要等待处理完数据后才设置";
    EXPECT_FALSE(gl_pWorldMarket->GetStock(10)->IsProfileUpdated());
    gl_pWorldMarket->GetStock(1)->SetProfileUpdated(true);

    gl_pWorldMarket->SetFinnhubInquiring(false);
    EXPECT_TRUE(gl_pWorldMarket->TaskInquiryFinnhubCompanyProfileConcise());
    inquiry = gl_pWorldMarket->GetFinnhubInquiry();
    EXPECT_EQ(inquiry.m_lInquiryIndex, __COMPANY_PROFILE_CONCISE__);
    EXPECT_EQ(inquiry.m_lStockIndex, 10) << "第二个待查询股票位置";
    EXPECT_TRUE(gl_pWorldMarket->GetStock(1)->IsProfileUpdated());
    EXPECT_FALSE(gl_pWorldMarket->GetStock(10)->IsProfileUpdated()) << "此更新标识需要等待处理完数据后才设置";
    gl_pWorldMarket->GetStock(10)->SetProfileUpdated(true);

    gl_pWorldMarket->SetFinnhubInquiring(false);
    EXPECT_FALSE(gl_pWorldMarket->TaskInquiryFinnhubCompanyProfileConcise()) << "第三次查询时没有找到待查询的股票";
    EXPECT_TRUE(gl_pWorldMarket->IsFinnhubStockProfileUpdated()) << "股票都查询完了";
    CString str = gl_systemMessage.PopInformationMessage();
    EXPECT_STREQ(str, _T("Finnhub股票简介更新完毕"));
  }

  TEST_F(CWorldMarketTest, TestTaskInquiryFinnhubDayLine) {
    CWorldStockPtr pStock;
    WebInquiry inquiry;

    gl_pWorldMarket->SetSystemReady(true);
    for (int i = 0; i < gl_pWorldMarket->GetTotalStock(); i++) {
      pStock = gl_pWorldMarket->GetStock(i);
      pStock->SetDayLineNeedUpdate(false);
    }
    gl_pWorldMarket->GetStock(10001)->SetDayLineNeedUpdate(true); // 测试数据库中，上海市场的股票排在前面（共2462个），美国市场的股票排在后面
    gl_pWorldMarket->GetStock(10010)->SetDayLineNeedUpdate(true);
    gl_pWorldMarket->SetFinnhubDayLineUpdated(true);
    EXPECT_FALSE(gl_pWorldMarket->TaskInquiryFinnhubDayLine()) << "DayLine Updated";

    gl_pWorldMarket->SetFinnhubDayLineUpdated(false);
    gl_pWorldMarket->SetFinnhubInquiring(true);
    EXPECT_FALSE(gl_pWorldMarket->TaskInquiryFinnhubDayLine()) << "其他FinnhubInquiry正在进行";

    gl_pWorldMarket->SetFinnhubInquiring(false);
    EXPECT_TRUE(gl_pWorldMarket->TaskInquiryFinnhubDayLine());
    EXPECT_TRUE(gl_pWorldMarket->IsFinnhubInquiring());
    inquiry = gl_pWorldMarket->GetFinnhubInquiry();
    EXPECT_EQ(inquiry.m_lInquiryIndex, __STOCK_CANDLES__);
    EXPECT_EQ(inquiry.m_lStockIndex, 10001) << "第一个待查询股票位置";
    EXPECT_FALSE(gl_pWorldMarket->GetStock(10001)->IsDayLineNeedUpdate());
    EXPECT_TRUE(gl_pWorldMarket->GetStock(10010)->IsDayLineNeedUpdate());

    gl_pWorldMarket->SetFinnhubInquiring(false);
    EXPECT_TRUE(gl_pWorldMarket->TaskInquiryFinnhubDayLine());
    inquiry = gl_pWorldMarket->GetFinnhubInquiry();
    EXPECT_EQ(inquiry.m_lInquiryIndex, __STOCK_CANDLES__);
    EXPECT_EQ(inquiry.m_lStockIndex, 10010) << "第二个待查询股票位置";
    EXPECT_FALSE(gl_pWorldMarket->GetStock(1)->IsDayLineNeedUpdate());
    EXPECT_FALSE(gl_pWorldMarket->GetStock(10)->IsDayLineNeedUpdate());

    gl_pWorldMarket->SetFinnhubInquiring(false);
    EXPECT_FALSE(gl_pWorldMarket->TaskInquiryFinnhubDayLine()) << "第三次查询时没有找到待查询的股票";
    EXPECT_TRUE(gl_pWorldMarket->IsFinnhubDayLineUpdated()) << "股票都查询完了";
    CString str = gl_systemMessage.PopInformationMessage();
    EXPECT_STREQ(str, _T("US Market日线历史数据更新完毕"));

    // 恢复原状
    for (int i = 0; i < gl_pWorldMarket->GetTotalStock(); i++) {
      pStock = gl_pWorldMarket->GetStock(i);
      pStock->SetDayLineNeedUpdate(true);
    }
  }

  TEST_F(CWorldMarketTest, TestTaskInquiryFinnhubRTQuote) {
    WebInquiry inquiry;

    gl_pWorldMarket->SetSystemReady(true);

    gl_pWorldMarket->SetFinnhubDayLineUpdated(false);
    gl_pWorldMarket->SetFinnhubInquiring(true);
    EXPECT_FALSE(gl_pWorldMarket->TaskInquiryFinnhubRTQuote()) << "其他FinnhubInquiry正在进行";

    gl_pWorldMarket->SetFinnhubInquiring(false);
    EXPECT_TRUE(gl_pWorldMarket->TaskInquiryFinnhubRTQuote());
    EXPECT_TRUE(gl_pWorldMarket->IsFinnhubInquiring());
    inquiry = gl_pWorldMarket->GetFinnhubInquiry();
    EXPECT_EQ(inquiry.m_lInquiryIndex, __STOCK_QUOTE__);
    EXPECT_EQ(inquiry.m_iPriority, 10);
  }

  TEST_F(CWorldMarketTest, TestTaskInquiryFinnhubPeer) {
    CWorldStockPtr pStock;
    WebInquiry inquiry;

    gl_pWorldMarket->SetSystemReady(true);
    for (int i = 0; i < gl_pWorldMarket->GetTotalStock(); i++) {
      pStock = gl_pWorldMarket->GetStock(i);
      pStock->SetPeerUpdated(true);
    }
    gl_pWorldMarket->GetStock(1)->SetPeerUpdated(false); // 测试数据库中，上海市场的股票排在前面（共2462个），美国市场的股票排在后面
    gl_pWorldMarket->GetStock(10)->SetPeerUpdated(false);
    gl_pWorldMarket->SetFinnhubPeerUpdated(true);
    EXPECT_FALSE(gl_pWorldMarket->TaskInquiryFinnhubPeer()) << "Peers Updated";

    gl_pWorldMarket->SetFinnhubPeerUpdated(false);
    gl_pWorldMarket->SetFinnhubInquiring(true);
    EXPECT_FALSE(gl_pWorldMarket->TaskInquiryFinnhubPeer()) << "其他FinnhubInquiry正在进行";

    gl_pWorldMarket->SetFinnhubInquiring(false);
    EXPECT_TRUE(gl_pWorldMarket->TaskInquiryFinnhubPeer());
    EXPECT_TRUE(gl_pWorldMarket->IsFinnhubInquiring());
    inquiry = gl_pWorldMarket->GetFinnhubInquiry();
    EXPECT_EQ(inquiry.m_lInquiryIndex, __PEERS__);
    EXPECT_EQ(inquiry.m_lStockIndex, 1) << "第一个待查询股票位置";
    EXPECT_FALSE(gl_pWorldMarket->GetStock(1)->IsPeerUpdated()) << "此更新标识需要等待处理完数据后才设置";
    EXPECT_FALSE(gl_pWorldMarket->GetStock(10)->IsPeerUpdated());
    gl_pWorldMarket->GetStock(1)->SetPeerUpdated(true);

    gl_pWorldMarket->SetFinnhubInquiring(false);
    EXPECT_TRUE(gl_pWorldMarket->TaskInquiryFinnhubPeer());
    inquiry = gl_pWorldMarket->GetFinnhubInquiry();
    EXPECT_EQ(inquiry.m_lInquiryIndex, __PEERS__);
    EXPECT_EQ(inquiry.m_lStockIndex, 10) << "第二个待查询股票位置";
    EXPECT_TRUE(gl_pWorldMarket->GetStock(1)->IsPeerUpdated());
    EXPECT_FALSE(gl_pWorldMarket->GetStock(10)->IsPeerUpdated()) << "此更新标识需要等待处理完数据后才设置";
    gl_pWorldMarket->GetStock(10)->SetPeerUpdated(true);

    gl_pWorldMarket->SetFinnhubInquiring(false);
    EXPECT_FALSE(gl_pWorldMarket->TaskInquiryFinnhubPeer()) << "第三次查询时没有找到待查询的股票";
    EXPECT_TRUE(gl_pWorldMarket->IsFinnhubPeerUpdated()) << "股票都查询完了";
    CString str = gl_systemMessage.PopInformationMessage();
    EXPECT_STREQ(str, _T("Finnhub Peer Updated"));
  }

  TEST_F(CWorldMarketTest, TestTaskInquiryFinnhubEconomicCalender) {
    WebInquiry inquiry;

    gl_pWorldMarket->SetFinnhubEconomicCalendarUpdated(true);
    EXPECT_FALSE(gl_pWorldMarket->TaskInquiryFinnhubEconomicCalender()) << "EconomicCalendar Updated";

    gl_pWorldMarket->SetFinnhubEconomicCalendarUpdated(false);
    gl_pWorldMarket->SetFinnhubInquiring(true);
    EXPECT_FALSE(gl_pWorldMarket->TaskInquiryFinnhubEconomicCalender()) << "其他FinnhubInquiry正在进行";

    gl_pWorldMarket->SetFinnhubInquiring(false);
    EXPECT_TRUE(gl_pWorldMarket->TaskInquiryFinnhubEconomicCalender());
    EXPECT_TRUE(gl_pWorldMarket->IsFinnhubInquiring());
    inquiry = gl_pWorldMarket->GetFinnhubInquiry();
    EXPECT_EQ(inquiry.m_lInquiryIndex, __ECONOMIC_CALENDAR__);
    EXPECT_EQ(inquiry.m_iPriority, 10);
  }

  TEST_F(CWorldMarketTest, TestTaskInquiryFinnhubEPSSurprise) {
    CWorldStockPtr pStock;
    WebInquiry inquiry;

    gl_pWorldMarket->SetSystemReady(true);
    for (int i = 0; i < gl_pWorldMarket->GetTotalStock(); i++) {
      pStock = gl_pWorldMarket->GetStock(i);
      pStock->SetEPSSurpriseUpdated(true);
    }
    gl_pWorldMarket->GetStock(1)->SetEPSSurpriseUpdated(false); // 测试数据库中，上海市场的股票排在前面（共2462个），美国市场的股票排在后面
    gl_pWorldMarket->GetStock(10)->SetEPSSurpriseUpdated(false);
    gl_pWorldMarket->SetFinnhubEPSSurpriseUpdated(true);
    EXPECT_FALSE(gl_pWorldMarket->TaskInquiryFinnhubEPSSurprise()) << "Finnhub EPS Surprise  Updated";

    gl_pWorldMarket->SetFinnhubEPSSurpriseUpdated(false);
    gl_pWorldMarket->SetFinnhubInquiring(true);
    EXPECT_FALSE(gl_pWorldMarket->TaskInquiryFinnhubEPSSurprise()) << "其他FinnhubInquiry正在进行";

    gl_pWorldMarket->SetFinnhubInquiring(false);
    EXPECT_TRUE(gl_pWorldMarket->TaskInquiryFinnhubEPSSurprise());
    EXPECT_TRUE(gl_pWorldMarket->IsFinnhubInquiring());
    inquiry = gl_pWorldMarket->GetFinnhubInquiry();
    EXPECT_EQ(inquiry.m_lInquiryIndex, __STOCK_EPS_SURPRISE__);
    EXPECT_EQ(inquiry.m_lStockIndex, 1) << "第一个待查询股票位置";
    EXPECT_TRUE(gl_pWorldMarket->GetStock(1)->IsEPSSurpriseUpdated());
    EXPECT_FALSE(gl_pWorldMarket->GetStock(10)->IsEPSSurpriseUpdated());
    gl_pWorldMarket->GetStock(1)->SetEPSSurpriseUpdated(true);

    gl_pWorldMarket->SetFinnhubInquiring(false);
    EXPECT_TRUE(gl_pWorldMarket->TaskInquiryFinnhubEPSSurprise());
    inquiry = gl_pWorldMarket->GetFinnhubInquiry();
    EXPECT_EQ(inquiry.m_lInquiryIndex, __STOCK_EPS_SURPRISE__);
    EXPECT_EQ(inquiry.m_lStockIndex, 10) << "第二个待查询股票位置";
    EXPECT_TRUE(gl_pWorldMarket->GetStock(1)->IsEPSSurpriseUpdated());
    EXPECT_TRUE(gl_pWorldMarket->GetStock(10)->IsEPSSurpriseUpdated());
    gl_pWorldMarket->GetStock(10)->SetPeerUpdated(true);

    gl_pWorldMarket->SetFinnhubInquiring(false);
    EXPECT_FALSE(gl_pWorldMarket->TaskInquiryFinnhubEPSSurprise()) << "第三次查询时没有找到待查询的股票";
    EXPECT_TRUE(gl_pWorldMarket->IsFinnhubEPSSurpriseUpdated()) << "股票都查询完了";
    CString str = gl_systemMessage.PopInformationMessage();
    EXPECT_STREQ(str, _T("Finnhub EPS Surprise Updated"));
  }

  TEST_F(CWorldMarketTest, TestTaskInquiryFinnhubForexExchange) {
    WebInquiry inquiry;

    gl_pWorldMarket->SetFinnhubForexExchangeUpdated(true);
    EXPECT_FALSE(gl_pWorldMarket->TaskInquiryFinnhubForexExchange()) << "FinnhubForexExchange Updated";

    gl_pWorldMarket->SetFinnhubForexExchangeUpdated(false);
    gl_pWorldMarket->SetFinnhubInquiring(true);
    EXPECT_FALSE(gl_pWorldMarket->TaskInquiryFinnhubForexExchange()) << "其他FinnhubInquiry正在进行";

    gl_pWorldMarket->SetFinnhubInquiring(false);
    EXPECT_TRUE(gl_pWorldMarket->TaskInquiryFinnhubForexExchange());
    EXPECT_TRUE(gl_pWorldMarket->IsFinnhubInquiring());
    inquiry = gl_pWorldMarket->GetFinnhubInquiry();
    EXPECT_EQ(inquiry.m_lInquiryIndex, __FOREX_EXCHANGE__);
    EXPECT_EQ(inquiry.m_iPriority, 10);
    EXPECT_FALSE(gl_pWorldMarket->IsFinnhubForexExchangeUpdated()) << "此标识需要等处理完数据后方设置";
  }

  TEST_F(CWorldMarketTest, TestTaskInquiryFinnhubForexSymbol) {
    WebInquiry inquiry;

    gl_pWorldMarket->SetFinnhubForexSymbolUpdated(true);
    EXPECT_FALSE(gl_pWorldMarket->TaskInquiryFinnhubForexSymbol()) << "ForexSymbol Updated";

    gl_pWorldMarket->SetFinnhubForexSymbolUpdated(false);
    gl_pWorldMarket->SetFinnhubInquiring(true);
    EXPECT_FALSE(gl_pWorldMarket->TaskInquiryFinnhubForexSymbol()) << "其他FinnhubInquiry正在进行";

    size_t lTotal = gl_pWorldMarket->GetForexExchangeSize();
    for (int i = 0; i < lTotal - 1; i++) {
      gl_pWorldMarket->SetFinnhubInquiring(false);
      EXPECT_TRUE(gl_pWorldMarket->TaskInquiryFinnhubForexSymbol());
      EXPECT_TRUE(gl_pWorldMarket->IsFinnhubInquiring());
      inquiry = gl_pWorldMarket->GetFinnhubInquiry();
      EXPECT_EQ(inquiry.m_lInquiryIndex, __FOREX_SYMBOLS__);
      EXPECT_EQ(inquiry.m_lStockIndex, i);
      EXPECT_EQ(inquiry.m_iPriority, 10);
      EXPECT_FALSE(gl_pWorldMarket->IsFinnhubForexSymbolUpdated());
    }
    gl_pWorldMarket->SetFinnhubInquiring(false);
    EXPECT_TRUE(gl_pWorldMarket->TaskInquiryFinnhubForexSymbol());
    EXPECT_TRUE(gl_pWorldMarket->IsFinnhubInquiring());
    inquiry = gl_pWorldMarket->GetFinnhubInquiry();
    EXPECT_EQ(inquiry.m_lInquiryIndex, __FOREX_SYMBOLS__);
    EXPECT_EQ(inquiry.m_lStockIndex, lTotal - 1);
    EXPECT_EQ(inquiry.m_iPriority, 10);
    EXPECT_TRUE(gl_pWorldMarket->IsFinnhubForexSymbolUpdated());
    gl_pWorldMarket->SetFinnhubForexSymbolUpdated(false); //恢复原状
  }

  TEST_F(CWorldMarketTest, TestTaskInquiryFinnhubForexDayLine) {
    CForexSymbolPtr pStock;
    WebInquiry inquiry;

    gl_pWorldMarket->SetSystemReady(true);
    for (int i = 0; i < gl_pWorldMarket->GetForexSymbolSize(); i++) {
      pStock = gl_pWorldMarket->GetForexSymbol(i);
      pStock->SetDayLineNeedUpdate(false);
    }
    gl_pWorldMarket->GetForexSymbol(1)->SetDayLineNeedUpdate(true);
    gl_pWorldMarket->GetForexSymbol(10)->SetDayLineNeedUpdate(true);
    gl_pWorldMarket->SetFinnhubForexDayLineUpdated(true);
    EXPECT_FALSE(gl_pWorldMarket->TaskInquiryFinnhubForexDayLine()) << "Finnhub Forex Symbol DayLine Updated";

    gl_pWorldMarket->SetFinnhubForexDayLineUpdated(false);
    gl_pWorldMarket->SetFinnhubInquiring(true);
    EXPECT_FALSE(gl_pWorldMarket->TaskInquiryFinnhubForexDayLine()) << "其他FinnhubInquiry正在进行";

    gl_pWorldMarket->SetFinnhubInquiring(false);
    EXPECT_TRUE(gl_pWorldMarket->TaskInquiryFinnhubForexDayLine());
    EXPECT_TRUE(gl_pWorldMarket->IsFinnhubInquiring());
    inquiry = gl_pWorldMarket->GetFinnhubInquiry();
    EXPECT_EQ(inquiry.m_lInquiryIndex, __FOREX_CANDLES__);
    EXPECT_EQ(inquiry.m_lStockIndex, 1) << "第一个待查询股票位置";
    EXPECT_FALSE(gl_pWorldMarket->GetForexSymbol(1)->IsDayLineNeedUpdate());
    EXPECT_TRUE(gl_pWorldMarket->GetForexSymbol(10)->IsDayLineNeedUpdate());

    gl_pWorldMarket->SetFinnhubInquiring(false);
    EXPECT_TRUE(gl_pWorldMarket->TaskInquiryFinnhubForexDayLine());
    inquiry = gl_pWorldMarket->GetFinnhubInquiry();
    EXPECT_EQ(inquiry.m_lInquiryIndex, __FOREX_CANDLES__);
    EXPECT_EQ(inquiry.m_lStockIndex, 10) << "第二个待查询股票位置";
    EXPECT_FALSE(gl_pWorldMarket->GetForexSymbol(1)->IsDayLineNeedUpdate());
    EXPECT_FALSE(gl_pWorldMarket->GetForexSymbol(10)->IsDayLineNeedUpdate());

    gl_pWorldMarket->SetFinnhubInquiring(false);
    EXPECT_FALSE(gl_pWorldMarket->TaskInquiryFinnhubForexDayLine()) << "第三次查询时没有找到待查询的股票";
    EXPECT_TRUE(gl_pWorldMarket->IsFinnhubForexDayLineUpdated()) << "股票都查询完了";
    CString str = gl_systemMessage.PopInformationMessage();
    EXPECT_STREQ(str, _T("Forex DayLine Updated"));

    for (int i = 0; i < gl_pWorldMarket->GetForexSymbolSize(); i++) {
      pStock = gl_pWorldMarket->GetForexSymbol(i);
      pStock->SetDayLineNeedUpdate(true);
    }
  }

  TEST_F(CWorldMarketTest, TestTaskInquiryTiingoCompanySymbol) {
    WebInquiry inquiry;

    gl_pWorldMarket->SetTiingoSymbolUpdated(true);
    EXPECT_FALSE(gl_pWorldMarket->TaskInquiryTiingoCompanySymbol()) << "TiingoCompanySymbol Updated";

    gl_pWorldMarket->SetTiingoSymbolUpdated(false);
    gl_pWorldMarket->SetTiingoInquiring(true);
    EXPECT_FALSE(gl_pWorldMarket->TaskInquiryTiingoCompanySymbol()) << "其他TiingoInquiry正在进行";

    gl_pWorldMarket->SetTiingoInquiring(false);
    EXPECT_TRUE(gl_pWorldMarket->TaskInquiryTiingoCompanySymbol());
    EXPECT_TRUE(gl_pWorldMarket->IsTiingoInquiring());
    inquiry = gl_pWorldMarket->GetTiingoInquiry();
    EXPECT_EQ(inquiry.m_lInquiryIndex, __COMPANY_SYMBOLS__);
    EXPECT_EQ(inquiry.m_iPriority, 10);
    EXPECT_FALSE(gl_pWorldMarket->IsTiingoSymbolUpdated()) << "此标识需要等处理完数据后方设置";
    CString str = gl_systemMessage.PopInformationMessage();
    EXPECT_STREQ(str, _T("Inquiry Tiingo Symbol"));
  }

  TEST_F(CWorldMarketTest, TestTaskInquiryTiingoDayLine) {
    CWorldStockPtr pStock;
    WebInquiry inquiry;
    long lStockIndex = 0;

    gl_pWorldMarket->SetSystemReady(true);
    for (int i = 0; i < gl_pWorldMarket->GetTotalStock(); i++) {
      pStock = gl_pWorldMarket->GetStock(i);
      pStock->SetDayLineNeedUpdate(false);
    }
    gl_pWorldMarket->GetChoicedStock(1)->SetDayLineNeedUpdate(true);
    gl_pWorldMarket->GetChoicedStock(3)->SetDayLineNeedUpdate(true);
    gl_pWorldMarket->SetTiingoDayLineUpdated(true);
    EXPECT_FALSE(gl_pWorldMarket->TaskInquiryTiingoDayLine()) << "DayLine Updated";

    gl_pWorldMarket->SetTiingoDayLineUpdated(false);
    gl_pWorldMarket->SetTiingoInquiring(true);
    EXPECT_FALSE(gl_pWorldMarket->TaskInquiryTiingoDayLine()) << "其他TiingoInquiry正在进行";

    gl_pWorldMarket->SetTiingoInquiring(false);
    EXPECT_TRUE(gl_pWorldMarket->TaskInquiryTiingoDayLine());
    EXPECT_TRUE(gl_pWorldMarket->IsTiingoInquiring());
    lStockIndex = gl_pWorldMarket->GetStockIndex(gl_pWorldMarket->GetChoicedStock(1)->GetSymbol());
    inquiry = gl_pWorldMarket->GetTiingoInquiry();
    EXPECT_EQ(inquiry.m_lInquiryIndex, __STOCK_CANDLES__);
    EXPECT_EQ(inquiry.m_lStockIndex, lStockIndex) << "第一个待查询股票位置";
    EXPECT_FALSE(gl_pWorldMarket->GetChoicedStock(1)->IsDayLineNeedUpdate());
    EXPECT_TRUE(gl_pWorldMarket->GetChoicedStock(3)->IsDayLineNeedUpdate());

    gl_pWorldMarket->SetTiingoInquiring(false);
    EXPECT_TRUE(gl_pWorldMarket->TaskInquiryTiingoDayLine());
    lStockIndex = gl_pWorldMarket->GetStockIndex(gl_pWorldMarket->GetChoicedStock(3)->GetSymbol());
    inquiry = gl_pWorldMarket->GetTiingoInquiry();
    EXPECT_EQ(inquiry.m_lInquiryIndex, __STOCK_CANDLES__);
    EXPECT_EQ(inquiry.m_lStockIndex, lStockIndex) << "第二个待查询股票位置";
    EXPECT_FALSE(gl_pWorldMarket->GetChoicedStock(1)->IsDayLineNeedUpdate());
    EXPECT_FALSE(gl_pWorldMarket->GetChoicedStock(3)->IsDayLineNeedUpdate());

    gl_pWorldMarket->SetTiingoInquiring(false);
    EXPECT_FALSE(gl_pWorldMarket->TaskInquiryTiingoDayLine()) << "第三次查询时没有找到待查询的股票";
    EXPECT_TRUE(gl_pWorldMarket->IsTiingoDayLineUpdated()) << "股票都查询完了";
    CString str = gl_systemMessage.PopInformationMessage();
    EXPECT_STREQ(str, _T("美国市场自选股票日线历史数据更新完毕"));

    // 恢复原状
    for (int i = 0; i < gl_pWorldMarket->GetTotalStock(); i++) {
      pStock = gl_pWorldMarket->GetStock(i);
      pStock->SetDayLineNeedUpdate(true);
    }
  }
}