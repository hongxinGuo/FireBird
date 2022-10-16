#include"pch.h"

#include"GeneralCheck.h"

#include"WorldMarket.h"

#include"ProductFinnhubCompanyProfile.h"

using namespace testing;

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

namespace StockAnalysisTest {
	class CFinnhubCompanyProfileTest : public ::testing::Test {
	protected:
		static void SetUpTestSuite(void) {
			GeneralCheck();
		}

		static void TearDownTestSuite(void) {
			GeneralCheck();
		}

		virtual void SetUp(void) override {
		}

		virtual void TearDown(void) override {
			// clearu
			GeneralCheck();
		}

	protected:
		CProductFinnhubCompanyProfile companyProfile;
	};

	TEST_F(CFinnhubCompanyProfileTest, TestInitialize) {
		EXPECT_EQ(companyProfile.GetIndex(), -1);
		EXPECT_STREQ(companyProfile.GetInquiringStr(), _T("https://finnhub.io/api/v1/stock/profile?symbol="));
	}

	TEST_F(CFinnhubCompanyProfileTest, TestCreatMessage) {
		gl_pWorldMarket->GetStock(1)->SetCompanyProfileUpdated(false);
		companyProfile.SetMarket(gl_pWorldMarket.get());
		companyProfile.SetIndex(1);
		EXPECT_STREQ(companyProfile.CreatMessage(), companyProfile.GetInquiringStr() + gl_pWorldMarket->GetStock(1)->GetSymbol());
		EXPECT_FALSE(gl_pWorldMarket->GetStock(1)->IsCompanyProfileUpdated()) << "接收到的数据处理后方设置此标识";

		gl_pWorldMarket->GetStock(1)->SetCompanyProfileUpdated(false);
	}

	// 格式不对(缺开始的‘{’），无法顺利Parser
	FinnhubWebData finnhubWebData2(2, _T("AAPL"), _T("\"address\":\"contentious  selectively\",\"city\":\"slaughterer\",\"country\":\"miscuing\",\"currency\":\"inveigles\",\"cusip\":\"Grable's\",\"description\":\"crooked ng Odis tint's\",\"employeeTotal\":\"jalopies\",\"exchange\":\"sieves abominating cuff's hesitation's debilitating\",\"finnhubIndustry\":\"culottes\",\"ggroup\":\"Ziegler's tendrils\",\"gind\":\"prairies  catalysis\",\"gsector\":\"habituate Scandinavians\",\"gsubind\":\"checkout  cherished\",\"ipo\":\"1980-12-12\",\"isin\":\"rapport\",\"logo\":\"freelancer's\",\"marketCapitalization\":8790583.5,\"naics\":\"mishmAlisha\",\"naicsNationalIndustry\":\"pollen jay's flops\",\"naicsSector\":\"smuggest\",\"naicsSubsector\":\"apprenticeship's Kringle\",\"name\":\"impediment's gondolier\",\"phone\":\"shootout's\",\"sedol\":\"decrescendi\",\"shareOutstanding\":75546.432,\"state\":\"Tweedledee\",\"ticker\":\"AAPL\",\"weburl\":\"gestated\"}"));
	// 数据缺乏address项
	FinnhubWebData finnhubWebData3(3, _T("AAPL"), _T("{\"adss\":\"contctively\",\"city\":\"slaughterer\",\"country\":\"miscuing\",\"currency\":\"inveigles\",\"cusip\":\"Grable's\",\"description\":\"crooked ng Odis tint's\",\"employeeTotal\":\"jalopies\",\"exchange\":\"sieves abominating cuff's hesitation's debilitating\",\"finnhubIndustry\":\"culottes\",\"ggroup\":\"Ziegler's tendrils\",\"gind\":\"prairies  catalysis\",\"gsector\":\"habituate Scandinavians\",\"gsubind\":\"checkout  cherished\",\"ipo\":\"1980-12-12\",\"isin\":\"rapport\",\"logo\":\"freelancer's\",\"marketCapitalization\":8790583.5,\"naics\":\"mishmAlisha\",\"naicsNationalIndustry\":\"pollen jay's flops\",\"naicsSector\":\"smuggest\",\"naicsSubsector\":\"apprenticeship's Kringle\",\"name\":\"impediment's gondolier\",\"phone\":\"shootout's\",\"sedol\":\"decrescendi\",\"shareOutstanding\":75546.432,\"state\":\"Tweedledee\",\"ticker\":\"AAPL\",\"weburl\":\"gestated\"}"));
	//dommy data
	FinnhubWebData finnhubWebData4(4, _T("AAPL"), _T("{}"));
	// 正确的数据
	FinnhubWebData finnhubWebData10(10, _T("AAPL"), _T("{\"address\":\"contentious  selectively\",\"city\":\"slaughterer\",\"country\":\"miscuing\",\"currency\":\"inveigles\",\"cusip\":\"Grable's\",\"description\":\"crooked ng Odis tint's\",\"employeeTotal\":\"jalopies\",\"exchange\":\"sieves abominating cuff's hesitation's debilitating\",\"finnhubIndustry\":\"culottes\",\"ggroup\":\"Ziegler's tendrils\",\"gind\":\"prairies  catalysis\",\"gsector\":\"habituate Scandinavians\",\"gsubind\":\"checkout  cherished\",\"ipo\":\"1980-12-12\",\"isin\":\"rapport\",\"logo\":\"freelancer's\",\"marketCapitalization\":8790583.5,\"naics\":\"mishmAlisha\",\"naicsNationalIndustry\":\"pollen jay's flops\",\"naicsSector\":\"smuggest\",\"naicsSubsector\":\"apprenticeship's Kringle\",\"name\":\"impediment's gondolier\",\"phone\":\"shootout's\",\"sedol\":\"decrescendi\",\"shareOutstanding\":75546.432,\"state\":\"Tweedledee\",\"ticker\":\"AAPL\",\"weburl\":\"gestated\"}"));

