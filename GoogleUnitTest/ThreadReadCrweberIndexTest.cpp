#include"pch.h"

#include"globedef.h"
#include"WebInquirer.h"

#include"MockCrweberIndexWebInquiry.h"

#include"thread.h"

using namespace testing;

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

namespace StockAnalysisTest {
	class CThreadCrweberIndexTest : public ::testing::Test
	{
	protected:
		static void SetUpTestSuite(void) {
		}

		static void TearDownTestSuite(void) {
		}

		virtual void SetUp(void) override {
		}

		virtual void TearDown(void) override {
		}
		CMockCrweberIndexWebInquiry crweberIndexInquiry;
	};

	TEST_F(CThreadCrweberIndexTest, TestThreadCrweberIndex) {
		int iCreatingThread = gl_ThreadStatus.GetNumberOfSavingThread();
		EXPECT_CALL(crweberIndexInquiry, ReadWebData())
			.Times(1)
			.WillOnce(Return(false));
		crweberIndexInquiry.__TESTSetBuffer(_T("testData"));
		EXPECT_EQ(ThreadReadCrweberIndex(&crweberIndexInquiry), (UINT)5);
		EXPECT_EQ(gl_ThreadStatus.GetNumberOfSavingThread(), iCreatingThread);
		EXPECT_EQ(gl_WebInquirer.GetPotenDailyBriefingDataSize(), 0);

		EXPECT_CALL(crweberIndexInquiry, ReadWebData())
			.Times(1)
			.WillOnce(Return(true));
		crweberIndexInquiry.__TESTSetBuffer(_T("testData"));
		EXPECT_EQ(ThreadReadCrweberIndex(&crweberIndexInquiry), (UINT)5);
		EXPECT_EQ(gl_ThreadStatus.GetNumberOfSavingThread(), iCreatingThread);
		EXPECT_EQ(gl_WebInquirer.GetCrweberDataSize(), 1);
		CWebDataPtr pWebData = gl_WebInquirer.PopCrweberData();
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
		EXPECT_FALSE(gl_WebInquirer.IsReadingCrweberIndex());
	}
}