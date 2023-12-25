#include"pch.h"

#include"GeneralCheck.h"

#include"TimeConvert.h"
#include"WorldMarket.h"

#include"ProductFinnhubCompanyNews.h"

using namespace testing;

namespace FireBirdTest {
	class CFinnhubCompanyNewsTest : public Test {
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
		CProductFinnhubCompanyNews companyNews;
	};

	TEST_F(CFinnhubCompanyNewsTest, TestInitialize) {
		EXPECT_EQ(companyNews.GetIndex(), -1);
		EXPECT_STREQ(companyNews.GetInquiryFunction(), _T("https://finnhub.io/api/v1/company-news?symbol="));
	}

	TEST_F(CFinnhubCompanyNewsTest, TestCreatMessage) {
		char buffer[50]{};
		int year = 0, month = 0, day = 0;

		gl_dataContainerFinnhubStock.GetStock(1)->SetUpdateCompanyNews(true);
		companyNews.SetMarket(gl_pWorldMarket);
		companyNews.SetIndex(1);
		const CWorldStockPtr pStock = gl_dataContainerFinnhubStock.GetStock(1);
		CString strMessage = companyNews.GetInquiryFunction() + pStock->GetSymbol();
		const int iUpdateDate = pStock->GetCompanyNewsUpdateDate();
		XferDateToYearMonthDay(iUpdateDate, year, month, day);
		sprintf_s(buffer, _T("%4d-%02d-%02d"), year, month, day);
		CString strTemp = buffer;
		strMessage += _T("&from=");
		strMessage += strTemp;
		const int iMarketDate = gl_pWorldMarket->GetMarketDate();
		XferDateToYearMonthDay(iMarketDate, year, month, day);
		sprintf_s(buffer, _T("%4d-%02d-%02d"), year, month, day);
		strTemp = buffer;
		strMessage += _T("&to=");
		strMessage += strTemp;

		EXPECT_STREQ(companyNews.CreateMessage(), strMessage);
		EXPECT_TRUE(gl_dataContainerFinnhubStock.GetStock(1)->IsUpdateCompanyNews()) << "处理接收到的数据后才设置此标识";

		gl_dataContainerFinnhubStock.GetStock(1)->SetUpdateCompanyNews(true);
	}

	// 格式不对(缺开始的‘[’），无法顺利Parser
	Test_FinnhubWebData finnhubWebDataCompanyNews2(2, _T("AAPL"), _T("{\"category\":\"company news\",\"datetime\":1569550360,\"headline\":\"More\",\"id\":25286,\"image\":\"https://img.etimg.com/thumb/msid-71321314,width-1070,height-580,imgsize-481831,overlay-economictimes/photo.jpg\",\"related\":\"AAPL\",\"source\":\"The Economic Times India\",\"summary\":\"NEW DELHI\",\"url\":\"https://economictimes.indiatimes.com/industry/cons-products/electronics/more-sops-needed-to-boost-electronic-manufacturing-top-govt-official/articleshow/71321308.cms\"}]"));

	// 数据缺乏category项
	Test_FinnhubWebData finnhubWebDataCompanyNews3(3, _T("AAPL"), _T("[{\"missing\":\"company news\",\"datetime\":1569550360,\"headline\":\"More\",\"id\":25286,\"image\":\"https://img.etimg.com/thumb/msid-71321314,width-1070,height-580,imgsize-481831,overlay-economictimes/photo.jpg\",\"related\":\"AAPL\",\"source\":\"The Economic Times India\",\"summary\":\"NEW DELHI\",\"url\":\"https://economictimes.indiatimes.com/industry/cons-products/electronics/more-sops-needed-to-boost-electronic-manufacturing-top-govt-official/articleshow/71321308.cms\"}]"));
	// 空数据
	Test_FinnhubWebData finnhubWebDataCompanyNews4(4, _T("AAPL"), _T("{}"));
	// 无权利访问
	Test_FinnhubWebData finnhubWebDataCompanyNews5(5, _T("AAPL"), _T("{\"error\":\"You don't have access to this resource.\"}"));
	// 正确的数据
	Test_FinnhubWebData finnhubWebDataCompanyNews10(10, _T("AAPL"), _T("[{\"category\":\"company news\",\"datetime\":1,\"headline\":\"More\",\"id\":25286,\"image\":\"https://img.etimg.com/thumb/msid-71321314,width-1070,height-580,imgsize-481831,overlay-economictimes/photo.jpg\",\"related\":\"AAPL\",\"source\":\"The Economic Times India\",\"summary\":\"NEW DELHI\",\"url\":\"https://economictimes.indiatimes.com/industry/cons-products/electronics/more-sops-needed-to-boost-electronic-manufacturing-top-govt-official/articleshow/71321308.cms\"},{\"category\":\"company news\",\"datetime\":1569550361,\"headline\":\"More2\",\"id\":25287,\"image\":\"https://img.etimg.com/thumb/msid-71321314,width-1070,height-580,imgsize-481831,overlay-economictimes/photo.jpg\",\"related\":\"AAPL\",\"source\":\"The Economic Times India\",\"summary\":\"NEW DELHI2\",\"url\":\"https://economictimes.indiatimes.com/industry/cons-products/electronics/more-sops-needed-to-boost-electronic-manufacturing-top-govt-official/articleshow/71321308.cms\"}]"));

