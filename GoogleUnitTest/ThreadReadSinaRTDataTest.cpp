#include"pch.h"

#include"globedef.h"
#include"GeneralCheck.h"
#include"WebInquirer.h"

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
			SinaRTWebInquiry.SetReadingWebData(true);
		}

		virtual void TearDown(void) override {
		}
		CMockSinaRTWebInquiry SinaRTWebInquiry;
	};

	TEST_F(CThreadReadSinaRTDataTest, TestThreadReadSinaRTData) {
		int iCreatingThread = gl_ThreadStatus.GetNumberOfWebInquiringThread();
		EXPECT_CALL(SinaRTWebInquiry, ReadWebData())
			.Times(1)
			.WillOnce(Return(false));
		SinaRTWebInquiry.__TESTSetBuffer(_T("testData"));
		EXPECT_EQ(ThreadReadVirtualWebData(&SinaRTWebInquiry), (UINT)1);
		EXPECT_EQ(gl_ThreadStatus.GetNumberOfWebInquiringThread(), iCreatingThread);
		EXPECT_EQ(gl_WebInquirer.GetSinaRTDataSize(), 0);

		EXPECT_CALL(SinaRTWebInquiry, ReadWebData())
			.Times(1)
			.WillOnce(Return(true));
		SinaRTWebInquiry.__TESTSetBuffer(_T("testData"));
		SinaRTWebInquiry.SetReadingWebData(true);
		EXPECT_EQ(ThreadReadVirtualWebData(&SinaRTWebInquiry), (UINT)1);
		EXPECT_EQ(gl_ThreadStatus.GetNumberOfWebInquiringThread(), iCreatingThread);
		EXPECT_EQ(gl_WebInquirer.GetSinaRTDataSize(), 1);
		CWebDataPtr pWebData = gl_WebInquirer.PopSinaRTData();
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