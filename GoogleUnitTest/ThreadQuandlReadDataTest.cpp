#include"pch.h"

#include"ThreadStatus.h"

#include"GeneralCheck.h"

#include"WorldMarket.h"

#include"WebInquirer.h"
#include"MockQuandlWebInquiry.h"
#include"QuandlDataSource.h"

#include"thread.h"

using namespace testing;

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

namespace StockAnalysisTest {
	class CThreadReadQuandlDataTest : public ::testing::Test
	{
	protected:
		static void SetUpTestSuite(void) {
		}

		static void TearDownTestSuite(void) {
			GeneralCheck();
		}

		virtual void SetUp(void) override {
			QuandlWebInquiry.SetReadingWebData(true);
		}

		virtual void TearDown(void) override {
		}
		CMockQuandlWebInquiry QuandlWebInquiry;
	};

	TEST_F(CThreadReadQuandlDataTest, TestThreadReadQuandlData) {
		int iCreatingThread = gl_ThreadStatus.GetNumberOfWebInquiringThread();

		gl_pDataSourceQuandl->SetDataReceived(false);
		EXPECT_CALL(QuandlWebInquiry, ReadingWebData())
			.Times(1)
			.WillOnce(Return(false));
		QuandlWebInquiry.__TESTSetBuffer(_T("testData"));
		EXPECT_EQ(ThreadReadVirtualWebData(&QuandlWebInquiry), (UINT)1);
		EXPECT_EQ(gl_ThreadStatus.GetNumberOfWebInquiringThread(), iCreatingThread);
		EXPECT_EQ(gl_pDataSourceQuandl->GetReceivedDataSize(), 0);
		EXPECT_THAT(gl_systemMessage.ErrorMessageSize(), 1);
		gl_systemMessage.PopErrorMessage();

		CString strMessage = _T("{\"test\":\"testData\"}");
		gl_pDataSourceQuandl->SetDataReceived(false);
		EXPECT_CALL(QuandlWebInquiry, ReadingWebData())
			.Times(1)
			.WillOnce(Return(true));
		QuandlWebInquiry.__TESTSetBuffer(strMessage);
		QuandlWebInquiry.SetReadingWebData(true);
		EXPECT_EQ(ThreadReadVirtualWebData(&QuandlWebInquiry), (UINT)1);
		EXPECT_EQ(gl_ThreadStatus.GetNumberOfWebInquiringThread(), iCreatingThread);
		EXPECT_EQ(gl_pDataSourceQuandl->GetReceivedDataSize(), 1);
		CWebDataPtr pWebData = gl_pDataSourceQuandl->GetReceivedData();
		EXPECT_EQ(pWebData->GetBufferLength(), strMessage.GetLength());
		EXPECT_EQ(QuandlWebInquiry.GetBufferSize(), 1024 * 1024) << "重置缓冲区大小为默认值";
		EXPECT_TRUE(pWebData->IsParsed());
		EXPECT_TRUE(pWebData->IsJSonContentType());
	}
}