#include"pch.h"

#include "dataBaseConnector.h"
#include"WorldMarket.h"
#include"GeneralCheck.h"

#include"FinnhubCrypto.h"

using namespace testing;

namespace FireBirdTest {
	class CFinnhubCryptoSymbolTest : public ::testing::Test {
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

	TEST_F(CFinnhubCryptoSymbolTest, TestInitialize) {
		CFinnhubCrypto FinnhubCryptoSymbol;
		EXPECT_EQ(FinnhubCryptoSymbol.GetDescription(), "");
		EXPECT_EQ(FinnhubCryptoSymbol.GetDisplaySymbol(), "");
		EXPECT_EQ(FinnhubCryptoSymbol.GetSymbol(), "");
		EXPECT_EQ(FinnhubCryptoSymbol.GetExchange(), "");
		EXPECT_EQ(FinnhubCryptoSymbol.GetDayLineStartDate(), 29900101);
		EXPECT_EQ(FinnhubCryptoSymbol.GetDayLineEndDate(), 19800101);
		EXPECT_EQ(FinnhubCryptoSymbol.GetIPOStatus(), _STOCK_NOT_CHECKED_);
		EXPECT_TRUE(FinnhubCryptoSymbol.IsUpdateDayLine());
		EXPECT_FALSE(FinnhubCryptoSymbol.IsUpdateDayLineDB());
		EXPECT_FALSE(FinnhubCryptoSymbol.IsUpdateProfileDB());
	}

	TEST_F(CFinnhubCryptoSymbolTest, TestGetRatio) {
		const CFinnhubCrypto symbol;

		EXPECT_EQ(symbol.GetRatio(), 1000);
	}

	TEST_F(CFinnhubCryptoSymbolTest, TestGetDescription) {
		CFinnhubCrypto symbol;

		symbol.SetDescription("abcdefg");
		EXPECT_EQ(symbol.GetDescription(), "abcdefg");
	}

	TEST_F(CFinnhubCryptoSymbolTest, TestGetDisplaySymbol) {
		CFinnhubCrypto symbol;

		symbol.SetDisplaySymbol("abcdefg");
		EXPECT_EQ(symbol.GetDisplaySymbol(), "abcdefg");
	}

	TEST_F(CFinnhubCryptoSymbolTest, TestGetSymbol) {
		CFinnhubCrypto symbol;

		symbol.SetSymbol("abcdefg");
		EXPECT_EQ(symbol.GetSymbol(), "abcdefg");
	}

	TEST_F(CFinnhubCryptoSymbolTest, TestGetDayLineStartDate) {
		CFinnhubCrypto symbol;

		symbol.SetDayLineStartDate(20202020);
		EXPECT_EQ(symbol.GetDayLineStartDate(), 20202020);
	}

	TEST_F(CFinnhubCryptoSymbolTest, TestGetDayLineEndDate) {
		CFinnhubCrypto symbol;

		symbol.SetDayLineEndDate(10101010);
		EXPECT_EQ(symbol.GetDayLineEndDate(), 10101010);
	}

	TEST_F(CFinnhubCryptoSymbolTest, TestGetIPOStatus) {
		CFinnhubCrypto symbol;

		symbol.SetIPOStatus(_STOCK_IPOED_);
		EXPECT_EQ(symbol.GetIPOStatus(), _STOCK_IPOED_);
	}

	TEST_F(CFinnhubCryptoSymbolTest, TestGetUpdateDayLineDB) {
		CFinnhubCrypto symbol;

		EXPECT_FALSE(symbol.IsUpdateDayLineDB());
		symbol.SetUpdateDayLineDB(true);
		EXPECT_TRUE(symbol.IsUpdateDayLineDB());
	}

	TEST_F(CFinnhubCryptoSymbolTest, TestSetCheckDayLineStatus) {
		CFinnhubCrypto symbol;

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

	TEST_F(CFinnhubCryptoSymbolTest, TestHaveNewDayLineData) {
		CFinnhubCrypto symbol;
		CDayLinesPtr pvDayLine = make_shared<vector<CDayLine>>();

		CDayLine dayLine;
		dayLine.SetDate(20200102);
		dayLine.SetClose(100); // 确保此数据为有效数据
		pvDayLine->push_back(dayLine);

		EXPECT_FALSE(symbol.HaveNewDayLineData());
		symbol.UpdateDayLine(pvDayLine);
		symbol.SetDayLineEndDate(20200101); // 比日线最新数据旧
		EXPECT_TRUE(symbol.HaveNewDayLineData());
	}

	TEST_F(CFinnhubCryptoSymbolTest, TestUpdateDayLineStartEndDate) {
		CFinnhubCrypto symbol;
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

	TEST_F(CFinnhubCryptoSymbolTest, TestGetFinnhubDayLineInquiringString1) {
		CFinnhubCrypto symbol;

		symbol.SetSymbol("ABCDE");
		const string str = symbol.GetFinnhubDayLineInquiryParam(123456789);
		EXPECT_EQ(str, "ABCDE&resolution=D&from=91920789&to=123456789") << "91920789比123456789早一年（365天）";
	}

	TEST_F(CFinnhubCryptoSymbolTest, TestGetFinnhubDayLineInquiringString2) {
		CFinnhubCrypto symbol;

		symbol.SetSymbol("ABCDE");
		const string str = symbol.GetFinnhubDayLineInquiryParam(1131536000);
		EXPECT_EQ(str, "ABCDE&resolution=D&from=1100000000&to=1131536000") << "365 * 24 * 3600 = 31536000";
	}

	TEST_F(CFinnhubCryptoSymbolTest, TestUpdateDayLineDB) {
		CFinnhubCrypto FinnhubCryptoSymbol, FinnhubCryptoSymbol2;
		CDayLine dayLine;
		CDayLinesPtr pvDayLine = make_shared<vector<CDayLine>>();

		dayLine.SetDate(19800101);
		dayLine.SetClose(100);
		dayLine.SetStockSymbol("NewSymbol");
		dayLine.SetExchange("Test"); // 用于删除测试数据
		pvDayLine->push_back(dayLine);
		FinnhubCryptoSymbol.SetSymbol("NewSymbol"); // 新代码
		FinnhubCryptoSymbol.UpdateDayLine(pvDayLine);

		FinnhubCryptoSymbol.UpdateDayLineDB();

		using namespace StockMarket;
		const auto& t = FinnhubCryptoDayline{};
		auto db = gl_dbStockMarket.get();
		auto tx = start_transaction(db);

		auto result = db(select(all_of(t)).from(t).where(t.Symbol == "NewSymbol").order_by(t.Date.asc()));
		size_t rows = result.size();
		EXPECT_EQ(rows, 1) << "新存储数据的行数";
		auto& row = result.front();
		EXPECT_EQ(row.Date.value(), 19800101) << "新存储数据的日期";

		db(sqlpp::remove_from(t).where(t.Symbol == "NewSymbol"));
		tx.commit();
	}
}
