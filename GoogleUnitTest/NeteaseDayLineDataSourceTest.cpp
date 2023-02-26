#include"pch.h"

#include"GeneralCheck.h"

#include"NeteaseDayLineDataSource.h"
#include"WorldMarket.h"
#include"ChinaMarket.h"
#include "ChinaStockCodeConverter.h"
#include "TimeConvert.h"

using namespace testing;

namespace FireBirdTest {
	class CNeteaseDayLineDataSourceTest : public Test {
	protected:
		static void SetUpTestSuite(void) {
			GeneralCheck();
		}

		static void TearDownTestSuite(void) {
			GeneralCheck();
		}

		void SetUp(void) override {
			GeneralCheck();
		}

		void TearDown(void) override {
			// clearUp
			gl_pChinaMarket->SetSystemReady(true);
			GeneralCheck();
		}

	protected:
		CNeteaseDayLineDataSource NeteaseDayLineDataSource;
	};

	TEST_F(CNeteaseDayLineDataSourceTest, TestInitialize) {
		EXPECT_STREQ(NeteaseDayLineDataSource.GetInquiryFunction(), _T("http://quotes.money.163.com/service/chddata.html?code="));
		EXPECT_STREQ(NeteaseDayLineDataSource.GetInquirySuffix(), _T("&fields=TCLOSE;HIGH;LOW;TOPEN;LCLOSE;CHG;TURNOVER;VOTURNOVER;VATURNOVER;TCAP;MCAP"));
		EXPECT_STREQ(NeteaseDayLineDataSource.GetInquiryToken(), _T(""));
	}

	TEST_F(CNeteaseDayLineDataSourceTest, TestSetDownLoadingStockCode) {
		EXPECT_STREQ(NeteaseDayLineDataSource.GetDownLoadingStockCode(), _T(""));
		NeteaseDayLineDataSource.SetDownLoadingStockCode(_T("1000001"));
		EXPECT_STREQ(NeteaseDayLineDataSource.GetDownLoadingStockCode(), _T("1000001"));
		NeteaseDayLineDataSource.SetDownLoadingStockCode(_T("0600001"));
		EXPECT_STREQ(NeteaseDayLineDataSource.GetDownLoadingStockCode(), _T("0600001"));
		NeteaseDayLineDataSource.SetDownLoadingStockCode(_T("2600001"));
		EXPECT_STREQ(NeteaseDayLineDataSource.GetDownLoadingStockCode(), _T("2600001"));
	};

	TEST_F(CNeteaseDayLineDataSourceTest, TestParseData) {
		const CWebDataPtr pData = make_shared<CWebData>();
		EXPECT_FALSE(NeteaseDayLineDataSource.ParseData(pData)) << "无需解析";
	}

	TEST_F(CNeteaseDayLineDataSourceTest, TestGenerateInquiryMessage1) {
		gl_pSinaRTDataSource->SetErrorCode(12002);
		EXPECT_TRUE(gl_systemStatus.IsWebBusy());

		EXPECT_FALSE(NeteaseDayLineDataSource.GenerateInquiryMessage(120000)) << "Web Busy";

		gl_pSinaRTDataSource->SetErrorCode(0);
	}

	TEST_F(CNeteaseDayLineDataSourceTest, TestGenerateInquiryMessage2) {
		//gl_pSinaRTDataSource->SetErrorCode(0);
		gl_pChinaMarket->TEST_SetFormattedMarketTime(120000); // 空闲时间
		EXPECT_FALSE(gl_systemStatus.IsWebBusy());
		EXPECT_TRUE(gl_pChinaMarket->IsSystemReady());
		EXPECT_TRUE(gl_pChinaMarket->IsDummyTime());
		NeteaseDayLineDataSource.SetInquiring(false);

		EXPECT_TRUE(NeteaseDayLineDataSource.GenerateInquiryMessage(120000));
		EXPECT_TRUE(NeteaseDayLineDataSource.HaveInquiry());

		const auto pProduct = NeteaseDayLineDataSource.GetCurrentProduct();
		const CString strMessage = pProduct->GetInquiryFunction();
		CString strSymbol = strMessage.Left(61);
		strSymbol = strSymbol.Right(7);
		const auto pStock = gl_pChinaMarket->GetStock(XferNeteaseToStandard(strSymbol));
		EXPECT_FALSE(pStock->IsDayLineNeedUpdate());

		// 恢复原状
		pStock->SetDayLineNeedUpdate(true);
	}

	TEST_F(CNeteaseDayLineDataSourceTest, TestCreateProduct) {
		EXPECT_EQ(gl_pChinaMarket->GetDayLineNeedUpdateNumber(), gl_pChinaMarket->GetTotalStock());
		gl_pChinaMarket->SetSystemReady(true);

		NeteaseDayLineDataSource.CreateProduct();

		const auto pProduct = NeteaseDayLineDataSource.GetCurrentProduct();
		EXPECT_TRUE(pProduct != nullptr);
		EXPECT_STREQ(typeid(*pProduct).name(), _T("class CProductNeteaseDayLine"));

		const CString str = pProduct->GetInquiryFunction();
		EXPECT_STREQ(str.Left(54), _T("http://quotes.money.163.com/service/chddata.html?code="));
		EXPECT_STREQ(str.Right(81), _T("&fields=TCLOSE;HIGH;LOW;TOPEN;LCLOSE;CHG;TURNOVER;VOTURNOVER;VATURNOVER;TCAP;MCAP"));
		gl_pChinaMarket->SetSystemReady(false);
		EXPECT_EQ(gl_pChinaMarket->GetDayLineNeedUpdateNumber() + 1, gl_pChinaMarket->GetTotalStock()) << "已经有一个无需更新日线了";

		// 恢复原态
		for (int i = 0; i < gl_pChinaMarket->GetTotalStock(); i++) {
			gl_pChinaMarket->GetStock(i)->SetDayLineNeedUpdate(true);
		}
	}

	TEST_F(CNeteaseDayLineDataSourceTest, TestCreateWebDataAfterSucceedReading) {
		NeteaseDayLineDataSource.TESTSetBuffer(_T("{ \"data\": 2}"));
		const time_t tUTCTime = GetUTCTime();
		NeteaseDayLineDataSource.SetDownLoadingStockCode(_T("TEST"));
		TestSetUTCTime(0);

		const auto pWebData = NeteaseDayLineDataSource.CreateWebDataAfterSucceedReading();

		EXPECT_TRUE(pWebData != nullptr);
		EXPECT_FALSE(pWebData->IsParsed()) << "网易日线不是json制式，无需解析";
		EXPECT_STREQ(pWebData->GetStockCode(), _T("TEST")) << "会设置StockCode";
		EXPECT_EQ(pWebData->GetTime(), 0) << "设置为当前的UTCTime";
		EXPECT_TRUE(pWebData->GetDataBuffer() == _T("{ \"data\": 2}"));

		// restore
		TestSetUTCTime(tUTCTime);
	}
}
