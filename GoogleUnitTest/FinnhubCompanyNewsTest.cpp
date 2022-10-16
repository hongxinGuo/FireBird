#include"pch.h"

#include"GeneralCheck.h"

#include"TimeConvert.h"
#include"WorldMarket.h"

#include"ProductFinnhubCompanyNews.h"

using namespace testing;

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

namespace StockAnalysisTest {
	class CFinnhubCompanyNewsTest : public ::testing::Test {
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
		CProductFinnhubCompanyNews companyNews;
	};

	TEST_F(CFinnhubCompanyNewsTest, TestInitialize) {
		EXPECT_EQ(companyNews.GetIndex(), -1);
		EXPECT_STREQ(companyNews.GetInquiringStr(), _T("https://finnhub.io/api/v1/stock/company-news?symbol="));
	}

	TEST_F(CFinnhubCompanyNewsTest, TestCreatMessage) {
		char buffer[50]{};
		int iUpdateDate = 0, iMarketDate = 0;
		int year = 0, month = 0, day = 0;
		CString strTemp, strMessage;

		gl_pWorldMarket->GetStock(1)->SetCompanyNewsUpdated(false);
		companyNews.SetMarket(gl_pWorldMarket.get());
		companyNews.SetIndex(1);
		CWorldStockPtr pStock = ((CWorldMarket*)(companyNews.GetMarket()))->GetStock(1);
		strMessage = companyNews.GetInquiringStr() + pStock->GetSymbol();
		iUpdateDate = pStock->GetCompanyNewsUpdateDate();
		XferDateToYearMonthDay(iUpdateDate, year, month, day);
		sprintf_s(buffer, _T("%4d-%02d-%02d"), year, month, day);
		strTemp = buffer;
		strMessage += _T("&from=");
		strMessage += strTemp;
		iMarketDate = gl_pWorldMarket->GetMarketDate();
		XferDateToYearMonthDay(iUpdateDate, year, month, day);
		sprintf_s(buffer, _T("%4d-%02d-%02d"), year, month, day);
		strTemp = buffer;
		strMessage += _T("&to=");
		strMessage += strTemp;

		EXPECT_STREQ(companyNews.CreatMessage(), strMessage);
		EXPECT_FALSE(gl_pWorldMarket->GetStock(1)->IsCompanyNewsUpdated()) << "处理接收到的数据后才设置此标识";

		gl_pWorldMarket->GetStock(1)->SetCompanyNewsUpdated(false);
	}

	// 格式不对(缺开始的‘[’），无法顺利Parser
	FinnhubWebData finnhubWebDataCompanyNews2(2, _T("AAPL"), _T("{\"category\":\"company news\",\"datetime\":1569550360,\"headline\":\"More\",\"id\":25286,\"image\":\"https://img.etimg.com/thumb/msid-71321314,width-1070,height-580,imgsize-481831,overlay-economictimes/photo.jpg\",\"related\":\"AAPL\",\"source\":\"The Economic Times India\",\"summary\":\"NEW DELHI\",\"url\":\"https://economictimes.indiatimes.com/industry/cons-products/electronics/more-sops-needed-to-boost-electronic-manufacturing-top-govt-official/articleshow/71321308.cms\"}]"));

	// 数据缺乏category项
	FinnhubWebData finnhubWebDataCompanyNews3(3, _T("AAPL"), _T("[{\"missing\":\"company news\",\"datetime\":1569550360,\"headline\":\"More\",\"id\":25286,\"image\":\"https://img.etimg.com/thumb/msid-71321314,width-1070,height-580,imgsize-481831,overlay-economictimes/photo.jpg\",\"related\":\"AAPL\",\"source\":\"The Economic Times India\",\"summary\":\"NEW DELHI\",\"url\":\"https://economictimes.indiatimes.com/industry/cons-products/electronics/more-sops-needed-to-boost-electronic-manufacturing-top-govt-official/articleshow/71321308.cms\"}]"));
	// 空数据
	FinnhubWebData finnhubWebDataCompanyNews4(4, _T("AAPL"), _T("{}"));
	// 正确的数据
	FinnhubWebData finnhubWebDataCompanyNews10(10, _T("AAPL"), _T("[{\"category\":\"company news\",\"datetime\":1,\"headline\":\"More\",\"id\":25286,\"image\":\"https://img.etimg.com/thumb/msid-71321314,width-1070,height-580,imgsize-481831,overlay-economictimes/photo.jpg\",\"related\":\"AAPL\",\"source\":\"The Economic Times India\",\"summary\":\"NEW DELHI\",\"url\":\"https://economictimes.indiatimes.com/industry/cons-products/electronics/more-sops-needed-to-boost-electronic-manufacturing-top-govt-official/articleshow/71321308.cms\"},{\"category\":\"company news\",\"datetime\":1569550361,\"headline\":\"More2\",\"id\":25287,\"image\":\"https://img.etimg.com/thumb/msid-71321314,width-1070,height-580,imgsize-481831,overlay-economictimes/photo.jpg\",\"related\":\"AAPL\",\"source\":\"The Economic Times India\",\"summary\":\"NEW DELHI2\",\"url\":\"https://economictimes.indiatimes.com/industry/cons-products/electronics/more-sops-needed-to-boost-electronic-manufacturing-top-govt-official/articleshow/71321308.cms\"}]"));

