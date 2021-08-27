///////////////////////////////////////////////////////////////////////////////////////////
//
// ����ʹ��Mock�����PotenDailyBriefingMarket�ĺ�����λ�ڴ��ļ��С�
//
///////////////////////////////////////////////////////////////////////////////////////////
#include"pch.h"

#include"globedef.h"

#include"Thread.h"
#include"WebInquirer.h"

#include"MockPotenDailyBriefingMarket.h"

#include"GeneralCheck.h"

using namespace testing;

using namespace std;
#include<memory>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

namespace StockAnalysisTest {
	class CMockPotenDailyBirefingMarketTest : public ::testing::Test
	{
	protected:
		static void SetUpTestSuite(void) {
			ASSERT_FALSE(gl_fNormalMode);
		}
		static void TearDownTestSuite(void) {
			EXPECT_EQ(gl_pChinaMarket->GetCurrentStock(), nullptr) << gl_pChinaMarket->GetCurrentStock()->GetSymbol();
		}
		virtual void SetUp(void) override {
			GeneralCheck();
		}

		virtual void TearDown(void) override {
			// clearup

			GeneralCheck();
		}

		CMockPotenDailyBriefingMarket potenDailyBreifingMarket;
	};

	TEST_F(CMockPotenDailyBirefingMarketTest, TestThreadSavePotenData) {
		EXPECT_CALL(potenDailyBreifingMarket, SaveCurrentData)
			.Times(1);
		ThreadSavePotenData(&potenDailyBreifingMarket);
	}
}