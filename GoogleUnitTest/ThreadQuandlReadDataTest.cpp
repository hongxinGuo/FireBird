#include"pch.h"

#include"ThreadStatus.h"

#include"GeneralCheck.h"

#include"WorldMarket.h"

#include"MockQuandlWebInquiry.h"
#include"QuandlDataSource.h"

#include"thread.h"

using namespace testing;

namespace FireBirdTest {
	class CThreadReadQuandlDataTest : public ::testing::Test {
	protected:
		static void SetUpTestSuite(void) { }

		static void TearDownTestSuite(void) {
			GeneralCheck();
		}

		void SetUp(void) override {
			QuandlWebInquiry.SetDataSource(gl_pQuandlDataSource.get());
			QuandlWebInquiry.SetInquiringWebData(true);
		}

		void TearDown(void) override { }

		CMockQuandlWebInquiry QuandlWebInquiry;
	};

	TEST_F(CThreadReadQuandlDataTest, TestThreadReadQuandlData) {
		gl_pQuandlDataSource->SetInquiring(true);
		const int iCreatingThread = gl_ThreadStatus.GetNumberOfWebInquiringThread();

		gl_pQuandlDataSource->SetWebInquiryFinished(false);
		EXPECT_CALL(QuandlWebInquiry, ReadingWebData())
			.Times(1)
			.WillOnce(Return(false));
		QuandlWebInquiry.TESTSetBuffer(_T("testData"));
		EXPECT_EQ(ThreadReadVirtualWebData(&QuandlWebInquiry), static_cast<UINT>(1));
		EXPECT_EQ(gl_ThreadStatus.GetNumberOfWebInquiringThread(), iCreatingThread);
		EXPECT_EQ(gl_pQuandlDataSource->GetReceivedDataSize(), 0);

		gl_pQuandlDataSource->SetInquiring(true);
		const CString strMessage = _T("{\"test\":\"testData\"}");
		gl_pQuandlDataSource->SetWebInquiryFinished(false);
		EXPECT_CALL(QuandlWebInquiry, ReadingWebData())
			.Times(1)
			.WillOnce(Return(true));
		QuandlWebInquiry.TESTSetBuffer(strMessage);
		QuandlWebInquiry.SetInquiringWebData(true);
		EXPECT_EQ(ThreadReadVirtualWebData(&QuandlWebInquiry), static_cast<UINT>(1));
		EXPECT_EQ(gl_ThreadStatus.GetNumberOfWebInquiringThread(), iCreatingThread);
		EXPECT_EQ(gl_pQuandlDataSource->GetReceivedDataSize(), 1);
		const CWebDataPtr pWebData = gl_pQuandlDataSource->GetReceivedData();
		EXPECT_EQ(pWebData->GetBufferLength(), strMessage.GetLength());
		EXPECT_EQ(QuandlWebInquiry.GetBufferSize(), 1024 * 1024) << "重置缓冲区大小为默认值";
		EXPECT_TRUE(pWebData->IsParsed());
		EXPECT_TRUE(pWebData->IsJSonContentType());
	}
}
