#include"pch.h"
#include"globedef.h"

#include"FinnhubForexSymbol.h"

namespace StockAnalysisTest {
  class CFinnhubForexSymbolTest : public ::testing::Test
  {
  protected:
    static void SetUpTestSuite(void) { // 本测试类的初始化函数
      ASSERT_FALSE(gl_fNormalMode);
    }

    static void TearDownTestSuite() {
      ASSERT_FALSE(gl_fNormalMode);
    }

    virtual void SetUp(void) override {
      ASSERT_FALSE(gl_fNormalMode);
      ASSERT_TRUE(gl_fTestMode);
    }

    virtual void TearDown(void) override {
      // clearup
    }
  };

  TEST_F(CFinnhubForexSymbolTest, TestInitialize) {
    CFinnhubForexSymbol FinnhubForexSymbol;
    EXPECT_STREQ(FinnhubForexSymbol.GetDescription(), _T(" "));
    EXPECT_STREQ(FinnhubForexSymbol.GetDisplaySymbol(), _T(" "));
    EXPECT_STREQ(FinnhubForexSymbol.GetSymbol(), _T(" "));
    EXPECT_STREQ(FinnhubForexSymbol.GetExchangeCode(), _T(" "));
    EXPECT_EQ(FinnhubForexSymbol.GetDayLineStartDate(), 29900101);
    EXPECT_EQ(FinnhubForexSymbol.GetDayLineEndDate(), 19800101);
    EXPECT_EQ(FinnhubForexSymbol.GetIPOStatus(), __STOCK_NOT_CHECKED__);
    EXPECT_TRUE(FinnhubForexSymbol.IsDayLineNeedUpdate());
    EXPECT_FALSE(FinnhubForexSymbol.IsDayLineNeedSaving());
    EXPECT_FALSE(FinnhubForexSymbol.IsUpdateProfileDB());
  }

  TEST_F(CFinnhubForexSymbolTest, TestGetDescription) {
    CFinnhubForexSymbol symbol;

    symbol.SetDescription(_T("abcdefg"));
    EXPECT_STREQ(symbol.GetDescription(), _T("abcdefg"));
  }

  TEST_F(CFinnhubForexSymbolTest, TestGetDisplaySymbol) {
    CFinnhubForexSymbol symbol;

    symbol.SetDisplaySymbol(_T("abcdefg"));
    EXPECT_STREQ(symbol.GetDisplaySymbol(), _T("abcdefg"));
  }
  TEST_F(CFinnhubForexSymbolTest, TestGetSymbol) {
    CFinnhubForexSymbol symbol;

    symbol.SetSymbol(_T("abcdefg"));
    EXPECT_STREQ(symbol.GetSymbol(), _T("abcdefg"));
  }

  TEST_F(CFinnhubForexSymbolTest, TestGetDayLineStartDate) {
    CFinnhubForexSymbol symbol;

    symbol.SetDayLineStartDate(20202020);
    EXPECT_EQ(symbol.GetDayLineStartDate(), 20202020);
  }
  TEST_F(CFinnhubForexSymbolTest, TestGetDayLineEndDate) {
    CFinnhubForexSymbol symbol;

    symbol.SetDayLineEndDate(10101010);
    EXPECT_EQ(symbol.GetDayLineEndDate(), 10101010);
  }

  TEST_F(CFinnhubForexSymbolTest, TestGetIPOStatus) {
    CFinnhubForexSymbol symbol;

    symbol.SetIPOStatus(__STOCK_IPOED__);
    EXPECT_EQ(symbol.GetIPOStatus(), __STOCK_IPOED__);
  }
  TEST_F(CFinnhubForexSymbolTest, TestGetDayLineNeedSaving) {
    CFinnhubForexSymbol symbol;

    EXPECT_FALSE(symbol.IsDayLineNeedSaving());
    symbol.SetDayLineNeedSaving(true);
    EXPECT_TRUE(symbol.IsDayLineNeedSaving());
    EXPECT_TRUE(symbol.IsDayLineNeedSavingAndClearFlag());
    EXPECT_FALSE(symbol.IsDayLineNeedSaving());
  }

