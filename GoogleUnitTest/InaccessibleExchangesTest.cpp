#include"pch.h"

#include"WorldMarket.h"

#include"GeneralCheck.h"

#include"FinnhubInaccessibleExchange.h"
#include"nlohmannJsonDeclaration.h" // 按照顺序输出json，必须使用此ordered_json,以保证解析后的数据与解析前的顺序一致。

namespace {
	std::string s_sFinnhubInaccessibleExchange = R"(
{ "UpdateDate" : 20221205,
 "InaccessibleExchange" :
[
{
	"Function" : "StockFundamentalsCompanyProfileConcise",
	"Exchange" : ["SS", "SZ"]
},
{
	"Function" : "StockFundamentalsBasicFinancials",
	"Exchange" : ["SS", "SZ"]
}
]})";
}

namespace FireBirdTest {
	class CFinnhubInaccessibleExchangeTest : public testing::Test {
		void SetUp() override {
			SCOPED_TRACE("");
			GeneralCheck();
		}

		void TearDown() override {
			gl_systemConfiguration.SetWorkingMode(false);
			gl_finnhubInaccessibleExchange.SetUpdateDB(false);

			SCOPED_TRACE("");
			GeneralCheck();
		}
	};

	TEST_F(CFinnhubInaccessibleExchangeTest, TestGlobeVariable) {
		nlohmannJson jsFinnhubInaccessibleExchange = nlohmannJson::parse(s_sFinnhubInaccessibleExchange);
		CInaccessible exchange;
		const string s2 = jsFinnhubInaccessibleExchange["InaccessibleExchange"][0]["Function"];
		exchange.SetFunctionString(s2);
		for (const auto& i : jsFinnhubInaccessibleExchange["InaccessibleExchange"][0]["Exchange"]) {
			string s = i;
			exchange.AddSymbol(s);
		}
		EXPECT_EQ(jsFinnhubInaccessibleExchange["UpdateDate"], 20221205);
		EXPECT_EQ(exchange.GetFunctionString(), "StockFundamentalsCompanyProfileConcise");
		EXPECT_EQ(exchange.GetSymbol(0), "SS");
		EXPECT_EQ(exchange.GetSymbol(1), "SZ");
	}

	TEST_F(CFinnhubInaccessibleExchangeTest, TestDeleteAllUSExchange) {
		EXPECT_FALSE(gl_finnhubInaccessibleExchange.HaveExchange(STOCK_PRICE_CANDLES_, "US"));
		EXPECT_FALSE(gl_finnhubInaccessibleExchange.HaveExchange(COMPANY_NEWS_, "US"));
		EXPECT_FALSE(gl_finnhubInaccessibleExchange.HaveExchange(INSIDER_TRANSACTION_, "US"));

		// 这三个接口已存在
		gl_finnhubInaccessibleExchange.AddExchange(COMPANY_NEWS_, "US");
		gl_finnhubInaccessibleExchange.AddExchange(MARKET_HOLIDAY_, "US");
		gl_finnhubInaccessibleExchange.AddExchange(STOCK_PRICE_CANDLES_, "US");

		EXPECT_TRUE(gl_finnhubInaccessibleExchange.HaveExchange(STOCK_PRICE_CANDLES_, "US"));
		EXPECT_TRUE(gl_finnhubInaccessibleExchange.HaveExchange(COMPANY_NEWS_, "US"));
		EXPECT_TRUE(gl_finnhubInaccessibleExchange.HaveExchange(MARKET_HOLIDAY_, "US"));

		gl_finnhubInaccessibleExchange.DeleteAllUSExchange();

		EXPECT_FALSE(gl_finnhubInaccessibleExchange.HaveExchange(STOCK_PRICE_CANDLES_, "US"));
		EXPECT_FALSE(gl_finnhubInaccessibleExchange.HaveExchange(COMPANY_NEWS_, "US"));
		EXPECT_FALSE(gl_finnhubInaccessibleExchange.HaveExchange(MARKET_HOLIDAY_, "US"));

		// 恢复原状
		gl_finnhubInaccessibleExchange.SetUpdateDB(false);
	}

	TEST_F(CFinnhubInaccessibleExchangeTest, TestAddExchange) {
		const auto pExchange = make_shared<CInaccessible>();
		pExchange->SetFunctionString("WebSocketTrades");
		pExchange->AddSymbol("SS");
		pExchange->AddSymbol("SZ");

		gl_finnhubInaccessibleExchange.SetExchange(gl_FinnhubInquiryType.GetInquiryType("WebSocketTrades"), pExchange);
		gl_finnhubInaccessibleExchange.AddExchange(gl_FinnhubInquiryType.GetInquiryType("WebSocketTrades"), "US");

		auto p = gl_finnhubInaccessibleExchange.GetExchange(gl_FinnhubInquiryType.GetInquiryType("WebSocketTrades"));
		EXPECT_TRUE(p->HaveSymbol("US"));
		gl_finnhubInaccessibleExchange.DeleteExchange(gl_FinnhubInquiryType.GetInquiryType("WebSocketTrades"), "US");

		p = gl_finnhubInaccessibleExchange.GetExchange(gl_FinnhubInquiryType.GetInquiryType("WebSocketTrades"));
		EXPECT_FALSE(p->HaveSymbol("US"));

		// 恢复原状
		gl_finnhubInaccessibleExchange.SetUpdateDB(false);
	}

