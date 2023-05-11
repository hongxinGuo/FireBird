#include"pch.h"

#include"GeneralCheck.h"

#include"WorldMarket.h"

#include"ProductFinnhubCompanyProfileConcise.h"

using namespace testing;

namespace FireBirdTest {
	class CFinnhubCompanyProfileConciseTest : public Test {
	protected:
		static void SetUpTestSuite() {
			SCOPED_TRACE(""); GeneralCheck();
		}

		static void TearDownTestSuite() {
			SCOPED_TRACE(""); GeneralCheck();
		}

		void SetUp() override {
			SCOPED_TRACE(""); GeneralCheck();
		}

		void TearDown() override {
			// clearUp
			SCOPED_TRACE(""); GeneralCheck();
		}

	protected:
		CProductFinnhubCompanyProfileConcise companyProfileConcise;
	};

	TEST_F(CFinnhubCompanyProfileConciseTest, TestInitialize) {
		EXPECT_EQ(companyProfileConcise.GetIndex(), -1);
		EXPECT_STREQ(companyProfileConcise.GetInquiryFunction(), _T("https://finnhub.io/api/v1/stock/profile2?symbol="));
	}

	TEST_F(CFinnhubCompanyProfileConciseTest, TestCreatMessage) {
		gl_pWorldMarket->GetStock(1)->SetUpdateCompanyProfile(true);
		companyProfileConcise.SetMarket(gl_pWorldMarket.get());
		companyProfileConcise.SetIndex(1);
		EXPECT_STREQ(companyProfileConcise.CreateMessage(), companyProfileConcise.GetInquiryFunction() + gl_pWorldMarket->GetStock(1)->GetSymbol());
		EXPECT_TRUE(gl_pWorldMarket->GetStock(1)->IsUpdateCompanyProfile()) << "������յ������ݺ�����ô˱�ʶ";

		gl_pWorldMarket->GetStock(1)->SetUpdateCompanyProfile(true);
	}

	// ��ʽ����(ȱ��ʼ�ġ�{�������޷�˳��Parser
	FinnhubWebData finnhubWebData12(2, _T("AAPL"), _T("\"country\":\"US\",\"currency\":\"USD\",\"exchange\":\"NASDAQ NMS - GLOBAL MARKET\",\"finnhubIndustry\":\"Technology\",\"ipo\":\"1980-12-12\",\"logo\":\"https://finnhub.io/api/logo?symbol=AAPL\",\"marketCapitalization\":2014236,\"name\":\"Apple Inc\",\"phone\":\"14089961010.0\",\"shareOutstanding\":16788.096,\"ticker\":\"AAPL\",\"weburl\":\"https://www.apple.com/\"}"));
	// ����ȱ��country��
	FinnhubWebData finnhubWebData13(3, _T("AAPL"), _T("{\"counry\":\"US\",\"currency\":\"USD\",\"exchange\":\"NASDAQ NMS - GLOBAL MARKET\",\"finnhubIndustry\":\"Technology\",\"ipo\":\"1980-12-12\",\"logo\":\"https://finnhub.io/api/logo?symbol=AAPL\",\"marketCapitalization\":2014236,\"name\":\"Apple Inc\",\"phone\":\"14089961010.0\",\"shareOutstanding\":16788.096,\"ticker\":\"AAPL\",\"weburl\":\"https://www.apple.com/\"}"));
	// ������
	FinnhubWebData finnhubWebData14(4, _T("AAPL"), _T("{}"));
	// ��ȷ������
	FinnhubWebData finnhubWebData20(10, _T("AAPL"), _T("{\"country\":\"US\",\"currency\":\"USD\",\"exchange\":\"NASDAQ NMS - GLOBAL MARKET\",\"finnhubIndustry\":\"Technology\",\"ipo\":\"1980-12-12\",\"logo\":\"https://finnhub.io/api/logo?symbol=AAPL\",\"marketCapitalization\":2014236,\"name\":\"Apple Inc\",\"phone\":\"14089961010.0\",\"shareOutstanding\":16788.096,\"ticker\":\"AAPL\",\"weburl\":\"https://www.apple.com/\"}"));

