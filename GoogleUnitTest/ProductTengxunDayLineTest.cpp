#include"pch.h"

#include"GeneralCheck.h"

#include"ChinaMarket.h"

#include"ProductTengxunDayLine.h"
#include "WebData.h"

using namespace testing;

namespace FireBirdTest {
	class CProductTengxunDayLineTest : public Test {
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
			tengxunDayLine.SetMarket(gl_pChinaMarket);
			tengxunDayLine.SetInquiryNumber(1);
		}

		void TearDown() override {
			// clearUp
			SCOPED_TRACE("");
			GeneralCheck();
		}

	protected:
		CProductTengxunDayLine tengxunDayLine;
	};

	TEST_F(CProductTengxunDayLineTest, TestInitialize) {
		EXPECT_STREQ(tengxunDayLine.GetInquiryFunction(), _T(""));
	}

	TEST_F(CProductTengxunDayLineTest, TestCreateMessage) {
		EXPECT_STREQ(tengxunDayLine.CreateMessage(), _T("")) << "腾讯日线数据的申请字符串由CTengxunDayLineWebInquiry类完成，本Product无需动作，只是返回strInquiry";
	}

	TEST_F(CProductTengxunDayLineTest, TestParseAndStoreWebData) {
		auto pData = make_shared<CWebData>();
		shared_ptr<vector<CWebDataPtr>> pvWebData = make_shared<vector<CWebDataPtr>>();
		const CString strTengxunDayLine = _T("{\"code\":0,\"msg\":\"\",\"data\":{\"sh600601\":{\"day\":[[\"2023-01-19\",\"2.550\",\"2.600\",\"2.610\",\"2.550\",\"86162.000\"],[\"2023-01-20\",\"2.600\",\"2.620\",\"2.620\",\"2.590\",\"100735.000\"]],\"qt\":{},\"mx_price\":{\"mx\":[],\"price\":[]},\"prec\":\"2.560\",\"version\":\"16\"}}}");
		pData->Test_SetBuffer_(strTengxunDayLine);
		pData->SetStockCode(_T("600601.SS"));
		pvWebData->push_back(pData);
		auto pData2 = make_shared<CWebData>();
		const CString strTengxunDayLine2 = _T("{\"code\":0,\"msg\":\"\",\"data\":{\"sh600601\":{\"day\":[[\"2023-01-23\",\"2.550\",\"2.650\",\"2.610\",\"2.550\",\"86162.000\"],[\"2023-01-24\",\"2.600\",\"2.620\",\"2.620\",\"2.590\",\"100735.000\"]],\"qt\":{},\"mx_price\":{\"mx\":[],\"price\":[]},\"prec\":\"2.560\",\"version\":\"16\"}}}");
		pData2->Test_SetBuffer_(strTengxunDayLine2);
		pData2->SetStockCode(_T("600601.SS"));
		pvWebData->push_back(pData2);
		tengxunDayLine.SetInquiryNumber(2); // 测试版要确保此值大于等于数据量，否则会引发断言失败。

		tengxunDayLine.ParseAndStoreWebData(pvWebData);

		EXPECT_EQ(gl_qDayLine.size_approx(), 1) << "两个数据皆为同一个股票日线，故而合二为一了";
		CDayLineWebDataPtr pTengxunDayLine;
		gl_qDayLine.try_dequeue(pTengxunDayLine);
		EXPECT_EQ(pTengxunDayLine->GetStockCode(), _T("600601.SS"));

		const auto vDayLine = pTengxunDayLine->GetProcessedDayLine();
		EXPECT_EQ(vDayLine.size(), 4) << "两个数据各有两个日期的日线数据";
		EXPECT_STREQ(vDayLine.at(0)->m_strStockSymbol.c_str(), _T("600601.SS")) << "腾讯日线不从数据中解析股票代码，而是直接使用被查询股票的代码";
		EXPECT_EQ(vDayLine.at(0)->m_lDate, 20230119);
		EXPECT_EQ(vDayLine.at(0)->m_lLastClose, 0) << "第一个数据没有收盘数据";
		EXPECT_EQ(vDayLine.at(1)->m_lDate, 20230120);
		EXPECT_EQ(vDayLine.at(1)->m_lLastClose, 2600);
		EXPECT_EQ(vDayLine.at(2)->m_lDate, 20230123);
		EXPECT_EQ(vDayLine.at(2)->m_lLastClose, 2620);
		EXPECT_EQ(vDayLine.at(3)->m_lDate, 20230124);
		EXPECT_EQ(vDayLine.at(3)->m_lLastClose, 2650);
	}
}
