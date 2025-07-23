#include"pch.h"

#include"GeneralCheck.h"

#include"WorldMarket.h"

#include"ProductFinnhubCompanyProfile.h"

#include"TestWebData.h"

using namespace testing;

namespace FireBirdTest {
	class CFinnhubCompanyProfileTest : public Test {
	protected:
		static void SetUpTestSuite() {
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

	protected:
		CProductFinnhubCompanyProfile companyProfile;
	};

	TEST_F(CFinnhubCompanyProfileTest, TestInitialize) {
		EXPECT_EQ(companyProfile.GetIndex(), 0);
		EXPECT_STREQ(companyProfile.GetInquiryFunction().c_str(), _T("https://finnhub.io/api/v1/stock/profile?symbol="));
	}

	TEST_F(CFinnhubCompanyProfileTest, TestCreatMessage) {
		gl_dataContainerFinnhubStock.GetItem(1)->SetUpdateCompanyProfile(true);
		companyProfile.SetMarket(gl_pWorldMarket);
		companyProfile.SetIndex(1);
		EXPECT_STREQ(companyProfile.CreateMessage().c_str(), (companyProfile.GetInquiryFunction() + gl_dataContainerFinnhubStock.GetItem(1)->GetSymbol()).c_str());
		EXPECT_TRUE(gl_dataContainerFinnhubStock.GetItem(1)->IsUpdateCompanyProfile()) << "接收到的数据处理后方设置此标识";

		gl_dataContainerFinnhubStock.GetItem(1)->SetUpdateCompanyProfile(true);
	}

	// 格式不对(缺开始的‘{’），无法顺利Parser
	Test_FinnhubWebData finnhubWebData5(5, _T("AAPL"), _T("\"address\":\"contentious  selectively\",\"city\":\"slaughterer\",\"country\":\"miscuing\",\"currency\":\"inveigles\",\"cusip\":\"Grable's\",\"description\":\"crooked ng Odis tint's\",\"employeeTotal\":\"jalopies\",\"exchange\":\"sieves abominating cuff's hesitation's debilitating\",\"finnhubIndustry\":\"culottes\",\"ggroup\":\"Ziegler's tendrils\",\"gind\":\"prairies  catalysis\",\"gsector\":\"habituate Scandinavians\",\"gsubind\":\"checkout  cherished\",\"ipo\":\"1980-12-12\",\"isin\":\"rapport\",\"logo\":\"freelancer's\",\"marketCapitalization\":8790583.5,\"naics\":\"mishmAlisha\",\"naicsNationalIndustry\":\"pollen jay's flops\",\"naicsSector\":\"smuggest\",\"naicsSubsector\":\"apprenticeship's Kringle\",\"name\":\"impediment's gondolier\",\"phone\":\"shootout's\",\"sedol\":\"decrescendi\",\"shareOutstanding\":75546.432,\"state\":\"Tweedledee\",\"ticker\":\"AAPL\",\"weburl\":\"gestated\"}"));
	// 数据缺乏address项
	Test_FinnhubWebData finnhubWebData6(6, _T("AAPL"), _T("{\"adss\":\"contctively\",\"city\":\"slaughterer\",\"country\":\"miscuing\",\"currency\":\"inveigles\",\"cusip\":\"Grable's\",\"description\":\"crooked ng Odis tint's\",\"employeeTotal\":\"jalopies\",\"exchange\":\"sieves abominating cuff's hesitation's debilitating\",\"finnhubIndustry\":\"culottes\",\"ggroup\":\"Ziegler's tendrils\",\"gind\":\"prairies  catalysis\",\"gsector\":\"habituate Scandinavians\",\"gsubind\":\"checkout  cherished\",\"ipo\":\"1980-12-12\",\"isin\":\"rapport\",\"logo\":\"freelancer's\",\"marketCapitalization\":8790583.5,\"naics\":\"mishmAlisha\",\"naicsNationalIndustry\":\"pollen jay's flops\",\"naicsSector\":\"smuggest\",\"naicsSubsector\":\"apprenticeship's Kringle\",\"name\":\"impediment's gondolier\",\"phone\":\"shootout's\",\"sedol\":\"decrescendi\",\"shareOutstanding\":75546.432,\"state\":\"Tweedledee\",\"ticker\":\"AAPL\",\"weburl\":\"gestated\"}"));
	//dummy data
	Test_FinnhubWebData finnhubWebData7(7, _T("AAPL"), _T("{}"));
	// 正确的数据
	Test_FinnhubWebData finnhubWebData10(10, _T("AAPL"), _T("{\"address\":\"contentious  selectively\",\"city\":\"slaughterer\",\"country\":\"miscuing\",\"currency\":\"inveigles\",\"cusip\":\"Grable's\",\"description\":\"crooked ng Odis tint's\",\"employeeTotal\":\"jalopies\",\"exchange\":\"sieves abominating cuff's hesitation's debilitating\",\"finnhubIndustry\":\"culottes\",\"ggroup\":\"Ziegler's tendrils\",\"gind\":\"prairies  catalysis\",\"gsector\":\"habituate Scandinavians\",\"gsubind\":\"checkout  cherished\",\"ipo\":\"1980-12-12\",\"isin\":\"rapport\",\"logo\":\"freelancer's\",\"marketCapitalization\":8790583.5,\"naics\":\"mishmAlisha\",\"naicsNationalIndustry\":\"pollen jay's flops\",\"naicsSector\":\"smuggest\",\"naicsSubsector\":\"apprenticeship's Kringle\",\"name\":\"impediment's gondolier\",\"phone\":\"shootout's\",\"sedol\":\"decrescendi\",\"shareOutstanding\":75546.432,\"state\":\"Tweedledee\",\"ticker\":\"AAPL\",\"weburl\":\"gestated\"}"));

