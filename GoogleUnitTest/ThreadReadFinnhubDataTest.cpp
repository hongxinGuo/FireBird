#include"pch.h"

#include"ThreadStatus.h"

#include"GeneralCheck.h"

#include"WorldMarket.h"
#include"FinnhubDataSource.h"
#include"TiingoDataSource.h"

#include"MockFinnhubWebInquiry.h"

#include"FinnhubDataSource.h"
#include"TiingoDataSource.h"

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
			FinnhubWebInquiry.SetDataSource(gl_pFinnhubDataSource.get());
			FinnhubWebInquiry.SetReadingWebData(true);
		}

		virtual void TearDown(void) override {
		}
		CMockFinnhubWebInquiry FinnhubWebInquiry;
	};

	TEST_F(CThreadReadFinnhubDataTest, TestThreadReadFinnhubData) {
		int iCreatingThread = gl_ThreadStatus.GetNumberOfWebInquiringThread();

		gl_pFinnhubDataSource->SetWebInquiryFinished(false);
		EXPECT_CALL(FinnhubWebInquiry, ReadingWebData())
			.Times(1)
			.WillOnce(Return(false));
		FinnhubWebInquiry._TESTSetBuffer(_T("testData"));
		EXPECT_EQ(ThreadReadVirtualWebData(&FinnhubWebInquiry), (UINT)1);
		EXPECT_EQ(gl_ThreadStatus.GetNumberOfWebInquiringThread(), iCreatingThread);
		EXPECT_THAT(gl_pFinnhubDataSource->GetReceivedDataSize(), 0);

		CString strMessage = _T("{\"test\":\"testData\"}");
		gl_pFinnhubDataSource->SetWebInquiryFinished(false);
		EXPECT_CALL(FinnhubWebInquiry, ReadingWebData())
			.Times(1)
			.WillOnce(Return(true));
		FinnhubWebInquiry._TESTSetBuffer(strMessage);
		FinnhubWebInquiry.SetReadingWebData(true);
		EXPECT_EQ(ThreadReadVirtualWebData(&FinnhubWebInquiry), (UINT)1);
		EXPECT_EQ(gl_ThreadStatus.GetNumberOfWebInquiringThread(), iCreatingThread);
		EXPECT_THAT(gl_pFinnhubDataSource->GetReceivedDataSize(), 1);
		CWebDataPtr pWebData = gl_pFinnhubDataSource->GetReceivedData();
		EXPECT_EQ(FinnhubWebInquiry.GetBufferSize(), 1024 * 1024) << "重置缓冲区大小为默认值";
		EXPECT_EQ(pWebData->GetBufferLength(), strMessage.GetLength());
		EXPECT_TRUE(pWebData->IsParsed());
		EXPECT_TRUE(pWebData->IsJSonContentType());
	}
}