#include"pch.h"

#include"WorldMarket.h"
#include"GeneralCheck.h"

#include"FinnhubForex.h"
#include"SetFinnhubForexSymbol.h"
#include"SetForexDayLine.h"

using namespace testing;

namespace FireBirdTest {
	class CFinnhubForexSymbolTest : public ::testing::Test {
	protected:
		static void SetUpTestSuite() { // 本测试类的初始化函数
			SCOPED_TRACE("");
			GeneralCheck();
		}

		static void TearDownTestSuite() {
			SCOPED_TRACE("");
			GeneralCheck();
		}

		void SetUp() override {
			SCOPED_TRACE("");
			GeneralCheck();
		}

		void TearDown() override {
			// clearUp
			SCOPED_TRACE("");
			GeneralCheck();
		}
	};

	TEST_F(CFinnhubForexSymbolTest, TestInitialize) {
		CFinnhubForex FinnhubForexSymbol;
		EXPECT_EQ(FinnhubForexSymbol.GetDescription(), "");
		EXPECT_EQ(FinnhubForexSymbol.GetDisplaySymbol(), "");
		EXPECT_EQ(FinnhubForexSymbol.GetSymbol(), "");
		EXPECT_EQ(FinnhubForexSymbol.GetExchangeCode(), "");
		EXPECT_EQ(FinnhubForexSymbol.GetDayLineStartDate(), 29900101);
		EXPECT_EQ(FinnhubForexSymbol.GetDayLineEndDate(), 19800101);
		EXPECT_EQ(FinnhubForexSymbol.GetIPOStatus(), _STOCK_NOT_CHECKED_);
		EXPECT_TRUE(FinnhubForexSymbol.IsUpdateDayLine());
		EXPECT_FALSE(FinnhubForexSymbol.IsUpdateDayLineDB());
		EXPECT_FALSE(FinnhubForexSymbol.IsUpdateProfileDB());
	}

	TEST_F(CFinnhubForexSymbolTest, TestGetRatio) {
		const CFinnhubForex symbol;

		EXPECT_EQ(symbol.GetRatio(), 1000);
	}

	TEST_F(CFinnhubForexSymbolTest, TestGetDescription) {
		CFinnhubForex symbol;

		symbol.SetDescription("abcdefg");
		EXPECT_EQ(symbol.GetDescription(), "abcdefg");
	}

	TEST_F(CFinnhubForexSymbolTest, TestGetDisplaySymbol) {
		CFinnhubForex symbol;

		symbol.SetDisplaySymbol("abcdefg");
		EXPECT_EQ(symbol.GetDisplaySymbol(), "abcdefg");
	}

	TEST_F(CFinnhubForexSymbolTest, TestGetSymbol) {
		CFinnhubForex symbol;

		symbol.SetSymbol("abcdefg");
		EXPECT_EQ(symbol.GetSymbol(), "abcdefg");
	}

	TEST_F(CFinnhubForexSymbolTest, TestGetDayLineStartDate) {
		CFinnhubForex symbol;

		symbol.SetDayLineStartDate(20202020);
		EXPECT_EQ(symbol.GetDayLineStartDate(), 20202020);
	}

	TEST_F(CFinnhubForexSymbolTest, TestGetDayLineEndDate) {
		CFinnhubForex symbol;

		symbol.SetDayLineEndDate(10101010);
		EXPECT_EQ(symbol.GetDayLineEndDate(), 10101010);
	}

	TEST_F(CFinnhubForexSymbolTest, TestGetIPOStatus) {
		CFinnhubForex symbol;

		symbol.SetIPOStatus(_STOCK_IPOED_);
		EXPECT_EQ(symbol.GetIPOStatus(), _STOCK_IPOED_);
	}

	TEST_F(CFinnhubForexSymbolTest, TestGetUpdateDayLineDB) {
		CFinnhubForex symbol;

		EXPECT_FALSE(symbol.IsUpdateDayLineDB());
		symbol.SetUpdateDayLineDB(true);
		EXPECT_TRUE(symbol.IsUpdateDayLineDB());
	}

	TEST_F(CFinnhubForexSymbolTest, TestSetCheckDayLineStatus) {
		CFinnhubForex symbol;

		EXPECT_TRUE(symbol.IsUpdateDayLine());
		symbol.SetIPOStatus(_STOCK_NULL_);
		symbol.SetCheckingDayLineStatus();
		EXPECT_FALSE(symbol.IsUpdateDayLine());

		symbol.SetUpdateDayLine(true);
		symbol.SetIPOStatus(_STOCK_IPOED_);
		symbol.SetDayLineEndDate(gl_pWorldMarket->GetLastTradeDate());
		symbol.SetCheckingDayLineStatus();
		EXPECT_FALSE(symbol.IsUpdateDayLine());
	}