	class ProcessFinnhubStockProfileConciseTest : public TestWithParam<FinnhubWebData*> {
	protected:
		void SetUp() override {
			SCOPED_TRACE(""); GeneralCheck();
			const FinnhubWebData* pData = GetParam();
			m_lIndex = pData->m_lIndex;
			m_pStock = gl_pWorldMarket->GetStock(pData->m_strSymbol);
			EXPECT_TRUE(m_pStock != nullptr);
			m_pStock->SetCountry(_T(""));
			m_pWebData = pData->m_pData;
			m_pWebData->CreateJson();
			m_pWebData->SetJSonContentType(true);
			m_FinnhubCompanyProfileConcise.SetMarket(gl_pWorldMarket.get());
			m_FinnhubCompanyProfileConcise.SetIndex(gl_pWorldMarket->GetStockIndex(pData->m_strSymbol));
		}

		void TearDown() override {
			// clearUp
			while (gl_systemMessage.ErrorMessageSize() > 0) gl_systemMessage.PopErrorMessage();
			m_pStock->SetProfileUpdateDate(19800101);
			m_pStock->SetUpdateCompanyProfile(true);
			m_pStock->SetUpdateProfileDB(false);

			SCOPED_TRACE(""); GeneralCheck();
		}

	public:
		long m_lIndex;
		CWorldStockPtr m_pStock;
		CWebDataPtr m_pWebData;
		CProductFinnhubCompanyProfileConcise m_FinnhubCompanyProfileConcise;
	};

	INSTANTIATE_TEST_SUITE_P(TestParseFinnhubStockProfileConcise1, ProcessFinnhubStockProfileConciseTest, testing::Values(&finnhubWebData0,
		                         &finnhubWebData1, &finnhubWebData12,&finnhubWebData13, &finnhubWebData14, &finnhubWebData20));

	TEST_P(ProcessFinnhubStockProfileConciseTest, TestProcessStockProfileConcise0) {
		bool fSucceed = false;
		fSucceed = m_FinnhubCompanyProfileConcise.ParseAndStoreWebData(m_pWebData);
		switch (m_lIndex) {
		case 0: // ������
			EXPECT_TRUE(fSucceed);
			EXPECT_FALSE(m_pStock->IsUpdateCompanyProfile());
			EXPECT_TRUE(m_pStock->IsUpdateProfileDB());
			EXPECT_EQ(m_pStock->GetProfileUpdateDate(), gl_pWorldMarket->GetMarketDate());
			break;
		case 1: // ��Ȩ�����ʵ�����
			EXPECT_TRUE(fSucceed);
			EXPECT_FALSE(m_pStock->IsUpdateCompanyProfile());
			EXPECT_TRUE(m_pStock->IsUpdateProfileDB());
			EXPECT_EQ(m_pStock->GetProfileUpdateDate(), gl_pWorldMarket->GetMarketDate());
			break;
		case 2: // ��ʽ����
			EXPECT_FALSE(fSucceed);
			EXPECT_FALSE(m_pStock->IsUpdateCompanyProfile());
			EXPECT_FALSE(m_pStock->IsUpdateProfileDB());
			EXPECT_NE(m_pStock->GetProfileUpdateDate(), gl_pWorldMarket->GetMarketDate());
			break;
		case 3: // ȱ��address��
			EXPECT_FALSE(fSucceed);
			EXPECT_STRNE(m_pStock->GetCountry(), _T("US")) << "û�и�ֵ����";
			EXPECT_FALSE(m_pStock->IsUpdateCompanyProfile());
			EXPECT_FALSE(m_pStock->IsUpdateProfileDB());
			EXPECT_NE(m_pStock->GetProfileUpdateDate(), gl_pWorldMarket->GetMarketDate());
			break;
		case 4: // ������
			EXPECT_TRUE(fSucceed);
			EXPECT_FALSE(m_pStock->IsUpdateCompanyProfile());
			EXPECT_TRUE(m_pStock->IsUpdateProfileDB());
			EXPECT_EQ(m_pStock->GetProfileUpdateDate(), gl_pWorldMarket->GetMarketDate());
			break;
		case 10:
			EXPECT_TRUE(fSucceed);
			EXPECT_STREQ(m_pStock->GetTicker(), _T("AAPL"));
			EXPECT_STREQ(m_pStock->GetCountry(), _T("US"));
			EXPECT_FALSE(m_pStock->IsUpdateCompanyProfile());
			EXPECT_TRUE(m_pStock->IsUpdateProfileDB());
			EXPECT_EQ(m_pStock->GetProfileUpdateDate(), gl_pWorldMarket->GetMarketDate());
			break;
		default:
			break;
		}
	}
}
