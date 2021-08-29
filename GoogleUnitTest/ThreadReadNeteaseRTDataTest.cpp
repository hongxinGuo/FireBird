#include"pch.h"

#include"globedef.h"
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
		}

		virtual void TearDown(void) override {
		}
		CMockNeteaseRTWebInquiry NeteaseRTWebInquiry;
	};

	TEST_F(CThreadReadNeteaseRTDataTest, TestThreadReadNeteaseRTData) {
		int iCreatingThread = gl_ThreadStatus.GetNumberOfWebInquiringThread();
		EXPECT_CALL(NeteaseRTWebInquiry, ReadWebData())
			.Times(1)
			.WillOnce(Return(false));
		NeteaseRTWebInquiry.__TESTSetBuffer(_T("testData"));
		EXPECT_EQ(ThreadReadNeteaseRTData(&NeteaseRTWebInquiry), (UINT)3);
		EXPECT_EQ(gl_ThreadStatus.GetNumberOfWebInquiringThread(), iCreatingThread);
		EXPECT_EQ(gl_WebInquirer.GetNeteaseRTDataSize(), 0);

		EXPECT_CALL(NeteaseRTWebInquiry, ReadWebData())
			.Times(1)
			.WillOnce(Return(true));
		NeteaseRTWebInquiry.__TESTSetBuffer(_T("testData"));
		EXPECT_EQ(ThreadReadNeteaseRTData(&NeteaseRTWebInquiry), (UINT)3);
		EXPECT_EQ(gl_ThreadStatus.GetNumberOfWebInquiringThread(), iCreatingThread);
		EXPECT_EQ(gl_WebInquirer.GetNeteaseRTDataSize(), 1);
		CWebDataPtr pWebData = gl_WebInquirer.PopNeteaseRTData();
		EXPECT_EQ(pWebData->GetBufferLength(), 8);
		char buffer[30];
		int i = 0;
		while ((i < 30) && (pWebData->GetData(i) != 0x000)) {
			buffer[i] = pWebData->GetData(i);
			i++;
		}
		buffer[i] = 0x000;
		CString str = buffer;
		EXPECT_STREQ(str, _T("testData"));
	}
}