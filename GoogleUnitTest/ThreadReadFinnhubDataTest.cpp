#include"pch.h"

#include"ThreadStatus.h"

#include"GeneralCheck.h"

#include"WorldMarket.h"
#include"FinnhubDataSource.h"

#include"MockFinnhubWebInquiry.h"

#include"thread.h"

using namespace testing;

namespace FireBirdTest {
	class CThreadReadFinnhubDataTest : public Test {
	protected:
		static void SetUpTestSuite(void) {
			GeneralCheck();
		}

		static void TearDownTestSuite(void) {
			GeneralCheck();
		}

		void SetUp(void) override {
			FinnhubWebInquiry.SetDataSource(gl_pFinnhubDataSource.get());
			FinnhubWebInquiry.SetInquiringWebData(true);
		}

		void TearDown(void) override {
		}

		CMockFinnhubWebInquiry FinnhubWebInquiry;
	};

	TEST_F(CThreadReadFinnhubDataTest, TestThreadReadFinnhubData) {
		gl_pFinnhubDataSource->SetInquiring(true);
		int iCreatingThread = gl_ThreadStatus.GetNumberOfWebInquiringThread();

		gl_pFinnhubDataSource->SetWebInquiryFinished(false);
		EXPECT_CALL(FinnhubWebInquiry, ReadingWebData())
			.Times(1)
			.WillOnce(Return(false));
		FinnhubWebInquiry.TESTSetBuffer(_T("testData"));
		EXPECT_EQ(ThreadReadVirtualWebData(&FinnhubWebInquiry), static_cast<UINT>(1));
		EXPECT_EQ(gl_ThreadStatus.GetNumberOfWebInquiringThread(), iCreatingThread);
		EXPECT_THAT(gl_pFinnhubDataSource->GetReceivedDataSize(), 0);

		gl_pFinnhubDataSource->SetInquiring(true);
		CString strMessage = _T("{\"test\":\"testData\"}");
		gl_pFinnhubDataSource->SetWebInquiryFinished(false);
		EXPECT_CALL(FinnhubWebInquiry, ReadingWebData())
			.Times(1)
			.WillOnce(Return(true));
		FinnhubWebInquiry.TESTSetBuffer(strMessage);
		FinnhubWebInquiry.SetInquiringWebData(true);
		EXPECT_EQ(ThreadReadVirtualWebData(&FinnhubWebInquiry), static_cast<UINT>(1));
		EXPECT_EQ(gl_ThreadStatus.GetNumberOfWebInquiringThread(), iCreatingThread);
		EXPECT_THAT(gl_pFinnhubDataSource->GetReceivedDataSize(), 1);
		CWebDataPtr pWebData = gl_pFinnhubDataSource->GetReceivedData();
		EXPECT_EQ(FinnhubWebInquiry.GetBufferSize(), 1024 * 1024) << "重置缓冲区大小为默认值";
		EXPECT_EQ(pWebData->GetBufferLength(), strMessage.GetLength());
		EXPECT_TRUE(pWebData->IsParsed());
		EXPECT_TRUE(pWebData->IsJSonContentType());
	}
}
