#include"pch.h"

#include"WorldMarket.h"
#include"GeneralCheck.h"

#include"FinnhubForexSymbol.h"
#include"SetFinnhubForexSymbol.h"
#include"SetForexDayLine.h"

using namespace testing;

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

namespace FireBirdTest {
	class CFinnhubForexSymbolTest : public ::testing::Test {
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
			// clearUp
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
		EXPECT_EQ(FinnhubForexSymbol.GetIPOStatus(), _STOCK_NOT_CHECKED_);
		EXPECT_TRUE(FinnhubForexSymbol.IsDayLineNeedUpdate());
		EXPECT_FALSE(FinnhubForexSymbol.IsDayLineNeedSaving());
		EXPECT_FALSE(FinnhubForexSymbol.IsUpdateProfileDB());
	}

	TEST_F(CFinnhubForexSymbolTest, TestGetRatio) {
		CFinnhubForexSymbol symbol;

		EXPECT_EQ(symbol.GetRatio(), 1000);
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

		symbol.SetIPOStatus(_STOCK_IPOED_);
		EXPECT_EQ(symbol.GetIPOStatus(), _STOCK_IPOED_);
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
		symbol.SetIPOStatus(_STOCK_NULL_);
		symbol.SetCheckingDayLineStatus();
		EXPECT_FALSE(symbol.IsDayLineNeedUpdate());

		symbol.SetDayLineNeedUpdate(true);
		symbol.SetIPOStatus(_STOCK_IPOED_);
		symbol.SetDayLineEndDate(gl_pWorldMarket->GetLastTradeDate());
		symbol.SetCheckingDayLineStatus();
		EXPECT_FALSE(symbol.IsDayLineNeedUpdate());
	}

	TEST_F(CFinnhubForexSymbolTest, TestHaveNewDayLineData) {
		CFinnhubForexSymbol symbol;
		vector<CDayLinePtr> vDayLine;

		CDayLinePtr pDayLine;
		pDayLine = make_shared<CDayLine>();
		pDayLine->SetDate(20300102);
		pDayLine->SetClose(100);
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
		pDayLine->SetClose(100);
		vDayLine.push_back(pDayLine);
		pDayLine = make_shared<CDayLine>();
		pDayLine->SetDate(20200105);
		pDayLine->SetClose(100);
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
		EXPECT_STREQ(str, _T("ABCDE&resolution=D&from=315601200&to=123456789")) << "当前时间小于19800101，315601200就是美东标准时间的19800101";
	}

	TEST_F(CFinnhubForexSymbolTest, TestGetFinnhubDayLineInquiringString2) {
		CFinnhubForexSymbol symbol;
		CString str;

		symbol.SetSymbol(_T("ABCDE"));
		str = symbol.GetFinnhubDayLineInquiryString(1131536000);
		EXPECT_STREQ(str, _T("ABCDE&resolution=D&from=1100000000&to=1131536000")) << "365 * 24 * 3600 = 31536000";
	}

