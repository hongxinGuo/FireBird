#include"pch.h"

#include"ChinaMarket.h"
#include"GeneralCheck.h"

#include"MockTengxunDayLineDataSource.h"
#include "ProductTengxunDayLine.h"

using namespace testing;

namespace FireBirdTest {
	CMockTengxunDayLineDataSourcePtr m_pMockTengxunDayLineDataSource; // 网易日线历史数据

	class CMockTengxunDayLineDataSourceTest : public ::testing::Test {
	protected:
		static void SetUpTestSuite(void) {
			GeneralCheck();
			EXPECT_EQ(gl_pChinaMarket->GetDayLineNeedUpdateNumber(), gl_pChinaMarket->GetTotalStock());
		}

		static void TearDownTestSuite(void) {
			EXPECT_EQ(gl_pChinaMarket->GetDayLineNeedUpdateNumber(), gl_pChinaMarket->GetTotalStock());
			GeneralCheck();
		}

		void SetUp(void) override {
			ASSERT_FALSE(gl_systemStatus.IsWorkingMode());
			m_pMockTengxunDayLineDataSource = make_shared<CMockTengxunDayLineDataSource>();
			gl_pChinaMarket->CalculateTime();
			m_pMockTengxunDayLineDataSource->ResetDownLoadingStockCode();
			EXPECT_TRUE(gl_pChinaMarket->IsResetMarket());
			EXPECT_EQ(gl_pChinaMarket->GetDayLineNeedUpdateNumber(), gl_pChinaMarket->GetTotalStock());
		}

		void TearDown(void) override {
			// clearUp
			m_pMockTengxunDayLineDataSource = nullptr;
			GeneralCheck();

			EXPECT_EQ(gl_pChinaMarket->GetDayLineNeedUpdateNumber(), gl_pChinaMarket->GetTotalStock());
			EXPECT_TRUE(gl_pChinaMarket->IsResetMarket());
			gl_pChinaMarket->SetResetMarket(true);
			gl_pChinaMarket->SetSystemReady(false);
			gl_pChinaMarket->SetCurrentStockChanged(false);
			for (int i = 0; i < gl_pChinaMarket->GetTotalStock(); i++) {
				gl_pChinaMarket->GetStock(i)->SetDayLineNeedUpdate(true);
			}
			EXPECT_EQ(gl_pChinaMarket->GetDayLineNeedUpdateNumber(), gl_pChinaMarket->GetTotalStock());
		}

	protected:
		CTengxunDayLineDataSource TengxunDayLineDataSource;
	};

	TEST_F(CMockTengxunDayLineDataSourceTest, TestInitialize) {
		EXPECT_STREQ(TengxunDayLineDataSource.GetInquiryFunction(), _T(""));
		EXPECT_STREQ(TengxunDayLineDataSource.GetInquirySuffix(), _T(",2000,,"));
		EXPECT_STREQ(TengxunDayLineDataSource.GetInquiryToken(), _T(""));
	}

	TEST_F(CMockTengxunDayLineDataSourceTest, TestIsUpdateDayLine) {
		EXPECT_TRUE(TengxunDayLineDataSource.IsUpdateDayLine());
		TengxunDayLineDataSource.SetUpdateDayLine(false);
		EXPECT_FALSE(TengxunDayLineDataSource.IsUpdateDayLine());
		TengxunDayLineDataSource.SetUpdateDayLine(true);
		EXPECT_TRUE(TengxunDayLineDataSource.IsUpdateDayLine());
	}

	TEST_F(CMockTengxunDayLineDataSourceTest, TestParseData) {
		const auto pData = make_shared<CWebData>();
		EXPECT_FALSE(TengxunDayLineDataSource.ParseData(pData));
	}

	TEST_F(CMockTengxunDayLineDataSourceTest, TestUpdateStatus) {
		EXPECT_TRUE(TengxunDayLineDataSource.UpdateStatus());
	}