	TEST_F(CFinnhubForexSymbolTest, TestHaveNewDayLineData) {
		CFinnhubForex symbol;
		CDayLinesPtr pvDayLine = make_shared<vector<CDayLinePtr>>();

		const CDayLinePtr pDayLine = make_shared<CDayLine>();
		pDayLine->SetDate(20300102);
		pDayLine->SetClose(100);
		pvDayLine->push_back(pDayLine);

		EXPECT_FALSE(symbol.HaveNewDayLineData());
		symbol.UpdateDayLine(pvDayLine);
		symbol.SetDayLineEndDate(20200101); // 比日线最新数据旧
		EXPECT_TRUE(symbol.HaveNewDayLineData());
	}

	TEST_F(CFinnhubForexSymbolTest, TestUpdateDayLineStartEndDate) {
		CFinnhubForex symbol;
		CDayLinesPtr pvDayLine = make_shared<vector<CDayLinePtr>>();

		symbol.SetDayLineEndDate(19900101);
		symbol.SetDayLineStartDate(19900101);
		symbol.UpdateDayLineStartEndDate();
		EXPECT_EQ(symbol.GetDayLineStartDate(), 29900101);
		EXPECT_EQ(symbol.GetDayLineEndDate(), 19800101);

		EXPECT_EQ(symbol.GetDayLineSize(), 0);
		CDayLinePtr pDayLine = make_shared<CDayLine>();
		pDayLine->SetDate(20200102);
		pDayLine->SetClose(100);
		pvDayLine->push_back(pDayLine);
		pDayLine = make_shared<CDayLine>();
		pDayLine->SetDate(20200105);
		pDayLine->SetClose(100);
		pvDayLine->push_back(pDayLine);
		symbol.UpdateDayLine(pvDayLine);
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
		CFinnhubForex symbol;

		symbol.SetSymbol("ABCDE");
		const string str = symbol.GetFinnhubDayLineInquiryParam(123456789);
		if (gl_pWorldMarket->GetTimeZone() == 4 * 3600) { // 
			EXPECT_EQ(str, "ABCDE&resolution=D&from=315601200&to=123456789") << "当前时间小于19800101，315601200就是美东标准时间的19800101";
		}
		else {
			EXPECT_EQ(str, "ABCDE&resolution=D&from=315604800&to=123456789") << "当前时间小于19800101，315601200就是美东标准时间的19800101";
		}
	}
	TEST_F(CFinnhubForexSymbolTest, TestGetFinnhubDayLineInquiringString2) {
		CFinnhubForex symbol;

		symbol.SetSymbol("ABCDE");
		const string str = symbol.GetFinnhubDayLineInquiryParam(1131536000);
		EXPECT_EQ(str, "ABCDE&resolution=D&from=1100000000&to=1131536000") << "365 * 24 * 3600 = 31536000";
	}

