#include"pch.h"

#include"ChinaMarket.h"
#include"GeneralCheck.h"

#include"MockNeteaseRTDataSource.h"

using namespace testing;

namespace FireBirdTest {
	class CMockNeteaseRTDataSourceTest : public ::testing::Test {
	protected:
		static void SetUpTestSuite(void) {
			GeneralCheck();
		}

		static void TearDownTestSuite(void) {
			GeneralCheck();
		}

		void SetUp(void) override {
			GeneralCheck();

			EXPECT_TRUE(gl_pChinaMarket->IsResetMarket());
		}

		void TearDown(void) override {
			// clearUp
			gl_pChinaMarket->SetSystemReady(false);
			GeneralCheck();
		}

		CMockNeteaseRTDataSource m_MockNeteaseRTDataSource; // 网易实时数据采集
	};

	TEST_F(CMockNeteaseRTDataSourceTest, TestInitialize) {
		EXPECT_STREQ(m_MockNeteaseRTDataSource.GetInquiryFunction(), _T("http://api.money.126.net/data/feed/"));
		EXPECT_STREQ(m_MockNeteaseRTDataSource.GetInquiryToken(), _T(""));
		EXPECT_EQ(m_MockNeteaseRTDataSource.GetInquiringNumber(), 900) << _T("DEBUG模式下网易默认值");
	}

	TEST_F(CMockNeteaseRTDataSourceTest, TestUpdateStatus) {
		EXPECT_TRUE(m_MockNeteaseRTDataSource.UpdateStatus()) << "网易实时数据无需更新状态，直接返回";
	}

	TEST_F(CMockNeteaseRTDataSourceTest, TestParseData) {
		const auto pData = make_shared<CWebData>();
		EXPECT_TRUE(m_MockNeteaseRTDataSource.ParseData(pData)) << "网易实时数据无需解析";
	}

	TEST_F(CMockNeteaseRTDataSourceTest, TestInquireRTData) {
		m_MockNeteaseRTDataSource.SetInquiring(true);
		EXPECT_FALSE(m_MockNeteaseRTDataSource.InquireRTData(1010));

		m_MockNeteaseRTDataSource.SetInquiring(false);
		EXPECT_TRUE(m_MockNeteaseRTDataSource.InquireRTData(1010));
		EXPECT_EQ(m_MockNeteaseRTDataSource.GetInquiryQueueSize(), 1);
		auto pProduct = m_MockNeteaseRTDataSource.GetInquiry();
		EXPECT_STREQ(typeid(*pProduct).name(), _T("class CProductNeteaseRT"));
	}

	TEST_F(CMockNeteaseRTDataSourceTest, TestGetWebData) {
		m_MockNeteaseRTDataSource.SetInquiringWebData(true);
		EXPECT_FALSE(m_MockNeteaseRTDataSource.GetWebData());
		m_MockNeteaseRTDataSource.SetInquiringWebData(false);
		gl_pChinaMarket->SetSystemReady(true);
		EXPECT_CALL(m_MockNeteaseRTDataSource, StartReadingThread)
			.Times(1);
		m_MockNeteaseRTDataSource.GetWebData();
		EXPECT_TRUE(m_MockNeteaseRTDataSource.IsInquiringWebData()) << _T("此标志由工作线程负责重置。此处调用的是Mock类，故而此标识没有重置");
	}

	TEST_F(CMockNeteaseRTDataSourceTest, TestPrepareNextInquiringStr) {
		gl_pChinaMarket->SetSystemReady(true);
		EXPECT_TRUE(m_MockNeteaseRTDataSource.PrepareNextInquiringString());
		const CString str = m_MockNeteaseRTDataSource.GetInquiringString();
		EXPECT_STREQ(str.Left(35), _T("http://api.money.126.net/data/feed/"));
	}
}
