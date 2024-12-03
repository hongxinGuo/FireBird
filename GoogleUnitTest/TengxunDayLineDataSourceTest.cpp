#include"pch.h"

#include"GeneralCheck.h"

#include"TengxunDayLineDataSource.h"
//#include"WorldMarket.h"
#include"ChinaMarket.h"
#include "ProductTengxunDayLine.h"

using namespace testing;

namespace FireBirdTest {
	class CTengxunDayLineDataSourceTest : public Test {
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
			EXPECT_FALSE(TengxunDayLineDataSource.HaveInquiry());
			SCOPED_TRACE("");
			GeneralCheck();
		}

	protected:
		CTengxunDayLineDataSource TengxunDayLineDataSource;
	};

	TEST_F(CTengxunDayLineDataSourceTest, TestInitialize) {
		EXPECT_STREQ(TengxunDayLineDataSource.GetInquiryFunction(), _T("https://web.ifzq.gtimg.cn/appstock/app/fqkline/get?param="));
		EXPECT_STREQ(TengxunDayLineDataSource.GetInquirySuffix(), _T(",2000,,"));
		EXPECT_STREQ(TengxunDayLineDataSource.GetInquiryToken(), _T(""));
	}

	TEST_F(CTengxunDayLineDataSourceTest, TestIsUpdateDayLine) {
		EXPECT_TRUE(TengxunDayLineDataSource.IsUpdateDayLine());
		TengxunDayLineDataSource.SetUpdateDayLine(false);
		EXPECT_FALSE(TengxunDayLineDataSource.IsUpdateDayLine());
		TengxunDayLineDataSource.SetUpdateDayLine(true);
		EXPECT_TRUE(TengxunDayLineDataSource.IsUpdateDayLine());
	}

	TEST_F(CTengxunDayLineDataSourceTest, TestSetDownLoadingStockCode) {
		EXPECT_STREQ(TengxunDayLineDataSource.GetDownLoadingStockCode(), _T(""));
		TengxunDayLineDataSource.SetDownLoadingStockCode(_T("000001.SS"));
		EXPECT_STREQ(TengxunDayLineDataSource.GetDownLoadingStockCode(), _T("000001.SS"));
		TengxunDayLineDataSource.SetDownLoadingStockCode(_T("0600001"));
		EXPECT_STREQ(TengxunDayLineDataSource.GetDownLoadingStockCode(), _T("0600001"));
		TengxunDayLineDataSource.SetDownLoadingStockCode(_T("2600001"));
		EXPECT_STREQ(TengxunDayLineDataSource.GetDownLoadingStockCode(), _T("2600001"));
	}

	TEST_F(CTengxunDayLineDataSourceTest, TestInquireDayLine1) {
		TengxunDayLineDataSource.SetInquiring(true);

		EXPECT_FALSE(TengxunDayLineDataSource.Inquire());
	}

	TEST_F(CTengxunDayLineDataSourceTest, TestInquireDayLine2) {
		TengxunDayLineDataSource.SetUpdateDayLine(false);

		EXPECT_FALSE(TengxunDayLineDataSource.Inquire());
	}

	TEST_F(CTengxunDayLineDataSourceTest, TestInquireDayLine3) {
		EXPECT_FALSE(TengxunDayLineDataSource.HaveInquiry());
		TengxunDayLineDataSource.SetInquiring(false);
		TengxunDayLineDataSource.SetUpdateDayLine(true);
		for (long l = 0; l < gl_dataContainerChinaStock.Size(); l++) {
			gl_dataContainerChinaStock.GetStock(l)->SetUpdateDayLine(false);
		}
		gl_dataContainerChinaStock.GetStock(0)->SetUpdateDayLine(true);
		gl_dataContainerChinaStock.GetStock(10)->SetUpdateDayLine(true);

		EXPECT_FALSE(TengxunDayLineDataSource.IsInquiring());
		EXPECT_TRUE(TengxunDayLineDataSource.Inquire()) << GetUTCTime();
		EXPECT_GT(TengxunDayLineDataSource.InquiryQueueSize(), 0);
		EXPECT_TRUE(TengxunDayLineDataSource.IsInquiring());
		EXPECT_TRUE(TengxunDayLineDataSource.HaveInquiry());
		EXPECT_STREQ(TengxunDayLineDataSource.GetDownLoadingStockCode(), _T("000001.SS"));
		TengxunDayLineDataSource.SetInquiring(false);
		while (TengxunDayLineDataSource.InquiryQueueSize() > 0) TengxunDayLineDataSource.GetCurrentProduct();
		EXPECT_FALSE(TengxunDayLineDataSource.HaveInquiry());

		EXPECT_TRUE(TengxunDayLineDataSource.Inquire());
		EXPECT_TRUE(TengxunDayLineDataSource.IsInquiring());
		EXPECT_TRUE(TengxunDayLineDataSource.HaveInquiry());
		EXPECT_STREQ(TengxunDayLineDataSource.GetDownLoadingStockCode(), _T("000006.SS"));
		TengxunDayLineDataSource.SetInquiring(false);
		while (TengxunDayLineDataSource.InquiryQueueSize() > 0) TengxunDayLineDataSource.GetCurrentProduct();
		EXPECT_FALSE(TengxunDayLineDataSource.HaveInquiry());

		EXPECT_FALSE(TengxunDayLineDataSource.Inquire()) << "查询完了";
		EXPECT_EQ(gl_systemMessage.InformationSize(), 1);
		EXPECT_STREQ(gl_systemMessage.PopInformationMessage(), _T("中国市场股票日线历史数据更新完毕"));
		EXPECT_FALSE(TengxunDayLineDataSource.HaveInquiry());

		// 恢复原状
		for (long l = 0; l < gl_dataContainerChinaStock.Size(); l++) {
			gl_dataContainerChinaStock.GetStock(l)->SetUpdateDayLine(true);
		}
	}

	TEST_F(CTengxunDayLineDataSourceTest, TestCreateProduct1) {
		gl_pChinaMarket->TEST_SetFormattedMarketDate(20230201);
		const auto pStock = gl_dataContainerChinaStock.GetStock(_T("600008.SS"));
		const long lEndDate = pStock->GetDayLineEndDate();
		pStock->SetDayLineEndDate(20200101); // 日期间隔小于八年

		const auto vProduct = TengxunDayLineDataSource.CreateProduct(pStock);

		EXPECT_EQ(vProduct.size(), 1) << "日期间隔小于八年，只有一个申请";
		const auto pProduct = vProduct.at(0);
		EXPECT_STREQ(typeid(*pProduct).name(), _T("class CProductTengxunDayLine"));
		EXPECT_EQ(pProduct->GetIndex(), gl_dataContainerChinaStock.GetOffset(pStock));
		EXPECT_STREQ(pProduct->GetInquiryFunction(), _T("https://web.ifzq.gtimg.cn/appstock/app/fqkline/get?param=sh600008,day,2019-12-31,2023-02-01,2000,,")) << "起始日期为日线结束日期的前一天";
		EXPECT_EQ(dynamic_pointer_cast<CProductTengxunDayLine>(pProduct)->GetInquiryNumber(), 1);

		// 恢复原状
		pStock->SetDayLineEndDate(lEndDate);
	}

	TEST_F(CTengxunDayLineDataSourceTest, TestCreateProduct2) {
		gl_pChinaMarket->TEST_SetFormattedMarketDate(20230201);
		const auto pStock = gl_dataContainerChinaStock.GetStock(_T("600008.SS"));
		const long lEndDate = pStock->GetDayLineEndDate();
		pStock->SetDayLineEndDate(20100101); // 日期间隔大于八年小于十六年

		const auto vProduct = TengxunDayLineDataSource.CreateProduct(pStock);

		EXPECT_EQ(vProduct.size(), 2) << "日期间隔大于八年小于十六年，有两个申请";
		const auto pProduct1 = vProduct.at(0);
		const auto pProduct2 = vProduct.at(1);
		EXPECT_STREQ(typeid(*pProduct1).name(), _T("class CProductTengxunDayLine"));
		EXPECT_EQ(pProduct1->GetIndex(), gl_dataContainerChinaStock.GetOffset(pStock));
		EXPECT_STREQ(pProduct1->GetInquiryFunction(), _T("https://web.ifzq.gtimg.cn/appstock/app/fqkline/get?param=sh600008,day,2009-12-31,2015-12-31,2000,,")) << "起始日期为日线结束日期的前一天";
		EXPECT_EQ(dynamic_pointer_cast<CProductTengxunDayLine>(pProduct1)->GetInquiryNumber(), 2);

		EXPECT_STREQ(typeid(*pProduct2).name(), _T("class CProductTengxunDayLine"));
		EXPECT_EQ(pProduct2->GetIndex(), gl_dataContainerChinaStock.GetOffset(pStock));
		EXPECT_STREQ(pProduct2->GetInquiryFunction(), _T("https://web.ifzq.gtimg.cn/appstock/app/fqkline/get?param=sh600008,day,2016-01-01,2023-02-01,2000,,"));
		EXPECT_EQ(dynamic_pointer_cast<CProductTengxunDayLine>(pProduct1)->GetInquiryNumber(), 2);

		// 恢复原状
		pStock->SetDayLineEndDate(lEndDate);
	}

	TEST_F(CTengxunDayLineDataSourceTest, TestCreateProduct3) {
		gl_pChinaMarket->TEST_SetFormattedMarketDate(20230201);
		const auto pStock = gl_dataContainerChinaStock.GetStock(_T("600008.SS"));
		const long lEndDate = pStock->GetDayLineEndDate();
		pStock->SetDayLineEndDate(20000101); // 日期间隔大于十六年

		const auto vProduct = TengxunDayLineDataSource.CreateProduct(pStock);

		EXPECT_EQ(vProduct.size(), 4) << "日期间隔大于十六年，有四个申请";
		const auto pProduct1 = vProduct.at(0);
		const auto pProduct2 = vProduct.at(1);
		const auto pProduct3 = vProduct.at(2);
		const auto pProduct4 = vProduct.at(3);
		EXPECT_STREQ(typeid(*pProduct1).name(), _T("class CProductTengxunDayLine"));
		EXPECT_EQ(pProduct1->GetIndex(), gl_dataContainerChinaStock.GetOffset(pStock));
		EXPECT_EQ(dynamic_pointer_cast<CProductTengxunDayLine>(pProduct1)->GetInquiryNumber(), 4);
		EXPECT_STREQ(pProduct1->GetInquiryFunction(), _T("https://web.ifzq.gtimg.cn/appstock/app/fqkline/get?param=sh600008,day,1999-12-31,2005-12-31,2000,,")) << "起始日期为日线结束日期的前一天";
		EXPECT_STREQ(pProduct2->GetInquiryFunction(), _T("https://web.ifzq.gtimg.cn/appstock/app/fqkline/get?param=sh600008,day,2006-01-01,2012-12-31,2000,,")) << "起始日期为日线结束日期的前一天";
		EXPECT_STREQ(pProduct3->GetInquiryFunction(), _T("https://web.ifzq.gtimg.cn/appstock/app/fqkline/get?param=sh600008,day,2013-01-01,2019-12-31,2000,,")) << "起始日期为日线结束日期的前一天";
		EXPECT_STREQ(pProduct4->GetInquiryFunction(), _T("https://web.ifzq.gtimg.cn/appstock/app/fqkline/get?param=sh600008,day,2020-01-01,2023-02-01,2000,,")) << "起始日期为日线结束日期的前一天";

		// 恢复原状
		pStock->SetDayLineEndDate(lEndDate);
	}
}
