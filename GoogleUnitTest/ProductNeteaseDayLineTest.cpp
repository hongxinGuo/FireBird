#include"pch.h"

#include"ConcurrentQueueHeader.h"
#include"GeneralCheck.h"

import FireBird.Market.China;

import FireBird.WebProduct.NeteaseDayLine;

using namespace testing;

namespace FireBirdTest {
	class CProductNeteaseDayLineTest : public Test {
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
			neteaseDayLine.SetMarket(gl_pChinaMarket);
		}

		void TearDown() override {
			// clearUp
			SCOPED_TRACE("");
			GeneralCheck();
		}

	protected:
		CProductNeteaseDayLine neteaseDayLine;
	};

	TEST_F(CProductNeteaseDayLineTest, TestInitialize) {
		EXPECT_STREQ(neteaseDayLine.GetInquiryFunction(), _T("http://quotes.money.163.com/service/chddata.html?code="));
	}

	TEST_F(CProductNeteaseDayLineTest, TestCreateMessage) {
		EXPECT_STREQ(neteaseDayLine.CreateMessage(), _T("http://quotes.money.163.com/service/chddata.html?code=")) << "网易日线数据的申请字符串由CNeteaseDayLineWebInquiry类完成，本Product无需动作";
	}

	TEST_F(CProductNeteaseDayLineTest, TestParseAndStoreWebData) {
		const auto pData = make_shared<CWebData>();
		const CString strNeteaseDayLine = _T("日期,股票代码,名称,收盘价,最高价,最低价,开盘价,前收盘,涨跌额,换手率,成交量,成交金额,总市值,流通市值\r\n2019-07-24,'600000,浦发银行,11.49,11.56,11.43,11.43,11.48,0.01,0.0638,17927898,206511000.0,3.37255403762e+11,3.229122472e+11\r\n2019-07-23,'600000,浦发银行,11.49,11.56,11.43,11.43,11.48,0.01,0.0638,17927898,206511000.0,3.37255403762e+11,3.229122472e+11\r\n");
		pData->Test_SetBuffer_(strNeteaseDayLine);
		pData->SetStockCode(_T("600000.SS"));
		neteaseDayLine.ParseAndStoreWebData(pData);

		EXPECT_EQ(gl_qDayLine.size_approx(), 1);
		CDayLineWebDataPtr pNeteaseDayLine;
		gl_qDayLine.try_dequeue(pNeteaseDayLine);
		EXPECT_EQ(pNeteaseDayLine->GetStockCode(), _T("600000.SS"));

		const auto vDayLine = pNeteaseDayLine->GetProcessedDayLine();
		EXPECT_EQ(vDayLine.size(), 2);
		EXPECT_STREQ(vDayLine.at(0)->m_strStockSymbol, _T("600000.SS")) << "网易日线不从数据中解析股票代码，而是直接使用被查询股票的代码";
		EXPECT_EQ(vDayLine.at(0)->m_lDate, 20190723) << "网易日线逆序发送，最新的日期在最前面。处理后的数据进行正序排序";
		EXPECT_EQ(vDayLine.at(1)->m_lDate, 20190724);
	}
}
