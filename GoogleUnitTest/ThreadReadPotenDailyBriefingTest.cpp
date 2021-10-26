#include"pch.h"

#include"globedef.h"
#include"GeneralCheck.h"

#include"WebInquirer.h"
#include"MockPotenDailyBriefingWebInquiry.h"

#include"thread.h"

using namespace testing;

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

namespace StockAnalysisTest {
	class CPotenDailyBriefingThreadTest : public ::testing::Test
	{
	protected:
		static void SetUpTestSuite(void) {
			GeneralCheck();
		}

		static void TearDownTestSuite(void) {
			GeneralCheck();
		}

		virtual void SetUp(void) override {
		}

		virtual void TearDown(void) override {
		}
		CMockPotenDailyBriefingWebInquiry potenDailyBriefingInquiry;
	};

	TEST_F(CPotenDailyBriefingThreadTest, TestThreadReadPotenDailyBriefing) {
		time_t tt = 0;

		int iCreatingThread = gl_ThreadStatus.GetNumberOfSavingThread();
		EXPECT_CALL(potenDailyBriefingInquiry, ReadWebData())
			.Times(1)
			.WillOnce(Return(false));
		potenDailyBriefingInquiry.__TESTSetBuffer(_T("testData"));
		potenDailyBriefingInquiry.SetInquiringDate(20100101);
		EXPECT_EQ(ThreadReadVirtualWebData(&potenDailyBriefingInquiry), (UINT)1);
		EXPECT_EQ(gl_ThreadStatus.GetNumberOfSavingThread(), iCreatingThread);
		EXPECT_EQ(gl_WebInquirer.GetPotenDailyBriefingDataSize(), 0);

		EXPECT_CALL(potenDailyBriefingInquiry, ReadWebData())
			.Times(1)
			.WillOnce(Return(true));
		potenDailyBriefingInquiry.__TESTSetBuffer(_T("testData"));
		potenDailyBriefingInquiry.SetInquiringDate(20100101);
		tt = gl_pPotenDailyBriefingMarket->TransferToUTCTime(20100101);
		EXPECT_EQ(ThreadReadVirtualWebData(&potenDailyBriefingInquiry), (UINT)1);
		EXPECT_EQ(gl_ThreadStatus.GetNumberOfSavingThread(), iCreatingThread);
		EXPECT_EQ(gl_WebInquirer.GetPotenDailyBriefingDataSize(), 1);
		CWebDataPtr pWebData = gl_WebInquirer.PopPotenDailyBriefingData();
		EXPECT_EQ(pWebData->GetBufferLength(), 8);
		char buffer[30];
		int i = 0;
		while ((i < 30) && (pWebData->GetData(i) != 0x000)) {
			buffer[i] = pWebData->GetData(i);
			i++;
		}
		buffer[i] = 0x000;
		CString str = buffer;
		EXPECT_STREQ(str, _T("testData"));
		EXPECT_EQ(pWebData->GetTime(), tt);
		EXPECT_FALSE(gl_WebInquirer.IsReadingPotenDailyBriefing());
	}
}