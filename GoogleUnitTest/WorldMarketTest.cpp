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
      // clearu
    }

  protected:
  };

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
}