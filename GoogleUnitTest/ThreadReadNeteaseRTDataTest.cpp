#include"pch.h"

#include"ThreadStatus.h"
#include"GeneralCheck.h"
#include"MockNeteaseRTWebInquiry.h"
#include"NeteaseRTDataSource.h"

#include"thread.h"

using namespace testing;

namespace FireBirdTest {
	class CThreadReadNeteaseRTDataTest : public ::testing::Test {
	protected:
		static void SetUpTestSuite(void) {
			GeneralCheck();
		}

		static void TearDownTestSuite(void) {
			GeneralCheck();
		}

		void SetUp(void) override {
			NeteaseRTWebInquiry.SetDataSource(gl_pNeteaseRTDataSource.get());
			NeteaseRTWebInquiry.SetReadingWebData(true);
		}

		void TearDown(void) override { }

		CMockNeteaseRTWebInquiry NeteaseRTWebInquiry;
	};

	TEST_F(CThreadReadNeteaseRTDataTest, TestThreadReadNeteaseRTData1) {
		int iCreatingThread = gl_ThreadStatus.GetNumberOfWebInquiringThread();

		EXPECT_CALL(NeteaseRTWebInquiry, ReadingWebData())
			.Times(1)
			.WillOnce(Return(false));
		NeteaseRTWebInquiry.TESTSetBuffer(_T("_ntes_quote_callback({\"0600270\": {\"time\": \"2020/04/23 08:30:01\", \"code\": \"0600270\",\"name\":\"don't use chinese character\",\"update\": \"2020/04/23 08:30:01\"} });")); // 要采用网易实时数据制式
		EXPECT_EQ(ThreadReadVirtualWebData(&NeteaseRTWebInquiry), (UINT)1);
		EXPECT_EQ(gl_ThreadStatus.GetNumberOfWebInquiringThread(), iCreatingThread);
	}

	TEST_F(CThreadReadNeteaseRTDataTest, TestThreadReadNeteaseRTData2) {
		gl_pNeteaseRTDataSource->SetInquiring(true);
		int iCreatingThread = gl_ThreadStatus.GetNumberOfWebInquiringThread();
		EXPECT_CALL(NeteaseRTWebInquiry, ReadingWebData())
			.Times(1)
			.WillOnce(Return(true));
		NeteaseRTWebInquiry.TESTSetBuffer(_T("_ntes_quote_callback({\"0600270\": {\"time\": \"2020/04/23 08:30:01\", \"code\": \"0600270\",\"name\":\"don't use chinese character\",\"update\": \"2020/04/23 08:30:01\"} });")); // 要采用网易实时数据制式
		NeteaseRTWebInquiry.SetReadingWebData(true);
		EXPECT_EQ(ThreadReadVirtualWebData(&NeteaseRTWebInquiry), (UINT)1);
		EXPECT_EQ(gl_ThreadStatus.GetNumberOfWebInquiringThread(), iCreatingThread);
		EXPECT_EQ(gl_pNeteaseRTDataSource->GetReceivedDataSize(), 1);

		// 恢复原状
		gl_pNeteaseRTDataSource->GetReceivedData();
	}
}
