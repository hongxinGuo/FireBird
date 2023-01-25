#include"pch.h"

#include"GeneralCheck.h"

#include"ChinaMarket.h"

#include"ProductTengxunDayLine.h"

using namespace testing;

namespace FireBirdTest {
	class CProductTengxunDayLineTest : public Test {
	protected:
		static void SetUpTestSuite(void) {
			GeneralCheck();
		}

		static void TearDownTestSuite(void) {
			GeneralCheck();
		}

		void SetUp(void) override {
		}

		void TearDown(void) override {
			// clearUp
			GeneralCheck();
		}

	protected:
		CProductTengxunDayLine tengxunDayLine;
	};

	TEST_F(CProductTengxunDayLineTest, TestInitialize) {
		EXPECT_STREQ(tengxunDayLine.GetInquiry(), _T("https://web.ifzq.gtimg.cn/appstock/app/fqkline/get?param="));
	}

	TEST_F(CProductTengxunDayLineTest, TestCreateMessage) {
		EXPECT_STREQ(tengxunDayLine.CreateMessage(), _T("https://web.ifzq.gtimg.cn/appstock/app/fqkline/get?param=")) << "腾讯日线数据的申请字符串由CTengxunDayLineWebInquiry类完成，本Product无需动作";
	}

	TEST_F(CProductTengxunDayLineTest, TestParseAndStoreWebData) {
		const auto pData = make_shared<CWebData>();
		const CString strTengxunDayLine = _T("{\"code\":0,\"msg\":\"\",\"data\":{\"sh600601\":{\"day\":[[\"2023-01-19\",\"2.550\",\"2.600\",\"2.610\",\"2.550\",\"86162.000\"],[\"2023-01-20\",\"2.600\",\"2.620\",\"2.620\",\"2.590\",\"100735.000\"]],\"qt\":{},\"mx_price\":{\"mx\":[],\"price\":[]},\"prec\":\"2.560\",\"version\":\"16\"}}}");
		pData->Test_SetBuffer_(strTengxunDayLine);
		pData->SetStockCode(_T("600601.SS"));
		tengxunDayLine.ParseAndStoreWebData(pData);

		EXPECT_EQ(gl_pChinaMarket->DayLineSize(), 1);
		const auto pTengxunDayLine = gl_pChinaMarket->PopDayLine();
		EXPECT_EQ(pTengxunDayLine->GetStockCode(), _T("600601.SS"));

		const auto vDayLine = pTengxunDayLine->GetProcessedDayLine();
		EXPECT_EQ(vDayLine.size(), 2);
		EXPECT_STREQ(vDayLine.at(0)->m_strStockSymbol, _T("600601.SS")) << "腾讯日线不从数据中解析股票代码，而是直接使用被查询股票的代码";
		EXPECT_EQ(vDayLine.at(0)->m_lDate, 20230119);
		EXPECT_EQ(vDayLine.at(0)->m_lLastClose, 0) << "第一个数据没有收盘数据";
		EXPECT_EQ(vDayLine.at(1)->m_lDate, 20230120);
		EXPECT_EQ(vDayLine.at(1)->m_lLastClose, 2600);
	}
}