	TEST_F(CMockTengxunDayLineDataSourceTest, TestUpdateStatusAfterSucceed) {
		const auto pData = make_shared<CWebData>();
		TengxunDayLineDataSource.SetDownLoadingStockCode(_T("Test"));
		TengxunDayLineDataSource.UpdateStatusAfterSucceed(pData);

		EXPECT_STREQ(pData->GetStockCode(), _T("Test"));
	}

	TEST_F(CMockTengxunDayLineDataSourceTest, TestGetWebData) {
		EXPECT_EQ(gl_pChinaMarket->GetDayLineNeedUpdateNumber(), gl_pChinaMarket->GetTotalStock());
		m_pMockTengxunDayLineDataSource->SetInquiringWebData(false);
		gl_pChinaMarket->SetSystemReady(true);
		EXPECT_CALL(*m_pMockTengxunDayLineDataSource, StartReadingThread)
		.Times(1);
		m_pMockTengxunDayLineDataSource->ProcessInquiryMessage();
		EXPECT_TRUE(m_pMockTengxunDayLineDataSource->IsInquiringWebData()) << _T("此标志由工作线程负责重置。此处调用的是Mock类，故而此标识没有重置");
		EXPECT_EQ(gl_pChinaMarket->GetDayLineNeedUpdateNumber(), gl_pChinaMarket->GetTotalStock());

		gl_pChinaMarket->GetStock(0)->SetDayLineNeedUpdate(true);
	}

	TEST_F(CMockTengxunDayLineDataSourceTest, TestSetDownLoadingStockCode) {
		EXPECT_STREQ(TengxunDayLineDataSource.GetDownLoadingStockCode(), _T(""));
		TengxunDayLineDataSource.SetDownLoadingStockCode(_T("000001.SS"));
		EXPECT_STREQ(TengxunDayLineDataSource.GetDownLoadingStockCode(), _T("000001.SS"));
		TengxunDayLineDataSource.SetDownLoadingStockCode(_T("0600001"));
		EXPECT_STREQ(TengxunDayLineDataSource.GetDownLoadingStockCode(), _T("0600001"));
		TengxunDayLineDataSource.SetDownLoadingStockCode(_T("2600001"));
		EXPECT_STREQ(TengxunDayLineDataSource.GetDownLoadingStockCode(), _T("2600001"));
	}

	TEST_F(CMockTengxunDayLineDataSourceTest, TestCreateProduct1) {
		gl_pChinaMarket->TEST_SetFormattedMarketDate(20230201);
		const auto pStock = gl_pChinaMarket->GetStock(_T("600008.SS"));
		const long lEndDate = pStock->GetDayLineEndDate();
		pStock->SetDayLineEndDate(20200101); // 日期间隔小于八年

		const auto vProduct = TengxunDayLineDataSource.CreateProduct(pStock);

		EXPECT_EQ(vProduct.size(), 1) << "日期间隔小于八年，只有一个申请";
		const auto pProduct = vProduct.at(0);
		EXPECT_STREQ(typeid(*pProduct).name(), _T("class CProductTengxunDayLine"));
		EXPECT_EQ(pProduct->GetIndex(), gl_pChinaMarket->GetStockIndex(pStock));
		EXPECT_STREQ(pProduct->GetInquiry(), _T("https://web.ifzq.gtimg.cn/appstock/app/fqkline/get?param=sh600008,day,2019-12-31,2023-02-01,2000,,")) << "起始日期为日线结束日期的前一天";
		EXPECT_EQ(dynamic_pointer_cast<CProductTengxunDayLine>(pProduct)->GetInquiryNumber(), 1);

		// 恢复原状
		pStock->SetDayLineEndDate(lEndDate);
	}

