#include"pch.h"

#include"GeneralCheck.h"

#include"WorldMarket.h"

#include"ProductFinnhubCompanyProfileConcise.h"

using namespace testing;

namespace FireBirdTest {
	class CFinnhubCompanyProfileConciseTest : public ::testing::Test {
	protected:
		static void SetUpTestSuite(void) {
			GeneralCheck();
		}

		static void TearDownTestSuite(void) {
			GeneralCheck();
		}

		void SetUp(void) override { }

		void TearDown(void) override {
			// clearUp
			GeneralCheck();
		}

	protected:
		CProductFinnhubCompanyProfileConcise companyProfileConcise;
	};

	TEST_F(CFinnhubCompanyProfileConciseTest, TestInitialize) {
		EXPECT_EQ(companyProfileConcise.GetIndex(), -1);
		EXPECT_STREQ(companyProfileConcise.GetInquiry(), _T("https://finnhub.io/api/v1/stock/profile2?symbol="));
	}

	TEST_F(CFinnhubCompanyProfileConciseTest, TestCreatMessage) {
		gl_pWorldMarket->GetStock(1)->SetCompanyProfileUpdated(false);
		companyProfileConcise.SetMarket(gl_pWorldMarket.get());
		companyProfileConcise.SetIndex(1);
		EXPECT_STREQ(companyProfileConcise.CreateMessage(), companyProfileConcise.GetInquiry() + gl_pWorldMarket->GetStock(1)->GetSymbol());
		EXPECT_FALSE(gl_pWorldMarket->GetStock(1)->IsCompanyProfileUpdated()) << "处理接收到的数据后才设置此标识";

		gl_pWorldMarket->GetStock(1)->SetCompanyProfileUpdated(false);
	}

	// 格式不对(缺开始的‘{’），无法顺利Parser
	FinnhubWebData finnhubWebData12(2, _T("AAPL"), _T("\"country\":\"US\",\"currency\":\"USD\",\"exchange\":\"NASDAQ NMS - GLOBAL MARKET\",\"finnhubIndustry\":\"Technology\",\"ipo\":\"1980-12-12\",\"logo\":\"https://finnhub.io/api/logo?symbol=AAPL\",\"marketCapitalization\":2014236,\"name\":\"Apple Inc\",\"phone\":\"14089961010.0\",\"shareOutstanding\":16788.096,\"ticker\":\"AAPL\",\"weburl\":\"https://www.apple.com/\"}"));
	// 数据缺乏country项
	FinnhubWebData finnhubWebData13(3, _T("AAPL"), _T("{\"counry\":\"US\",\"currency\":\"USD\",\"exchange\":\"NASDAQ NMS - GLOBAL MARKET\",\"finnhubIndustry\":\"Technology\",\"ipo\":\"1980-12-12\",\"logo\":\"https://finnhub.io/api/logo?symbol=AAPL\",\"marketCapitalization\":2014236,\"name\":\"Apple Inc\",\"phone\":\"14089961010.0\",\"shareOutstanding\":16788.096,\"ticker\":\"AAPL\",\"weburl\":\"https://www.apple.com/\"}"));
	// 空数据
	FinnhubWebData finnhubWebData14(4, _T("AAPL"), _T("{}"));
	// 正确的数据
	FinnhubWebData finnhubWebData20(10, _T("AAPL"), _T("{\"country\":\"US\",\"currency\":\"USD\",\"exchange\":\"NASDAQ NMS - GLOBAL MARKET\",\"finnhubIndustry\":\"Technology\",\"ipo\":\"1980-12-12\",\"logo\":\"https://finnhub.io/api/logo?symbol=AAPL\",\"marketCapitalization\":2014236,\"name\":\"Apple Inc\",\"phone\":\"14089961010.0\",\"shareOutstanding\":16788.096,\"ticker\":\"AAPL\",\"weburl\":\"https://www.apple.com/\"}"));

	class ProcessFinnhubStockProfileConciseTest : public::testing::TestWithParam<FinnhubWebData*> {
	protected:
		void SetUp(void) override {
			GeneralCheck();
			FinnhubWebData* pData = GetParam();
			m_lIndex = pData->m_lIndex;
			m_pStock = gl_pWorldMarket->GetStock(pData->m_strSymbol);
			EXPECT_TRUE(m_pStock != nullptr);
			m_pStock->SetCountry(_T(""));
			m_pWebData = pData->m_pData;
			m_pWebData->CreateNlohmannJson();
			m_pWebData->SetJSonContentType(true);
			m_FinnhubCompanyProfileConcise.SetMarket(gl_pWorldMarket.get());
			m_FinnhubCompanyProfileConcise.SetIndex(gl_pWorldMarket->GetStockIndex(pData->m_strSymbol));
		}

		void TearDown(void) override {
			// clearUp
			while (gl_systemMessage.ErrorMessageSize() > 0) gl_systemMessage.PopErrorMessage();
			m_pStock->SetProfileUpdateDate(19800101);
			m_pStock->SetCompanyProfileUpdated(false);
			m_pStock->SetUpdateProfileDB(false);

			GeneralCheck();
		}

	public:
		long m_lIndex;
		CWorldStockPtr m_pStock;
		CWebDataPtr m_pWebData;
		CProductFinnhubCompanyProfileConcise m_FinnhubCompanyProfileConcise;
	};

	INSTANTIATE_TEST_SUITE_P(TestParseFinnhubStockProfileConcise1, ProcessFinnhubStockProfileConciseTest, testing::Values(&finnhubWebData12,
		                         &finnhubWebData13, &finnhubWebData14, &finnhubWebData20));

	TEST_P(ProcessFinnhubStockProfileConciseTest, TestProcessStockProfileConcise0) {
		bool fSucceed = false;
		fSucceed = m_FinnhubCompanyProfileConcise.ParseAndStoreWebData(m_pWebData);
		switch (m_lIndex) {
		case 2: // 格式不对
			EXPECT_FALSE(fSucceed);
			EXPECT_TRUE(m_pStock->IsCompanyProfileUpdated());
			EXPECT_FALSE(m_pStock->IsUpdateProfileDB());
			EXPECT_NE(m_pStock->GetProfileUpdateDate(), gl_pWorldMarket->GetMarketDate());
			break;
		case 3: // 缺乏address项
			EXPECT_FALSE(fSucceed);
			EXPECT_STRNE(m_pStock->GetCountry(), _T("US")) << "没有赋值此项";
			EXPECT_TRUE(m_pStock->IsCompanyProfileUpdated());
			EXPECT_FALSE(m_pStock->IsUpdateProfileDB());
			EXPECT_NE(m_pStock->GetProfileUpdateDate(), gl_pWorldMarket->GetMarketDate());
			break;
		case 4: // 空数据
			EXPECT_TRUE(fSucceed);
			EXPECT_TRUE(m_pStock->IsCompanyProfileUpdated());
			EXPECT_TRUE(m_pStock->IsUpdateProfileDB());
			EXPECT_EQ(m_pStock->GetProfileUpdateDate(), gl_pWorldMarket->GetMarketDate());
			break;
		case 10:
			EXPECT_TRUE(fSucceed);
			EXPECT_STREQ(m_pStock->GetTicker(), _T("AAPL"));
			EXPECT_STREQ(m_pStock->GetCountry(), _T("US"));
			EXPECT_TRUE(m_pStock->IsCompanyProfileUpdated());
			EXPECT_TRUE(m_pStock->IsUpdateProfileDB());
			EXPECT_EQ(m_pStock->GetProfileUpdateDate(), gl_pWorldMarket->GetMarketDate());
			break;
		default:
			break;
		}
	}
}
