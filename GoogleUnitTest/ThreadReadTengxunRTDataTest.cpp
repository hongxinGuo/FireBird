#include"pch.h"

#include"ThreadStatus.h"
#include"GeneralCheck.h"
#include"MockTengxunRTWebInquiry.h"

#include"thread.h"
#include"TengxunRTDataSource.h"

using namespace testing;

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

namespace StockAnalysisTest {
	class CThreadReadTengxunRTDataTest : public ::testing::Test
	{
	protected:
		static void SetUpTestSuite(void) {
		}

		static void TearDownTestSuite(void) {
			GeneralCheck();
		}

		virtual void SetUp(void) override {
			TengxunRTWebInquiry.SetDataSource(gl_pTengxunRTDataSource.get());
			TengxunRTWebInquiry.SetReadingWebData(true);
		}

		virtual void TearDown(void) override {
		}
		CMockTengxunRTWebInquiry TengxunRTWebInquiry;
	};

	TEST_F(CThreadReadTengxunRTDataTest, TestThreadReadTengxunRTData) {
		int iCreatingThread = gl_ThreadStatus.GetNumberOfWebInquiringThread();
		EXPECT_CALL(TengxunRTWebInquiry, ReadingWebData())
			.Times(1)
			.WillOnce(Return(false));
		TengxunRTWebInquiry._TESTSetBuffer(_T("v_sz000001=\"51~平安银行~000001~15.59~15.90~15.75~1046363~518391~527971~15.58~2365~15.57~802~15.56~1855~15.55~2316~15.54~320~15.59~661~15.60~15381~15.61~3266~15.62~450~15.63~520~~20190930154003~-0.31~-1.95~15.89~15.57~15.59/1046363/1645828527~1046363~164583~0.54~11.27~~15.89~15.57~2.01~3025.36~3025.38~1.15~17.49~14.31~0.73~-12617~15.73~9.82~12.19~~~1.24~164582.85~0.00~0~~GP-A~68.91~~0.82\";\n"));
		EXPECT_EQ(ThreadReadVirtualWebData(&TengxunRTWebInquiry), (UINT)1);
		EXPECT_EQ(gl_ThreadStatus.GetNumberOfWebInquiringThread(), iCreatingThread);

		EXPECT_CALL(TengxunRTWebInquiry, ReadingWebData())
			.Times(1)
			.WillOnce(Return(true));
		TengxunRTWebInquiry._TESTSetBuffer(_T("v_sz000001=\"51~平安银行~000001~15.59~15.90~15.75~1046363~518391~527971~15.58~2365~15.57~802~15.56~1855~15.55~2316~15.54~320~15.59~661~15.60~15381~15.61~3266~15.62~450~15.63~520~~20190930154003~-0.31~-1.95~15.89~15.57~15.59/1046363/1645828527~1046363~164583~0.54~11.27~~15.89~15.57~2.01~3025.36~3025.38~1.15~17.49~14.31~0.73~-12617~15.73~9.82~12.19~~~1.24~164582.85~0.00~0~~GP-A~68.91~~0.82\";\n"));
		TengxunRTWebInquiry.SetReadingWebData(true);
		EXPECT_EQ(ThreadReadVirtualWebData(&TengxunRTWebInquiry), (UINT)1);
		EXPECT_EQ(gl_ThreadStatus.GetNumberOfWebInquiringThread(), iCreatingThread);

		EXPECT_EQ(gl_pTengxunRTDataSource->GetReceivedDataSize(), 1);

		// 恢复原状
		gl_pTengxunRTDataSource->GetReceivedData();
	}
}