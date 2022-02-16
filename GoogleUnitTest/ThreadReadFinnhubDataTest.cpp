#include"pch.h"

#include"globedef.h"
#include"GeneralCheck.h"

#include"WebInquirer.h"
#include"MockFinnhubWebInquiry.h"

#include"thread.h"

using namespace testing;

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

namespace StockAnalysisTest {
	class CThreadReadFinnhubDataTest : public ::testing::Test
	{
	protected:
		static void SetUpTestSuite(void) {
			GeneralCheck();
		}

		static void TearDownTestSuite(void) {
			GeneralCheck();
		}

		virtual void SetUp(void) override {
			FinnhubWebInquiry.SetReadingWebData(true);
		}

		virtual void TearDown(void) override {
		}
		CMockFinnhubWebInquiry FinnhubWebInquiry;
	};

	TEST_F(CThreadReadFinnhubDataTest, TestThreadReadFinnhubData) {
		int iCreatingThread = gl_ThreadStatus.GetNumberOfWebInquiringThread();

		gl_pWorldMarket->SetFinnhubDataReceived(false);
		EXPECT_CALL(FinnhubWebInquiry, ReadWebData())
			.Times(1)
			.WillOnce(Return(false));
		FinnhubWebInquiry.__TESTSetBuffer(_T("testData"));
		EXPECT_EQ(ThreadReadVirtualWebData(&FinnhubWebInquiry), (UINT)1);
		EXPECT_EQ(gl_ThreadStatus.GetNumberOfWebInquiringThread(), iCreatingThread);
		EXPECT_EQ(gl_WebInquirer.GetFinnhubDataSize(), 0);
		EXPECT_THAT(gl_systemMessage.GetErrorMessageDequeSize(), 1);
		gl_systemMessage.PopErrorMessage();

		gl_pWorldMarket->SetFinnhubDataReceived(false);
		EXPECT_CALL(FinnhubWebInquiry, ReadWebData())
			.Times(1)
			.WillOnce(Return(true));
		FinnhubWebInquiry.__TESTSetBuffer(_T("testData"));
		FinnhubWebInquiry.SetReadingWebData(true);
		EXPECT_EQ(ThreadReadVirtualWebData(&FinnhubWebInquiry), (UINT)1);
		EXPECT_EQ(gl_ThreadStatus.GetNumberOfWebInquiringThread(), iCreatingThread);
		EXPECT_EQ(gl_WebInquirer.GetFinnhubDataSize(), 1);
		CWebDataPtr pWebData = gl_WebInquirer.PopFinnhubData();
		EXPECT_EQ(pWebData->GetBufferLength(), 8);
		char buffer[30];
		int i = 0;
		while ((i < pWebData->GetBufferLength())) {
			buffer[i] = pWebData->GetData(i);
			i++;
		}
		buffer[i] = 0x000;
		CString str = buffer;
		EXPECT_STREQ(str, _T("testData"));
	}
}