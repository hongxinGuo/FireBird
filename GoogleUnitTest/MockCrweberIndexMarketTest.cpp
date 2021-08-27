#include"pch.h"

#include"globedef.h"
#include"ChinaStock.h"
#include"MockCrweberIndexMarket.h"
#include"WebInquirer.h"
#include"Thread.h"

#include"GeneralCheck.h"

using namespace testing;

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

namespace StockAnalysisTest {
	extern CMockCrweberIndexMarketPtr gl_pMockCrweberIndexMarket;

	class CMockCrweberIndexMarketTest : public ::testing::Test
	{
	protected:
		static void SetUpTestSuite(void) {
			ASSERT(gl_pMockCrweberIndexMarket != nullptr);
		}

		static void TearDownTestSuite(void) {
		}

		virtual void SetUp(void) override {
			ASSERT_FALSE(gl_fNormalMode);
			gl_pMockCrweberIndexMarket->SetDatabaseLoaded(false);
			gl_pMockCrweberIndexMarket->SetPermitResetMarket(true);
			gl_pMockCrweberIndexMarket->SetReadyToRun(true);
			gl_pMockCrweberIndexMarket->SetResetMarket(true);
			gl_pMockCrweberIndexMarket->SetNewestUpdateDate(0);
			ASSERT_FALSE(gl_fNormalMode);
			ASSERT_TRUE(gl_fTestMode);
		}

		virtual void TearDown(void) override {
			// clearup
			gl_pMockCrweberIndexMarket->SetDatabaseLoaded(false);
			gl_pMockCrweberIndexMarket->SetPermitResetMarket(true);
			gl_pMockCrweberIndexMarket->SetReadyToRun(true);
			gl_pMockCrweberIndexMarket->SetResetMarket(true);
			gl_pMockCrweberIndexMarket->SetNewestUpdateDate(0);
			GeneralCheck();
		}
	};

	TEST_F(CMockCrweberIndexMarketTest, TestThreadMaintainCrweberDB) {
		EXPECT_CALL(*gl_pMockCrweberIndexMarket, LoadDatabase)
			.Times(1);
		EXPECT_CALL(*gl_pMockCrweberIndexMarket, SaveDatabase)
			.Times(1);
		EXPECT_EQ(ThreadMaintainCrweberDB(gl_pMockCrweberIndexMarket.get()), (UINT)24);
	}

	TEST_F(CMockCrweberIndexMarketTest, TestThreadSaveCrweberDB) {
		CCrweberIndexPtr pIndex = make_shared<CCrweberIndex>();

		EXPECT_CALL(*gl_pMockCrweberIndexMarket, SaveCrweberIndexData(pIndex))
			.Times(1);
		EXPECT_EQ(ThreadSaveCrweberDB(gl_pMockCrweberIndexMarket.get(), pIndex), (UINT)23);
	}
}