	class ProcessFinnhubStockCompanyNewsTest : public TestWithParam<Test_FinnhubWebData*> {
	protected:
		void SetUp() override {
			SCOPED_TRACE("");
			GeneralCheck();
			const Test_FinnhubWebData* pData = GetParam();
			m_lIndex = pData->m_lIndex;
			m_pStock = gl_dataContainerFinnhubStock.GetStock(pData->m_strSymbol);
			EXPECT_TRUE(m_pStock != nullptr);
			m_pStock->SetCountry(_T(""));
			m_pWebData = pData->m_pData;
			m_pWebData->CreateJson();
			m_pWebData->SetJSonContentType(true);
			m_FinnhubCompanyNews.SetMarket(gl_pWorldMarket);
			m_FinnhubCompanyNews.SetIndex(gl_dataContainerFinnhubStock.GetOffset(pData->m_strSymbol));
		}

		void TearDown() override {
			// clearUp
			while (gl_systemMessage.ErrorMessageSize() > 0) gl_systemMessage.PopErrorMessage();
			m_pStock->SetCompanyNewsUpdateDate(19800101);
			m_pStock->SetUpdateCompanyNews(true);
			m_pStock->SetUpdateCompanyNewsDB(false);
			m_pStock->SetUpdateProfileDB(false);

			SCOPED_TRACE("");
			GeneralCheck();
		}

	public:
		long m_lIndex;
		CWorldStockPtr m_pStock;
		CWebDataPtr m_pWebData;
		CProductFinnhubCompanyNews m_FinnhubCompanyNews;
	};

	INSTANTIATE_TEST_SUITE_P(TestParseFinnhubStockCompanyNews1, ProcessFinnhubStockCompanyNewsTest, testing::Values(&finnhubWebDataCompanyNews2,
		                         &finnhubWebDataCompanyNews3, &finnhubWebDataCompanyNews4, &finnhubWebDataCompanyNews5, &finnhubWebDataCompanyNews10));

	TEST_P(ProcessFinnhubStockCompanyNewsTest, TestProcessStockCompanyNews0) {
		bool fSucceed = false;
		fSucceed = m_FinnhubCompanyNews.ParseAndStoreWebData(m_pWebData);
		switch (m_lIndex) {
		case 2: // 格式不对
			EXPECT_TRUE(fSucceed);
			EXPECT_FALSE(m_pStock->IsUpdateCompanyNews());
			EXPECT_FALSE(m_pStock->IsUpdateCompanyNewsDB());
			EXPECT_EQ(m_pStock->GetCompanyNewsUpdateDate(), gl_pWorldMarket->GetMarketDate());
			EXPECT_TRUE(m_pStock->IsUpdateProfileDB());
			break;
		case 3: // 缺乏address项
			EXPECT_TRUE(fSucceed);
			EXPECT_STRNE(m_pStock->GetCountry(), _T("US")) << "没有赋值此项";
			EXPECT_FALSE(m_pStock->IsUpdateCompanyNews());
			EXPECT_FALSE(m_pStock->IsUpdateCompanyNewsDB());
			EXPECT_EQ(m_pStock->GetCompanyNewsUpdateDate(), gl_pWorldMarket->GetMarketDate());
			EXPECT_TRUE(m_pStock->IsUpdateProfileDB());
			break;
		case 4: // 空数据
			EXPECT_TRUE(fSucceed);
			EXPECT_FALSE(m_pStock->IsUpdateCompanyNews());
			EXPECT_FALSE(m_pStock->IsUpdateCompanyNewsDB());
			EXPECT_EQ(m_pStock->GetCompanyNewsUpdateDate(), gl_pWorldMarket->GetMarketDate());
			EXPECT_TRUE(m_FinnhubCompanyNews.IsVoidData()) << "此标识已设置";
			EXPECT_TRUE(m_pStock->IsUpdateProfileDB());
			break;
		case 5: // 无权利访问
			EXPECT_TRUE(fSucceed);
			EXPECT_FALSE(m_pStock->IsUpdateCompanyNews());
			EXPECT_FALSE(m_pStock->IsUpdateCompanyNewsDB());
			EXPECT_EQ(m_pStock->GetCompanyNewsUpdateDate(), gl_pWorldMarket->GetMarketDate());
			EXPECT_TRUE(m_FinnhubCompanyNews.IsNoRightToAccess()) << "无权利访问标识已设置";
			EXPECT_TRUE(m_pStock->IsUpdateProfileDB());
			break;
		case 10:
			EXPECT_TRUE(fSucceed);
			EXPECT_STREQ(m_pStock->GetTicker(), _T("AAPL"));
			EXPECT_EQ(m_pStock->GetCompanyNewsSize(), 2);
			EXPECT_EQ(m_pStock->GetCompanyNewsDateTime(0), 19700101000001);
			EXPECT_FALSE(m_pStock->IsUpdateCompanyNews());
			EXPECT_TRUE(m_pStock->IsUpdateCompanyNewsDB());
			EXPECT_EQ(m_pStock->GetCompanyNewsUpdateDate(), gl_pWorldMarket->GetMarketDate());
			EXPECT_TRUE(m_pStock->IsUpdateProfileDB());
			break;
		default:
			break;
		}
	}
}
