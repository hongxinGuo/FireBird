#include"pch.h"
#include"globedef.h"
#include"GeneralCheck.h"

#include"FinnhubCryptoSymbol.h"
#include"SetFinnhubCryptoSymbol.h"

using namespace testing;

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

namespace StockAnalysisTest {
	class CFinnhubCryptoSymbolTest : public ::testing::Test
	{
	protected:
		static void SetUpTestSuite(void) { // 本测试类的初始化函数
			GeneralCheck();
		}

		static void TearDownTestSuite() {
			GeneralCheck();
		}

		virtual void SetUp(void) override {
			GeneralCheck();
		}

		virtual void TearDown(void) override {
			// clearup
		}
	};

	TEST_F(CFinnhubCryptoSymbolTest, TestInitialize) {
		CFinnhubCryptoSymbol FinnhubCryptoSymbol;
		EXPECT_STREQ(FinnhubCryptoSymbol.GetDescription(), _T(" "));
		EXPECT_STREQ(FinnhubCryptoSymbol.GetDisplaySymbol(), _T(" "));
		EXPECT_STREQ(FinnhubCryptoSymbol.GetSymbol(), _T(" "));
		EXPECT_STREQ(FinnhubCryptoSymbol.GetExchangeCode(), _T(" "));
		EXPECT_EQ(FinnhubCryptoSymbol.GetDayLineStartDate(), 29900101);
		EXPECT_EQ(FinnhubCryptoSymbol.GetDayLineEndDate(), 19800101);
		EXPECT_EQ(FinnhubCryptoSymbol.GetIPOStatus(), __STOCK_NOT_CHECKED__);
		EXPECT_TRUE(FinnhubCryptoSymbol.IsDayLineNeedUpdate());
		EXPECT_FALSE(FinnhubCryptoSymbol.IsDayLineNeedSaving());
		EXPECT_FALSE(FinnhubCryptoSymbol.IsUpdateProfileDB());
	}

	TEST_F(CFinnhubCryptoSymbolTest, TestGetRatio) {
		CFinnhubCryptoSymbol symbol;

		EXPECT_EQ(symbol.GetRatio(), 1000);
	}

	TEST_F(CFinnhubCryptoSymbolTest, TestGetDescription) {
		CFinnhubCryptoSymbol symbol;

		symbol.SetDescription(_T("abcdefg"));
		EXPECT_STREQ(symbol.GetDescription(), _T("abcdefg"));
	}

	TEST_F(CFinnhubCryptoSymbolTest, TestGetDisplaySymbol) {
		CFinnhubCryptoSymbol symbol;

		symbol.SetDisplaySymbol(_T("abcdefg"));
		EXPECT_STREQ(symbol.GetDisplaySymbol(), _T("abcdefg"));
	}
	TEST_F(CFinnhubCryptoSymbolTest, TestGetSymbol) {
		CFinnhubCryptoSymbol symbol;

		symbol.SetSymbol(_T("abcdefg"));
		EXPECT_STREQ(symbol.GetSymbol(), _T("abcdefg"));
	}

	TEST_F(CFinnhubCryptoSymbolTest, TestGetDayLineStartDate) {
		CFinnhubCryptoSymbol symbol;

		symbol.SetDayLineStartDate(20202020);
		EXPECT_EQ(symbol.GetDayLineStartDate(), 20202020);
	}
	TEST_F(CFinnhubCryptoSymbolTest, TestGetDayLineEndDate) {
		CFinnhubCryptoSymbol symbol;

		symbol.SetDayLineEndDate(10101010);
		EXPECT_EQ(symbol.GetDayLineEndDate(), 10101010);
	}

	TEST_F(CFinnhubCryptoSymbolTest, TestGetIPOStatus) {
		CFinnhubCryptoSymbol symbol;

		symbol.SetIPOStatus(__STOCK_IPOED__);
		EXPECT_EQ(symbol.GetIPOStatus(), __STOCK_IPOED__);
	}
	TEST_F(CFinnhubCryptoSymbolTest, TestGetDayLineNeedSaving) {
		CFinnhubCryptoSymbol symbol;

		EXPECT_FALSE(symbol.IsDayLineNeedSaving());
		symbol.SetDayLineNeedSaving(true);
		EXPECT_TRUE(symbol.IsDayLineNeedSaving());
		EXPECT_TRUE(symbol.IsDayLineNeedSavingAndClearFlag());
		EXPECT_FALSE(symbol.IsDayLineNeedSaving());
	}

