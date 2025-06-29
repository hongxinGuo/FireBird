#include"pch.h"

#include"GeneralCheck.h"

#include"NeteaseDayLineDataSource.h"
//#include"WorldMarket.h"
#include"ChinaMarket.h"
#include "ChinaStockCodeConverter.h"
#include "SinaRTDataSource.h"

using namespace testing;

namespace FireBirdTest {
	class CNeteaseDayLineDataSourceTest : public Test {
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
			gl_pChinaMarket->SetSystemReady(true);
			SCOPED_TRACE("");
			GeneralCheck();
		}

	protected:
		CNeteaseDayLineDataSource NeteaseDayLineDataSource;
	};

	TEST_F(CNeteaseDayLineDataSourceTest, TestInitialize) {
		EXPECT_STREQ(NeteaseDayLineDataSource.GetInquiryFunction().c_str(), _T("http://quotes.money.163.com/service/chddata.html?code="));
		EXPECT_STREQ(NeteaseDayLineDataSource.GetInquirySuffix().c_str(), _T("&fields=TCLOSE;HIGH;LOW;TOPEN;LCLOSE;CHG;TURNOVER;VOTURNOVER;VATURNOVER;TCAP;MCAP"));
		EXPECT_STREQ(NeteaseDayLineDataSource.GetInquiryToken().c_str(), _T(""));
	}

	TEST_F(CNeteaseDayLineDataSourceTest, TestSetDownLoadingStockCode) {
		EXPECT_TRUE(NeteaseDayLineDataSource.GetDownLoadingStockCode().compare( _T("")) == 0);
		NeteaseDayLineDataSource.SetDownLoadingStockCode(_T("1000001"));
		EXPECT_TRUE(NeteaseDayLineDataSource.GetDownLoadingStockCode().compare( _T("1000001")) == 0);
		NeteaseDayLineDataSource.SetDownLoadingStockCode(_T("0600001"));
		EXPECT_TRUE(NeteaseDayLineDataSource.GetDownLoadingStockCode().compare( _T("0600001")) == 0);
		NeteaseDayLineDataSource.SetDownLoadingStockCode(_T("2600001"));
		EXPECT_TRUE(NeteaseDayLineDataSource.GetDownLoadingStockCode().compare( _T("2600001")) == 0);
	};

	TEST_F(CNeteaseDayLineDataSourceTest, TestGenerateInquiryMessage1) {
		gl_pSinaRTDataSource->SetWebError(true);
		EXPECT_TRUE(gl_systemConfiguration.IsWebBusy());

		EXPECT_FALSE(NeteaseDayLineDataSource.GenerateInquiryMessage(120000)) << "Web Busy";

		gl_pSinaRTDataSource->SetWebError(false);
	}

	TEST_F(CNeteaseDayLineDataSourceTest, TestGenerateInquiryMessage2) {
		//gl_pSinaRTDataSource->SetWebError(false);
		gl_pChinaMarket->TEST_SetFormattedMarketTime(120000); // 空闲时间
		EXPECT_FALSE(gl_systemConfiguration.IsWebBusy());
		EXPECT_TRUE(gl_pChinaMarket->IsSystemReady());
		EXPECT_TRUE(gl_pChinaMarket->IsDummyTime());
		NeteaseDayLineDataSource.SetInquiring(false);

		EXPECT_TRUE(NeteaseDayLineDataSource.GenerateInquiryMessage(120000));
		EXPECT_TRUE(NeteaseDayLineDataSource.HaveInquiry());

		const auto pProduct = NeteaseDayLineDataSource.GetCurrentProduct();
		const string strMessage = pProduct->GetInquiryFunction();
		string strSymbol = strMessage.substr(0, 61);
		strSymbol = strSymbol.substr(strSymbol.length() - 7, 7);
		const auto pStock = gl_dataContainerChinaStock.GetStock(XferNeteaseToStandard(strSymbol));
		EXPECT_FALSE(pStock->IsUpdateDayLine());

		// 恢复原状
		pStock->SetUpdateDayLine(true);
	}

	TEST_F(CNeteaseDayLineDataSourceTest, TestCreateProduct) {
		EXPECT_EQ(gl_dataContainerChinaStock.GetDayLineNeedUpdateNumber(), gl_dataContainerChinaStock.Size());
		gl_pChinaMarket->SetSystemReady(true);

		NeteaseDayLineDataSource.CreateProduct();

		const auto pProduct = NeteaseDayLineDataSource.GetCurrentProduct();
		EXPECT_TRUE(pProduct != nullptr);
		EXPECT_STREQ(typeid(*pProduct).name(), _T("class CProductNeteaseDayLine"));

		const string str = pProduct->GetInquiryFunction();
		EXPECT_STREQ(str.substr(0, 54).c_str(), _T("http://quotes.money.163.com/service/chddata.html?code="));
		EXPECT_STREQ(str.substr(str.length() - 81, 81).c_str(), _T("&fields=TCLOSE;HIGH;LOW;TOPEN;LCLOSE;CHG;TURNOVER;VOTURNOVER;VATURNOVER;TCAP;MCAP"));
		gl_pChinaMarket->SetSystemReady(false);
		EXPECT_EQ(gl_dataContainerChinaStock.GetDayLineNeedUpdateNumber() + 1, gl_dataContainerChinaStock.Size()) << "已经有一个无需更新日线了";

		// 恢复原态
		for (int i = 0; i < gl_dataContainerChinaStock.Size(); i++) {
			gl_dataContainerChinaStock.GetStock(i)->SetUpdateDayLine(true);
		}
	}
}
