#include"pch.h"

#include"globedef.h"
#include"ThreadStatus.h"
#include"SystemMessage.h"

#include"GeneralCheck.h"

#include"WorldMarket.h"

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
		EXPECT_CALL(FinnhubWebInquiry, ReadingWebData())
			.Times(1)
			.WillOnce(Return(false));
		FinnhubWebInquiry.__TESTSetBuffer(_T("testData"));
		EXPECT_EQ(ThreadReadVirtualWebData(&FinnhubWebInquiry), (UINT)1);
		EXPECT_EQ(gl_ThreadStatus.GetNumberOfWebInquiringThread(), iCreatingThread);
		EXPECT_EQ(gl_WebInquirer.FinnhubDataSize(), 0);

		CString strMessage = _T("{\"test\":\"testData\"}");
		gl_pWorldMarket->SetFinnhubDataReceived(false);
		EXPECT_CALL(FinnhubWebInquiry, ReadingWebData())
			.Times(1)
			.WillOnce(Return(true));
		FinnhubWebInquiry.__TESTSetBuffer(strMessage);
		FinnhubWebInquiry.SetReadingWebData(true);
		EXPECT_EQ(ThreadReadVirtualWebData(&FinnhubWebInquiry), (UINT)1);
		EXPECT_EQ(gl_ThreadStatus.GetNumberOfWebInquiringThread(), iCreatingThread);
		EXPECT_EQ(gl_WebInquirer.FinnhubDataSize(), 1);
		CWebDataPtr pWebData = gl_WebInquirer.PopFinnhubData();
		EXPECT_EQ(FinnhubWebInquiry.GetBufferSize(), 1024 * 1024) << "重置缓冲区大小为默认值";
		EXPECT_EQ(pWebData->GetBufferLength(), strMessage.GetLength());
		EXPECT_TRUE(pWebData->IsParsed());
		EXPECT_TRUE(pWebData->IsJSonContentType());
	}
}