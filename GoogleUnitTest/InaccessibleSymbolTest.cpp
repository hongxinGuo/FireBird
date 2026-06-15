#include"pch.h"

#include"GeneralCheck.h"

#include"WorldMarket.h"
#include"InaccessibleSymbol.h"
#include "Initialization.h"
#include"nlohmannJsonDeclaration.h" // 按照顺序输出json，必须使用此ordered_json,以保证解析后的数据与解析前的顺序一致。

namespace {
	std::string s_sFinnhubInaccessibleExchange = R"(
{ "UpdateDate" : 20221205,
 "InaccessibleSymbol" :
[
{
	"Function" : "StockFundamentalsCompanyProfileConcise",
	"Symbol" : ["SS", "SZ"]
},
{
	"Function" : "StockFundamentalsBasicFinancials",
	"Symbol" : ["SS", "SZ"]
}
]})";

	std::string Test_gl_sTiingoInaccessibleStock = R"(
{ "UpdateDate" : 20221205,
 "InaccessibleSymbol" :
[
{
	"Function" : "StockDayLine",
	"Symbol" : ["A", "AA"]
},
{
	"Function" : "StockCompanyProfile",
	"Symbol" : ["AAPL", "IBM"]
}
]})";
}

namespace FireBirdTest {
	class CInaccessibleSymbolTest : public testing::Test {
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

	TEST_F(CInaccessibleSymbolTest, TestGlobeVariable) {
		nlohmannJson jsFinnhubInaccessibleExchange = nlohmannJson::parse(s_sFinnhubInaccessibleExchange);
		CInaccessible exchange;
		const string s2 = jsFinnhubInaccessibleExchange["InaccessibleSymbol"][0]["Function"];
		exchange.SetFunctionString(s2);
		for (const auto& i : jsFinnhubInaccessibleExchange["InaccessibleSymbol"][0]["Symbol"]) {
			string s = i;
			exchange.AddSymbol(s);
		}
		EXPECT_EQ(jsFinnhubInaccessibleExchange["UpdateDate"], 20221205);
		EXPECT_EQ(exchange.GetFunctionString(), "StockFundamentalsCompanyProfileConcise");
		EXPECT_EQ(exchange.GetSymbol(0), "SS");
		EXPECT_EQ(exchange.GetSymbol(1), "SZ");
	}

	TEST_F(CInaccessibleSymbolTest, TestDeleteAllUSExchange) {
		EXPECT_FALSE(gl_finnhubInaccessibleExchange.IsInaccessible(STOCK_PRICE_CANDLES_, "US"));
		EXPECT_FALSE(gl_finnhubInaccessibleExchange.IsInaccessible(COMPANY_NEWS_, "US"));
		EXPECT_FALSE(gl_finnhubInaccessibleExchange.IsInaccessible(INSIDER_TRANSACTION_, "US"));

		// 这三个接口已存在
		gl_finnhubInaccessibleExchange.AddInaccessible(COMPANY_NEWS_, "US");
		gl_finnhubInaccessibleExchange.AddInaccessible(MARKET_HOLIDAY_, "US");
		gl_finnhubInaccessibleExchange.AddInaccessible(STOCK_PRICE_CANDLES_, "US");

		EXPECT_TRUE(gl_finnhubInaccessibleExchange.IsInaccessible(STOCK_PRICE_CANDLES_, "US"));
		EXPECT_TRUE(gl_finnhubInaccessibleExchange.IsInaccessible(COMPANY_NEWS_, "US"));
		EXPECT_TRUE(gl_finnhubInaccessibleExchange.IsInaccessible(MARKET_HOLIDAY_, "US"));

		DeleteAllFinnhubInaccessibleUSExchange();

		EXPECT_FALSE(gl_finnhubInaccessibleExchange.IsInaccessible(STOCK_PRICE_CANDLES_, "US"));
		EXPECT_FALSE(gl_finnhubInaccessibleExchange.IsInaccessible(COMPANY_NEWS_, "US"));
		EXPECT_FALSE(gl_finnhubInaccessibleExchange.IsInaccessible(MARKET_HOLIDAY_, "US"));

		// 恢复原状
		gl_finnhubInaccessibleExchange.SetUpdateDB(false);
	}

	TEST_F(CInaccessibleSymbolTest, TestAddSymbol) {
		const auto pInaccessible = make_shared<CInaccessible>();
		pInaccessible->SetFunctionString("WebSocketTrades");
		pInaccessible->AddSymbol("SS");
		pInaccessible->AddSymbol("SZ");

		gl_finnhubInaccessibleExchange.SetInaccessible(gl_FinnhubInquiryType.GetInquiryType("WebSocketTrades"), pInaccessible);
		gl_finnhubInaccessibleExchange.AddInaccessible(gl_FinnhubInquiryType.GetInquiryType("WebSocketTrades"), "US");

		auto p = gl_finnhubInaccessibleExchange.GetInaccessible(gl_FinnhubInquiryType.GetInquiryType("WebSocketTrades"));
		EXPECT_TRUE(p->HaveSymbol("US"));
		gl_finnhubInaccessibleExchange.DeleteInaccessible(gl_FinnhubInquiryType.GetInquiryType("WebSocketTrades"), "US");

		p = gl_finnhubInaccessibleExchange.GetInaccessible(gl_FinnhubInquiryType.GetInquiryType("WebSocketTrades"));
		EXPECT_FALSE(p->HaveSymbol("US"));

		// 恢复原状
		gl_finnhubInaccessibleExchange.SetUpdateDB(false);
	}