	TEST_F(CMockTengxunDayLineDataSourceTest, TestCreateProduct2) {
		gl_pChinaMarket->TEST_SetFormattedMarketDate(20230201);
		const auto pStock = gl_pChinaMarket->GetStock(_T("600008.SS"));
		const long lEndDate = pStock->GetDayLineEndDate();
		pStock->SetDayLineEndDate(20100101); // 日期间隔大于八年小于十六年

		const auto vProduct = TengxunDayLineDataSource.CreateProduct(pStock);

		EXPECT_EQ(vProduct.size(), 2) << "日期间隔大于八年小于十六年，有两个申请";
		const auto pProduct1 = vProduct.at(0);
		const auto pProduct2 = vProduct.at(1);
		EXPECT_STREQ(typeid(*pProduct1).name(), _T("class CProductTengxunDayLine"));
		EXPECT_EQ(pProduct1->GetIndex(), gl_pChinaMarket->GetStockIndex(pStock));
		EXPECT_STREQ(pProduct1->GetInquiry(), _T("https://web.ifzq.gtimg.cn/appstock/app/fqkline/get?param=sh600008,day,2009-12-31,2015-12-31,2000,,")) << "起始日期为日线结束日期的前一天";
		EXPECT_EQ(dynamic_pointer_cast<CProductTengxunDayLine>(pProduct1)->GetInquiryNumber(), 2);

		EXPECT_STREQ(typeid(*pProduct2).name(), _T("class CProductTengxunDayLine"));
		EXPECT_EQ(pProduct2->GetIndex(), gl_pChinaMarket->GetStockIndex(pStock));
		EXPECT_STREQ(pProduct2->GetInquiry(), _T("https://web.ifzq.gtimg.cn/appstock/app/fqkline/get?param=sh600008,day,2016-01-01,2023-02-01,2000,,"));
		EXPECT_EQ(dynamic_pointer_cast<CProductTengxunDayLine>(pProduct1)->GetInquiryNumber(), 2);

		// 恢复原状
		pStock->SetDayLineEndDate(lEndDate);
	}

	TEST_F(CMockTengxunDayLineDataSourceTest, TestCreateProduct3) {
		gl_pChinaMarket->TEST_SetFormattedMarketDate(20230201);
		const auto pStock = gl_pChinaMarket->GetStock(_T("600008.SS"));
		const long lEndDate = pStock->GetDayLineEndDate();
		pStock->SetDayLineEndDate(20000101); // 日期间隔大于十六年

		const auto vProduct = TengxunDayLineDataSource.CreateProduct(pStock);

		EXPECT_EQ(vProduct.size(), 4) << "日期间隔大于十六年，有四个申请";
		const auto pProduct1 = vProduct.at(0);
		const auto pProduct2 = vProduct.at(1);
		const auto pProduct3 = vProduct.at(2);
		const auto pProduct4 = vProduct.at(3);
		EXPECT_STREQ(typeid(*pProduct1).name(), _T("class CProductTengxunDayLine"));
		EXPECT_EQ(pProduct1->GetIndex(), gl_pChinaMarket->GetStockIndex(pStock));
		EXPECT_EQ(dynamic_pointer_cast<CProductTengxunDayLine>(pProduct1)->GetInquiryNumber(), 4);
		EXPECT_STREQ(pProduct1->GetInquiry(), _T("https://web.ifzq.gtimg.cn/appstock/app/fqkline/get?param=sh600008,day,1999-12-31,2005-12-31,2000,,")) << "起始日期为日线结束日期的前一天";
		EXPECT_STREQ(pProduct2->GetInquiry(), _T("https://web.ifzq.gtimg.cn/appstock/app/fqkline/get?param=sh600008,day,2006-01-01,2012-12-31,2000,,")) << "起始日期为日线结束日期的前一天";
		EXPECT_STREQ(pProduct3->GetInquiry(), _T("https://web.ifzq.gtimg.cn/appstock/app/fqkline/get?param=sh600008,day,2013-01-01,2019-12-31,2000,,")) << "起始日期为日线结束日期的前一天";
		EXPECT_STREQ(pProduct4->GetInquiry(), _T("https://web.ifzq.gtimg.cn/appstock/app/fqkline/get?param=sh600008,day,2020-01-01,2023-02-01,2000,,")) << "起始日期为日线结束日期的前一天";

		// 恢复原状
		pStock->SetDayLineEndDate(lEndDate);
	}
}