  TEST_F(CFinnhubForexSymbolTest, TestSetCheckDayLineStatus) {
    CFinnhubForexSymbol symbol;

    EXPECT_TRUE(symbol.IsDayLineNeedUpdate());
    symbol.SetIPOStatus(__STOCK_NULL__);
    symbol.SetCheckingDayLineStatus();
    EXPECT_FALSE(symbol.IsDayLineNeedUpdate());

    symbol.SetDayLineNeedUpdate(true);
    symbol.SetIPOStatus(__STOCK_IPOED__);
    symbol.SetDayLineEndDate(gl_pWorldMarket->GetLastTradeDate());
    symbol.SetCheckingDayLineStatus();
    EXPECT_FALSE(symbol.IsDayLineNeedUpdate());
  }

  TEST_F(CFinnhubForexSymbolTest, TestHaveNewDayLineData) {
    CFinnhubForexSymbol symbol;
    vector<CDayLinePtr> vDayLine;

    CDayLinePtr pDayLine;
    pDayLine = make_shared<CDayLine>();
    pDayLine->SetDate(20200102);
    vDayLine.push_back(pDayLine);

    EXPECT_FALSE(symbol.HaveNewDayLineData());
    symbol.UpdateDayLine(vDayLine);
    symbol.SetDayLineEndDate(20200101); // 比日线最新数据旧
    EXPECT_TRUE(symbol.HaveNewDayLineData());
  }

  TEST_F(CFinnhubForexSymbolTest, TestUpdateDayLineStartEndDate) {
    CFinnhubForexSymbol symbol;
    vector<CDayLinePtr> vDayLine;
    CDayLinePtr pDayLine;

    symbol.SetDayLineEndDate(19900101);
    symbol.SetDayLineStartDate(19900101);
    symbol.UpdateDayLineStartEndDate();
    EXPECT_EQ(symbol.GetDayLineStartDate(), 29900101);
    EXPECT_EQ(symbol.GetDayLineEndDate(), 19800101);

    EXPECT_EQ(symbol.GetDayLineSize(), 0);
    pDayLine = make_shared<CDayLine>();
    pDayLine->SetDate(20200102);
    vDayLine.push_back(pDayLine);
    pDayLine = make_shared<CDayLine>();
    pDayLine->SetDate(20200105);
    vDayLine.push_back(pDayLine);
    symbol.UpdateDayLine(vDayLine);
    EXPECT_EQ(symbol.GetDayLineSize(), 2);

    EXPECT_FALSE(symbol.IsUpdateProfileDB());
    symbol.UpdateDayLineStartEndDate();
    EXPECT_EQ(symbol.GetDayLineStartDate(), 20200102);
    EXPECT_EQ(symbol.GetDayLineEndDate(), 20200105);
    EXPECT_TRUE(symbol.IsUpdateProfileDB());

    symbol.UnloadDayLine();
    EXPECT_EQ(symbol.GetDayLineSize(), 0);
  }

  TEST_F(CFinnhubForexSymbolTest, TestGetFinnhubDayLineInquiringString1) {
    CFinnhubForexSymbol symbol;
    CString str;

    symbol.SetSymbol(_T("ABCDE"));
    str = symbol.GetFinnhubDayLineInquiryString(123456789);
    EXPECT_STREQ(str, _T("ABCDE&resolution=D&from=315558000&to=123456789")) << "当前时间小于19800101，315558000就是19800101";
  }

  TEST_F(CFinnhubForexSymbolTest, TestGetFinnhubDayLineInquiringString2) {
    CFinnhubForexSymbol symbol;
    CString str;

    symbol.SetSymbol(_T("ABCDE"));
    str = symbol.GetFinnhubDayLineInquiryString(1131536000);
    EXPECT_STREQ(str, _T("ABCDE&resolution=D&from=1100000000&to=1131536000")) << "365 * 24 * 3600 = 31536000";
  }

