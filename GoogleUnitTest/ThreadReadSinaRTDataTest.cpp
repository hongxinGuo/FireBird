#include"pch.h"

#include"ThreadStatus.h"
#include"GeneralCheck.h"

#include"SinaRTDataSource.h"

#include"MockSinaRTWebInquiry.h"

#include"thread.h"

using namespace testing;

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

namespace StockAnalysisTest {
	class CThreadReadSinaRTDataTest : public ::testing::Test
	{
	protected:
		static void SetUpTestSuite(void) {
		}

		static void TearDownTestSuite(void) {
			GeneralCheck();
		}

		virtual void SetUp(void) override {
			SinaRTWebInquiry.SetDataSource(gl_pSinaRTDataSource.get());
			SinaRTWebInquiry.SetReadingWebData(true);
		}

		virtual void TearDown(void) override {
		}
		CMockSinaRTWebInquiry SinaRTWebInquiry;
	};

	TEST_F(CThreadReadSinaRTDataTest, TestThreadReadSinaRTData) {
		int iCreatingThread = gl_ThreadStatus.GetNumberOfWebInquiringThread();

		EXPECT_CALL(SinaRTWebInquiry, ReadingWebData())
			.Times(1)
			.WillOnce(Return(false));
		SinaRTWebInquiry._TESTSetBuffer(_T("testData"));
		EXPECT_EQ(ThreadReadVirtualWebData(&SinaRTWebInquiry), (UINT)1);
		EXPECT_EQ(gl_ThreadStatus.GetNumberOfWebInquiringThread(), iCreatingThread);

		EXPECT_CALL(SinaRTWebInquiry, ReadingWebData())
			.Times(1)
			.WillOnce(Return(true));
		SinaRTWebInquiry._TESTSetBuffer(_T("var hq_str_sh600000=\"浦发银行,10.120,9.940,10.090,10.180,9.960,10.090,10.100,42609358,428744766.000,453445,10.090,312600,10.080,173519,10.070,242800,10.060,136700,10.050,3600,10.100,3200,10.120,5100,10.130,129900,10.150,476200,10.160,2020-03-20,15:00:00,00,\";\nvar hq_str_sh600004=\"白云机场,13.010,12.900,13.300,13.350,12.990,13.300,13.320,9689808,127814773.000,4810,13.300,11100,13.290,5800,13.280,5200,13.270,10800,13.260,1600,13.320,61100,13.330,46100,13.340,114467,13.350,35000,13.360,2020-03-20,15:00:09,00,\";\nvar hq_str_sh600008=\"首创股份,3.050,3.040,3.050,3.080,3.030,3.050,3.060,22087562,67408587.000,760200,3.050,625400,3.040,570900,3.030,329000,3.020,202000,3.010,9200,3.060,112100,3.070,491897,3.080,449600,3.090,664140,3.100,2020-03-20,15:00:09,00,\";\n"));
		SinaRTWebInquiry.SetReadingWebData(true);
		EXPECT_EQ(ThreadReadVirtualWebData(&SinaRTWebInquiry), (UINT)1);
		EXPECT_EQ(gl_ThreadStatus.GetNumberOfWebInquiringThread(), iCreatingThread);
		EXPECT_EQ(gl_pSinaRTDataSource->GetReceivedDataSize(), 1);

		// 恢复原状
		CWebDataPtr pRTData = gl_pSinaRTDataSource->GetReceivedData();
	}
}