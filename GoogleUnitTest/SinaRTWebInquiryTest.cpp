#include"pch.h"

#include"globedef.h"
#include"ChinaMarket.h"
#include"GeneralCheck.h"

#include"MockSinaRTWebInquiry.h"

using namespace std;
#include<atomic>

using namespace testing;

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

namespace StockAnalysisTest {
	class CSinaRTWebInquiryTest : public ::testing::Test {
	protected:
		static void SetUpTestSuite(void) {
			GeneralCheck();
			gl_pChinaMarket->SetSinaStockRTDataInquiringIndex(0);
		}

		static void TearDownTestSuite(void) {
			while (gl_ThreadStatus.IsSavingThreadRunning()) Sleep(1);
			GeneralCheck();
		}

		virtual void SetUp(void) override {
			GeneralCheck();
		}

		virtual void TearDown(void) override {
			// clearup
			gl_pChinaMarket->SetSinaStockRTDataInquiringIndex(0);
		}
	public:
		CMockSinaRTWebInquiry m_SinaRTWebInquiry; // 新浪实时数据采集
	};

	TEST_F(CSinaRTWebInquiryTest, TestInitialize) {
		EXPECT_STREQ(m_SinaRTWebInquiry.GetHeaders(), _T("User-Agent:FireBird\r\nReferer:https://finance.sina.com.cn\r\n")) << "新浪实时数据服务器需要提供此报头信息，Referer为有用，User-Agent部分只用于说明格式";
		EXPECT_STREQ(m_SinaRTWebInquiry.GetInquiringStringPrefix(), _T("https://hq.sinajs.cn/list=")) << "新浪实时数据服务器已使用https";
		EXPECT_STREQ(m_SinaRTWebInquiry.GetInquiringStringSuffix(), _T(""));
		EXPECT_FALSE(m_SinaRTWebInquiry.IsReportStatus());
		EXPECT_THAT(m_SinaRTWebInquiry.GetShortestInquiringInterval(), 400) << "每400毫秒查询一次，即可在3秒内查询6000个股票（850 * 7），完成完整轮询一次的任务";
		EXPECT_EQ(m_SinaRTWebInquiry.GetInquiringNumber(), 850) << _T("新浪默认值");
	}

	TEST_F(CSinaRTWebInquiryTest, TestStartReadingThread) {
		EXPECT_FALSE(m_SinaRTWebInquiry.IsReadingWebData());
		EXPECT_EQ(m_SinaRTWebInquiry.GetByteReaded(), 0);
	}

	TEST_F(CSinaRTWebInquiryTest, TestGetWebData) {
		m_SinaRTWebInquiry.SetReadingWebData(true);
		EXPECT_FALSE(m_SinaRTWebInquiry.GetWebData());
		m_SinaRTWebInquiry.SetReadingWebData(false);
		gl_pChinaMarket->SetSystemReady(true);
		EXPECT_CALL(m_SinaRTWebInquiry, StartReadingThread)
			.Times(1);
		m_SinaRTWebInquiry.GetWebData();
		EXPECT_TRUE(m_SinaRTWebInquiry.IsReadingWebData()) << _T("此标志由工作线程负责重置。此处调用的是Mock类，故而此标识没有重置");
	}

	TEST_F(CSinaRTWebInquiryTest, TestReportStatus) {
		EXPECT_TRUE(m_SinaRTWebInquiry.ReportStatus(1));
	}

	TEST_F(CSinaRTWebInquiryTest, TestProcessData) {
		INT64 llTotal = gl_pChinaMarket->GetRTDataReceived();
		CString strRTData = _T("var hq_str_sh600000=\"浦发银行,10.120,9.940,10.090,10.180,9.960,10.090,10.100,42609358,428744766.000,453445,10.090,312600,10.080,173519,10.070,242800,10.060,136700,10.050,3600,10.100,3200,10.120,5100,10.130,129900,10.150,476200,10.160,2020-03-20,15:00:00,00,\";\nvar hq_str_sh600004=\"白云机场,13.010,12.900,13.300,13.350,12.990,13.300,13.320,9689808,127814773.000,4810,13.300,11100,13.290,5800,13.280,5200,13.270,10800,13.260,1600,13.320,61100,13.330,46100,13.340,114467,13.350,35000,13.360,2020-03-20,15:00:09,00,\";\nvar hq_str_sh600008=\"首创股份,3.050,3.040,3.050,3.080,3.030,3.050,3.060,22087562,67408587.000,760200,3.050,625400,3.040,570900,3.030,329000,3.020,202000,3.010,9200,3.060,112100,3.070,491897,3.080,449600,3.090,664140,3.100,2020-03-20,15:00:09,00,\";\n");
		CWebDataPtr pData = make_shared<CWebData>();
		m_SinaRTWebInquiry.__TESTSetBuffer(strRTData);
		m_SinaRTWebInquiry.TransferData(pData);
		EXPECT_TRUE(m_SinaRTWebInquiry.ProcessData(pData));
		EXPECT_EQ(gl_pChinaMarket->GetRTDataReceived(), llTotal + 3);
		EXPECT_EQ(gl_WebRTDataContainer.GetSinaDataSize(), 3);

		// 恢复原状
		for (int i = 0; i < 3; i++) {
			CWebRTDataPtr pRTData = gl_WebRTDataContainer.PopSinaData();
		}
	}
}