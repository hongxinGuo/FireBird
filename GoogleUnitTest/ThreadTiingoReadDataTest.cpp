#include"pch.h"

#include"ThreadStatus.h"
#include"GeneralCheck.h"

#include"WorldMarket.h"
#include"FinnhubDataSource.h"
#include"TiingoDataSource.h"

#include"MockTiingoWebInquiry.h"

#include"thread.h"

using namespace testing;

namespace FireBirdTest {
	class CThreadReadTiingoDataTest : public ::testing::Test {
	protected:
		static void SetUpTestSuite(void) {
			GeneralCheck();
		}

		static void TearDownTestSuite(void) {
			GeneralCheck();
		}

		void SetUp(void) override {
			TiingoWebInquiry.SetDataSource(gl_pTiingoDataSource.get());
			TiingoWebInquiry.SetInquiringWebData(true);
		}

		void TearDown(void) override {
		}

		CMockTiingoWebInquiry TiingoWebInquiry;
	};

	TEST_F(CThreadReadTiingoDataTest, TestThreadReadTiingoData) {
		gl_pTiingoDataSource->SetInquiring(true);
		const int iCreatingThread = gl_ThreadStatus.GetNumberOfWebInquiringThread();

		EXPECT_CALL(TiingoWebInquiry, ReadingWebData())
			.Times(1)
			.WillOnce(Return(false));
		TiingoWebInquiry.TESTSetBuffer(_T("testData"));
		TiingoWebInquiry.SetInquiringWebData(true);
		EXPECT_EQ(ThreadReadVirtualWebData(&TiingoWebInquiry), static_cast<UINT>(1));
		EXPECT_EQ(gl_ThreadStatus.GetNumberOfWebInquiringThread(), iCreatingThread);
		EXPECT_EQ(gl_pTiingoDataSource->GetReceivedDataSize(), 0);

		gl_pTiingoDataSource->SetInquiring(true);
		const CString strMessage = _T("{\"test\":\"testData\"}");
		EXPECT_CALL(TiingoWebInquiry, ReadingWebData())
			.Times(1)
			.WillOnce(Return(true));
		TiingoWebInquiry.TESTSetBuffer(strMessage);
		TiingoWebInquiry.SetInquiringWebData(true);
		EXPECT_EQ(ThreadReadVirtualWebData(&TiingoWebInquiry), static_cast<UINT>(1));
		EXPECT_EQ(gl_ThreadStatus.GetNumberOfWebInquiringThread(), iCreatingThread);
		EXPECT_EQ(gl_pTiingoDataSource->GetReceivedDataSize(), 1);
		const CWebDataPtr pWebData = gl_pTiingoDataSource->GetReceivedData();
		EXPECT_EQ(TiingoWebInquiry.GetBufferSize(), 1024 * 1024) << "重置缓冲区大小为默认值";
		EXPECT_EQ(pWebData->GetBufferLength(), strMessage.GetLength());
		EXPECT_TRUE(pWebData->IsParsed());
		EXPECT_TRUE(pWebData->IsJSonContentType());
	}
}