	class ProcessFinnhubStockProfileTest : public::testing::TestWithParam<FinnhubWebData*> {
	protected:
		virtual void SetUp(void) override {
			GeneralCheck();
			FinnhubWebData* pData = GetParam();
			m_lIndex = pData->m_lIndex;
			m_pStock = gl_pWorldMarket->GetStock(pData->m_strSymbol);
			EXPECT_TRUE(m_pStock != nullptr);
			m_pStock->SetCompanyProfileUpdated(false);
			m_pStock->SetProfileUpdateDate(19700101);
			m_pStock->SetCity(_T(""));
			m_pWebData = pData->m_pData;
			m_pWebData->CreatePropertyTree();
			m_pWebData->SetJSonContentType(true);
			m_finnhubCompanyProfile.SetMarket(gl_pWorldMarket.get());
			m_finnhubCompanyProfile.SetIndex(gl_pWorldMarket->GetStockIndex(pData->m_strSymbol));
		}
		virtual void TearDown(void) override {
			// clearup
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
		CProductFinnhubCompanyProfile m_finnhubCompanyProfile;
	};

	INSTANTIATE_TEST_SUITE_P(TestProcessFinnhubStockProfile, ProcessFinnhubStockProfileTest, testing::Values(&finnhubWebData2,
		&finnhubWebData3, &finnhubWebData4, &finnhubWebData10));

	TEST_P(ProcessFinnhubStockProfileTest, TestProcessStockProfile0) {
		bool fSucceed = false;
		fSucceed = m_finnhubCompanyProfile.ParseAndStoreWebData(m_pWebData);
		switch (m_lIndex) {
		case 2: // 格式不对
			EXPECT_FALSE(fSucceed);
			EXPECT_TRUE(m_pStock->IsCompanyProfileUpdated());
			EXPECT_FALSE(m_pStock->IsUpdateProfileDB());
			EXPECT_NE(m_pStock->GetProfileUpdateDate(), gl_pWorldMarket->GetMarketDate());
			break;
		case 3: // 缺乏address项
			EXPECT_FALSE(fSucceed);
			EXPECT_STRNE(m_pStock->GetCity(), _T("slaughterer")) << "没有赋值此项";
			EXPECT_TRUE(m_pStock->IsCompanyProfileUpdated());
			EXPECT_FALSE(m_pStock->IsUpdateProfileDB());
			EXPECT_NE(m_pStock->GetProfileUpdateDate(), gl_pWorldMarket->GetMarketDate());
			break;
		case 4: // dommy data
			EXPECT_TRUE(fSucceed);
			EXPECT_STREQ(m_pStock->GetTicker(), _T("AAPL"));
			EXPECT_TRUE(m_pStock->IsCompanyProfileUpdated());
			EXPECT_TRUE(m_pStock->IsUpdateProfileDB());
			EXPECT_EQ(m_pStock->GetProfileUpdateDate(), gl_pWorldMarket->GetMarketDate());
			break;
		case 10:
			EXPECT_TRUE(fSucceed);
			EXPECT_STREQ(m_pStock->GetTicker(), _T("AAPL"));
			EXPECT_STREQ(m_pStock->GetCity(), _T("slaughterer"));
			EXPECT_TRUE(m_pStock->IsCompanyProfileUpdated());
			EXPECT_TRUE(m_pStock->IsUpdateProfileDB());
			EXPECT_EQ(m_pStock->GetProfileUpdateDate(), gl_pWorldMarket->GetMarketDate());
			break;
		default:
			break;
		}
	}
}