	class ProcessFinnhubStockProfileTest : public TestWithParam<Test_FinnhubWebData*> {
	protected:
		void SetUp() override {
			SCOPED_TRACE("");
			GeneralCheck();
			const Test_FinnhubWebData* pData = GetParam();
			m_lIndex = pData->m_lIndex;
			m_pStock = gl_dataContainerFinnhubStock.GetItem(pData->m_strSymbol.c_str());
			EXPECT_TRUE(m_pStock != nullptr);
			m_pStock->SetUpdateCompanyProfile(true);
			m_pStock->SetProfileUpdateDate(19700101);
			m_pStock->SetCity(_T(""));
			m_pWebData = pData->m_pData;
			m_finnhubCompanyProfile.__Test_checkAccessRight(m_pWebData);

			m_finnhubCompanyProfile.SetMarket(gl_pWorldMarket);
			m_finnhubCompanyProfile.SetIndex(gl_dataContainerFinnhubStock.GetOffset(pData->m_strSymbol.c_str()));
		}

		void TearDown() override {
			// clearUp
			while (gl_systemMessage.ErrorMessageSize() > 0) gl_systemMessage.PopErrorMessage();
			m_pStock->SetProfileUpdateDate(19800101);
			m_pStock->SetUpdateCompanyProfile(true);
			m_pStock->SetUpdateProfileDB(false);

			SCOPED_TRACE("");
			GeneralCheck();
		}

	public:
		long m_lIndex;
		CFinnhubStockPtr m_pStock;
		CWebDataPtr m_pWebData;
		CProductFinnhubCompanyProfile m_finnhubCompanyProfile;
	};

	INSTANTIATE_TEST_SUITE_P(TestProcessFinnhubStockProfile, ProcessFinnhubStockProfileTest, testing::Values(&finnhubWebData0, &finnhubWebData1, &finnhubWebData5,
		                         &finnhubWebData6, &finnhubWebData7, &finnhubWebData10));

	TEST_P(ProcessFinnhubStockProfileTest, TestProcessStockProfile0) {
		m_finnhubCompanyProfile.ParseAndStoreWebData(m_pWebData);
		switch (m_lIndex) {
		case 0:
			EXPECT_STREQ(m_pStock->GetExchangeCode().c_str(), _T("US")) << "交易所代码不使用读取的数据";
			EXPECT_STREQ(m_pStock->GetTicker().c_str(), _T("AAPL"));
			EXPECT_FALSE(m_pStock->IsUpdateCompanyProfile());
			EXPECT_TRUE(m_pStock->IsUpdateProfileDB());
			EXPECT_EQ(m_pStock->GetProfileUpdateDate(), gl_pWorldMarket->GetMarketDate());
			break;
		case 1:
			EXPECT_STREQ(m_pStock->GetExchangeCode().c_str(), _T("US")) << "交易所代码不使用读取的数据";
			EXPECT_STREQ(m_pStock->GetTicker().c_str(), _T("AAPL"));
			EXPECT_FALSE(m_pStock->IsUpdateCompanyProfile());
			EXPECT_TRUE(m_pStock->IsUpdateProfileDB());
			EXPECT_EQ(m_pStock->GetProfileUpdateDate(), gl_pWorldMarket->GetMarketDate());
			break;
		case 5: // 格式不对
			EXPECT_STREQ(m_pStock->GetExchangeCode().c_str(), _T("US")) << "交易所代码不使用读取的数据";
			EXPECT_FALSE(m_pStock->IsUpdateCompanyProfile());
			EXPECT_FALSE(m_pStock->IsUpdateProfileDB());
			EXPECT_NE(m_pStock->GetProfileUpdateDate(), gl_pWorldMarket->GetMarketDate());
			break;
		case 6: // 缺乏address项
			EXPECT_STREQ(m_pStock->GetExchangeCode().c_str(), _T("US")) << "交易所代码不使用读取的数据";
			EXPECT_STRNE(m_pStock->GetCity().c_str(), _T("slaughterer")) << "没有赋值此项";
			EXPECT_FALSE(m_pStock->IsUpdateCompanyProfile());
			EXPECT_FALSE(m_pStock->IsUpdateProfileDB());
			EXPECT_NE(m_pStock->GetProfileUpdateDate(), gl_pWorldMarket->GetMarketDate());
			break;
		case 7: // dummy data
			EXPECT_STREQ(m_pStock->GetExchangeCode().c_str(), _T("US")) << "交易所代码不使用读取的数据";
			EXPECT_STREQ(m_pStock->GetTicker().c_str(), _T("AAPL"));
			EXPECT_FALSE(m_pStock->IsUpdateCompanyProfile());
			EXPECT_TRUE(m_pStock->IsUpdateProfileDB());
			EXPECT_EQ(m_pStock->GetProfileUpdateDate(), gl_pWorldMarket->GetMarketDate());
			break;
		case 10:
			EXPECT_STREQ(m_pStock->GetExchangeCode().c_str(), _T("US")) << "交易所代码不使用读取的数据";
			EXPECT_STREQ(m_pStock->GetTicker().c_str(), _T("AAPL"));
			EXPECT_STREQ(m_pStock->GetCity().c_str(), _T("slaughterer"));
			EXPECT_FALSE(m_pStock->IsUpdateCompanyProfile());
			EXPECT_TRUE(m_pStock->IsUpdateProfileDB());
			EXPECT_EQ(m_pStock->GetProfileUpdateDate(), gl_pWorldMarket->GetMarketDate());
			break;
		default:
			break;
		}
	}
}
