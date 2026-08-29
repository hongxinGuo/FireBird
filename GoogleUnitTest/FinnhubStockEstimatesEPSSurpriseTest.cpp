#include"pch.h"

#include <gtest/gtest.h>
#include"GeneralCheck.h"

#include"WorldMarket.h"
#include"TimeConvert.h"
#include "ContainerFinnhubStock.h"
#include"FinnhubStock.h"
#include"ProductFinnhubStockEstimatesEPSSurprise.h"
#include "SystemMessage.h"
#include"EPSSurprise.h"
#include"TestWebData.h"

using namespace testing;

using namespace std;

namespace FireBirdTest {
	class CFinnhubStockEstimatesEPSSurpriseTest : public Test {
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
		CProductFinnhubStockEstimatesEPSSurprise stockEstimatesEPSSurprise;
	};

	TEST_F(CFinnhubStockEstimatesEPSSurpriseTest, TestInitialize) {
		EXPECT_EQ(stockEstimatesEPSSurprise.GetIndex(), 0);
		EXPECT_EQ(stockEstimatesEPSSurprise.GetInquiryFunction(), "https://finnhub.io/api/v1/stock/earnings?symbol=");
	}

	TEST_F(CFinnhubStockEstimatesEPSSurpriseTest, TestCreatMessage) {
		stockEstimatesEPSSurprise.SetIndex(1);
		EXPECT_TRUE(stockEstimatesEPSSurprise.CreateMessage()->front() == stockEstimatesEPSSurprise.GetInquiryFunction() + gl_dataContainerFinnhubStock.GetItem(1)->GetSymbol());
	}

	TEST_F(CFinnhubStockEstimatesEPSSurpriseTest, TestProcessWebData) {
		// 由MockWorldMarketTest负责测试
	}

	namespace {
		// 格式不对(缺开始的‘[’），无法顺利Parser
		Test_FinnhubWebData finnhubWebData122(2, "AAPL", R"({"actual":1.68,"estimate":1.555857,"period":"2020-12-31","symbol":"AAPL"},{"actual":0.73,"estimate":0.7142244,"period":"2020-09-30","symbol":"AAPL"},{"actual":0.645,"estimate":0.5211078,"period":"2020-06-30","symbol":"AAPL"},{"actual":0.6375,"estimate":0.5765856,"period":"2020-03-31","symbol":"AAPL"}])");
		// 第一个数据缺乏actual
		Test_FinnhubWebData finnhubWebData123(3, "AAPL", R"([{"Missing":1.68,"estimate":1.555857,"period":"2020-12-31","symbol":"AAPL"},{"actual":0.73,"estimate":0.7142244,"period":"2020-09-30","symbol":"AAPL"},{"actual":0.645,"estimate":0.5211078,"period":"2020-06-30","symbol":"AAPL"},{"actual":0.6375,"estimate":0.5765856,"period":"2020-03-31","symbol":"AAPL"}])");
		// 第二个数据缺乏actual
		Test_FinnhubWebData finnhubWebData124(4, "AAPL", R"([{"actual":1.68,"estimate":1.555857,"period":"2020-12-31","symbol":"AAPL"},{"Missing":0.73,"estimate":0.7142244,"period":"2020-09-30","symbol":"AAPL"},{"actual":0.645,"estimate":0.5211078,"period":"2020-06-30","symbol":"AAPL"},{"actual":0.6375,"estimate":0.5765856,"period":"2020-03-31","symbol":"AAPL"}])");
		// 第三个数据缺乏actual
		Test_FinnhubWebData finnhubWebData125(5, "AAPL", R"([{"actual":1.68,"estimate":1.555857,"period":"2020-12-31","symbol":"AAPL"},{"actual":0.73,"estimate":0.7142244,"period":"2020-09-30","symbol":"AAPL"},{"Missing":0.645,"estimate":0.5211078,"period":"2020-06-30","symbol":"AAPL"},{"actual":0.6375,"estimate":0.5765856,"period":"2020-03-31","symbol":"AAPL"}])");
		// 正确的数据
		Test_FinnhubWebData finnhubWebData130(10, "AAPL", R"([{"actual":1.68,"estimate":1.555857,"period":"2020-12-31","symbol":"AAPL"},{"actual":0.73,"estimate":0.7142244,"period":"2020-09-30","symbol":"AAPL"},{"actual":0.645,"estimate":0.5211078,"period":"2020-06-30","symbol":"AAPL"},{"actual":0.6375,"estimate":0.5765856,"period":"2020-03-31","symbol":"AAPL"}])");
	}