	TEST_F(CFinnhubForexSymbolTest, TestAppendSymbol) {
		CSetFinnhubForexSymbol setFinnhubForexSymbol, setFinnhubForexSymbol2;
		CFinnhubForex FinnhubForexSymbol, FinnhubForexSymbol2;

		FinnhubForexSymbol.SetDescription("abc");
		FinnhubForexSymbol.SetDisplaySymbol("cba");
		FinnhubForexSymbol.SetSymbol("AAABC");
		FinnhubForexSymbol.SetExchangeCode("US");
		FinnhubForexSymbol.SetDayLineStartDate(20000101);
		FinnhubForexSymbol.SetDayLineEndDate(10000101);
		FinnhubForexSymbol.SetIPOStatus(_STOCK_DELISTED_);
		FinnhubForexSymbol.SetUpdateDayLine(false);
		FinnhubForexSymbol.SetUpdateDayLineDB(true);
		FinnhubForexSymbol.SetUpdateProfileDB(true);

		ASSERT(!gl_systemConfiguration.IsWorkingMode());
		setFinnhubForexSymbol.Open();
		setFinnhubForexSymbol.m_pDatabase->BeginTrans();
		FinnhubForexSymbol.AppendSymbol(setFinnhubForexSymbol);
		setFinnhubForexSymbol.m_pDatabase->CommitTrans();
		setFinnhubForexSymbol.Close();

		setFinnhubForexSymbol2.m_strFilter = "[Symbol] = 'AAABC'";
		setFinnhubForexSymbol2.Open();
		EXPECT_TRUE(!setFinnhubForexSymbol2.IsEOF()) << "此时已经存入了AAABC";
		FinnhubForexSymbol2.LoadSymbol(setFinnhubForexSymbol2);
		EXPECT_EQ(FinnhubForexSymbol.GetDescription(), "abc");
		EXPECT_EQ(FinnhubForexSymbol.GetDisplaySymbol(), "cba");
		EXPECT_EQ(FinnhubForexSymbol.GetSymbol(), "AAABC");
		EXPECT_EQ(FinnhubForexSymbol.GetExchangeCode(), "US");
		EXPECT_EQ(FinnhubForexSymbol.GetDayLineStartDate(), 20000101);
		EXPECT_EQ(FinnhubForexSymbol.GetDayLineEndDate(), 10000101);
		EXPECT_EQ(FinnhubForexSymbol.GetIPOStatus(), _STOCK_DELISTED_);
		EXPECT_FALSE(FinnhubForexSymbol.IsUpdateDayLine());
		EXPECT_TRUE(FinnhubForexSymbol.IsUpdateDayLineDB());
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
		CFinnhubForex FinnhubForexSymbol, FinnhubForexSymbol2;

		FinnhubForexSymbol.SetDescription("abc");
		FinnhubForexSymbol.SetDisplaySymbol("cba");
		FinnhubForexSymbol.SetSymbol("AAABB");
		FinnhubForexSymbol.SetExchangeCode("US");
		FinnhubForexSymbol.SetDayLineStartDate(20000101);
		FinnhubForexSymbol.SetDayLineEndDate(10000101);
		FinnhubForexSymbol.SetIPOStatus(_STOCK_DELISTED_);
		FinnhubForexSymbol.SetUpdateDayLine(false);
		FinnhubForexSymbol.SetUpdateDayLineDB(true);
		FinnhubForexSymbol.SetUpdateProfileDB(true);

		ASSERT(!gl_systemConfiguration.IsWorkingMode());
		setFinnhubForexSymbol.Open();
		setFinnhubForexSymbol.m_pDatabase->BeginTrans();
		FinnhubForexSymbol.AppendSymbol(setFinnhubForexSymbol);
		setFinnhubForexSymbol.m_pDatabase->CommitTrans();
		setFinnhubForexSymbol.Close();

		// 改成新值
		FinnhubForexSymbol.SetDescription("abc changed");
		FinnhubForexSymbol.SetDisplaySymbol("changed");
		FinnhubForexSymbol.SetSymbol("AAABB");
		FinnhubForexSymbol.SetExchangeCode("US changed");
		FinnhubForexSymbol.SetDayLineStartDate(101);
		FinnhubForexSymbol.SetDayLineEndDate(101);
		FinnhubForexSymbol.SetIPOStatus(_STOCK_IPOED_);
		FinnhubForexSymbol.SetUpdateDayLine(TRUE);
		FinnhubForexSymbol.SetUpdateDayLineDB(FALSE);
		FinnhubForexSymbol.SetUpdateProfileDB(FALSE);

		setFinnhubForexSymbol3.m_strFilter = "[Symbol] = 'AAABB'";
		setFinnhubForexSymbol3.Open();
		setFinnhubForexSymbol3.m_pDatabase->BeginTrans();
		FinnhubForexSymbol.UpdateSymbol(setFinnhubForexSymbol3);
		setFinnhubForexSymbol3.m_pDatabase->CommitTrans();
		setFinnhubForexSymbol3.Close();

		setFinnhubForexSymbol2.m_strFilter = "[Symbol] = 'AAABB'";
		setFinnhubForexSymbol2.Open();
		EXPECT_TRUE(!setFinnhubForexSymbol2.IsEOF()) << "此时已经存入了AA";
		FinnhubForexSymbol2.LoadSymbol(setFinnhubForexSymbol2);
		EXPECT_EQ(FinnhubForexSymbol.GetDescription(), "abc changed");
		EXPECT_EQ(FinnhubForexSymbol.GetDisplaySymbol(), "changed");
		EXPECT_EQ(FinnhubForexSymbol.GetSymbol(), "AAABB");
		EXPECT_EQ(FinnhubForexSymbol.GetExchangeCode(), "US changed");
		EXPECT_EQ(FinnhubForexSymbol.GetDayLineStartDate(), 101);
		EXPECT_EQ(FinnhubForexSymbol.GetDayLineEndDate(), 101);
		EXPECT_EQ(FinnhubForexSymbol.GetIPOStatus(), _STOCK_IPOED_);
		EXPECT_TRUE(FinnhubForexSymbol.IsUpdateDayLine());
		EXPECT_FALSE(FinnhubForexSymbol.IsUpdateDayLineDB());
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
		CFinnhubForex FinnhubForexSymbol, FinnhubForexSymbol2;
		auto pDayLine = make_shared<CDayLine>();
		CSetForexDayLine setForexDayLine;
		CDayLinesPtr pvDayLine = make_shared<vector<CDayLinePtr>>();

		pDayLine->SetDate(19800101);
		pDayLine->SetClose(100);
		pDayLine->SetStockSymbol("OANDA:AUD_SGD");
		pvDayLine->push_back(pDayLine);
		FinnhubForexSymbol.SetSymbol("OANDA:AUD_SGD");
		FinnhubForexSymbol.UpdateDayLine(pvDayLine);

		EXPECT_THAT(FinnhubForexSymbol.GetDayLineStartDate(), Eq(29900101));

		FinnhubForexSymbol.UpdateDayLineDB();

		setForexDayLine.m_strFilter = "[Symbol] = 'OANDA:AUD_SGD'";
		setForexDayLine.m_strSort = "[Date]";
		setForexDayLine.Open();
		EXPECT_EQ(setForexDayLine.m_Date, 19800101) << "刚存储的数据";
		setForexDayLine.m_pDatabase->BeginTrans();
		setForexDayLine.Delete();
		setForexDayLine.m_pDatabase->CommitTrans();
		setForexDayLine.Close();
	}
}
