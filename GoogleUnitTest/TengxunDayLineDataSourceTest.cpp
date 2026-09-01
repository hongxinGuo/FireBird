#include"pch.h"

#include <gtest/gtest.h>
#include"GeneralCheck.h"

#include"TengxunDayLineDataSource.h"
#include"TimeConvert.h"

#include"ChinaMarket.h"
#include "ContainerChinaStock.h"
#include "ProductTengxunDayLine.h"
#include"ChinaStock.h"

using namespace testing;
using namespace std;

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
			marketDateTime = gl_pChinaMarket->GetMarketClock();
		}

		void TearDown() override {
			// clearUp
			gl_pChinaMarket->TEST_SetMarketDate(s_currentChinaMarketDate);

			EXPECT_FALSE(TengxunDayLineDataSource.HaveInquiry());
			SCOPED_TRACE("");
			gl_pChinaMarket->TEST_SetMarketTime(marketDateTime);
			GeneralCheck();
		}

	protected:
		chrono::local_seconds marketDateTime{ toLocalTime(0) };
		CTengxunDayLineDataSource TengxunDayLineDataSource;
	};

	TEST_F(CTengxunDayLineDataSourceTest, TestInitialize) {
		EXPECT_EQ(TengxunDayLineDataSource.GetInquiryFunction(), "https://web.ifzq.gtimg.cn/appstock/app/fqkline/get?param=");
		EXPECT_EQ(TengxunDayLineDataSource.GetInquirySuffix(), ",2000,,");
		EXPECT_EQ(TengxunDayLineDataSource.GetToken(), "");
	}

	TEST_F(CTengxunDayLineDataSourceTest, TestIsUpdateDayLine) {
		EXPECT_TRUE(TengxunDayLineDataSource.IsUpdateDayLine());
		TengxunDayLineDataSource.SetUpdateDayLine(false);
		EXPECT_FALSE(TengxunDayLineDataSource.IsUpdateDayLine());
		TengxunDayLineDataSource.SetUpdateDayLine(true);
		EXPECT_TRUE(TengxunDayLineDataSource.IsUpdateDayLine());
	}

	TEST_F(CTengxunDayLineDataSourceTest, TestSetDownLoadingStockCode) {
		EXPECT_TRUE(TengxunDayLineDataSource.GetDownLoadingStockCode().empty());
		TengxunDayLineDataSource.SetDownLoadingStockCode("000001.SS");
		EXPECT_TRUE(TengxunDayLineDataSource.GetDownLoadingStockCode() == "000001.SS");
		TengxunDayLineDataSource.SetDownLoadingStockCode("0600001");
		EXPECT_TRUE(TengxunDayLineDataSource.GetDownLoadingStockCode() == "0600001");
		TengxunDayLineDataSource.SetDownLoadingStockCode("2600001");
		EXPECT_TRUE(TengxunDayLineDataSource.GetDownLoadingStockCode() == "2600001");
	}

	TEST_F(CTengxunDayLineDataSourceTest, TestInquireDayLine1) {
		TengxunDayLineDataSource.SetInquiring(true);

		EXPECT_FALSE(TengxunDayLineDataSource.GenerateInquireDayLine());
	}

	TEST_F(CTengxunDayLineDataSourceTest, TestInquireDayLine2) {
		TengxunDayLineDataSource.SetUpdateDayLine(false);

		EXPECT_FALSE(TengxunDayLineDataSource.GenerateInquireDayLine());
	}

	TEST_F(CTengxunDayLineDataSourceTest, TestCreateProduct1) {
		gl_pChinaMarket->TEST_SetMarketDate(toLocalDays(20230201));
		const auto pStock = gl_dataContainerChinaStock.GetStock("600008.SS");
		const chrono::local_days lEndDate = pStock->GetDayLineEndDate();
		pStock->SetDayLineEndDate(toLocalDays(20200101)); // 日期间隔小于八年

		const auto vProduct = TengxunDayLineDataSource.CreateProduct(pStock);

		EXPECT_EQ(vProduct.size(), 1) << "日期间隔小于八年，只有一个申请";
		const auto& pProduct = vProduct.at(0);
		EXPECT_STREQ(typeid(*pProduct).name(), "class CProductTengxunDayLine");
		EXPECT_EQ(pProduct->GetIndex(), gl_dataContainerChinaStock.GetOffset(pStock));
		EXPECT_EQ(pProduct->GetInquiryFunction(), "https://web.ifzq.gtimg.cn/appstock/app/fqkline/get?param=sh600008,day,2019-12-31,2023-02-01,2000,,") << "起始日期为日线结束日期的前一天";
		EXPECT_EQ(dynamic_pointer_cast<CProductTengxunDayLine>(pProduct)->GetInquiryNumber(), 1);

		// 恢复原状
		pStock->SetDayLineEndDate(lEndDate);
	}

	TEST_F(CTengxunDayLineDataSourceTest, TestCreateProduct2) {
		gl_pChinaMarket->TEST_SetMarketDate(toLocalDays(20230201));
		const auto pStock = gl_dataContainerChinaStock.GetStock("600008.SS");
		const chrono::local_days lEndDate = pStock->GetDayLineEndDate();
		pStock->SetDayLineEndDate(toLocalDays(20110101)); // 日期间隔大于七年小于十四年

		const auto vProduct = TengxunDayLineDataSource.CreateProduct(pStock);

		EXPECT_EQ(vProduct.size(), 2) << "日期间隔大于七年小于十四年，有两个申请";
		const auto& pProduct1 = vProduct.at(0);
		const auto& pProduct2 = vProduct.at(1);
		EXPECT_STREQ(typeid(*pProduct1).name(), "class CProductTengxunDayLine");
		EXPECT_EQ(pProduct1->GetIndex(), gl_dataContainerChinaStock.GetOffset(pStock));
		EXPECT_EQ(pProduct1->GetInquiryFunction(), "https://web.ifzq.gtimg.cn/appstock/app/fqkline/get?param=sh600008,day,2010-12-31,2016-12-31,2000,,") << "起始日期为日线结束日期的前一天";
		EXPECT_EQ(dynamic_pointer_cast<CProductTengxunDayLine>(pProduct1)->GetInquiryNumber(), 2);

		EXPECT_STREQ(typeid(*pProduct2).name(), "class CProductTengxunDayLine");
		EXPECT_EQ(pProduct2->GetIndex(), gl_dataContainerChinaStock.GetOffset(pStock));
		EXPECT_EQ(pProduct2->GetInquiryFunction(), "https://web.ifzq.gtimg.cn/appstock/app/fqkline/get?param=sh600008,day,2017-01-01,2023-02-01,2000,,");
		EXPECT_EQ(dynamic_pointer_cast<CProductTengxunDayLine>(pProduct1)->GetInquiryNumber(), 2);

		// 恢复原状
		pStock->SetDayLineEndDate(lEndDate);
	}

	TEST_F(CTengxunDayLineDataSourceTest, TestCreateProduct3) {
		gl_pChinaMarket->TEST_SetMarketDate(toLocalDays(20230201));
		const auto pStock = gl_dataContainerChinaStock.GetStock("600008.SS");
		const chrono::local_days lEndDate = pStock->GetDayLineEndDate();
		pStock->SetDayLineEndDate(toLocalDays(20000101)); // 日期间隔大于十六年

		const auto vProduct = TengxunDayLineDataSource.CreateProduct(pStock);

		EXPECT_EQ(vProduct.size(), 4) << "日期间隔大于十六年，有四个申请";
		const auto& pProduct1 = vProduct.at(0);
		const auto& pProduct2 = vProduct.at(1);
		const auto& pProduct3 = vProduct.at(2);
		const auto& pProduct4 = vProduct.at(3);
		EXPECT_STREQ(typeid(*pProduct1).name(), "class CProductTengxunDayLine");
		EXPECT_EQ(pProduct1->GetIndex(), gl_dataContainerChinaStock.GetOffset(pStock));
		EXPECT_EQ(dynamic_pointer_cast<CProductTengxunDayLine>(pProduct1)->GetInquiryNumber(), 4);
		EXPECT_EQ(pProduct1->GetInquiryFunction(), "https://web.ifzq.gtimg.cn/appstock/app/fqkline/get?param=sh600008,day,1999-12-31,2005-12-31,2000,,") << "起始日期为日线结束日期的前一天";
		EXPECT_EQ(pProduct2->GetInquiryFunction(), "https://web.ifzq.gtimg.cn/appstock/app/fqkline/get?param=sh600008,day,2006-01-01,2012-12-31,2000,,") << "起始日期为日线结束日期的前一天";
		EXPECT_EQ(pProduct3->GetInquiryFunction(), "https://web.ifzq.gtimg.cn/appstock/app/fqkline/get?param=sh600008,day,2013-01-01,2019-12-31,2000,,") << "起始日期为日线结束日期的前一天";
		EXPECT_EQ(pProduct4->GetInquiryFunction(), "https://web.ifzq.gtimg.cn/appstock/app/fqkline/get?param=sh600008,day,2020-01-01,2023-02-01,2000,,") << "起始日期为日线结束日期的前一天";

		// 恢复原状
		pStock->SetDayLineEndDate(lEndDate);
	}
}
