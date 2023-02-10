#include"pch.h"

#include"GeneralCheck.h"

#include"NeteaseDayLineDataSource.h"
#include"WorldMarket.h"
#include"ChinaMarket.h"
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

		void SetUp(void) override { }

		void TearDown(void) override {
			// clearUp
			GeneralCheck();
		}

	protected:
		CNeteaseDayLineDataSource NeteaseDayLineDataSource;
	};

	TEST_F(CNeteaseDayLineDataSourceTest, TestParseData) {
		const CWebDataPtr pData = make_shared<CWebData>();
		EXPECT_TRUE(NeteaseDayLineDataSource.ParseData(pData));
	}

	TEST_F(CNeteaseDayLineDataSourceTest, TestCreateProduct) {
		EXPECT_EQ(gl_pChinaMarket->GetDayLineNeedUpdateNumber(), gl_pChinaMarket->GetTotalStock());
		gl_pChinaMarket->SetSystemReady(true);

		NeteaseDayLineDataSource.CreateProduct();

		const auto pProduct = NeteaseDayLineDataSource.GetCurrentProduct();
		EXPECT_TRUE(pProduct != nullptr);
		const CString str = pProduct->GetInquiryFunction();
		EXPECT_STREQ(str.Left(54), _T("http://quotes.money.163.com/service/chddata.html?code="));
		EXPECT_STREQ(str.Right(81), _T("&fields=TCLOSE;HIGH;LOW;TOPEN;LCLOSE;CHG;TURNOVER;VOTURNOVER;VATURNOVER;TCAP;MCAP"));
		gl_pChinaMarket->SetSystemReady(false);
		EXPECT_EQ(gl_pChinaMarket->GetDayLineNeedUpdateNumber() + 1, gl_pChinaMarket->GetTotalStock()) << "�Ѿ���һ���������������";

		// �ָ�ԭ̬
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
		EXPECT_FALSE(pWebData->IsParsed()) << "�������߲���json��ʽ���������";
		EXPECT_STREQ(pWebData->GetStockCode(), _T("TEST")) << "������StockCode";
		EXPECT_EQ(pWebData->GetTime(), 0) << "����Ϊ��ǰ��UTCTime";
		EXPECT_TRUE(pWebData->GetDataBuffer() == _T("{ \"data\": 2}"));

		// restore
		TestSetUTCTime(tUTCTime);
	}
}
