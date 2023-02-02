#include"pch.h"

#include"ChinaMarket.h"
#include"GeneralCheck.h"

#include"MockTengxunDayLineDataSource.h"
#include "ProductTengxunDayLine.h"

using namespace testing;

namespace FireBirdTest {
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
			gl_pChinaMarket->CalculateTime();
			m_MockTengxunDayLineDataSource.ResetDownLoadingStockCode();
			EXPECT_TRUE(gl_pChinaMarket->IsResetMarket());
			EXPECT_EQ(gl_pChinaMarket->GetDayLineNeedUpdateNumber(), gl_pChinaMarket->GetTotalStock());
		}

		void TearDown(void) override {
			// clearUp
			GeneralCheck();

			EXPECT_EQ(gl_pChinaMarket->GetDayLineNeedUpdateNumber(), gl_pChinaMarket->GetTotalStock());
			EXPECT_TRUE(gl_pChinaMarket->IsResetMarket());
			gl_pChinaMarket->SetResetMarket(true);
			gl_pChinaMarket->SetSystemReady(false);
			gl_pChinaMarket->SetCurrentStockChanged(false);
			m_MockTengxunDayLineDataSource.ResetDownLoadingStockCode();
			for (int i = 0; i < gl_pChinaMarket->GetTotalStock(); i++) {
				gl_pChinaMarket->GetStock(i)->SetDayLineNeedUpdate(true);
			}
			EXPECT_EQ(gl_pChinaMarket->GetDayLineNeedUpdateNumber(), gl_pChinaMarket->GetTotalStock());
		}

		CMockTengxunDayLineDataSource m_MockTengxunDayLineDataSource; // 网易日线历史数据
	};

	TEST_F(CMockTengxunDayLineDataSourceTest, TestInitialize) {
		EXPECT_STREQ(m_MockTengxunDayLineDataSource.GetInquiryFunction(), _T("https://web.ifzq.gtimg.cn/appstock/app/fqkline/get?param="));
		EXPECT_STREQ(m_MockTengxunDayLineDataSource.GetInquirySuffix(), _T(",2000,,"));
		EXPECT_STREQ(m_MockTengxunDayLineDataSource.GetInquiryToken(), _T(""));
	}

	TEST_F(CMockTengxunDayLineDataSourceTest, TestIsUpdateDayLine) {
		EXPECT_TRUE(m_MockTengxunDayLineDataSource.IsUpdateDayLine());
		m_MockTengxunDayLineDataSource.SetUpdateDayLine(false);
		EXPECT_FALSE(m_MockTengxunDayLineDataSource.IsUpdateDayLine());
		m_MockTengxunDayLineDataSource.SetUpdateDayLine(true);
		EXPECT_TRUE(m_MockTengxunDayLineDataSource.IsUpdateDayLine());
	}

	TEST_F(CMockTengxunDayLineDataSourceTest, TestParseData) {
		const auto pData = make_shared<CWebData>();
		EXPECT_FALSE(m_MockTengxunDayLineDataSource.ParseData(pData));
	}

	TEST_F(CMockTengxunDayLineDataSourceTest, TestUpdateStatus) {
		EXPECT_TRUE(m_MockTengxunDayLineDataSource.UpdateStatus());
	}

	TEST_F(CMockTengxunDayLineDataSourceTest, TestUpdateStatusAfterSucceed) {
		const auto pData = make_shared<CWebData>();
		m_MockTengxunDayLineDataSource.SetDownLoadingStockCode(_T("Test"));
		m_MockTengxunDayLineDataSource.UpdateStatusAfterSucceed(pData);

		EXPECT_STREQ(pData->GetStockCode(), _T("Test"));
	}

	TEST_F(CMockTengxunDayLineDataSourceTest, TestGetWebData) {
		EXPECT_EQ(gl_pChinaMarket->GetDayLineNeedUpdateNumber(), gl_pChinaMarket->GetTotalStock());
		m_MockTengxunDayLineDataSource.SetInquiringWebData(true);
		EXPECT_FALSE(m_MockTengxunDayLineDataSource.GetWebData());
		m_MockTengxunDayLineDataSource.SetInquiringWebData(false);
		gl_pChinaMarket->SetSystemReady(true);
		EXPECT_CALL(m_MockTengxunDayLineDataSource, PrepareNextInquiringString())
			.Times(1)
			.WillOnce(Return(true))
			.RetiresOnSaturation();
		EXPECT_CALL(m_MockTengxunDayLineDataSource, StartReadingThread)
			.Times(1);
		m_MockTengxunDayLineDataSource.GetWebData();
		EXPECT_TRUE(m_MockTengxunDayLineDataSource.IsInquiringWebData()) << _T("此标志由工作线程负责重置。此处调用的是Mock类，故而此标识没有重置");
		EXPECT_EQ(gl_pChinaMarket->GetDayLineNeedUpdateNumber(), gl_pChinaMarket->GetTotalStock());

		gl_pChinaMarket->GetStock(0)->SetDayLineNeedUpdate(true);
	}

	TEST_F(CMockTengxunDayLineDataSourceTest, TestSetDownLoadingStockCode) {
		EXPECT_STREQ(m_MockTengxunDayLineDataSource.GetDownLoadingStockCode(), _T(""));
		m_MockTengxunDayLineDataSource.SetDownLoadingStockCode(_T("000001.SS"));
		EXPECT_STREQ(m_MockTengxunDayLineDataSource.GetDownLoadingStockCode(), _T("000001.SS"));
		m_MockTengxunDayLineDataSource.SetDownLoadingStockCode(_T("0600001"));
		EXPECT_STREQ(m_MockTengxunDayLineDataSource.GetDownLoadingStockCode(), _T("0600001"));
		m_MockTengxunDayLineDataSource.SetDownLoadingStockCode(_T("2600001"));
		EXPECT_STREQ(m_MockTengxunDayLineDataSource.GetDownLoadingStockCode(), _T("2600001"));
	}

	TEST_F(CMockTengxunDayLineDataSourceTest, TestPrepareNextInquiringStr) {
		EXPECT_EQ(gl_pChinaMarket->GetDayLineNeedUpdateNumber(), gl_pChinaMarket->GetTotalStock());
		CString str;
		gl_pChinaMarket->SetSystemReady(true);
		for (int i = 0; i < 4; i++) {
			if (gl_pTengxunDayLineDataSource->PrepareNextInquiringString()) {
				str = gl_pTengxunDayLineDataSource->GetInquiringString();
				EXPECT_STREQ(str.Left(57), _T("https://web.ifzq.gtimg.cn/appstock/app/fqkline/get?param="));
			}
			else {
				EXPECT_EQ(str.GetLength(), 0);
			}
		}
		gl_pChinaMarket->SetSystemReady(false);
		EXPECT_LE(gl_pChinaMarket->GetDayLineNeedUpdateNumber(), gl_pChinaMarket->GetTotalStock());
		// 目前将索引移入函数内，作为静态变量存在，故而无法知道确切位置了。
		//EXPECT_FALSE(gl_pChinaMarket->GetStock(0)->IsDayLineNeedUpdate());

		// 恢复原态
		for (int i = 0; i < gl_pChinaMarket->GetTotalStock(); i++) {
			gl_pChinaMarket->GetStock(i)->SetDayLineNeedUpdate(true);
		}
	}

	TEST_F(CMockTengxunDayLineDataSourceTest, TestCreateProduct1) {
		gl_pChinaMarket->TEST_SetFormattedMarketDate(20230201);
		const auto pStock = gl_pChinaMarket->GetStock(_T("600008.SS"));
		const long lEndDate = pStock->GetDayLineEndDate();
		pStock->SetDayLineEndDate(20200101); // 日期间隔小于八年

		const auto vProduct = m_MockTengxunDayLineDataSource.CreateProduct(pStock);

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

		const auto vProduct = m_MockTengxunDayLineDataSource.CreateProduct(pStock);

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

		const auto vProduct = m_MockTengxunDayLineDataSource.CreateProduct(pStock);

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
