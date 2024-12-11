#include"pch.h"

 

import FireBird.Market.World;

#include"GeneralCheck.h"

import FireBird.System.Inaccessible;
import FireBird.FinnhubInquiryType;
import FireBird.System.FinnhubInaccessibleExchange;

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
		nlohmann::ordered_json jsFinnhubInaccessibleExchange = nlohmann::ordered_json::parse(Test_gl_sFinnhubInaccessibleExchange);
		CInaccessible exchange;
		const string s2 = jsFinnhubInaccessibleExchange[_T("InaccessibleExchange")][0][_T("Function")];
		exchange.SetFunctionString(s2.c_str());
		for (int i = 0; i < jsFinnhubInaccessibleExchange[_T("InaccessibleExchange")][0][_T("Exchange")].size(); i++) {
			string s = jsFinnhubInaccessibleExchange[_T("InaccessibleExchange")][0][_T("Exchange")][i];
			exchange.AddSymbol(s.c_str());
		}
		EXPECT_EQ(jsFinnhubInaccessibleExchange["UpdateDate"], 20221205);
		EXPECT_STREQ(exchange.GetFunctionString(), _T("StockFundamentalsCompanyProfileConcise"));
		EXPECT_STREQ(exchange.GetSymbol(0), _T("SS"));
		EXPECT_STREQ(exchange.GetSymbol(1), _T("SZ"));
	}

	TEST_F(CFinnhubInaccessibleExchangeTest, TestDeleteExchange) {
		const auto pExchange = make_shared<CInaccessible>();
		pExchange->SetFunctionString(_T("WebSocketTrades"));
		pExchange->AddSymbol(_T("SS"));
		pExchange->AddSymbol(_T("SZ"));

		gl_finnhubInaccessibleExchange.SetExchange(gl_FinnhubInquiryType.GetInquiryType(_T("WebSocketTrades")), pExchange);
		gl_finnhubInaccessibleExchange.DeleteExchange(gl_FinnhubInquiryType.GetInquiryType(_T("WebSocketTrades")), _T("SS"));

		const auto p = gl_finnhubInaccessibleExchange.GetExchange(gl_FinnhubInquiryType.GetInquiryType(_T("WebSocketTrades")));
		EXPECT_FALSE(p->HaveSymbol(_T("SS")));

		// »Ö¸´Ô­×´
		gl_finnhubInaccessibleExchange.DeleteExchange(gl_FinnhubInquiryType.GetInquiryType(_T("WebSocketTrades")), _T("SZ"));
		gl_finnhubInaccessibleExchange.SetUpdateDB(false);
	}

	TEST_F(CFinnhubInaccessibleExchangeTest, TestSaveDB1) {
		DeleteFile(gl_systemConfiguration.GetConfigurationFileDirectory() + _T("FinnhubInaccessibleExchangeTest.json"));
		gl_finnhubInaccessibleExchange.SetDefaultFileName(_T("FinnhubInaccessibleExchangeTest.json"));

		const auto pExchange = make_shared<CInaccessible>();
		pExchange->SetFunctionString(_T("WebSocketTrades"));
		pExchange->AddSymbol(_T("SS"));
		pExchange->AddSymbol(_T("SZ"));

		gl_finnhubInaccessibleExchange.SetUpdateDate(20230101);
		gl_finnhubInaccessibleExchange.SetExchange(gl_finnhubInaccessibleExchange.GetFinnhubInquiryIndex(pExchange->GetFunctionString()), pExchange);
		gl_finnhubInaccessibleExchange.UpdateJson();
		gl_finnhubInaccessibleExchange.SaveDB();

		gl_finnhubInaccessibleExchange.Clear();
		gl_finnhubInaccessibleExchange.LoadDB();
		gl_finnhubInaccessibleExchange.Update();
		EXPECT_EQ(gl_finnhubInaccessibleExchange.GetUpdateDate(), 20230101);
		EXPECT_EQ(gl_finnhubInaccessibleExchange.GetItemSize(), 1);
		EXPECT_STREQ(gl_finnhubInaccessibleExchange.GetExchange(gl_finnhubInaccessibleExchange.GetFinnhubInquiryIndex(pExchange->GetFunctionString()))->GetFunctionString(),
		             _T("WebSocketTrades"));
		EXPECT_EQ(gl_finnhubInaccessibleExchange.GetExchange(gl_finnhubInaccessibleExchange.GetFinnhubInquiryIndex(pExchange->GetFunctionString()))->SymbolSize(), 2);
		const CString str = gl_finnhubInaccessibleExchange.GetExchange(gl_finnhubInaccessibleExchange.GetFinnhubInquiryIndex(pExchange->GetFunctionString()))->GetSymbol(0);
		EXPECT_STREQ(str, _T("SS"));

		// »Ö¸´Ô­×´
		DeleteFile(gl_systemConfiguration.GetConfigurationFileDirectory() + _T("FinnhubInaccessibleExchangeTest.json"));
		gl_finnhubInaccessibleExchange.SetDefaultFileName(_T("FinnhubInaccessibleExchange.json"));
	}

	TEST_F(CFinnhubInaccessibleExchangeTest, TestSaveDB2) {
		DeleteFile(gl_systemConfiguration.GetConfigurationFileDirectory() + _T("FinnhubInaccessibleExchangeTest.json"));
		gl_finnhubInaccessibleExchange.SetDefaultFileName(_T("FinnhubInaccessibleExchangeTest.json"));

		const auto pExchange = make_shared<CInaccessible>();
		pExchange->SetFunctionString(_T("WebSocketTrades"));
		pExchange->AddSymbol(_T("SS"));
		pExchange->AddSymbol(_T("SZ"));

		gl_finnhubInaccessibleExchange.SetUpdateDate(20230101);
		gl_finnhubInaccessibleExchange.SetExchange(gl_finnhubInaccessibleExchange.GetFinnhubInquiryIndex(pExchange->GetFunctionString()), pExchange);
		gl_finnhubInaccessibleExchange.UpdateJson();
		gl_finnhubInaccessibleExchange.SaveDB();

		const CString strFileDirectory = gl_systemConfiguration.GetConfigurationFileDirectory();
		gl_finnhubInaccessibleExchange.Clear();
		gl_finnhubInaccessibleExchange.LoadDB(strFileDirectory);
		gl_finnhubInaccessibleExchange.Update();
		EXPECT_EQ(gl_finnhubInaccessibleExchange.GetUpdateDate(), 20230101);
		EXPECT_EQ(gl_finnhubInaccessibleExchange.GetItemSize(), 1);
		EXPECT_STREQ(gl_finnhubInaccessibleExchange.GetExchange(gl_finnhubInaccessibleExchange.GetFinnhubInquiryIndex(pExchange->GetFunctionString()))->GetFunctionString(),
		             _T("WebSocketTrades"));
		EXPECT_EQ(gl_finnhubInaccessibleExchange.GetExchange(gl_finnhubInaccessibleExchange.GetFinnhubInquiryIndex(pExchange->GetFunctionString()))->SymbolSize(), 2);
		const CString str = gl_finnhubInaccessibleExchange.GetExchange(gl_finnhubInaccessibleExchange.GetFinnhubInquiryIndex(pExchange->GetFunctionString()))->GetSymbol(0);
		EXPECT_STREQ(str, _T("SS"));

		// »Ö¸´Ô­×´
		DeleteFile(gl_systemConfiguration.GetConfigurationFileDirectory() + _T("FinnhubInaccessibleExchangeTest.json"));
		gl_finnhubInaccessibleExchange.SetDefaultFileName(_T("FinnhubInaccessibleExchange.json"));
	}
}