	TEST_F(CFinnhubForexSymbolTest, TestAppendSymbol) {
		CSetFinnhubForexSymbol setFinnhubForexSymbol, setFinnhubForexSymbol2;
		CFinnhubForexSymbol FinnhubForexSymbol, FinnhubForexSymbol2;

		FinnhubForexSymbol.SetDescription(_T("abc"));
		FinnhubForexSymbol.SetDisplaySymbol(_T("cba"));
		FinnhubForexSymbol.SetSymbol(_T("AAABC"));
		FinnhubForexSymbol.SetExchangeCode(_T("US"));
		FinnhubForexSymbol.SetDayLineStartDate(20000101);
		FinnhubForexSymbol.SetDayLineEndDate(10000101);
		FinnhubForexSymbol.SetIPOStatus(_STOCK_DELISTED_);
		FinnhubForexSymbol.SetDayLineNeedUpdate(false);
		FinnhubForexSymbol.SetDayLineNeedSaving(true);
		FinnhubForexSymbol.SetUpdateProfileDB(true);

		ASSERT(!gl_systemStatus.IsWorkingMode());
		setFinnhubForexSymbol.Open();
		setFinnhubForexSymbol.m_pDatabase->BeginTrans();
		FinnhubForexSymbol.AppendSymbol(setFinnhubForexSymbol);
		setFinnhubForexSymbol.m_pDatabase->CommitTrans();
		setFinnhubForexSymbol.Close();

		setFinnhubForexSymbol2.m_strFilter = _T("[Symbol] = 'AAABC'");
		setFinnhubForexSymbol2.Open();
		EXPECT_TRUE(!setFinnhubForexSymbol2.IsEOF()) << "此时已经存入了AAABC";
		FinnhubForexSymbol2.LoadSymbol(setFinnhubForexSymbol2);
		EXPECT_STREQ(FinnhubForexSymbol.GetDescription(), _T("abc"));
		EXPECT_STREQ(FinnhubForexSymbol.GetDisplaySymbol(), _T("cba"));
		EXPECT_STREQ(FinnhubForexSymbol.GetSymbol(), _T("AAABC"));
		EXPECT_STREQ(FinnhubForexSymbol.GetExchangeCode(), _T("US"));
		EXPECT_EQ(FinnhubForexSymbol.GetDayLineStartDate(), 20000101);
		EXPECT_EQ(FinnhubForexSymbol.GetDayLineEndDate(), 10000101);
		EXPECT_EQ(FinnhubForexSymbol.GetIPOStatus(), _STOCK_DELISTED_);
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

	TEST_F(CFinnhubForexSymbolTest, TestUpdateSymbol) {
		CSetFinnhubForexSymbol setFinnhubForexSymbol, setFinnhubForexSymbol2, setFinnhubForexSymbol3;
		CFinnhubForexSymbol FinnhubForexSymbol, FinnhubForexSymbol2;

		FinnhubForexSymbol.SetDescription(_T("abc"));
		FinnhubForexSymbol.SetDisplaySymbol(_T("cba"));
		FinnhubForexSymbol.SetSymbol(_T("AAABB"));
		FinnhubForexSymbol.SetExchangeCode(_T("US"));
		FinnhubForexSymbol.SetDayLineStartDate(20000101);
		FinnhubForexSymbol.SetDayLineEndDate(10000101);
		FinnhubForexSymbol.SetIPOStatus(_STOCK_DELISTED_);
		FinnhubForexSymbol.SetDayLineNeedUpdate(false);
		FinnhubForexSymbol.SetDayLineNeedSaving(true);
		FinnhubForexSymbol.SetUpdateProfileDB(true);

		ASSERT(!gl_systemStatus.IsWorkingMode());
		setFinnhubForexSymbol.Open();
		setFinnhubForexSymbol.m_pDatabase->BeginTrans();
		FinnhubForexSymbol.AppendSymbol(setFinnhubForexSymbol);
		setFinnhubForexSymbol.m_pDatabase->CommitTrans();
		setFinnhubForexSymbol.Close();

		// 改成新值
		FinnhubForexSymbol.SetDescription(_T("abc changed"));
		FinnhubForexSymbol.SetDisplaySymbol(_T("changed"));
		FinnhubForexSymbol.SetSymbol(_T("AAABB"));
		FinnhubForexSymbol.SetExchangeCode(_T("US changed"));
		FinnhubForexSymbol.SetDayLineStartDate(101);
		FinnhubForexSymbol.SetDayLineEndDate(101);
		FinnhubForexSymbol.SetIPOStatus(_STOCK_IPOED_);
		FinnhubForexSymbol.SetDayLineNeedUpdate(TRUE);
		FinnhubForexSymbol.SetDayLineNeedSaving(FALSE);
		FinnhubForexSymbol.SetUpdateProfileDB(FALSE);

		setFinnhubForexSymbol3.m_strFilter = _T("[Symbol] = 'AAABB'");
		setFinnhubForexSymbol3.Open();
		setFinnhubForexSymbol3.m_pDatabase->BeginTrans();
		FinnhubForexSymbol.UpdateSymbol(setFinnhubForexSymbol3);
		setFinnhubForexSymbol3.m_pDatabase->CommitTrans();
		setFinnhubForexSymbol3.Close();

		setFinnhubForexSymbol2.m_strFilter = _T("[Symbol] = 'AAABB'");
		setFinnhubForexSymbol2.Open();
		EXPECT_TRUE(!setFinnhubForexSymbol2.IsEOF()) << "此时已经存入了AA";
		FinnhubForexSymbol2.LoadSymbol(setFinnhubForexSymbol2);
		EXPECT_STREQ(FinnhubForexSymbol.GetDescription(), _T("abc changed"));
		EXPECT_STREQ(FinnhubForexSymbol.GetDisplaySymbol(), _T("changed"));
		EXPECT_STREQ(FinnhubForexSymbol.GetSymbol(), _T("AAABB"));
		EXPECT_STREQ(FinnhubForexSymbol.GetExchangeCode(), _T("US changed"));
		EXPECT_EQ(FinnhubForexSymbol.GetDayLineStartDate(), 101);
		EXPECT_EQ(FinnhubForexSymbol.GetDayLineEndDate(), 101);
		EXPECT_EQ(FinnhubForexSymbol.GetIPOStatus(), _STOCK_IPOED_);
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

		pDayLine->SetDate(19800101);
		pDayLine->SetClose(100);
		pDayLine->SetStockSymbol(_T("OANDA:AUD_SGD"));
		vDayLine.push_back(pDayLine);
		FinnhubForexSymbol.SetSymbol(_T("OANDA:AUD_SGD"));
		FinnhubForexSymbol.UpdateDayLine(vDayLine);

		EXPECT_THAT(FinnhubForexSymbol.GetDayLineStartDate(), Eq(29900101));

		FinnhubForexSymbol.UpdateDayLineDB();

		setForexDayLine.m_strFilter = _T("[Symbol] = 'OANDA:AUD_SGD'");
		setForexDayLine.m_strSort = _T("[Date]");
		setForexDayLine.Open();
		EXPECT_EQ(setForexDayLine.m_Date, 19800101) << "刚存储的数据";
		setForexDayLine.m_pDatabase->BeginTrans();
		setForexDayLine.Delete();
		setForexDayLine.m_pDatabase->CommitTrans();
		setForexDayLine.Close();
	}
}