  TEST_F(CFinnhubForexSymbolTest, TestAppend) {
    CSetFinnhubForexSymbol setFinnhubForexSymbol, setFinnhubForexSymbol2;
    CFinnhubForexSymbol FinnhubForexSymbol, FinnhubForexSymbol2;

    FinnhubForexSymbol.SetDescription(_T("abc"));
    FinnhubForexSymbol.SetDisplaySymbol(_T("cba"));
    FinnhubForexSymbol.SetSymbol(_T("AAAAA"));
    FinnhubForexSymbol.SetExchangeCode(_T("US"));
    FinnhubForexSymbol.SetDayLineStartDate(20000101);
    FinnhubForexSymbol.SetDayLineEndDate(10000101);
    FinnhubForexSymbol.SetIPOStatus(__STOCK_DELISTED__);
    FinnhubForexSymbol.SetDayLineNeedUpdate(false);
    FinnhubForexSymbol.SetDayLineNeedSaving(true);
    FinnhubForexSymbol.SetUpdateProfileDB(true);

    ASSERT(!gl_fNormalMode);
    setFinnhubForexSymbol.Open();
    setFinnhubForexSymbol.m_pDatabase->BeginTrans();
    FinnhubForexSymbol.Append(setFinnhubForexSymbol);
    setFinnhubForexSymbol.m_pDatabase->CommitTrans();
    setFinnhubForexSymbol.Close();

    setFinnhubForexSymbol2.m_strFilter = _T("[Symbol] = 'AAAAA'");
    setFinnhubForexSymbol2.Open();
    EXPECT_TRUE(!setFinnhubForexSymbol2.IsEOF()) << "此时已经存入了AA";
    FinnhubForexSymbol2.Load(setFinnhubForexSymbol2);
    EXPECT_STREQ(FinnhubForexSymbol.GetDescription(), _T("abc"));
    EXPECT_STREQ(FinnhubForexSymbol.GetDisplaySymbol(), _T("cba"));
    EXPECT_STREQ(FinnhubForexSymbol.GetSymbol(), _T("AAAAA"));
    EXPECT_STREQ(FinnhubForexSymbol.GetExchangeCode(), _T("US"));
    EXPECT_EQ(FinnhubForexSymbol.GetDayLineStartDate(), 20000101);
    EXPECT_EQ(FinnhubForexSymbol.GetDayLineEndDate(), 10000101);
    EXPECT_EQ(FinnhubForexSymbol.GetIPOStatus(), __STOCK_DELISTED__);
    EXPECT_FALSE(FinnhubForexSymbol.IsDayLineNeedUpdate());
    EXPECT_TRUE(FinnhubForexSymbol.IsDayLineNeedSaving());
    EXPECT_TRUE(FinnhubForexSymbol.IsUpdateProfileDB());
    setFinnhubForexSymbol2.m_pDatabase->BeginTrans();
    while (!setFinnhubForexSymbol2.IsEOF()) {
      setFinnhubForexSymbol2.Delete();
      setFinnhubForexSymbol2.MoveNext();
    }
    setFinnhubForexSymbol2.m_pDatabase->CommitTrans();
    setFinnhubForexSymbol2.Close();
  }

