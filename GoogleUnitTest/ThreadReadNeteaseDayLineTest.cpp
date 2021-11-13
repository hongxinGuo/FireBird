#include"pch.h"

#include"globedef.h"
#include"GeneralCheck.h"
#include"WebInquirer.h"
#include"MockNeteaseDayLineWebInquiry.h"

#include"thread.h"

using namespace testing;

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

namespace StockAnalysisTest {
	class CThreadReadNeteaseDayLineTest : public ::testing::Test
	{
	protected:
		static void SetUpTestSuite(void) {
		}

		static void TearDownTestSuite(void) {
			GeneralCheck();
		}

		virtual void SetUp(void) override {
		}

		virtual void TearDown(void) override {
		}
		CMockNeteaseDayLineWebInquiry NeteaseDayLineWebInquiry;
	};

	TEST_F(CThreadReadNeteaseDayLineTest, TestThreadReadNeteaseDayLine) {
		int iCreatingThread = gl_ThreadStatus.GetNumberOfWebInquiringThread();
		EXPECT_CALL(NeteaseDayLineWebInquiry, ReadWebData())
			.Times(1)
			.WillOnce(Return(false));
		NeteaseDayLineWebInquiry.__TESTSetBuffer(_T("testData"));
		NeteaseDayLineWebInquiry.SetDownLoadingStockCode(_T("600601.SS"));
		NeteaseDayLineWebInquiry.SetReadingWebData(true);
		EXPECT_EQ(ThreadReadVirtualWebData(&NeteaseDayLineWebInquiry), (UINT)1);
		EXPECT_EQ(gl_ThreadStatus.GetNumberOfWebInquiringThread(), iCreatingThread);
		EXPECT_THAT(gl_systemMessage.GetErrorMessageDequeSize(), 1);
		gl_systemMessage.PopErrorMessage();

		EXPECT_FALSE(NeteaseDayLineWebInquiry.IsReadingWebData());
		NeteaseDayLineWebInquiry.SetReadingWebData(true);
		EXPECT_CALL(NeteaseDayLineWebInquiry, ReadWebData())
			.Times(1)
			.WillOnce(Return(true));
		NeteaseDayLineWebInquiry.__TESTSetBuffer(_T("testData"));
		EXPECT_EQ(ThreadReadVirtualWebData(&NeteaseDayLineWebInquiry), (UINT)1);
		EXPECT_EQ(gl_ThreadStatus.GetNumberOfWebInquiringThread(), iCreatingThread);
		EXPECT_TRUE(gl_pChinaMarket->IsDayLineNeedProcess());

		gl_WebInquirer.PopNeteaseDayLineData();
	}
}