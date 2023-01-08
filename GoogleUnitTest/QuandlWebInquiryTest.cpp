#include"pch.h"

#include"WorldMarket.h"

#include"QuandlDataSource.h"
#include"MockQuandlWebInquiry.h"

#include"GeneralCheck.h"

using namespace testing;

namespace FireBirdTest {
	static bool m_fSystemStatus;

	class CQuandlWebInquiryTest : public ::testing::Test {
	protected:
		static void SetUpTestSuite(void) {
			m_fSystemStatus = gl_pWorldMarket->IsSystemReady();
			gl_pWorldMarket->SetSystemReady(true); // Quandl引擎必须等待系统初始化后才可使用。
			GeneralCheck();
		}

		static void TearDownTestSuite(void) {
			gl_pWorldMarket->SetSystemReady(m_fSystemStatus);
			GeneralCheck();
		}

		void SetUp(void) override {
			GeneralCheck();

			m_QuandlWebInquiry.SetDataSource(gl_pQuandlDataSource.get());
			gl_pWorldMarket->CalculateTime();
		}

		void TearDown(void) override {
			// clearUp
			gl_pWorldMarket->SetResetMarket(true);
			GeneralCheck();
		}

		CMockQuandlWebInquiry m_QuandlWebInquiry; // 网易日线历史数据
	};

	TEST_F(CQuandlWebInquiryTest, TestInitialize) {
		EXPECT_STREQ(m_QuandlWebInquiry.GetInquiryFunction(), _T(""));
		EXPECT_STREQ(m_QuandlWebInquiry.GetInquiryToken(), _T(""));
		EXPECT_STREQ(m_QuandlWebInquiry.GetConnectionName(), _T("Quandl"));
	}

	TEST_F(CQuandlWebInquiryTest, TestGetWebData) {
		m_QuandlWebInquiry.SetReadingWebData(true);
		EXPECT_FALSE(m_QuandlWebInquiry.GetWebData());
		m_QuandlWebInquiry.SetReadingWebData(false);
		gl_pWorldMarket->SetSystemReady(true);
		EXPECT_CALL(m_QuandlWebInquiry, StartReadingThread)
			.Times(1);
		m_QuandlWebInquiry.GetWebData();
		EXPECT_TRUE(m_QuandlWebInquiry.IsReadingWebData()) << _T("此标志由工作线程负责重置。此处调用的是Mock类，故而此标识没有重置");
	}

	TEST_F(CQuandlWebInquiryTest, TestReportStatus) {
		EXPECT_TRUE(m_QuandlWebInquiry.ReportStatus(1));
	}

	TEST_F(CQuandlWebInquiryTest, TestPrepareNextInquiringStr) {
		CString str;
		gl_pWorldMarket->SetSystemReady(true);
		for (int i = 0; i < 4; i++) {
			if (m_QuandlWebInquiry.PrepareNextInquiringString()) {
				str = m_QuandlWebInquiry.GetInquiringString();
				EXPECT_STREQ(str, _T("&api_key=")) << "默认状态下，各字符串皆为空，故而只剩下&api_key=";
			}
			else
				EXPECT_EQ(str.GetLength(), 0);
		}
		gl_pWorldMarket->SetSystemReady(false);
	}

	TEST_F(CQuandlWebInquiryTest, TestGetNextInquiringMiddleStr) {
		EXPECT_STREQ(m_QuandlWebInquiry.GetNextInquiringMiddleString(1, true), _T("")) << "此函数不执行任何动作";
	}
}
