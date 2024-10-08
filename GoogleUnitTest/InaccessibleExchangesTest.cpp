#include"pch.h"

#include"WorldMarket.h"

#include"GeneralCheck.h"

#include"FinnhubInaccessibleExchange.h"
#include"nlohmannJsonDeclaration.h" // 按照顺序输出json，必须使用此ordered_json,以保证解析后的数据与解析前的顺序一致。

namespace FireBirdTest {
	class CInaccessibleExchangesTest : public testing::Test {
		void SetUp() override {
			SCOPED_TRACE("");
			GeneralCheck();

			vExchanges.push_back(_T("US"));
			vExchanges.push_back(_T("SS"));
			vExchanges.push_back(_T("SZ"));
			inaccessibleExchange.Assign(_T("COMPANY_NEWS"), 1, vExchanges);
		}

		void TearDown() override {
			gl_systemConfiguration.SetWorkingMode(false);

			SCOPED_TRACE("");
			GeneralCheck();
		}

	protected:
		vector<CString> vExchanges;
		CInaccessibleExchanges inaccessibleExchange;
	};

	TEST_F(CInaccessibleExchangesTest, TestAssign) {
		EXPECT_TRUE(inaccessibleExchange.HaveExchange(_T("US"))) << "默认包括US";
		EXPECT_TRUE(inaccessibleExchange.HaveExchange(_T("SZ"))) << "默认包括US";
		EXPECT_TRUE(inaccessibleExchange.HaveExchange(_T("SS"))) << "默认包括US";
		EXPECT_FALSE(inaccessibleExchange.HaveExchange(_T("FA"))) << "默认不包括FA";
		const vector<CString> v{ _T("US2"), _T("SZ2"), _T("SS2") };
		inaccessibleExchange.Assign(_T("Another"), 2, v);

		EXPECT_TRUE(inaccessibleExchange.HaveExchange(_T("US2")));
		EXPECT_TRUE(inaccessibleExchange.HaveExchange(_T("SZ2")));
		EXPECT_TRUE(inaccessibleExchange.HaveExchange(_T("SS2")));
		EXPECT_FALSE(inaccessibleExchange.HaveExchange(_T("US")));
		EXPECT_FALSE(inaccessibleExchange.HaveExchange(_T("SZ")));
		EXPECT_FALSE(inaccessibleExchange.HaveExchange(_T("SS")));
	}

	TEST_F(CInaccessibleExchangesTest, TestAddExchange) {
		EXPECT_FALSE(inaccessibleExchange.HaveExchange(_T("FA"))) << "默认不包括FA";
		inaccessibleExchange.AddExchange(_T("FA"));
		EXPECT_TRUE(inaccessibleExchange.HaveExchange(_T("FA"))) << "添加后就包括FA了";
	}

	TEST_F(CInaccessibleExchangesTest, TestDeleteExchange) {
		EXPECT_TRUE(inaccessibleExchange.HaveExchange(_T("US"))) << "默认包括US";
		inaccessibleExchange.DeleteExchange(_T("US"));
		EXPECT_FALSE(inaccessibleExchange.HaveExchange(_T("US"))) << "删除后就不包括US了";
	}

