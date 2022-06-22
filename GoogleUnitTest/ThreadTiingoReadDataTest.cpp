#include"pch.h"

#include"globedef.h"
#include"ThreadStatus.h"
#include"GeneralCheck.h"

#include"WorldMarket.h"

#include"WebInquirer.h"
#include"MockTiingoWebInquiry.h"

#include"thread.h"

using namespace testing;

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

namespace StockAnalysisTest {
	class CThreadReadTiingoDataTest : public ::testing::Test
	{
	protected:
		static void SetUpTestSuite(void) {
			GeneralCheck();
		}

		static void TearDownTestSuite(void) {
			GeneralCheck();
		}

		virtual void SetUp(void) override {
			TiingoWebInquiry.SetReadingWebData(true);
		}

		virtual void TearDown(void) override {
		}
		CMockTiingoWebInquiry TiingoWebInquiry;
	};

	TEST_F(CThreadReadTiingoDataTest, TestThreadReadTiingoData) {
		int iCreatingThread = gl_ThreadStatus.GetNumberOfWebInquiringThread();

		gl_pWorldMarket->SetTiingoDataReceived(false);
		EXPECT_CALL(TiingoWebInquiry, ReadingWebData())
			.Times(1)
			.WillOnce(Return(false));
		TiingoWebInquiry.__TESTSetBuffer(_T("testData"));
		TiingoWebInquiry.SetReadingWebData(true);
		EXPECT_EQ(ThreadReadVirtualWebData(&TiingoWebInquiry), (UINT)1);
		EXPECT_EQ(gl_ThreadStatus.GetNumberOfWebInquiringThread(), iCreatingThread);
		EXPECT_EQ(gl_pDataSourceTiingo->GetReceivedDataSize(), 0);
		EXPECT_THAT(gl_systemMessage.ErrorMessageSize(), 1) << "Tiingo工作线程报告出错";
		gl_systemMessage.PopErrorMessage();

		CString strMessage = _T("{\"test\":\"testData\"}");
		gl_pWorldMarket->SetTiingoDataReceived(false);
		EXPECT_CALL(TiingoWebInquiry, ReadingWebData())
			.Times(1)
			.WillOnce(Return(true));
		TiingoWebInquiry.__TESTSetBuffer(strMessage);
		TiingoWebInquiry.SetReadingWebData(true);
		EXPECT_EQ(ThreadReadVirtualWebData(&TiingoWebInquiry), (UINT)1);
		EXPECT_EQ(gl_ThreadStatus.GetNumberOfWebInquiringThread(), iCreatingThread);
		EXPECT_EQ(gl_pDataSourceTiingo->GetReceivedDataSize(), 1);
		CWebDataPtr pWebData = gl_pDataSourceTiingo->GetReceivedData();
		EXPECT_EQ(TiingoWebInquiry.GetBufferSize(), 1024 * 1024) << "重置缓冲区大小为默认值";
		EXPECT_EQ(pWebData->GetBufferLength(), strMessage.GetLength());
		EXPECT_TRUE(pWebData->IsParsed());
		EXPECT_TRUE(pWebData->IsJSonContentType());
	}
}