	class ParseFinnhubEPSSurpriseTest : public TestWithParam<Test_FinnhubWebData*> {
	protected:
		void SetUp() override {
			SCOPED_TRACE("");
			GeneralCheck();
			const Test_FinnhubWebData* pData = GetParam();
			m_index = pData->m_index;
			m_pStock = gl_dataContainerFinnhubStock.GetItem(pData->m_strSymbol);
			EXPECT_TRUE(m_pStock != nullptr);
			m_text = pData->m_data;
			m_finnhubStockEstimatesEPSSurprise.Test_checkAccessRight_(m_text);

			m_pvEPSSurprise = nullptr;
		}

		void TearDown() override {
			// clearUp
			while (gl_systemMessage.ErrorMessageSize() > 0) gl_systemMessage.PopErrorMessage();
			m_pStock->SetUpdateCompanyProfile(true);
			m_pStock->SetUpdateProfileDB(false);
			SCOPED_TRACE("");
			GeneralCheck();
		}

	public:
		int m_index;
		CFinnhubStockPtr m_pStock;
		string m_text;
		CEPSSurprisesPtr m_pvEPSSurprise;
		CProductFinnhubStockEstimatesEPSSurprise m_finnhubStockEstimatesEPSSurprise;
	};

	INSTANTIATE_TEST_SUITE_P(TestParseFinnhubEPSSurprise1, ParseFinnhubEPSSurpriseTest,
	                         testing::Values(&finnhubWebData0, &finnhubWebData1, &finnhubWebData122, &finnhubWebData123, &finnhubWebData124,
		                         &finnhubWebData125, &finnhubWebData130));

	TEST_P(ParseFinnhubEPSSurpriseTest, TestParseFinnhubEPSSurprise0) {
		m_pvEPSSurprise = m_finnhubStockEstimatesEPSSurprise.Parse(m_text);
		switch (m_index) {
		case 0: // 空数据
			EXPECT_EQ(m_pvEPSSurprise->size(), 0);
			break;
		case 1: // 无权利访问的数据
			EXPECT_EQ(m_pvEPSSurprise->size(), 0);
			break;
		case 2: // 格式不对
			EXPECT_EQ(m_pvEPSSurprise->size(), 0);
			break;
		case 3: //第一个数据缺actual
			EXPECT_EQ(m_pvEPSSurprise->size(), 0) << "第一个数据极为错误的";
			break;
		case 4: // 第二个数据缺缺actual
			EXPECT_EQ(m_pvEPSSurprise->size(), 1) << "错误数据不存储";
			EXPECT_DOUBLE_EQ(m_pvEPSSurprise->at(0).m_dActual, 1.68);
			EXPECT_DOUBLE_EQ(m_pvEPSSurprise->at(0).m_dEstimate, 1.555857);
			EXPECT_EQ(m_pvEPSSurprise->at(0).m_lDate, toLocalDays(20201231));
			EXPECT_EQ(m_pvEPSSurprise->at(0).m_strSymbol, "AAPL");
			break;
		case 5: // 第三个数据缺CodeNo
			EXPECT_EQ(m_pvEPSSurprise->size(), 2) << "第三个数据是错误的";
			EXPECT_DOUBLE_EQ(m_pvEPSSurprise->at(0).m_dActual, 1.68);
			EXPECT_DOUBLE_EQ(m_pvEPSSurprise->at(0).m_dEstimate, 1.555857);
			EXPECT_EQ(m_pvEPSSurprise->at(0).m_lDate, toLocalDays(20201231));
			EXPECT_EQ(m_pvEPSSurprise->at(0).m_strSymbol, "AAPL");
			break;
		case 10:
			EXPECT_EQ(m_pvEPSSurprise->size(), 4);
			EXPECT_DOUBLE_EQ(m_pvEPSSurprise->at(0).m_dActual, 0.6375);
			EXPECT_DOUBLE_EQ(m_pvEPSSurprise->at(0).m_dEstimate, 0.5765856);
			EXPECT_EQ(m_pvEPSSurprise->at(0).m_lDate, toLocalDays(20200331));
			EXPECT_EQ(m_pvEPSSurprise->at(0).m_strSymbol, "AAPL");
			EXPECT_DOUBLE_EQ(m_pvEPSSurprise->at(3).m_dActual, 1.68) << "成功处理后，自动按日期排列，导致其被放置于最后";
			EXPECT_DOUBLE_EQ(m_pvEPSSurprise->at(3).m_dEstimate, 1.555857);
			EXPECT_EQ(m_pvEPSSurprise->at(3).m_lDate, toLocalDays(20201231));
			EXPECT_EQ(m_pvEPSSurprise->at(3).m_strSymbol, "AAPL");
			break;
		default:
			break;
		}
	}

}