  TEST_F(CFinnhubForexSymbolTest, TestUpdate) {
    CSetFinnhubForexSymbol setFinnhubForexSymbol, setFinnhubForexSymbol2, setFinnhubForexSymbol3;
    CFinnhubForexSymbol FinnhubForexSymbol, FinnhubForexSymbol2;

    FinnhubForexSymbol.SetDescription(_T("abc"));
    FinnhubForexSymbol.SetDisplaySymbol(_T("cba"));
    FinnhubForexSymbol.SetSymbol(_T("AAAAA"));
    FinnhubForexSymbol.SetExchangeCode(_T("US"));
    FinnhubForexSymbol.SetDayLineStartDate(20000101);
    FinnhubForexSymbol.SetDayLineEndDate(10000101);
    FinnhubForexSymbol.SetIPOStatus(__STOCK_DELISTED__);
    FinnhubForexSymbol.SetDayLineNeedUpdate(false);
    FinnhubForexSymbol.SetDayLineNeedSaving(true);
    FinnhubForexSymbol.SetUpdateProfileDB(true);

    ASSERT(!gl_fNormalMode);
    setFinnhubForexSymbol.Open();
    setFinnhubForexSymbol.m_pDatabase->BeginTrans();
    FinnhubForexSymbol.Append(setFinnhubForexSymbol);
    setFinnhubForexSymbol.m_pDatabase->CommitTrans();
    setFinnhubForexSymbol.Close();

    // 改成新值
    FinnhubForexSymbol.SetDescription(_T("abc changed"));
    FinnhubForexSymbol.SetDisplaySymbol(_T("changed"));
    FinnhubForexSymbol.SetSymbol(_T("AAAAA"));
    FinnhubForexSymbol.SetExchangeCode(_T("US changed"));
    FinnhubForexSymbol.SetDayLineStartDate(101);
    FinnhubForexSymbol.SetDayLineEndDate(101);
    FinnhubForexSymbol.SetIPOStatus(__STOCK_IPOED__);
    FinnhubForexSymbol.SetDayLineNeedUpdate(TRUE);
    FinnhubForexSymbol.SetDayLineNeedSaving(FALSE);
    FinnhubForexSymbol.SetUpdateProfileDB(FALSE);

    setFinnhubForexSymbol3.m_strFilter = _T("[Symbol] = 'AAAAA'");
    setFinnhubForexSymbol3.Open();
    setFinnhubForexSymbol3.m_pDatabase->BeginTrans();
    FinnhubForexSymbol.Update(setFinnhubForexSymbol3);
    setFinnhubForexSymbol3.m_pDatabase->CommitTrans();
    setFinnhubForexSymbol3.Close();

    setFinnhubForexSymbol2.m_strFilter = _T("[Symbol] = 'AAAAA'");
    setFinnhubForexSymbol2.Open();
    EXPECT_TRUE(!setFinnhubForexSymbol2.IsEOF()) << "此时已经存入了AA";
    FinnhubForexSymbol2.Load(setFinnhubForexSymbol2);
    EXPECT_STREQ(FinnhubForexSymbol.GetDescription(), _T("abc changed"));
    EXPECT_STREQ(FinnhubForexSymbol.GetDisplaySymbol(), _T("changed"));
    EXPECT_STREQ(FinnhubForexSymbol.GetSymbol(), _T("AAAAA"));
    EXPECT_STREQ(FinnhubForexSymbol.GetExchangeCode(), _T("US changed"));
    EXPECT_EQ(FinnhubForexSymbol.GetDayLineStartDate(), 101);
    EXPECT_EQ(FinnhubForexSymbol.GetDayLineEndDate(), 101);
    EXPECT_EQ(FinnhubForexSymbol.GetIPOStatus(), __STOCK_IPOED__);
    EXPECT_TRUE(FinnhubForexSymbol.IsDayLineNeedUpdate());
    EXPECT_FALSE(FinnhubForexSymbol.IsDayLineNeedSaving());
    EXPECT_FALSE(FinnhubForexSymbol.IsUpdateProfileDB());
    setFinnhubForexSymbol2.m_pDatabase->BeginTrans();
    while (!setFinnhubForexSymbol2.IsEOF()) {
      setFinnhubForexSymbol2.Delete();
      setFinnhubForexSymbol2.MoveNext();
    }
    setFinnhubForexSymbol2.m_pDatabase->CommitTrans();
    setFinnhubForexSymbol2.Close();
  }

  TEST_F(CFinnhubForexSymbolTest, TestSaveDayLine) {
    CFinnhubForexSymbol FinnhubForexSymbol, FinnhubForexSymbol2;
    CDayLinePtr pDayLine = make_shared<CDayLine>();
    vector<CDayLinePtr> vDayLine;
    CSetForexDayLine setForexDayLine;

    pDayLine->SetDate(10101010);
    pDayLine->SetSymbol(_T("abcdefg"));
    vDayLine.push_back(pDayLine);
    FinnhubForexSymbol.UpdateDayLine(vDayLine);
    FinnhubForexSymbol.SaveDayLine();

    setForexDayLine.m_strFilter = _T("[Symbol] = 'abcdefg'");
    setForexDayLine.Open();
    EXPECT_EQ(setForexDayLine.m_Date, 10101010);
    setForexDayLine.m_pDatabase->BeginTrans();
    setForexDayLine.Delete();
    setForexDayLine.m_pDatabase->CommitTrans();
    setForexDayLine.Close();
  }
}