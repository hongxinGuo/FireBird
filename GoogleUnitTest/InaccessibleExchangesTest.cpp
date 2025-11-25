#include"pch.h"

#include"WorldMarket.h"

#include"GeneralCheck.h"

#include"FinnhubInaccessibleExchange.h"
#include"nlohmannJsonDeclaration.h" // 按照顺序输出json，必须使用此ordered_json,以保证解析后的数据与解析前的顺序一致。

namespace FireBirdTest {
	class CFinnhubInaccessibleExchangeTest : public testing::Test {
		void SetUp() override {
			SCOPED_TRACE("");
			GeneralCheck();
		}

		void TearDown() override {
			gl_systemConfiguration.SetWorkingMode(false);

			SCOPED_TRACE("");
			GeneralCheck();
		}
	};

	TEST_F(CFinnhubInaccessibleExchangeTest, TestGlobeVariable) {
		json jsFinnhubInaccessibleExchange = json::parse(Test_gl_sFinnhubInaccessibleExchange);
		CInaccessible exchange;
		const string s2 = jsFinnhubInaccessibleExchange["InaccessibleExchange"][0]["Function"];
		exchange.SetFunctionString(s2);
		for (int i = 0; i < jsFinnhubInaccessibleExchange["InaccessibleExchange"][0]["Exchange"].size(); i++) {
			string s = jsFinnhubInaccessibleExchange["InaccessibleExchange"][0]["Exchange"][i];
			exchange.AddSymbol(s);
		}
		EXPECT_EQ(jsFinnhubInaccessibleExchange["UpdateDate"], 20221205);
		EXPECT_EQ(exchange.GetFunctionString(), "StockFundamentalsCompanyProfileConcise");
		EXPECT_EQ(exchange.GetSymbol(0), "SS");
		EXPECT_EQ(exchange.GetSymbol(1), "SZ");
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
		gl_finnhubInaccessibleExchange.SetDefaultFileName("FinnhubInaccessibleExchangeTest.json");

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
		EXPECT_EQ(gl_finnhubInaccessibleExchange.GetItemSize(), 1);
		EXPECT_EQ(gl_finnhubInaccessibleExchange.GetExchange(gl_finnhubInaccessibleExchange.GetFinnhubInquiryIndex(pExchange->GetFunctionString()))->GetFunctionString(), "WebSocketTrades");
		EXPECT_EQ(gl_finnhubInaccessibleExchange.GetExchange(gl_finnhubInaccessibleExchange.GetFinnhubInquiryIndex(pExchange->GetFunctionString()))->SymbolSize(), 2);
		const string str = gl_finnhubInaccessibleExchange.GetExchange(gl_finnhubInaccessibleExchange.GetFinnhubInquiryIndex(pExchange->GetFunctionString()))->GetSymbol(0);
		EXPECT_TRUE(str.compare( "SS") == 0);

		// 恢复原状
		std::filesystem::remove(gl_systemConfiguration.GetConfigurationFileDirectory() + "FinnhubInaccessibleExchangeTest.json");
		gl_finnhubInaccessibleExchange.SetDefaultFileName("FinnhubInaccessibleExchange.json");
	}

	TEST_F(CFinnhubInaccessibleExchangeTest, TestSaveDB2) {
		filesystem::remove(gl_systemConfiguration.GetConfigurationFileDirectory() + "FinnhubInaccessibleExchangeTest.json");
		gl_finnhubInaccessibleExchange.SetDefaultFileName("FinnhubInaccessibleExchangeTest.json");

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
		EXPECT_EQ(gl_finnhubInaccessibleExchange.GetItemSize(), 1);
		EXPECT_EQ(gl_finnhubInaccessibleExchange.GetExchange(gl_finnhubInaccessibleExchange.GetFinnhubInquiryIndex(pExchange->GetFunctionString()))->GetFunctionString(), "WebSocketTrades");
		EXPECT_EQ(gl_finnhubInaccessibleExchange.GetExchange(gl_finnhubInaccessibleExchange.GetFinnhubInquiryIndex(pExchange->GetFunctionString()))->SymbolSize(), 2);
		const string str = gl_finnhubInaccessibleExchange.GetExchange(gl_finnhubInaccessibleExchange.GetFinnhubInquiryIndex(pExchange->GetFunctionString()))->GetSymbol(0);
		EXPECT_EQ(str, "SS");

		// 恢复原状
		filesystem::remove(gl_systemConfiguration.GetConfigurationFileDirectory() + "FinnhubInaccessibleExchangeTest.json");
		gl_finnhubInaccessibleExchange.SetDefaultFileName("FinnhubInaccessibleExchange.json");
	}
}