	TEST_F(CFinnhubInaccessibleExchangeTest, TestDeleteExchange) {
		const auto pExchange = make_shared<CInaccessible>();
		pExchange->SetFunctionString("WebSocketTrades");
		pExchange->AddSymbol("SS");
		pExchange->AddSymbol("SZ");

		gl_finnhubInaccessibleExchange.SetExchange(gl_FinnhubInquiryType.GetInquiryType("WebSocketTrades"), pExchange);
		gl_finnhubInaccessibleExchange.DeleteExchange(gl_FinnhubInquiryType.GetInquiryType("WebSocketTrades"), "SS");

		const auto p = gl_finnhubInaccessibleExchange.GetExchange(gl_FinnhubInquiryType.GetInquiryType("WebSocketTrades"));
		EXPECT_FALSE(p->HaveSymbol("SS"));

		// 恢复原状
		gl_finnhubInaccessibleExchange.DeleteExchange(gl_FinnhubInquiryType.GetInquiryType("WebSocketTrades"), "SZ");
		gl_finnhubInaccessibleExchange.SetUpdateDB(false);
	}

	TEST_F(CFinnhubInaccessibleExchangeTest, TestSaveDB1) {
		filesystem::remove(gl_systemConfiguration.GetConfigurationFileDirectory() + "FinnhubInaccessibleExchangeTest.json");
		gl_finnhubInaccessibleExchange.SetFileName("FinnhubInaccessibleExchangeTest.json");

		const auto pExchange = make_shared<CInaccessible>();
		pExchange->SetFunctionString("WebSocketTrades");
		pExchange->AddSymbol("SS");
		pExchange->AddSymbol("SZ");

		gl_finnhubInaccessibleExchange.SetUpdateDate(20230101);
		gl_finnhubInaccessibleExchange.SetExchange(gl_finnhubInaccessibleExchange.GetFinnhubInquiryIndex(pExchange->GetFunctionString()), pExchange);
		gl_finnhubInaccessibleExchange.UpdateJson();
		gl_finnhubInaccessibleExchange.SaveDB();

		gl_finnhubInaccessibleExchange.Clear();
		gl_finnhubInaccessibleExchange.LoadDB();
		gl_finnhubInaccessibleExchange.Update();
		EXPECT_EQ(gl_finnhubInaccessibleExchange.GetUpdateDate(), 20230101);
		EXPECT_EQ(gl_finnhubInaccessibleExchange.GetExchange(gl_finnhubInaccessibleExchange.GetFinnhubInquiryIndex(pExchange->GetFunctionString()))->GetFunctionString(), "WebSocketTrades");
		EXPECT_EQ(gl_finnhubInaccessibleExchange.GetExchange(gl_finnhubInaccessibleExchange.GetFinnhubInquiryIndex(pExchange->GetFunctionString()))->Size(), 2);
		const string str = gl_finnhubInaccessibleExchange.GetExchange(gl_finnhubInaccessibleExchange.GetFinnhubInquiryIndex(pExchange->GetFunctionString()))->GetSymbol(0);
		EXPECT_TRUE(str == "SS");

		// 恢复原状
		std::filesystem::remove(gl_systemConfiguration.GetConfigurationFileDirectory() + "FinnhubInaccessibleExchangeTest.json");
		gl_finnhubInaccessibleExchange.SetFileName("FinnhubInaccessibleExchange.json");
	}

	TEST_F(CFinnhubInaccessibleExchangeTest, TestSaveDB2) {
		filesystem::remove(gl_systemConfiguration.GetConfigurationFileDirectory() + "FinnhubInaccessibleExchangeTest.json");
		gl_finnhubInaccessibleExchange.SetFileName("FinnhubInaccessibleExchangeTest.json");

		const auto pExchange = make_shared<CInaccessible>();
		pExchange->SetFunctionString("WebSocketTrades");
		pExchange->AddSymbol("SS");
		pExchange->AddSymbol("SZ");

		gl_finnhubInaccessibleExchange.SetUpdateDate(20230101);
		gl_finnhubInaccessibleExchange.SetExchange(gl_finnhubInaccessibleExchange.GetFinnhubInquiryIndex(pExchange->GetFunctionString()), pExchange);
		gl_finnhubInaccessibleExchange.UpdateJson();
		gl_finnhubInaccessibleExchange.SaveDB();

		const string strFileDirectory = gl_systemConfiguration.GetConfigurationFileDirectory();
		gl_finnhubInaccessibleExchange.Clear();
		gl_finnhubInaccessibleExchange.LoadDB(strFileDirectory);
		gl_finnhubInaccessibleExchange.Update();
		EXPECT_EQ(gl_finnhubInaccessibleExchange.GetUpdateDate(), 20230101);
		EXPECT_EQ(gl_finnhubInaccessibleExchange.GetExchange(gl_finnhubInaccessibleExchange.GetFinnhubInquiryIndex(pExchange->GetFunctionString()))->GetFunctionString(), "WebSocketTrades");
		EXPECT_EQ(gl_finnhubInaccessibleExchange.GetExchange(gl_finnhubInaccessibleExchange.GetFinnhubInquiryIndex(pExchange->GetFunctionString()))->Size(), 2);
		const string str = gl_finnhubInaccessibleExchange.GetExchange(gl_finnhubInaccessibleExchange.GetFinnhubInquiryIndex(pExchange->GetFunctionString()))->GetSymbol(0);
		EXPECT_EQ(str, "SS");

		// 恢复原状
		filesystem::remove(gl_systemConfiguration.GetConfigurationFileDirectory() + "FinnhubInaccessibleExchangeTest.json");
		gl_finnhubInaccessibleExchange.SetFileName("FinnhubInaccessibleExchange.json");
	}
}