	TEST_F(CInaccessibleSymbolTest, TestDeleteSymbol) {
		const auto pInaccessible = make_shared<CInaccessible>();
		pInaccessible->SetFunctionString("WebSocketTrades");
		pInaccessible->AddSymbol("SS");
		pInaccessible->AddSymbol("SZ");

		gl_finnhubInaccessibleExchange.SetInaccessible(gl_FinnhubInquiryType.GetInquiryType("WebSocketTrades"), pInaccessible);
		gl_finnhubInaccessibleExchange.DeleteInaccessible(gl_FinnhubInquiryType.GetInquiryType("WebSocketTrades"), "SS");

		const auto p = gl_finnhubInaccessibleExchange.GetInaccessible(gl_FinnhubInquiryType.GetInquiryType("WebSocketTrades"));
		EXPECT_FALSE(p->HaveSymbol("SS"));

		// 恢复原状
		gl_finnhubInaccessibleExchange.DeleteInaccessible(gl_FinnhubInquiryType.GetInquiryType("WebSocketTrades"), "SZ");
		gl_finnhubInaccessibleExchange.SetUpdateDB(false);
	}

	TEST_F(CInaccessibleSymbolTest, TestSaveDB1) {
		filesystem::remove(gl_systemConfiguration.GetConfigurationFileDirectory() + "FinnhubInaccessibleExchangeTest.json");
		gl_finnhubInaccessibleExchange.SetFileName("FinnhubInaccessibleExchangeTest.json");

		const auto pInaccessible = make_shared<CInaccessible>();
		pInaccessible->SetFunctionString("WebSocketTrades");
		pInaccessible->AddSymbol("SS");
		pInaccessible->AddSymbol("SZ");

		gl_finnhubInaccessibleExchange.SetUpdateDate(toLocalDays(20230101));
		gl_finnhubInaccessibleExchange.SetInaccessible(gl_finnhubInaccessibleExchange.GetInquiryIndex(pInaccessible->GetFunctionString()), pInaccessible);
		gl_finnhubInaccessibleExchange.UpdateJson();
		gl_finnhubInaccessibleExchange.SaveDB();

		gl_finnhubInaccessibleExchange.Clear();
		gl_finnhubInaccessibleExchange.LoadDB();
		gl_finnhubInaccessibleExchange.Update();
		EXPECT_EQ(gl_finnhubInaccessibleExchange.GetUpdateDate(), toLocalDays(20230101));
		EXPECT_EQ(gl_finnhubInaccessibleExchange.GetInaccessible(gl_finnhubInaccessibleExchange.GetInquiryIndex(pInaccessible->GetFunctionString()))->GetFunctionString(), "WebSocketTrades");
		EXPECT_EQ(gl_finnhubInaccessibleExchange.GetInaccessible(gl_finnhubInaccessibleExchange.GetInquiryIndex(pInaccessible->GetFunctionString()))->Size(), 2);
		const string str = gl_finnhubInaccessibleExchange.GetInaccessible(gl_finnhubInaccessibleExchange.GetInquiryIndex(pInaccessible->GetFunctionString()))->GetSymbol(0);
		EXPECT_TRUE(str == "SS");

		// 恢复原状
		std::filesystem::remove(gl_systemConfiguration.GetConfigurationFileDirectory() + "FinnhubInaccessibleExchangeTest.json");
		gl_finnhubInaccessibleExchange.SetFileName("FinnhubInaccessibleExchange.json");
	}

	TEST_F(CInaccessibleSymbolTest, TestSaveDB2) {
		filesystem::remove(gl_systemConfiguration.GetConfigurationFileDirectory() + "FinnhubInaccessibleExchangeTest.json");
		gl_finnhubInaccessibleExchange.SetFileName("FinnhubInaccessibleExchangeTest.json");

		const auto pInaccessible = make_shared<CInaccessible>();
		pInaccessible->SetFunctionString("WebSocketTrades");
		pInaccessible->AddSymbol("SS");
		pInaccessible->AddSymbol("SZ");

		gl_finnhubInaccessibleExchange.SetUpdateDate(toLocalDays(20230101));
		gl_finnhubInaccessibleExchange.SetInaccessible(gl_finnhubInaccessibleExchange.GetInquiryIndex(pInaccessible->GetFunctionString()), pInaccessible);
		gl_finnhubInaccessibleExchange.UpdateJson();
		gl_finnhubInaccessibleExchange.SaveDB();

		const string strFileDirectory = gl_systemConfiguration.GetConfigurationFileDirectory();
		gl_finnhubInaccessibleExchange.Clear();
		gl_finnhubInaccessibleExchange.LoadDB(strFileDirectory);
		gl_finnhubInaccessibleExchange.Update();
		EXPECT_EQ(gl_finnhubInaccessibleExchange.GetUpdateDate(), toLocalDays(20230101));
		EXPECT_EQ(gl_finnhubInaccessibleExchange.GetInaccessible(gl_finnhubInaccessibleExchange.GetInquiryIndex(pInaccessible->GetFunctionString()))->GetFunctionString(), "WebSocketTrades");
		EXPECT_EQ(gl_finnhubInaccessibleExchange.GetInaccessible(gl_finnhubInaccessibleExchange.GetInquiryIndex(pInaccessible->GetFunctionString()))->Size(), 2);
		const string str = gl_finnhubInaccessibleExchange.GetInaccessible(gl_finnhubInaccessibleExchange.GetInquiryIndex(pInaccessible->GetFunctionString()))->GetSymbol(0);
		EXPECT_EQ(str, "SS");

		// 恢复原状
		filesystem::remove(gl_systemConfiguration.GetConfigurationFileDirectory() + "FinnhubInaccessibleExchangeTest.json");
		gl_finnhubInaccessibleExchange.SetFileName("FinnhubInaccessibleExchange.json");
	}
}
