#include"pch.h"

#include"WorldMarket.h"

#include"MockQuandlDataSource.h"

#include"GeneralCheck.h"

using namespace testing;

namespace FireBirdTest {
	static bool m_fSystemStatus;
	CMockQuandlDataSourcePtr m_pMockQuandlDataSource; // 网易日线历史数据

	class CMockQuandlDataSourceTest : public ::testing::Test {
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
			gl_pWorldMarket->CalculateTime();
			m_pMockQuandlDataSource = make_shared<CMockQuandlDataSource>();
		}

		void TearDown(void) override {
			// clearUp
			m_pMockQuandlDataSource = nullptr;
			gl_pWorldMarket->SetResetMarket(true);
			GeneralCheck();
		}

	protected:
		CQuandlDataSource QuandlDataSource;
	};

	TEST_F(CMockQuandlDataSourceTest, TestInitialize) {
		EXPECT_STREQ(QuandlDataSource.GetInquiryFunction(), _T(""));
		EXPECT_STREQ(QuandlDataSource.GetInquiryToken(), _T(""));
	}

	TEST_F(CMockQuandlDataSourceTest, TestGetWebData) {
		m_pMockQuandlDataSource->SetInquiringWebData(false);
		gl_pWorldMarket->SetSystemReady(true);
		EXPECT_CALL(*m_pMockQuandlDataSource, StartReadingThread)
		.Times(1);
		m_pMockQuandlDataSource->GetWebData();
		EXPECT_TRUE(m_pMockQuandlDataSource->IsInquiringWebData()) << _T("此标志由工作线程负责重置。此处调用的是Mock类，故而此标识没有重置");
	}

	TEST_F(CMockQuandlDataSourceTest, TestPrepareNextInquiringStr) {
		CString str;
		gl_pWorldMarket->SetSystemReady(true);
		for (int i = 0; i < 4; i++) {
			if (QuandlDataSource.PrepareNextInquiringString()) {
				str = QuandlDataSource.GetInquiringString();
				EXPECT_STREQ(str, _T("&api_key=")) << "默认状态下，各字符串皆为空，故而只剩下&api_key=";
			}
			else
				EXPECT_EQ(str.GetLength(), 0);
		}
		gl_pWorldMarket->SetSystemReady(false);
	}
}
