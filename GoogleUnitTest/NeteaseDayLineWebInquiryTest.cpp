#include"pch.h"

#include"ChinaMarket.h"
#include"GeneralCheck.h"

#include"NeteaseDayLineDataSource.h"
#include"MockNeteaseDayLineWebInquiry.h"


using namespace testing;

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

namespace StockAnalysisTest {
	class CNeteaseDayLineWebInquiryTest : public ::testing::Test
	{
	protected:
		static void SetUpTestSuite(void) {
			GeneralCheck();
			EXPECT_EQ(gl_pChinaMarket->GetDayLineNeedUpdateNumber(), gl_pChinaMarket->GetTotalStock());
		}

		static void TearDownTestSuite(void) {
			EXPECT_EQ(gl_pChinaMarket->GetDayLineNeedUpdateNumber(), gl_pChinaMarket->GetTotalStock());
			GeneralCheck();
		}

		virtual void SetUp(void) override {
			ASSERT_FALSE(gl_systemStatus.IsWorkingMode());

			m_NeteaseDayLineWebInquiry.SetDataSource(gl_pNeteaseDayLineDataSource.get());
			m_MockNeteaseDayLineWebInquiry.SetDataSource(gl_pNeteaseDayLineDataSource.get());
			gl_pChinaMarket->CalculateTime();
			m_MockNeteaseDayLineWebInquiry.ResetDownLoadingStockCode();
			EXPECT_TRUE(gl_pChinaMarket->IsResetMarket());
			EXPECT_EQ(gl_pChinaMarket->GetDayLineNeedUpdateNumber(), gl_pChinaMarket->GetTotalStock());
		}

		virtual void TearDown(void) override {
			// clearUp
			GeneralCheck();

			EXPECT_EQ(gl_pChinaMarket->GetDayLineNeedUpdateNumber(), gl_pChinaMarket->GetTotalStock());
			EXPECT_TRUE(gl_pChinaMarket->IsResetMarket());
			gl_pChinaMarket->SetResetMarket(true);
			gl_pChinaMarket->SetSystemReady(false);
			gl_pChinaMarket->SetCurrentStockChanged(false);
			m_MockNeteaseDayLineWebInquiry.ResetDownLoadingStockCode();
			for (int i = 0; i < gl_pChinaMarket->GetTotalStock(); i++) {
				gl_pChinaMarket->GetStock(i)->SetDayLineNeedUpdate(true);
			}
			EXPECT_EQ(gl_pChinaMarket->GetDayLineNeedUpdateNumber(), gl_pChinaMarket->GetTotalStock());
		}
		CMockNeteaseDayLineWebInquiry m_MockNeteaseDayLineWebInquiry; // 网易日线历史数据
		CNeteaseDayLineWebInquiry m_NeteaseDayLineWebInquiry;
	};

	TEST_F(CNeteaseDayLineWebInquiryTest, TestInitialize) {
		EXPECT_STREQ(m_MockNeteaseDayLineWebInquiry.GetInquiryFunction(), _T("http://quotes.money.163.com/service/chddata.html?code="));
		EXPECT_STREQ(m_MockNeteaseDayLineWebInquiry.GetInquiryToken(), _T("&fields=TCLOSE;HIGH;LOW;TOPEN;LCLOSE;CHG;TURNOVER;VOTURNOVER;VATURNOVER;TCAP;MCAP"));
		EXPECT_STREQ(m_MockNeteaseDayLineWebInquiry.GetConnectionName(), _T("NeteaseDayLine"));
	}

	TEST_F(CNeteaseDayLineWebInquiryTest, TestGetWebData) {
		EXPECT_EQ(gl_pChinaMarket->GetDayLineNeedUpdateNumber(), gl_pChinaMarket->GetTotalStock());
		m_MockNeteaseDayLineWebInquiry.SetReadingWebData(true);
		EXPECT_FALSE(m_MockNeteaseDayLineWebInquiry.GetWebData());
		m_MockNeteaseDayLineWebInquiry.SetReadingWebData(false);
		gl_pChinaMarket->SetSystemReady(true);
		EXPECT_CALL(m_MockNeteaseDayLineWebInquiry, PrepareNextInquiringString())
			.Times(1)
			.WillOnce(Return(true))
			.RetiresOnSaturation();
		EXPECT_CALL(m_MockNeteaseDayLineWebInquiry, StartReadingThread)
			.Times(1);
		m_MockNeteaseDayLineWebInquiry.GetWebData();
		EXPECT_TRUE(m_MockNeteaseDayLineWebInquiry.IsReadingWebData()) << _T("此标志由工作线程负责重置。此处调用的是Mock类，故而此标识没有重置");
		EXPECT_EQ(gl_pChinaMarket->GetDayLineNeedUpdateNumber(), gl_pChinaMarket->GetTotalStock());

		gl_pChinaMarket->GetStock(0)->SetDayLineNeedUpdate(true);
	}

	TEST_F(CNeteaseDayLineWebInquiryTest, TestReportStatus) {
		EXPECT_TRUE(m_MockNeteaseDayLineWebInquiry.ReportStatus(1));
	}

	TEST_F(CNeteaseDayLineWebInquiryTest, TestSetDownLoadingStockCode) {
		EXPECT_STREQ(m_MockNeteaseDayLineWebInquiry.GetDownLoadingStockCode(), _T(""));
		m_MockNeteaseDayLineWebInquiry.SetDownLoadingStockCode(_T("1000001"));
		EXPECT_STREQ(m_MockNeteaseDayLineWebInquiry.GetDownLoadingStockCode(), _T("1000001"));
		m_MockNeteaseDayLineWebInquiry.SetDownLoadingStockCode(_T("0600001"));
		EXPECT_STREQ(m_MockNeteaseDayLineWebInquiry.GetDownLoadingStockCode(), _T("0600001"));
		m_MockNeteaseDayLineWebInquiry.SetDownLoadingStockCode(_T("2600001"));
		EXPECT_STREQ(m_MockNeteaseDayLineWebInquiry.GetDownLoadingStockCode(), _T("2600001"));
	}

	TEST_F(CNeteaseDayLineWebInquiryTest, TestPrepareNextInquiringStr) {
		EXPECT_EQ(gl_pChinaMarket->GetDayLineNeedUpdateNumber(), gl_pChinaMarket->GetTotalStock());
		CString str;
		gl_pChinaMarket->SetSystemReady(true);
		for (int i = 0; i < 4; i++) {
			if (m_NeteaseDayLineWebInquiry.PrepareNextInquiringString()) {
				str = m_NeteaseDayLineWebInquiry.GetInquiringString();
				EXPECT_STREQ(str.Left(54), _T("http://quotes.money.163.com/service/chddata.html?code="));
			}
			else EXPECT_EQ(str.GetLength(), 0);
		}
		gl_pChinaMarket->SetSystemReady(false);
		EXPECT_LT(gl_pChinaMarket->GetDayLineNeedUpdateNumber(), gl_pChinaMarket->GetTotalStock());
		// 目前将索引移入函数内，作为静态变量存在，故而无法知道确切位置了。
		//EXPECT_FALSE(gl_pChinaMarket->GetStock(0)->IsDayLineNeedUpdate());

		// 恢复原态
		for (int i = 0; i < gl_pChinaMarket->GetTotalStock(); i++) {
			gl_pChinaMarket->GetStock(i)->SetDayLineNeedUpdate(true);
		}
	}
}