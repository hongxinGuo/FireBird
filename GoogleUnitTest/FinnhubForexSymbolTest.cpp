#include"pch.h"

#include "dataBaseConnector.h"
#include"WorldMarket.h"
#include"GeneralCheck.h"

#include"FinnhubForex.h"

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
		EXPECT_EQ(FinnhubForexSymbol.GetExchange(), "");
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
		CDayLinesPtr pvDayLine = make_shared<vector<CDayLine>>();

		CDayLine dayLine;
		dayLine.SetDate(20300102);
		dayLine.SetClose(100);
		pvDayLine->push_back(dayLine);

		EXPECT_FALSE(symbol.HaveNewDayLineData());
		symbol.UpdateDayLine(pvDayLine);
		symbol.SetDayLineEndDate(20200101); // 比日线最新数据旧
		EXPECT_TRUE(symbol.HaveNewDayLineData());
	}

	TEST_F(CFinnhubForexSymbolTest, TestUpdateDayLineStartEndDate) {
		CFinnhubForex symbol;
		CDayLinesPtr pvDayLine = make_shared<vector<CDayLine>>();

		symbol.SetDayLineEndDate(19900101);
		symbol.SetDayLineStartDate(19900101);
		symbol.UpdateDayLineStartEndDate();
		EXPECT_EQ(symbol.GetDayLineStartDate(), 29900101);
		EXPECT_EQ(symbol.GetDayLineEndDate(), 19800101);

		EXPECT_EQ(symbol.GetDayLineSize(), 0);
		CDayLine dayLine;
		dayLine.SetDate(20200102);
		dayLine.SetClose(100);
		pvDayLine->push_back(dayLine);
		dayLine = CDayLine();
		dayLine.SetDate(20200105);
		dayLine.SetClose(100);
		pvDayLine->push_back(dayLine);
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

	TEST_F(CFinnhubForexSymbolTest, TestSaveDayLine) {
		CFinnhubForex FinnhubForexSymbol, FinnhubForexSymbol2;
		CDayLine dayLine;
		CDayLinesPtr pvDayLine = make_shared<vector<CDayLine>>();

		dayLine.SetDate(19800101);
		dayLine.SetClose(100);
		dayLine.SetStockSymbol("OANDA:AUD_SGD");
		dayLine.SetExchange("Test"); // 用于删除遗留的测试数据
		pvDayLine->push_back(dayLine);
		FinnhubForexSymbol.SetSymbol("OANDA:AUD_SGD");
		FinnhubForexSymbol.UpdateDayLine(pvDayLine);

		EXPECT_THAT(FinnhubForexSymbol.GetDayLineStartDate(), Eq(29900101));

		FinnhubForexSymbol.UpdateDayLineDB();

		using namespace StockMarket;
		const auto& t = FinnhubForexDayline{};
		auto db = gl_dbStockMarket.get();
		auto tx = sqlpp::start_transaction(db);

		auto result = db(select(all_of(t)).from(t).where(t.Symbol == "OANDA:AUD_SGD" && t.Date == 19800101).order_by(t.Date.asc()));
		size_t rows = result.size();
		EXPECT_EQ(rows, 1) << "刚存储的数据";
		auto& row = result.front();
		string exchange = row.Exchange;
		double value = row.Close * FinnhubForexSymbol.GetRatio();
		EXPECT_DOUBLE_EQ(value, 100);

		db(sqlpp::remove_from(t).where(t.Symbol == "OANDA:AUD_SGD" && t.Date == 19800101));
		tx.commit();
	}
}
