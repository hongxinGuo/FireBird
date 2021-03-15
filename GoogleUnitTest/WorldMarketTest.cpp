#include"pch.h"
#include"globedef.h"
#include"WebInquirer.h"

#include"WorldStock.h"
#include"WorldMarket.h"

#include"SetFinnhubForexExchange.h"

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
      gl_pWorldMarket->SetFinnhubForexDayLineUpdated(false);
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
      gl_pWorldMarket->SetFinnhubForexDayLineUpdated(false);

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
    EXPECT_FALSE(gl_pWorldMarket->TaskResetMarket(101010)) << "����������ϵͳʱ����Զ���ؼ�";

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

  TEST_F(CWorldMarketTest, TestGetCurrentFinnhubInquiry) {
    WebInquiry inquiry, inquiry2;
    inquiry.m_iPriority = 10;
    inquiry.m_lInquiryIndex = 20;
    inquiry.m_lStockIndex = 30;
    gl_pWorldMarket->SetCurrentFinnhubInquiry(inquiry);
    inquiry2 = gl_pWorldMarket->GetCurrentFinnhubInquiry();
    EXPECT_EQ(inquiry.m_iPriority, inquiry2.m_iPriority);
    EXPECT_EQ(inquiry.m_lInquiryIndex, inquiry2.m_lInquiryIndex);
    EXPECT_EQ(inquiry.m_lStockIndex, inquiry2.m_lStockIndex);
  }

  TEST_F(CWorldMarketTest, TestGetCurrentTiingoInquiry) {
    WebInquiry inquiry, inquiry2;
    inquiry.m_iPriority = 10;
    inquiry.m_lInquiryIndex = 20;
    inquiry.m_lStockIndex = 30;
    gl_pWorldMarket->SetCurrentTiingoInquiry(inquiry);
    inquiry2 = gl_pWorldMarket->GetCurrentTiingoInquiry();
    EXPECT_EQ(inquiry.m_iPriority, inquiry2.m_iPriority);
    EXPECT_EQ(inquiry.m_lInquiryIndex, inquiry2.m_lInquiryIndex);
    EXPECT_EQ(inquiry.m_lStockIndex, inquiry2.m_lStockIndex);
  }

  TEST_F(CWorldMarketTest, TestGetCurrentQuandlInquiry) {
    WebInquiry inquiry, inquiry2;
    inquiry.m_iPriority = 10;
    inquiry.m_lInquiryIndex = 20;
    inquiry.m_lStockIndex = 30;
    gl_pWorldMarket->SetCurrentQuandlInquiry(inquiry);
    inquiry2 = gl_pWorldMarket->GetCurrentQuandlInquiry();
    EXPECT_EQ(inquiry.m_iPriority, inquiry2.m_iPriority);
    EXPECT_EQ(inquiry.m_lInquiryIndex, inquiry2.m_lInquiryIndex);
    EXPECT_EQ(inquiry.m_lStockIndex, inquiry2.m_lStockIndex);
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
    EXPECT_EQ(gl_pWorldMarket->GetTotalStock(), 5059) << "Ĭ��״̬�����ݿ�����Ϊ5059(ȫ���Ϻ���Ʊ��С����������Ʊ)";
  }

  TEST_F(CWorldMarketTest, TestIsStock) {
    EXPECT_FALSE(gl_pWorldMarket->IsStock(_T("000000.SS")));
    EXPECT_TRUE(gl_pWorldMarket->IsStock(_T("000001.SS")));
    EXPECT_TRUE(gl_pWorldMarket->IsStock(_T("600601.SS")));
    EXPECT_TRUE(gl_pWorldMarket->IsStock(_T("A")));
    EXPECT_FALSE(gl_pWorldMarket->IsStock(_T("000001.SZ"))) << "Ŀǰ�������ݿ���ֻ���Ϻ���������Ʊ��";

    CWorldStockPtr pStock = make_shared<CWorldStock>();
    pStock->SetSymbol(_T("000000.SS"));
    EXPECT_FALSE(gl_pWorldMarket->IsStock(pStock));
    pStock->SetSymbol(_T("000001.SS"));
    EXPECT_TRUE(gl_pWorldMarket->IsStock(pStock));
    pStock->SetSymbol(_T("600601.SS"));
    EXPECT_TRUE(gl_pWorldMarket->IsStock(pStock));
    pStock->SetSymbol(_T("A"));
    EXPECT_TRUE(gl_pWorldMarket->IsStock(pStock));
    pStock->SetSymbol(_T("000001.SZ"));
    EXPECT_FALSE(gl_pWorldMarket->IsStock(pStock));
  }

  TEST_F(CWorldMarketTest, TestIsUpdateProfileDB) {
    CWorldStockPtr pStock = nullptr;
    for (int i = 0; i < gl_pWorldMarket->GetTotalStock(); i++) {
      pStock = gl_pWorldMarket->GetStock(i);
      pStock->SetUpdateProfileDB(false);
    }
    EXPECT_FALSE(gl_pWorldMarket->IsStockProfileNeedUpdate());

    gl_pWorldMarket->GetStock(0)->SetUpdateProfileDB(true);
    EXPECT_TRUE(gl_pWorldMarket->IsStockProfileNeedUpdate());

    gl_pWorldMarket->GetStock(0)->SetUpdateProfileDB(false);
    EXPECT_FALSE(gl_pWorldMarket->IsStockProfileNeedUpdate());
  }

  TEST_F(CWorldMarketTest, TestAddStock) {
    CWorldStockPtr pStock = make_shared<CWorldStock>();
    long lTotalStock = gl_pWorldMarket->GetTotalStock();
    pStock->SetSymbol(_T("000001.SZ"));

    EXPECT_FALSE(gl_pWorldMarket->IsStock(pStock));
    gl_pWorldMarket->AddStock(pStock);
    EXPECT_TRUE(gl_pWorldMarket->IsStock(pStock));
    EXPECT_EQ(gl_pWorldMarket->GetTotalStock(), lTotalStock + 1);
    gl_pWorldMarket->DeleteStock(pStock);
    EXPECT_FALSE(gl_pWorldMarket->IsStock(pStock));
    EXPECT_EQ(gl_pWorldMarket->GetTotalStock(), lTotalStock);
  }

  TEST_F(CWorldMarketTest, TestDeleteStock) {
    // do nothing. �Ѿ���TestAddStock�в�����DeleteStock����
    CWorldStockPtr pStock = nullptr;

    EXPECT_FALSE(gl_pWorldMarket->DeleteStock(pStock)) << "��ָ��";

    pStock = make_shared<CWorldStock>();
    pStock->SetSymbol(_T("000001.SZ"));
    EXPECT_FALSE(gl_pWorldMarket->DeleteStock(pStock)) << "�˹�Ʊ���벻�����ڴ��뼯��";
  }

  TEST_F(CWorldMarketTest, TestGetStock) {
    CWorldStockPtr pStock = gl_pWorldMarket->GetStock(0); // 000001.SS
    EXPECT_STREQ(pStock->GetSymbol(), _T("000001.SS")) << "��һ����Ʊ����Ϊ000001.SS";
    pStock = gl_pWorldMarket->GetStock(_T("000001.SS"));
    EXPECT_FALSE(pStock == nullptr);
    EXPECT_STREQ(pStock->GetDescription(), _T("SSE Composite Index"));
  }

  TEST_F(CWorldMarketTest, TestIsForexExchange) {
    EXPECT_FALSE(gl_pWorldMarket->IsForexExchange(_T("ABC")));
    EXPECT_TRUE(gl_pWorldMarket->IsForexExchange(_T("forex.com")));
    EXPECT_TRUE(gl_pWorldMarket->IsForexExchange(_T("ic markets")));
  }

  TEST_F(CWorldMarketTest, TestAddForexExchange) {
    long lTotalForexExchange = gl_pWorldMarket->GetForexExchangeSize();
    CString strForexExchange = _T("000001.SZ");

    EXPECT_FALSE(gl_pWorldMarket->IsForexExchange(strForexExchange));
    gl_pWorldMarket->AddForexExchange(strForexExchange);
    EXPECT_TRUE(gl_pWorldMarket->IsForexExchange(strForexExchange));
    EXPECT_EQ(gl_pWorldMarket->GetForexExchangeSize(), lTotalForexExchange + 1);
    gl_pWorldMarket->DeleteForexExchange(strForexExchange);
    EXPECT_FALSE(gl_pWorldMarket->IsForexExchange(strForexExchange));
    EXPECT_EQ(gl_pWorldMarket->GetForexExchangeSize(), lTotalForexExchange);
  }

  TEST_F(CWorldMarketTest, TestDeleteForexExchange) {
    // do nothing. �Ѿ���TestAddForexExchange�в�����DeleteForexExchange����
    EXPECT_FALSE(gl_pWorldMarket->DeleteForexExchange(_T("US.US.US"))) << "�˷����ڷ��ż��в�����";
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

  TEST_F(CWorldMarketTest, TestAddForexSymbol) {
    CForexSymbolPtr pForexSymbol = make_shared<CFinnhubForexSymbol>();
    long lTotalForexSymbol = gl_pWorldMarket->GetForexSymbolSize();
    pForexSymbol->SetSymbol(_T("000001.SZ"));

    EXPECT_FALSE(gl_pWorldMarket->IsForexSymbol(pForexSymbol));
    gl_pWorldMarket->AddForexSymbol(pForexSymbol);
    EXPECT_TRUE(gl_pWorldMarket->IsForexSymbol(pForexSymbol));
    EXPECT_EQ(gl_pWorldMarket->GetForexSymbolSize(), lTotalForexSymbol + 1);
    EXPECT_TRUE(gl_pWorldMarket->DeleteForexSymbol(pForexSymbol));
    EXPECT_FALSE(gl_pWorldMarket->IsForexSymbol(pForexSymbol));
    EXPECT_EQ(gl_pWorldMarket->GetForexSymbolSize(), lTotalForexSymbol);
  }

  TEST_F(CWorldMarketTest, TestDeleteForexSymbol) {
    // do nothing. �Ѿ���TestAddForexSymbol�в�����DeleteForexSymbol����
    CForexSymbolPtr pForexSymbol = nullptr;

    EXPECT_FALSE(gl_pWorldMarket->DeleteForexSymbol(pForexSymbol)) << "��ָ��";

    pForexSymbol = make_shared<CFinnhubForexSymbol>();
    pForexSymbol->SetSymbol(_T("000001.SZ"));
    EXPECT_FALSE(gl_pWorldMarket->DeleteForexSymbol(pForexSymbol)) << "�˷����ڷ��ż��в�����";
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

  TEST_F(CWorldMarketTest, TestAddCountry) {
    CCountryPtr pCountry = make_shared<CCountry>();
    long lTotalCountry = gl_pWorldMarket->GetTotalCountry();
    pCountry->m_strCountry = _T("SZ");

    EXPECT_FALSE(gl_pWorldMarket->IsCountry(pCountry));
    gl_pWorldMarket->AddCountry(pCountry);
    EXPECT_TRUE(gl_pWorldMarket->IsCountry(pCountry));
    EXPECT_EQ(gl_pWorldMarket->GetTotalCountry(), lTotalCountry + 1);
    gl_pWorldMarket->DeleteCountry(pCountry);
    EXPECT_FALSE(gl_pWorldMarket->IsCountry(pCountry));
    EXPECT_EQ(gl_pWorldMarket->GetTotalCountry(), lTotalCountry);
  }

  TEST_F(CWorldMarketTest, TestDeleteCountry) {
    // do nothing. �Ѿ���TestAddCountry�в�����DeleteCountry����

    CCountryPtr pCountry = nullptr;

    EXPECT_FALSE(gl_pWorldMarket->DeleteCountry(pCountry)) << "��ָ��";

    pCountry = make_shared<CCountry>();
    pCountry->m_strCountry = _T("SZ");
    EXPECT_FALSE(gl_pWorldMarket->DeleteCountry(pCountry)) << "�˹�Ʊ���벻�����ڴ��뼯��";
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
    // �ݻ�
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
    gl_pWorldMarket->UpdateCountryListDB(); // �˲��Ժ���ִ�����������һ��Countryû��ɾ�������ݿ��е�ɾ���ˣ���

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
    EXPECT_FALSE(gl_pWorldMarket->IsStock(pStock)); // ȷ����һ���¹�Ʊ����
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
    gl_pWorldMarket->DeleteStock(pStock); // �ָ�ԭ״
  }

  TEST_F(CWorldMarketTest, TaskUpdateForexExchangeDB) {
    CString strSymbol = _T("US.US.US");

    EXPECT_FALSE(gl_pWorldMarket->TaskUpdateForexExchangeDB()) << "û����Forex Exchange";

    EXPECT_FALSE(gl_pWorldMarket->IsForexExchange(strSymbol)); // ȷ����һ���¹�Ʊ����
    gl_pWorldMarket->AddForexExchange(strSymbol);
    EXPECT_TRUE(gl_pWorldMarket->TaskUpdateForexExchangeDB());

    CSetFinnhubForexExchange setForexExchange;
    setForexExchange.m_strFilter = _T("[Exchange] = 'US.US.US'");
    setForexExchange.Open();
    EXPECT_FALSE(setForexExchange.IsEOF());
    setForexExchange.m_pDatabase->BeginTrans();
    while (!setForexExchange.IsEOF()) {
      setForexExchange.Delete();
      setForexExchange.MoveNext();
    }
    setForexExchange.m_pDatabase->CommitTrans();
    setForexExchange.Close();

    gl_pWorldMarket->DeleteForexExchange(strSymbol); // �ָ�ԭ״
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
    EXPECT_FALSE(gl_pWorldMarket->TaskInquiryFinnhubCompanySymbol()) << "����FinnhubInquiry���ڽ���";

    gl_pWorldMarket->SetFinnhubInquiring(false);
    EXPECT_TRUE(gl_pWorldMarket->TaskInquiryFinnhubCompanySymbol());
    EXPECT_TRUE(gl_pWorldMarket->IsFinnhubInquiring());
    inquiry = gl_pWorldMarket->GetFinnhubInquiry();
    EXPECT_EQ(inquiry.m_lInquiryIndex, __COMPANY_SYMBOLS__);
    EXPECT_EQ(inquiry.m_lStockIndex, 1) << "��һ������ѯ����������";
    EXPECT_TRUE(gl_pWorldMarket->GetExchange(1)->m_fUpdated);
    EXPECT_FALSE(gl_pWorldMarket->GetExchange(10)->m_fUpdated);

    gl_pWorldMarket->SetFinnhubInquiring(false);
    EXPECT_TRUE(gl_pWorldMarket->TaskInquiryFinnhubCompanySymbol());
    inquiry = gl_pWorldMarket->GetFinnhubInquiry();
    EXPECT_EQ(inquiry.m_lInquiryIndex, __COMPANY_SYMBOLS__);
    EXPECT_EQ(inquiry.m_lStockIndex, 10) << "�ڶ�������ѯ����������";
    EXPECT_TRUE(gl_pWorldMarket->GetExchange(1)->m_fUpdated);
    EXPECT_TRUE(gl_pWorldMarket->GetExchange(10)->m_fUpdated);

    gl_pWorldMarket->SetFinnhubInquiring(false);
    EXPECT_FALSE(gl_pWorldMarket->TaskInquiryFinnhubCompanySymbol()) << "�����β�ѯʱû���ҵ�����ѯ�Ľ�����";
    EXPECT_TRUE(gl_pWorldMarket->IsFinnhubSymbolUpdated()) << "����������ѯ����";
    CString str = gl_systemMessage.PopInformationMessage();
    EXPECT_STREQ(str, _T("�������������ݲ�ѯ���"));
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
    EXPECT_FALSE(gl_pWorldMarket->TaskInquiryFinnhubCompanyProfileConcise()) << "����FinnhubInquiry���ڽ���";

    gl_pWorldMarket->SetFinnhubInquiring(false);
    EXPECT_TRUE(gl_pWorldMarket->TaskInquiryFinnhubCompanyProfileConcise());
    EXPECT_TRUE(gl_pWorldMarket->IsFinnhubInquiring());
    inquiry = gl_pWorldMarket->GetFinnhubInquiry();
    EXPECT_EQ(inquiry.m_lInquiryIndex, __COMPANY_PROFILE_CONCISE__);
    EXPECT_EQ(inquiry.m_lStockIndex, 1) << "��һ������ѯ��Ʊλ��";
    EXPECT_FALSE(gl_pWorldMarket->GetStock(1)->IsProfileUpdated()) << "�˸��±�ʶ��Ҫ�ȴ����������ݺ������";
    EXPECT_FALSE(gl_pWorldMarket->GetStock(10)->IsProfileUpdated());
    gl_pWorldMarket->GetStock(1)->SetProfileUpdated(true);

    gl_pWorldMarket->SetFinnhubInquiring(false);
    EXPECT_TRUE(gl_pWorldMarket->TaskInquiryFinnhubCompanyProfileConcise());
    inquiry = gl_pWorldMarket->GetFinnhubInquiry();
    EXPECT_EQ(inquiry.m_lInquiryIndex, __COMPANY_PROFILE_CONCISE__);
    EXPECT_EQ(inquiry.m_lStockIndex, 10) << "�ڶ�������ѯ��Ʊλ��";
    EXPECT_TRUE(gl_pWorldMarket->GetStock(1)->IsProfileUpdated());
    EXPECT_FALSE(gl_pWorldMarket->GetStock(10)->IsProfileUpdated()) << "�˸��±�ʶ��Ҫ�ȴ����������ݺ������";
    gl_pWorldMarket->GetStock(10)->SetProfileUpdated(true);

    gl_pWorldMarket->SetFinnhubInquiring(false);
    EXPECT_FALSE(gl_pWorldMarket->TaskInquiryFinnhubCompanyProfileConcise()) << "�����β�ѯʱû���ҵ�����ѯ�Ĺ�Ʊ";
    EXPECT_TRUE(gl_pWorldMarket->IsFinnhubStockProfileUpdated()) << "��Ʊ����ѯ����";
    CString str = gl_systemMessage.PopInformationMessage();
    EXPECT_STREQ(str, _T("Finnhub��Ʊ���������"));
  }

  TEST_F(CWorldMarketTest, TestTaskInquiryFinnhubDayLine) {
    CWorldStockPtr pStock;
    WebInquiry inquiry;

    gl_pWorldMarket->SetSystemReady(true);
    for (int i = 0; i < gl_pWorldMarket->GetTotalStock(); i++) {
      pStock = gl_pWorldMarket->GetStock(i);
      pStock->SetDayLineNeedUpdate(false);
    }
    gl_pWorldMarket->GetStock(3001)->SetDayLineNeedUpdate(true); // �������ݿ��У��Ϻ��г��Ĺ�Ʊ����ǰ�棨��2462�����������г��Ĺ�Ʊ���ں���
    gl_pWorldMarket->GetStock(3010)->SetDayLineNeedUpdate(true);
    gl_pWorldMarket->SetFinnhubDayLineUpdated(true);
    EXPECT_FALSE(gl_pWorldMarket->TaskInquiryFinnhubDayLine()) << "DayLine Updated";

    gl_pWorldMarket->SetFinnhubDayLineUpdated(false);
    gl_pWorldMarket->SetFinnhubInquiring(true);
    EXPECT_FALSE(gl_pWorldMarket->TaskInquiryFinnhubDayLine()) << "����FinnhubInquiry���ڽ���";

    gl_pWorldMarket->SetFinnhubInquiring(false);
    EXPECT_TRUE(gl_pWorldMarket->TaskInquiryFinnhubDayLine());
    EXPECT_TRUE(gl_pWorldMarket->IsFinnhubInquiring());
    inquiry = gl_pWorldMarket->GetFinnhubInquiry();
    EXPECT_EQ(inquiry.m_lInquiryIndex, __STOCK_CANDLES__);
    EXPECT_EQ(inquiry.m_lStockIndex, 3001) << "��һ������ѯ��Ʊλ��";
    EXPECT_FALSE(gl_pWorldMarket->GetStock(3001)->IsDayLineNeedUpdate());
    EXPECT_TRUE(gl_pWorldMarket->GetStock(3010)->IsDayLineNeedUpdate());

    gl_pWorldMarket->SetFinnhubInquiring(false);
    EXPECT_TRUE(gl_pWorldMarket->TaskInquiryFinnhubDayLine());
    inquiry = gl_pWorldMarket->GetFinnhubInquiry();
    EXPECT_EQ(inquiry.m_lInquiryIndex, __STOCK_CANDLES__);
    EXPECT_EQ(inquiry.m_lStockIndex, 3010) << "�ڶ�������ѯ��Ʊλ��";
    EXPECT_FALSE(gl_pWorldMarket->GetStock(3001)->IsDayLineNeedUpdate());
    EXPECT_FALSE(gl_pWorldMarket->GetStock(3010)->IsDayLineNeedUpdate());

    gl_pWorldMarket->SetFinnhubInquiring(false);
    EXPECT_FALSE(gl_pWorldMarket->TaskInquiryFinnhubDayLine()) << "�����β�ѯʱû���ҵ�����ѯ�Ĺ�Ʊ";
    EXPECT_TRUE(gl_pWorldMarket->IsFinnhubDayLineUpdated()) << "��Ʊ����ѯ����";
    CString str = gl_systemMessage.PopInformationMessage();
    EXPECT_STREQ(str, _T("US Market������ʷ���ݸ������"));

    // �ָ�ԭ״
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
    EXPECT_FALSE(gl_pWorldMarket->TaskInquiryFinnhubRTQuote()) << "����FinnhubInquiry���ڽ���";

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
    gl_pWorldMarket->GetStock(1)->SetPeerUpdated(false); // �������ݿ��У��Ϻ��г��Ĺ�Ʊ����ǰ�棨��2462�����������г��Ĺ�Ʊ���ں���
    gl_pWorldMarket->GetStock(10)->SetPeerUpdated(false);
    gl_pWorldMarket->SetFinnhubPeerUpdated(true);
    EXPECT_FALSE(gl_pWorldMarket->TaskInquiryFinnhubPeer()) << "Peers Updated";

    gl_pWorldMarket->SetFinnhubPeerUpdated(false);
    gl_pWorldMarket->SetFinnhubInquiring(true);
    EXPECT_FALSE(gl_pWorldMarket->TaskInquiryFinnhubPeer()) << "����FinnhubInquiry���ڽ���";

    gl_pWorldMarket->SetFinnhubInquiring(false);
    EXPECT_TRUE(gl_pWorldMarket->TaskInquiryFinnhubPeer());
    EXPECT_TRUE(gl_pWorldMarket->IsFinnhubInquiring());
    inquiry = gl_pWorldMarket->GetFinnhubInquiry();
    EXPECT_EQ(inquiry.m_lInquiryIndex, __PEERS__);
    EXPECT_EQ(inquiry.m_lStockIndex, 1) << "��һ������ѯ��Ʊλ��";
    EXPECT_FALSE(gl_pWorldMarket->GetStock(1)->IsPeerUpdated()) << "�˸��±�ʶ��Ҫ�ȴ����������ݺ������";
    EXPECT_FALSE(gl_pWorldMarket->GetStock(10)->IsPeerUpdated());
    gl_pWorldMarket->GetStock(1)->SetPeerUpdated(true);

    gl_pWorldMarket->SetFinnhubInquiring(false);
    EXPECT_TRUE(gl_pWorldMarket->TaskInquiryFinnhubPeer());
    inquiry = gl_pWorldMarket->GetFinnhubInquiry();
    EXPECT_EQ(inquiry.m_lInquiryIndex, __PEERS__);
    EXPECT_EQ(inquiry.m_lStockIndex, 10) << "�ڶ�������ѯ��Ʊλ��";
    EXPECT_TRUE(gl_pWorldMarket->GetStock(1)->IsPeerUpdated());
    EXPECT_FALSE(gl_pWorldMarket->GetStock(10)->IsPeerUpdated()) << "�˸��±�ʶ��Ҫ�ȴ����������ݺ������";
    gl_pWorldMarket->GetStock(10)->SetPeerUpdated(true);

    gl_pWorldMarket->SetFinnhubInquiring(false);
    EXPECT_FALSE(gl_pWorldMarket->TaskInquiryFinnhubPeer()) << "�����β�ѯʱû���ҵ�����ѯ�Ĺ�Ʊ";
    EXPECT_TRUE(gl_pWorldMarket->IsFinnhubPeerUpdated()) << "��Ʊ����ѯ����";
    CString str = gl_systemMessage.PopInformationMessage();
    EXPECT_STREQ(str, _T("Finnhub Peer Updated"));
  }

  TEST_F(CWorldMarketTest, TestTaskInquiryFinnhubEconomicCalendar) {
    WebInquiry inquiry;

    gl_pWorldMarket->SetFinnhubEconomicCalendarUpdated(true);
    EXPECT_FALSE(gl_pWorldMarket->TaskInquiryFinnhubEconomicCalendar()) << "EconomicCalendar Updated";

    gl_pWorldMarket->SetFinnhubEconomicCalendarUpdated(false);
    gl_pWorldMarket->SetFinnhubInquiring(true);
    EXPECT_FALSE(gl_pWorldMarket->TaskInquiryFinnhubEconomicCalendar()) << "����FinnhubInquiry���ڽ���";

    gl_pWorldMarket->SetFinnhubInquiring(false);
    EXPECT_TRUE(gl_pWorldMarket->TaskInquiryFinnhubEconomicCalendar());
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
    gl_pWorldMarket->GetStock(1)->SetEPSSurpriseUpdated(false); // �������ݿ��У��Ϻ��г��Ĺ�Ʊ����ǰ�棨��2462�����������г��Ĺ�Ʊ���ں���
    gl_pWorldMarket->GetStock(10)->SetEPSSurpriseUpdated(false);
    gl_pWorldMarket->SetFinnhubEPSSurpriseUpdated(true);
    EXPECT_FALSE(gl_pWorldMarket->TaskInquiryFinnhubEPSSurprise()) << "Finnhub EPS Surprise  Updated";

    gl_pWorldMarket->SetFinnhubEPSSurpriseUpdated(false);
    gl_pWorldMarket->SetFinnhubInquiring(true);
    EXPECT_FALSE(gl_pWorldMarket->TaskInquiryFinnhubEPSSurprise()) << "����FinnhubInquiry���ڽ���";

    gl_pWorldMarket->SetFinnhubInquiring(false);
    EXPECT_TRUE(gl_pWorldMarket->TaskInquiryFinnhubEPSSurprise());
    EXPECT_TRUE(gl_pWorldMarket->IsFinnhubInquiring());
    inquiry = gl_pWorldMarket->GetFinnhubInquiry();
    EXPECT_EQ(inquiry.m_lInquiryIndex, __STOCK_EPS_SURPRISE__);
    EXPECT_EQ(inquiry.m_lStockIndex, 1) << "��һ������ѯ��Ʊλ��";
    EXPECT_TRUE(gl_pWorldMarket->GetStock(1)->IsEPSSurpriseUpdated());
    EXPECT_FALSE(gl_pWorldMarket->GetStock(10)->IsEPSSurpriseUpdated());
    gl_pWorldMarket->GetStock(1)->SetEPSSurpriseUpdated(true);

    gl_pWorldMarket->SetFinnhubInquiring(false);
    EXPECT_TRUE(gl_pWorldMarket->TaskInquiryFinnhubEPSSurprise());
    inquiry = gl_pWorldMarket->GetFinnhubInquiry();
    EXPECT_EQ(inquiry.m_lInquiryIndex, __STOCK_EPS_SURPRISE__);
    EXPECT_EQ(inquiry.m_lStockIndex, 10) << "�ڶ�������ѯ��Ʊλ��";
    EXPECT_TRUE(gl_pWorldMarket->GetStock(1)->IsEPSSurpriseUpdated());
    EXPECT_TRUE(gl_pWorldMarket->GetStock(10)->IsEPSSurpriseUpdated());
    gl_pWorldMarket->GetStock(10)->SetPeerUpdated(true);

    gl_pWorldMarket->SetFinnhubInquiring(false);
    EXPECT_FALSE(gl_pWorldMarket->TaskInquiryFinnhubEPSSurprise()) << "�����β�ѯʱû���ҵ�����ѯ�Ĺ�Ʊ";
    EXPECT_TRUE(gl_pWorldMarket->IsFinnhubEPSSurpriseUpdated()) << "��Ʊ����ѯ����";
    CString str = gl_systemMessage.PopInformationMessage();
    EXPECT_STREQ(str, _T("Finnhub EPS Surprise Updated"));
  }

  TEST_F(CWorldMarketTest, TestTaskInquiryFinnhubForexExchange) {
    WebInquiry inquiry;

    gl_pWorldMarket->SetFinnhubForexExchangeUpdated(true);
    EXPECT_FALSE(gl_pWorldMarket->TaskInquiryFinnhubForexExchange()) << "FinnhubForexExchange Updated";

    gl_pWorldMarket->SetFinnhubForexExchangeUpdated(false);
    gl_pWorldMarket->SetFinnhubInquiring(true);
    EXPECT_FALSE(gl_pWorldMarket->TaskInquiryFinnhubForexExchange()) << "����FinnhubInquiry���ڽ���";

    gl_pWorldMarket->SetFinnhubInquiring(false);
    EXPECT_TRUE(gl_pWorldMarket->TaskInquiryFinnhubForexExchange());
    EXPECT_TRUE(gl_pWorldMarket->IsFinnhubInquiring());
    inquiry = gl_pWorldMarket->GetFinnhubInquiry();
    EXPECT_EQ(inquiry.m_lInquiryIndex, __FOREX_EXCHANGE__);
    EXPECT_EQ(inquiry.m_iPriority, 10);
    EXPECT_FALSE(gl_pWorldMarket->IsFinnhubForexExchangeUpdated()) << "�˱�ʶ��Ҫ�ȴ��������ݺ�����";
  }

  TEST_F(CWorldMarketTest, TestTaskInquiryFinnhubForexSymbol) {
    WebInquiry inquiry;

    gl_pWorldMarket->SetFinnhubForexSymbolUpdated(true);
    EXPECT_FALSE(gl_pWorldMarket->TaskInquiryFinnhubForexSymbol()) << "ForexSymbol Updated";

    gl_pWorldMarket->SetFinnhubForexSymbolUpdated(false);
    gl_pWorldMarket->SetFinnhubInquiring(true);
    EXPECT_FALSE(gl_pWorldMarket->TaskInquiryFinnhubForexSymbol()) << "����FinnhubInquiry���ڽ���";

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
    gl_pWorldMarket->SetFinnhubForexSymbolUpdated(false); //�ָ�ԭ״
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
    EXPECT_FALSE(gl_pWorldMarket->TaskInquiryFinnhubForexDayLine()) << "����FinnhubInquiry���ڽ���";

    gl_pWorldMarket->SetFinnhubInquiring(false);
    EXPECT_TRUE(gl_pWorldMarket->TaskInquiryFinnhubForexDayLine());
    EXPECT_TRUE(gl_pWorldMarket->IsFinnhubInquiring());
    inquiry = gl_pWorldMarket->GetFinnhubInquiry();
    EXPECT_EQ(inquiry.m_lInquiryIndex, __FOREX_CANDLES__);
    EXPECT_EQ(inquiry.m_lStockIndex, 1) << "��һ������ѯ��Ʊλ��";
    EXPECT_FALSE(gl_pWorldMarket->GetForexSymbol(1)->IsDayLineNeedUpdate());
    EXPECT_TRUE(gl_pWorldMarket->GetForexSymbol(10)->IsDayLineNeedUpdate());

    gl_pWorldMarket->SetFinnhubInquiring(false);
    EXPECT_TRUE(gl_pWorldMarket->TaskInquiryFinnhubForexDayLine());
    inquiry = gl_pWorldMarket->GetFinnhubInquiry();
    EXPECT_EQ(inquiry.m_lInquiryIndex, __FOREX_CANDLES__);
    EXPECT_EQ(inquiry.m_lStockIndex, 10) << "�ڶ�������ѯ��Ʊλ��";
    EXPECT_FALSE(gl_pWorldMarket->GetForexSymbol(1)->IsDayLineNeedUpdate());
    EXPECT_FALSE(gl_pWorldMarket->GetForexSymbol(10)->IsDayLineNeedUpdate());

    gl_pWorldMarket->SetFinnhubInquiring(false);
    EXPECT_FALSE(gl_pWorldMarket->TaskInquiryFinnhubForexDayLine()) << "�����β�ѯʱû���ҵ�����ѯ�Ĺ�Ʊ";
    EXPECT_TRUE(gl_pWorldMarket->IsFinnhubForexDayLineUpdated()) << "��Ʊ����ѯ����";
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
    EXPECT_FALSE(gl_pWorldMarket->TaskInquiryTiingoCompanySymbol()) << "����TiingoInquiry���ڽ���";

    gl_pWorldMarket->SetTiingoInquiring(false);
    EXPECT_TRUE(gl_pWorldMarket->TaskInquiryTiingoCompanySymbol());
    EXPECT_TRUE(gl_pWorldMarket->IsTiingoInquiring());
    inquiry = gl_pWorldMarket->GetTiingoInquiry();
    EXPECT_EQ(inquiry.m_lInquiryIndex, __COMPANY_SYMBOLS__);
    EXPECT_EQ(inquiry.m_iPriority, 10);
    EXPECT_FALSE(gl_pWorldMarket->IsTiingoSymbolUpdated()) << "�˱�ʶ��Ҫ�ȴ��������ݺ�����";
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
    EXPECT_FALSE(gl_pWorldMarket->TaskInquiryTiingoDayLine()) << "����TiingoInquiry���ڽ���";

    gl_pWorldMarket->SetTiingoInquiring(false);
    EXPECT_TRUE(gl_pWorldMarket->TaskInquiryTiingoDayLine());
    EXPECT_TRUE(gl_pWorldMarket->IsTiingoInquiring());
    lStockIndex = gl_pWorldMarket->GetStockIndex(gl_pWorldMarket->GetChoicedStock(1)->GetSymbol());
    inquiry = gl_pWorldMarket->GetTiingoInquiry();
    EXPECT_EQ(inquiry.m_lInquiryIndex, __STOCK_CANDLES__);
    EXPECT_EQ(inquiry.m_lStockIndex, lStockIndex) << "��һ������ѯ��Ʊλ��";
    EXPECT_FALSE(gl_pWorldMarket->GetChoicedStock(1)->IsDayLineNeedUpdate());
    EXPECT_TRUE(gl_pWorldMarket->GetChoicedStock(3)->IsDayLineNeedUpdate());

    gl_pWorldMarket->SetTiingoInquiring(false);
    EXPECT_TRUE(gl_pWorldMarket->TaskInquiryTiingoDayLine());
    lStockIndex = gl_pWorldMarket->GetStockIndex(gl_pWorldMarket->GetChoicedStock(3)->GetSymbol());
    inquiry = gl_pWorldMarket->GetTiingoInquiry();
    EXPECT_EQ(inquiry.m_lInquiryIndex, __STOCK_CANDLES__);
    EXPECT_EQ(inquiry.m_lStockIndex, lStockIndex) << "�ڶ�������ѯ��Ʊλ��";
    EXPECT_FALSE(gl_pWorldMarket->GetChoicedStock(1)->IsDayLineNeedUpdate());
    EXPECT_FALSE(gl_pWorldMarket->GetChoicedStock(3)->IsDayLineNeedUpdate());

    gl_pWorldMarket->SetTiingoInquiring(false);
    EXPECT_FALSE(gl_pWorldMarket->TaskInquiryTiingoDayLine()) << "�����β�ѯʱû���ҵ�����ѯ�Ĺ�Ʊ";
    EXPECT_TRUE(gl_pWorldMarket->IsTiingoDayLineUpdated()) << "��Ʊ����ѯ����";
    CString str = gl_systemMessage.PopInformationMessage();
    EXPECT_STREQ(str, _T("�����г���ѡ��Ʊ������ʷ���ݸ������"));

    // �ָ�ԭ״
    for (int i = 0; i < gl_pWorldMarket->GetTotalStock(); i++) {
      pStock = gl_pWorldMarket->GetStock(i);
      pStock->SetDayLineNeedUpdate(true);
    }
  }

  TEST_F(CWorldMarketTest, TestTaskCheckSystemReady) {
    gl_pWorldMarket->SetSystemReady(true);
    EXPECT_TRUE(gl_pWorldMarket->TaskCheckSystemReady());

    gl_pWorldMarket->SetSystemReady(false);
    gl_pWorldMarket->SetFinnhubSymbolUpdated(false);
    gl_pWorldMarket->SetFinnhubForexExchangeUpdated(false);
    gl_pWorldMarket->SetFinnhubForexSymbolUpdated(false);
    EXPECT_FALSE(gl_pWorldMarket->TaskCheckSystemReady());
    EXPECT_FALSE(gl_pWorldMarket->IsSystemReady());

    gl_pWorldMarket->SetFinnhubSymbolUpdated(true);
    EXPECT_FALSE(gl_pWorldMarket->TaskCheckSystemReady());
    EXPECT_FALSE(gl_pWorldMarket->IsSystemReady());

    gl_pWorldMarket->SetFinnhubForexExchangeUpdated(true);
    EXPECT_FALSE(gl_pWorldMarket->TaskCheckSystemReady());
    EXPECT_FALSE(gl_pWorldMarket->IsSystemReady());

    gl_pWorldMarket->SetFinnhubForexSymbolUpdated(true);
    EXPECT_TRUE(gl_pWorldMarket->TaskCheckSystemReady());
    EXPECT_TRUE(gl_pWorldMarket->IsSystemReady());
    CString str = gl_systemMessage.PopInformationMessage();
    EXPECT_STREQ(str, _T("�����г���ʼ�����"));
  }

  TEST_F(CWorldMarketTest, TestRebuildEPSSurprise) {
    CWorldStockPtr pStock = nullptr;
    for (int i = 0; i < gl_pWorldMarket->GetTotalStock(); i++) {
      pStock = gl_pWorldMarket->GetStock(i);
      pStock->SetLastEPSSurpriseUpdateDate(20200101);
      pStock->m_fEPSSurpriseUpdated = true;
    }
    gl_pWorldMarket->SetFinnhubEPSSurpriseUpdated(true);

    EXPECT_TRUE(gl_pWorldMarket->RebuildEPSSurprise());

    for (int i = 0; i < gl_pWorldMarket->GetTotalStock(); i++) {
      pStock = gl_pWorldMarket->GetStock(i);
      EXPECT_EQ(pStock->GetLastEPSSurpriseUpdateDate(), 19800101);
      EXPECT_FALSE(pStock->m_fEPSSurpriseUpdated);
    }
    EXPECT_FALSE(gl_pWorldMarket->IsFinnhubEPSSurpriseUpdated());
  }

  TEST_F(CWorldMarketTest, TestRebuildPeer) {
    CWorldStockPtr pStock = nullptr;
    for (int i = 0; i < gl_pWorldMarket->GetTotalStock(); i++) {
      pStock = gl_pWorldMarket->GetStock(i);
      pStock->SetPeerUpdateDate(20200101);
      pStock->SetPeerUpdated(true);
      pStock->SetUpdateProfileDB(false);
    }
    gl_pWorldMarket->SetFinnhubPeerUpdated(true);

    EXPECT_TRUE(gl_pWorldMarket->RebuildPeer());

    for (int i = 0; i < gl_pWorldMarket->GetTotalStock(); i++) {
      pStock = gl_pWorldMarket->GetStock(i);
      EXPECT_EQ(pStock->GetPeerUpdateDate(), 19800101);
      EXPECT_FALSE(pStock->IsPeerUpdated());
      EXPECT_TRUE(pStock->IsUpdateProfileDB());
    }
    EXPECT_FALSE(gl_pWorldMarket->IsFinnhubPeerUpdated());
  }

  TEST_F(CWorldMarketTest, TestRebuildStockDayLine) {
    CWorldStockPtr pStock = nullptr;
    for (int i = 0; i < gl_pWorldMarket->GetTotalStock(); i++) {
      pStock = gl_pWorldMarket->GetStock(i);
      pStock->SetIPOStatus(__STOCK_IPOED__);
      pStock->SetDayLineStartDate(20200101);
      pStock->SetDayLineEndDate(20200101);
      pStock->SetDayLineNeedUpdate(false);
      pStock->SetUpdateProfileDB(false);
    }
    gl_pWorldMarket->SetFinnhubStockProfileUpdated(true);

    EXPECT_TRUE(gl_pWorldMarket->RebuildStockDayLineDB());

    for (int i = 0; i < gl_pWorldMarket->GetTotalStock(); i++) {
      pStock = gl_pWorldMarket->GetStock(i);
      EXPECT_EQ(pStock->GetDayLineStartDate(), 29900101);
      EXPECT_EQ(pStock->GetDayLineEndDate(), 19800101);
      EXPECT_TRUE(pStock->IsDayLineNeedUpdate());
      EXPECT_TRUE(pStock->IsUpdateProfileDB());
    }
    EXPECT_FALSE(gl_pWorldMarket->IsFinnhubStockProfileUpdated());
  }
}