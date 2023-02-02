#include"pch.h"

#include"ThreadStatus.h"
#include"ChinaMarket.h"
#include"GeneralCheck.h"

#include"MockTengxunRTDataSource.h"

using namespace testing;

namespace FireBirdTest {
	class CMockTengxunRTDataSourceTest : public ::testing::Test {
	protected:
		static void SetUpTestSuite(void) {
			GeneralCheck();
		}

		static void TearDownTestSuite(void) {
			while (gl_ThreadStatus.IsSavingThreadRunning()) Sleep(1);
			GeneralCheck();
		}

		void SetUp(void) override {
			GeneralCheck();

			gl_pChinaMarket->SetTengxunRTDataInquiringIndex(0);
		}

		void TearDown(void) override {
			// clearUp
			gl_pChinaMarket->SetSystemReady(false);
			gl_pChinaMarket->SetTengxunRTDataInquiringIndex(0);
		}

	public:
		CMockTengxunRTDataSource m_CMockTengxunRTDataSource; // 腾讯实时数据采集
	};

	TEST_F(CMockTengxunRTDataSourceTest, TestInitialize) {
		EXPECT_STREQ(m_CMockTengxunRTDataSource.GetInquiryFunction(), _T("http://qt.gtimg.cn/q="));
		EXPECT_STREQ(m_CMockTengxunRTDataSource.GetInquiryToken(), _T(""));
		EXPECT_EQ(m_CMockTengxunRTDataSource.GetInquiringNumber(), 900) << _T("腾讯默认值");
	}

	TEST_F(CMockTengxunRTDataSourceTest, TestUpdateStatus) {
		EXPECT_TRUE(m_CMockTengxunRTDataSource.UpdateStatus()) << "网易实时数据无需更新状态，直接返回";
	}

	TEST_F(CMockTengxunRTDataSourceTest, TestParseData) {
		const auto pData = make_shared<CWebData>();
		EXPECT_TRUE(m_CMockTengxunRTDataSource.ParseData(pData)) << "网易实时数据无需解析";
	}

	TEST_F(CMockTengxunRTDataSourceTest, TestGetWebData) {
		m_CMockTengxunRTDataSource.SetInquiringWebData(true);
		EXPECT_FALSE(m_CMockTengxunRTDataSource.GetWebData());
		m_CMockTengxunRTDataSource.SetInquiringWebData(false);
		gl_pChinaMarket->SetSystemReady(true);
		EXPECT_CALL(m_CMockTengxunRTDataSource, StartReadingThread)
			.Times(1);
		m_CMockTengxunRTDataSource.GetWebData();
		EXPECT_TRUE(m_CMockTengxunRTDataSource.IsInquiringWebData()) << _T("此标志由工作线程负责重置。此处调用的是Mock类，故而此标识没有重置");
	}

	TEST_F(CMockTengxunRTDataSourceTest, TestPrepareNextInquiringStr) {
		gl_pChinaMarket->SetSystemReady(true);
		EXPECT_TRUE(m_CMockTengxunRTDataSource.PrepareNextInquiringString());
		CString str = m_CMockTengxunRTDataSource.GetInquiringString();
		EXPECT_STREQ(str.Left(21), _T("http://qt.gtimg.cn/q="));
		gl_pChinaMarket->SetSystemReady(false);
	}

	TEST_F(CMockTengxunRTDataSourceTest, TestIsTengxunRTDataInValid) {
		const CWebDataPtr pWebDataReceived = make_shared<CWebData>();
		CString str = _T("v_pv_none_match=\"1\";\n");
		pWebDataReceived->SetData(str.GetBuffer(), str.GetLength(), 0);
		pWebDataReceived->SetBufferLength(str.GetLength());
		pWebDataReceived->ResetCurrentPos();

		EXPECT_TRUE(m_CMockTengxunRTDataSource.IsInvalidTengxunRTData(*pWebDataReceived));
		EXPECT_EQ(pWebDataReceived->GetCurrentPos(), 0);

		str = _T("v_pv_none_mtch=\"1\";\n");
		pWebDataReceived->SetData(str.GetBuffer(), str.GetLength(), 0);
		pWebDataReceived->SetBufferLength(str.GetLength());
		pWebDataReceived->ResetCurrentPos();

		EXPECT_FALSE(m_CMockTengxunRTDataSource.IsInvalidTengxunRTData(*pWebDataReceived));
		EXPECT_EQ(pWebDataReceived->GetCurrentPos(), 0);
	}
}
