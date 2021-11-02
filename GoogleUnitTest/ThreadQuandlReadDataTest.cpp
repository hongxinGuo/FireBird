#include"pch.h"

#include"globedef.h"
#include"GeneralCheck.h"

#include"WebInquirer.h"
#include"MockQuandlWebInquiry.h"

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

		gl_pWorldMarket->SetQuandlDataReceived(false);
		EXPECT_CALL(QuandlWebInquiry, ReadWebData())
			.Times(1)
			.WillOnce(Return(false));
		QuandlWebInquiry.__TESTSetBuffer(_T("testData"));
		EXPECT_EQ(ThreadReadVirtualWebData(&QuandlWebInquiry), (UINT)1);
		EXPECT_EQ(gl_ThreadStatus.GetNumberOfWebInquiringThread(), iCreatingThread);
		EXPECT_EQ(gl_WebInquirer.GetQuandlDataSize(), 0);

		gl_pWorldMarket->SetQuandlDataReceived(false);
		EXPECT_CALL(QuandlWebInquiry, ReadWebData())
			.Times(1)
			.WillOnce(Return(true));
		QuandlWebInquiry.__TESTSetBuffer(_T("testData"));
		QuandlWebInquiry.SetReadingWebData(true);
		EXPECT_EQ(ThreadReadVirtualWebData(&QuandlWebInquiry), (UINT)1);
		EXPECT_EQ(gl_ThreadStatus.GetNumberOfWebInquiringThread(), iCreatingThread);
		EXPECT_EQ(gl_WebInquirer.GetQuandlDataSize(), 1);
		CWebDataPtr pWebData = gl_WebInquirer.PopQuandlData();
		EXPECT_EQ(pWebData->GetBufferLength(), 8) << "testData×Ö·û´®³¤¶ÈÎª8";
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