	TEST_F(CFinnhubCryptoSymbolTest, TestSetCheckDayLineStatus) {
		CFinnhubCryptoSymbol symbol;

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

	TEST_F(CFinnhubCryptoSymbolTest, TestHaveNewDayLineData) {
		CFinnhubCryptoSymbol symbol;
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

	TEST_F(CFinnhubCryptoSymbolTest, TestUpdateDayLineStartEndDate) {
		CFinnhubCryptoSymbol symbol;
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

	TEST_F(CFinnhubCryptoSymbolTest, TestGetFinnhubDayLineInquiringString1) {
		CFinnhubCryptoSymbol symbol;
		CString str;

		symbol.SetSymbol(_T("ABCDE"));
		str = symbol.GetFinnhubDayLineInquiryString(123456789);
		EXPECT_STREQ(str, _T("ABCDE&resolution=D&from=315572400&to=123456789")) << "当前时间小于19800101，315572400就是美东标准时间的19800101";
	}

	TEST_F(CFinnhubCryptoSymbolTest, TestGetFinnhubDayLineInquiringString2) {
		CFinnhubCryptoSymbol symbol;
		CString str;

		symbol.SetSymbol(_T("ABCDE"));
		str = symbol.GetFinnhubDayLineInquiryString(1131536000);
		EXPECT_STREQ(str, _T("ABCDE&resolution=D&from=1100000000&to=1131536000")) << "365 * 24 * 3600 = 31536000";
	}

	TEST_F(CFinnhubCryptoSymbolTest, TestAppend) {
		CSetFinnhubCryptoSymbol setFinnhubCryptoSymbol, setFinnhubCryptoSymbol2;
		CFinnhubCryptoSymbol FinnhubCryptoSymbol, FinnhubCryptoSymbol2;

		FinnhubCryptoSymbol.SetDescription(_T("abc"));
		FinnhubCryptoSymbol.SetDisplaySymbol(_T("cba"));
		FinnhubCryptoSymbol.SetSymbol(_T("AAAAA"));
		FinnhubCryptoSymbol.SetExchangeCode(_T("US"));
		FinnhubCryptoSymbol.SetDayLineStartDate(20000101);
		FinnhubCryptoSymbol.SetDayLineEndDate(10000101);
		FinnhubCryptoSymbol.SetIPOStatus(__STOCK_DELISTED__);
		FinnhubCryptoSymbol.SetDayLineNeedUpdate(false);
		FinnhubCryptoSymbol.SetDayLineNeedSaving(true);
		FinnhubCryptoSymbol.SetUpdateProfileDB(true);

		ASSERT(!gl_fNormalMode);
		setFinnhubCryptoSymbol.Open();
		setFinnhubCryptoSymbol.m_pDatabase->BeginTrans();
		FinnhubCryptoSymbol.AppendSymbol(setFinnhubCryptoSymbol);
		setFinnhubCryptoSymbol.m_pDatabase->CommitTrans();
		setFinnhubCryptoSymbol.Close();

		setFinnhubCryptoSymbol2.m_strFilter = _T("[Symbol] = 'AAAAA'");
		setFinnhubCryptoSymbol2.Open();
		EXPECT_TRUE(!setFinnhubCryptoSymbol2.IsEOF()) << "此时已经存入了AA";
		FinnhubCryptoSymbol2.LoadSymbol(setFinnhubCryptoSymbol2);
		EXPECT_STREQ(FinnhubCryptoSymbol.GetDescription(), _T("abc"));
		EXPECT_STREQ(FinnhubCryptoSymbol.GetDisplaySymbol(), _T("cba"));
		EXPECT_STREQ(FinnhubCryptoSymbol.GetSymbol(), _T("AAAAA"));
		EXPECT_STREQ(FinnhubCryptoSymbol.GetExchangeCode(), _T("US"));
		EXPECT_EQ(FinnhubCryptoSymbol.GetDayLineStartDate(), 20000101);
		EXPECT_EQ(FinnhubCryptoSymbol.GetDayLineEndDate(), 10000101);
		EXPECT_EQ(FinnhubCryptoSymbol.GetIPOStatus(), __STOCK_DELISTED__);
		EXPECT_FALSE(FinnhubCryptoSymbol.IsDayLineNeedUpdate());
		EXPECT_TRUE(FinnhubCryptoSymbol.IsDayLineNeedSaving());
		EXPECT_TRUE(FinnhubCryptoSymbol.IsUpdateProfileDB());
		setFinnhubCryptoSymbol2.m_pDatabase->BeginTrans();
		while (!setFinnhubCryptoSymbol2.IsEOF()) {
			setFinnhubCryptoSymbol2.Delete();
			setFinnhubCryptoSymbol2.MoveNext();
		}
		setFinnhubCryptoSymbol2.m_pDatabase->CommitTrans();
		setFinnhubCryptoSymbol2.Close();
	}

	TEST_F(CFinnhubCryptoSymbolTest, TestUpdate) {
		CSetFinnhubCryptoSymbol setFinnhubCryptoSymbol, setFinnhubCryptoSymbol2, setFinnhubCryptoSymbol3;
		CFinnhubCryptoSymbol FinnhubCryptoSymbol, FinnhubCryptoSymbol2;

		FinnhubCryptoSymbol.SetDescription(_T("abc"));
		FinnhubCryptoSymbol.SetDisplaySymbol(_T("cba"));
		FinnhubCryptoSymbol.SetSymbol(_T("AAAAA"));
		FinnhubCryptoSymbol.SetExchangeCode(_T("US"));
		FinnhubCryptoSymbol.SetDayLineStartDate(20000101);
		FinnhubCryptoSymbol.SetDayLineEndDate(10000101);
		FinnhubCryptoSymbol.SetIPOStatus(__STOCK_DELISTED__);
		FinnhubCryptoSymbol.SetDayLineNeedUpdate(false);
		FinnhubCryptoSymbol.SetDayLineNeedSaving(true);
		FinnhubCryptoSymbol.SetUpdateProfileDB(true);

		ASSERT(!gl_fNormalMode);
		setFinnhubCryptoSymbol.Open();
		setFinnhubCryptoSymbol.m_pDatabase->BeginTrans();
		FinnhubCryptoSymbol.AppendSymbol(setFinnhubCryptoSymbol);
		setFinnhubCryptoSymbol.m_pDatabase->CommitTrans();
		setFinnhubCryptoSymbol.Close();

		// 改成新值
		FinnhubCryptoSymbol.SetDescription(_T("abc changed"));
		FinnhubCryptoSymbol.SetDisplaySymbol(_T("changed"));
		FinnhubCryptoSymbol.SetSymbol(_T("AAAAA"));
		FinnhubCryptoSymbol.SetExchangeCode(_T("US changed"));
		FinnhubCryptoSymbol.SetDayLineStartDate(101);
		FinnhubCryptoSymbol.SetDayLineEndDate(101);
		FinnhubCryptoSymbol.SetIPOStatus(__STOCK_IPOED__);
		FinnhubCryptoSymbol.SetDayLineNeedUpdate(TRUE);
		FinnhubCryptoSymbol.SetDayLineNeedSaving(FALSE);
		FinnhubCryptoSymbol.SetUpdateProfileDB(FALSE);

		setFinnhubCryptoSymbol3.m_strFilter = _T("[Symbol] = 'AAAAA'");
		setFinnhubCryptoSymbol3.Open();
		setFinnhubCryptoSymbol3.m_pDatabase->BeginTrans();
		FinnhubCryptoSymbol.UpdateSymbol(setFinnhubCryptoSymbol3);
		setFinnhubCryptoSymbol3.m_pDatabase->CommitTrans();
		setFinnhubCryptoSymbol3.Close();

		setFinnhubCryptoSymbol2.m_strFilter = _T("[Symbol] = 'AAAAA'");
		setFinnhubCryptoSymbol2.Open();
		EXPECT_TRUE(!setFinnhubCryptoSymbol2.IsEOF()) << "此时已经存入了AA";
		FinnhubCryptoSymbol2.LoadSymbol(setFinnhubCryptoSymbol2);
		EXPECT_STREQ(FinnhubCryptoSymbol.GetDescription(), _T("abc changed"));
		EXPECT_STREQ(FinnhubCryptoSymbol.GetDisplaySymbol(), _T("changed"));
		EXPECT_STREQ(FinnhubCryptoSymbol.GetSymbol(), _T("AAAAA"));
		EXPECT_STREQ(FinnhubCryptoSymbol.GetExchangeCode(), _T("US changed"));
		EXPECT_EQ(FinnhubCryptoSymbol.GetDayLineStartDate(), 101);
		EXPECT_EQ(FinnhubCryptoSymbol.GetDayLineEndDate(), 101);
		EXPECT_EQ(FinnhubCryptoSymbol.GetIPOStatus(), __STOCK_IPOED__);
		EXPECT_TRUE(FinnhubCryptoSymbol.IsDayLineNeedUpdate());
		EXPECT_FALSE(FinnhubCryptoSymbol.IsDayLineNeedSaving());
		EXPECT_FALSE(FinnhubCryptoSymbol.IsUpdateProfileDB());
		setFinnhubCryptoSymbol2.m_pDatabase->BeginTrans();
		while (!setFinnhubCryptoSymbol2.IsEOF()) {
			setFinnhubCryptoSymbol2.Delete();
			setFinnhubCryptoSymbol2.MoveNext();
		}
		setFinnhubCryptoSymbol2.m_pDatabase->CommitTrans();
		setFinnhubCryptoSymbol2.Close();
	}

	TEST_F(CFinnhubCryptoSymbolTest, TestSaveDayLine) {
		CFinnhubCryptoSymbol FinnhubCryptoSymbol, FinnhubCryptoSymbol2;
		CDayLinePtr pDayLine = make_shared<CDayLine>();
		vector<CDayLinePtr> vDayLine;
		CSetCryptoDayLine setCryptoDayLine;

		pDayLine->SetDate(19800101);
		pDayLine->SetStockSymbol(_T("BINANCE:WINUSDC"));
		vDayLine.push_back(pDayLine);
		FinnhubCryptoSymbol.SetSymbol(_T("BINANCE:WINUSDC"));
		FinnhubCryptoSymbol.UpdateDayLine(vDayLine);

		EXPECT_THAT(FinnhubCryptoSymbol.GetDayLineStartDate(), Eq(29900101));

		FinnhubCryptoSymbol.SaveDayLine();

		EXPECT_THAT(FinnhubCryptoSymbol.GetDayLineStartDate(), Eq(20200817)) << "这个是数据库中该股票日线数据的起始日期";

		setCryptoDayLine.m_strFilter = _T("[Symbol] = 'BINANCE:WINUSDC'");
		setCryptoDayLine.m_strSort = _T("[Date]");
		setCryptoDayLine.Open();
		EXPECT_EQ(setCryptoDayLine.m_Date, 19800101);
		setCryptoDayLine.m_pDatabase->BeginTrans();
		setCryptoDayLine.Delete();
		setCryptoDayLine.m_pDatabase->CommitTrans();
		setCryptoDayLine.Close();
	}
	TEST_F(CFinnhubCryptoSymbolTest, TestUpdateDayLineDB) {
		CFinnhubCryptoSymbol FinnhubCryptoSymbol, FinnhubCryptoSymbol2;
		CDayLinePtr pDayLine = make_shared<CDayLine>();
		vector<CDayLinePtr> vDayLine;
		CSetCryptoDayLine setCryptoDayLine;

		FinnhubCryptoSymbol.SetDayLineStartDate(20200102);
		FinnhubCryptoSymbol.SetDayLineEndDate(19800101);

		pDayLine->SetDate(20000101);
		pDayLine->SetStockSymbol(_T("abcdefg"));
		vDayLine.push_back(pDayLine);
		FinnhubCryptoSymbol.UpdateDayLine(vDayLine);

		FinnhubCryptoSymbol.UpdateDayLineDB();

		EXPECT_TRUE(FinnhubCryptoSymbol.IsUpdateProfileDB());
		EXPECT_THAT(FinnhubCryptoSymbol.GetDayLineStartDate(), Eq(20000101));
		EXPECT_EQ(FinnhubCryptoSymbol.GetDayLineEndDate(), 20000101);

		setCryptoDayLine.m_strFilter = _T("[Symbol] = 'abcdefg'");
		setCryptoDayLine.Open();
		EXPECT_EQ(setCryptoDayLine.m_Date, 20000101);
		setCryptoDayLine.m_pDatabase->BeginTrans();
		setCryptoDayLine.Delete();
		setCryptoDayLine.m_pDatabase->CommitTrans();
		setCryptoDayLine.Close();
	}
}