	TEST_F(CInaccessibleExchangesTest, TestHaveExchange) {
		EXPECT_TRUE(inaccessibleExchange.HaveExchange(_T("US"))) << "默认包括US";
		EXPECT_TRUE(inaccessibleExchange.HaveExchange(_T("SZ"))) << "默认包括US";
		EXPECT_TRUE(inaccessibleExchange.HaveExchange(_T("SS"))) << "默认包括US";
		EXPECT_FALSE(inaccessibleExchange.HaveExchange(_T("FA"))) << "默认不包括FA";
	}
}

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
		CInaccessibleExchanges exchange;
		const string s2 = jsFinnhubInaccessibleExchange[_T("InaccessibleExchange")][0][_T("Function")];
		exchange.SetFunctionString(s2.c_str());
		for (int i = 0; i < jsFinnhubInaccessibleExchange[_T("InaccessibleExchange")][0][_T("Exchange")].size(); i++) {
			string s = jsFinnhubInaccessibleExchange[_T("InaccessibleExchange")][0][_T("Exchange")][i];
			exchange.AddExchange(s.c_str());
		}
		EXPECT_EQ(jsFinnhubInaccessibleExchange["UpdateDate"], 20221205);
		EXPECT_STREQ(exchange.GetFunctionString(), _T("StockFundamentalsCompanyProfileConcise"));
		EXPECT_STREQ(exchange.GetExchange(0), _T("SS"));
		EXPECT_STREQ(exchange.GetExchange(1), _T("SZ"));
	}

	TEST_F(CFinnhubInaccessibleExchangeTest, TestDeleteExchange) {
		const auto pExchange = make_shared<CInaccessibleExchanges>();
		pExchange->SetFunctionString(_T("WebSocketTrades"));
		pExchange->AddExchange(_T("SS"));
		pExchange->AddExchange(_T("SZ"));

		gl_finnhubInaccessibleExchange.SetExchange(gl_FinnhubInquiryType.GetInquiryType(_T("WebSocketTrades")), pExchange);
		gl_finnhubInaccessibleExchange.DeleteExchange(gl_FinnhubInquiryType.GetInquiryType(_T("WebSocketTrades")), _T("SS"));

		const auto p = gl_finnhubInaccessibleExchange.GetExchange(gl_FinnhubInquiryType.GetInquiryType(_T("WebSocketTrades")));
		EXPECT_FALSE(p->HaveExchange(_T("SS")));

		// 恢复原状
		gl_finnhubInaccessibleExchange.DeleteExchange(gl_FinnhubInquiryType.GetInquiryType(_T("WebSocketTrades")), _T("SZ"));
		gl_finnhubInaccessibleExchange.SetUpdateDB(false);
	}

	TEST_F(CFinnhubInaccessibleExchangeTest, TestSaveDB1) {
		DeleteFile(gl_systemConfiguration.GetConfigurationFileDirectory() + _T("FinnhubInaccessibleExchangeTest.json"));
		gl_finnhubInaccessibleExchange.SetDefaultFileName(_T("FinnhubInaccessibleExchangeTest.json"));

		const auto pExchange = make_shared<CInaccessibleExchanges>();
		pExchange->SetFunctionString(_T("WebSocketTrades"));
		pExchange->AddExchange(_T("SS"));
		pExchange->AddExchange(_T("SZ"));

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
		EXPECT_EQ(gl_finnhubInaccessibleExchange.GetExchange(gl_finnhubInaccessibleExchange.GetFinnhubInquiryIndex(pExchange->GetFunctionString()))->ExchangeSize(), 2);
		const CString str = gl_finnhubInaccessibleExchange.GetExchange(gl_finnhubInaccessibleExchange.GetFinnhubInquiryIndex(pExchange->GetFunctionString()))->GetExchange(0);
		EXPECT_STREQ(str, _T("SS"));

		// 恢复原状
		DeleteFile(gl_systemConfiguration.GetConfigurationFileDirectory() + _T("FinnhubInaccessibleExchangeTest.json"));
		gl_finnhubInaccessibleExchange.SetDefaultFileName(_T("FinnhubInaccessibleExchange.json"));
	}

	TEST_F(CFinnhubInaccessibleExchangeTest, TestSaveDB2) {
		DeleteFile(gl_systemConfiguration.GetConfigurationFileDirectory() + _T("FinnhubInaccessibleExchangeTest.json"));
		gl_finnhubInaccessibleExchange.SetDefaultFileName(_T("FinnhubInaccessibleExchangeTest.json"));

		const auto pExchange = make_shared<CInaccessibleExchanges>();
		pExchange->SetFunctionString(_T("WebSocketTrades"));
		pExchange->AddExchange(_T("SS"));
		pExchange->AddExchange(_T("SZ"));

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
		EXPECT_EQ(gl_finnhubInaccessibleExchange.GetExchange(gl_finnhubInaccessibleExchange.GetFinnhubInquiryIndex(pExchange->GetFunctionString()))->ExchangeSize(), 2);
		const CString str = gl_finnhubInaccessibleExchange.GetExchange(gl_finnhubInaccessibleExchange.GetFinnhubInquiryIndex(pExchange->GetFunctionString()))->GetExchange(0);
		EXPECT_STREQ(str, _T("SS"));

		// 恢复原状
		DeleteFile(gl_systemConfiguration.GetConfigurationFileDirectory() + _T("FinnhubInaccessibleExchangeTest.json"));
		gl_finnhubInaccessibleExchange.SetDefaultFileName(_T("FinnhubInaccessibleExchange.json"));
	}
}