	class ProcessFinnhubStockCompanyNewsTest : public::testing::TestWithParam<FinnhubWebData*> {
	protected:
		virtual void SetUp(void) override {
			GeneralCheck();
			FinnhubWebData* pData = GetParam();
			m_lIndex = pData->m_lIndex;
			m_pStock = gl_pWorldMarket->GetStock(pData->m_strSymbol);
			EXPECT_TRUE(m_pStock != nullptr);
			m_pStock->SetCountry(_T(""));
			m_pWebData = pData->m_pData;
			m_pWebData->CreatePropertyTree();
			m_pWebData->SetJSonContentType(true);
			m_FinnhubCompanyNews.SetMarket(gl_pWorldMarket.get());
			m_FinnhubCompanyNews.SetIndex(gl_pWorldMarket->GetStockIndex(pData->m_strSymbol));
		}
		virtual void TearDown(void) override {
			// clearup
			while (gl_systemMessage.ErrorMessageSize() > 0) gl_systemMessage.PopErrorMessage();
			m_pStock->SetCompanyNewsUpdateDate(19800101);
			m_pStock->SetCompanyNewsUpdated(false);
			m_pStock->SetUpdateCompanyNewsDB(false);

			GeneralCheck();
		}

	public:
		long m_lIndex;
		CWorldStockPtr m_pStock;
		CWebDataPtr m_pWebData;
		CProductFinnhubCompanyNews m_FinnhubCompanyNews;
	};

	INSTANTIATE_TEST_SUITE_P(TestParseFinnhubStockCompanyNews1, ProcessFinnhubStockCompanyNewsTest, testing::Values(&finnhubWebDataCompanyNews2,
		&finnhubWebDataCompanyNews3, &finnhubWebDataCompanyNews4, &finnhubWebDataCompanyNews10));

	TEST_P(ProcessFinnhubStockCompanyNewsTest, TestProcessStockCompanyNews0) {
		bool fSucceed = false;
		fSucceed = m_FinnhubCompanyNews.ParseAndStoreWebData(m_pWebData);
		switch (m_lIndex) {
		case 2: // 格式不对
			EXPECT_TRUE(fSucceed);
			EXPECT_TRUE(m_pStock->IsCompanyNewsUpdated());
			EXPECT_FALSE(m_pStock->IsUpdateCompanyNewsDB());
			EXPECT_EQ(m_pStock->GetCompanyNewsUpdateDate(), gl_pWorldMarket->GetMarketDate());
			break;
		case 3: // 缺乏address项
			EXPECT_TRUE(fSucceed);
			EXPECT_STRNE(m_pStock->GetCountry(), _T("US")) << "没有赋值此项";
			EXPECT_TRUE(m_pStock->IsCompanyNewsUpdated());
			EXPECT_FALSE(m_pStock->IsUpdateCompanyNewsDB());
			EXPECT_EQ(m_pStock->GetCompanyNewsUpdateDate(), gl_pWorldMarket->GetMarketDate());
			break;
		case 4: // 空数据
			EXPECT_TRUE(fSucceed);
			EXPECT_TRUE(m_pStock->IsCompanyNewsUpdated());
			EXPECT_FALSE(m_pStock->IsUpdateCompanyNewsDB());
			EXPECT_EQ(m_pStock->GetCompanyNewsUpdateDate(), gl_pWorldMarket->GetMarketDate());
			break;
		case 10:
			EXPECT_TRUE(fSucceed);
			EXPECT_STREQ(m_pStock->GetTicker(), _T("AAPL"));
			EXPECT_EQ(m_pStock->m_vCompanyNews.size(), 2);
			EXPECT_EQ(m_pStock->m_vCompanyNews.at(0)->m_llDateTime, 19700101000001);
			EXPECT_TRUE(m_pStock->IsCompanyNewsUpdated());
			EXPECT_TRUE(m_pStock->IsUpdateCompanyNewsDB());
			EXPECT_EQ(m_pStock->GetCompanyNewsUpdateDate(), gl_pWorldMarket->GetMarketDate());
			break;
		default:
			break;
		}
	}
}