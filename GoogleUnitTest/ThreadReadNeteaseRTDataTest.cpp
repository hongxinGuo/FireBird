#include"pch.h"

#include"globedef.h"
#include"ThreadStatus.h"
#include"SystemMessage.h"
#include"GeneralCheck.h"
#include"WebInquirer.h"
#include"MockNeteaseRTWebInquiry.h"

#include"thread.h"

using namespace testing;

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

namespace StockAnalysisTest {
	class CThreadReadNeteaseRTDataTest : public ::testing::Test
	{
	protected:
		static void SetUpTestSuite(void) {
		}

		static void TearDownTestSuite(void) {
			GeneralCheck();
		}

		virtual void SetUp(void) override {
			NeteaseRTWebInquiry.SetReadingWebData(true);
		}

		virtual void TearDown(void) override {
		}
		CMockNeteaseRTWebInquiry NeteaseRTWebInquiry;
	};

	TEST_F(CThreadReadNeteaseRTDataTest, TestThreadReadNeteaseRTData) {
		int iCreatingThread = gl_ThreadStatus.GetNumberOfWebInquiringThread();
		EXPECT_CALL(NeteaseRTWebInquiry, ReadingWebData())
			.Times(1)
			.WillOnce(Return(false));
		NeteaseRTWebInquiry.__TESTSetBuffer(_T("_ntes_quote_callback({\"0600270\": {\"time\": \"2020/04/23 08:30:01\", \"code\": \"0600270\",\"name\":\"don't use chinese character\",\"update\": \"2020/04/23 08:30:01\"} });")); // 要采用网易实时数据制式
		EXPECT_EQ(ThreadReadVirtualWebData(&NeteaseRTWebInquiry), (UINT)1);
		EXPECT_EQ(gl_ThreadStatus.GetNumberOfWebInquiringThread(), iCreatingThread);

		EXPECT_CALL(NeteaseRTWebInquiry, ReadingWebData())
			.Times(1)
			.WillOnce(Return(true));
		NeteaseRTWebInquiry.__TESTSetBuffer(_T("_ntes_quote_callback({\"0600270\": {\"time\": \"2020/04/23 08:30:01\", \"code\": \"0600270\",\"name\":\"don't use chinese character\",\"update\": \"2020/04/23 08:30:01\"} });")); // 要采用网易实时数据制式
		NeteaseRTWebInquiry.SetReadingWebData(true);
		EXPECT_EQ(ThreadReadVirtualWebData(&NeteaseRTWebInquiry), (UINT)1);
		EXPECT_EQ(gl_ThreadStatus.GetNumberOfWebInquiringThread(), iCreatingThread);
		EXPECT_EQ(gl_WebInquirer.GetNeteaseRTDataSize(), 1);

		// 恢复原状
		gl_WebInquirer.PopNeteaseRTData();
	}
}