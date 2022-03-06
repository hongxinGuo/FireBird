#include"pch.h"

#include"globedef.h"
#include"ChinaMarket.h"
#include"GeneralCheck.h"

#include"MockNeteaseRTWebInquiry.h"

using namespace std;
#include<atomic>

using namespace testing;

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

namespace StockAnalysisTest {
	class CNeteaseRTWebInquiryTest : public ::testing::Test
	{
	protected:
		static void SetUpTestSuite(void) {
			GeneralCheck();
		}

		static void TearDownTestSuite(void) {
			GeneralCheck();
		}

		virtual void SetUp(void) override {
			GeneralCheck();

			EXPECT_TRUE(gl_pChinaMarket->IsResetMarket());
		}

		virtual void TearDown(void) override {
			// clearup
			gl_pChinaMarket->SetSystemReady(false);
			GeneralCheck();
		}
		CMockNeteaseRTWebInquiry m_NeteaseRTWebInquiry; // 新浪实时数据采集
	};

	TEST_F(CNeteaseRTWebInquiryTest, TestInitialize) {
		EXPECT_STREQ(m_NeteaseRTWebInquiry.GetInquiringStringPrefix(), _T("http://api.money.126.net/data/feed/"));
		EXPECT_STREQ(m_NeteaseRTWebInquiry.GetInquiringStringSuffix(), _T(""));
		EXPECT_STREQ(m_NeteaseRTWebInquiry.GetConnectionName(), _T("NeteaseRT"));
		EXPECT_THAT(m_NeteaseRTWebInquiry.GetShortestInquiringInterval(), 400) << "默认400毫秒查询一次";
		EXPECT_EQ(m_NeteaseRTWebInquiry.GetInquiringNumber(), 900) << _T("网易默认值");
	}

	TEST_F(CNeteaseRTWebInquiryTest, TestGetWebData) {
		m_NeteaseRTWebInquiry.SetReadingWebData(true);
		EXPECT_FALSE(m_NeteaseRTWebInquiry.GetWebData());
		m_NeteaseRTWebInquiry.SetReadingWebData(false);
		gl_pChinaMarket->SetSystemReady(true);
		EXPECT_CALL(m_NeteaseRTWebInquiry, StartReadingThread)
			.Times(1);
		m_NeteaseRTWebInquiry.GetWebData();
		EXPECT_TRUE(m_NeteaseRTWebInquiry.IsReadingWebData()) << _T("此标志由工作线程负责重置。此处调用的是Mock类，故而此标识没有重置");
	}

	TEST_F(CNeteaseRTWebInquiryTest, TestReportStatus) {
		EXPECT_TRUE(m_NeteaseRTWebInquiry.ReportStatus(1));
	}

	TEST_F(CNeteaseRTWebInquiryTest, TestPrepareNextInquiringStr) {
		gl_pChinaMarket->SetSystemReady(true);
		EXPECT_TRUE(m_NeteaseRTWebInquiry.PrepareNextInquiringStr());
		CString str = m_NeteaseRTWebInquiry.GetInquiringString();
		EXPECT_STREQ(str.Left(35), _T("http://api.money.126.net/data/feed/"));
	}

	TEST_F(CNeteaseRTWebInquiryTest, TestParseData) {
		long llTotal = gl_pChinaMarket->GetRTDataReceived();
		CString strRTData = "_ntes_quote_callback({\"0600270\": {\"time\": \"2020/04/23 08:30:01\", \"code\": \"0600270\",\"name\":\"don't use chinese character\",\"update\": \"2020/04/23 08:30:01\"} });";
		CWebDataPtr pData = make_shared<CWebData>();

		m_NeteaseRTWebInquiry.__TESTSetBuffer(strRTData);
		EXPECT_TRUE(m_NeteaseRTWebInquiry.ParseData(pData));
		EXPECT_TRUE(pData->IsSucceedCreatePTree());
		EXPECT_EQ(gl_pChinaMarket->GetRTDataReceived(), llTotal) << "网易实时数据用于更新接收总数";
	}
}