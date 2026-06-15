#include"pch.h"

#include"GeneralCheck.h"

#include"VirtualWebProduct.h"
#include"NeteaseDayLineDataSource.h"
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
		EXPECT_EQ(NeteaseDayLineDataSource.GetInquiryFunction(), "http://quotes.money.163.com/service/chddata.html?code=");
		EXPECT_EQ(NeteaseDayLineDataSource.GetInquirySuffix(), "&fields=TCLOSE;HIGH;LOW;TOPEN;LCLOSE;CHG;TURNOVER;VOTURNOVER;VATURNOVER;TCAP;MCAP");
		EXPECT_EQ(NeteaseDayLineDataSource.GetInquiryToken(), "");
	}

	TEST_F(CNeteaseDayLineDataSourceTest, TestSetDownLoadingStockCode) {
		EXPECT_EQ(NeteaseDayLineDataSource.GetDownLoadingStockCode(), "");
		NeteaseDayLineDataSource.SetDownLoadingStockCode("1000001");
		EXPECT_EQ(NeteaseDayLineDataSource.GetDownLoadingStockCode(), "1000001");
		NeteaseDayLineDataSource.SetDownLoadingStockCode("0600001");
		EXPECT_EQ(NeteaseDayLineDataSource.GetDownLoadingStockCode(), "0600001");
		NeteaseDayLineDataSource.SetDownLoadingStockCode("2600001");
		EXPECT_EQ(NeteaseDayLineDataSource.GetDownLoadingStockCode(), "2600001");
	};

	TEST_F(CNeteaseDayLineDataSourceTest, TestGenerateInquiryMessage1) {
		gl_pSinaRTDataSource->SetWebError(true);
		EXPECT_TRUE(gl_systemConfiguration.IsWebBusy());

		EXPECT_FALSE(NeteaseDayLineDataSource.GenerateInquiryMessage(toLocalTime(120000))) << "Web Busy";

		gl_pSinaRTDataSource->SetWebError(false);
	}

	TEST_F(CNeteaseDayLineDataSourceTest, TestGenerateInquiryMessage2) {
		//gl_pSinaRTDataSource->SetWebError(false);
		gl_pChinaMarket->TEST_SetMarketTime(toLocalTime(120000)); // 空闲时间
		EXPECT_FALSE(gl_systemConfiguration.IsWebBusy());
		EXPECT_TRUE(gl_pChinaMarket->IsSystemReady());
		EXPECT_TRUE(gl_pChinaMarket->IsDummyTime());
		NeteaseDayLineDataSource.SetInquiring(false);

		EXPECT_TRUE(NeteaseDayLineDataSource.GenerateInquiryMessage(toLocalTime(120000)));
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
		//EXPECT_EQ(gl_dataContainerChinaStock.GetDayLineNeedUpdateNumber(), gl_dataContainerChinaStock.Size());
		auto size = gl_dataContainerChinaStock.GetDayLineNeedUpdateNumber();
		gl_pChinaMarket->SetSystemReady(true);

		NeteaseDayLineDataSource.CreateProduct();

		const auto pProduct = NeteaseDayLineDataSource.GetCurrentProduct();
		EXPECT_TRUE(pProduct != nullptr);
		EXPECT_STREQ(typeid(*pProduct).name(), "class CProductNeteaseDayLine");

		const string str = pProduct->GetInquiryFunction();
		EXPECT_EQ(str.substr(0, 54), "http://quotes.money.163.com/service/chddata.html?code=");
		EXPECT_EQ(str.substr(str.length() - 81, 81), "&fields=TCLOSE;HIGH;LOW;TOPEN;LCLOSE;CHG;TURNOVER;VOTURNOVER;VATURNOVER;TCAP;MCAP");
		gl_pChinaMarket->SetSystemReady(false);
		EXPECT_EQ(gl_dataContainerChinaStock.GetDayLineNeedUpdateNumber() + 1, size) << "已经有一个无需更新日线了";

		// 恢复原态
		for (int i = 0; i < gl_dataContainerChinaStock.Size(); i++) {
			gl_dataContainerChinaStock.GetStock(i)->SetUpdateDayLine(true);
		}
	}
}
