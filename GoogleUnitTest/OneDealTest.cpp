#include"pch.h"
#include"globedef.h"
#include"GeneralCheck.h"

#include"OneDeal.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

namespace StockAnalysisTest {
	class COneDealTest : public ::testing::Test
	{
	protected:
		static void SetUpTestSuite(void) { // 本测试类的初始化函数
			GeneralCheck();
		}

		static void TearDownTestSuite() {
			GeneralCheck();
			EXPECT_EQ(gl_pChinaMarket->GetDayLineNeedProcessNumber(), 0);
		}

		virtual void SetUp(void) override {
			GeneralCheck();
		}

		virtual void TearDown(void) override {
			// clearup
		}
	};

	TEST_F(COneDealTest, TestInitialize) {
		COneDeal oneDeal;
		EXPECT_EQ(oneDeal.GetMarketTime(), 0);
		EXPECT_EQ(oneDeal.GetType(), 0);
		EXPECT_EQ(oneDeal.GetPrice(), 0);
		EXPECT_EQ(oneDeal.GetVolume(), 0);
	}

	TEST_F(COneDealTest, TestGetTime) {
		COneDeal oneDeal;
		oneDeal.SetTime(20202020);
		EXPECT_EQ(oneDeal.GetMarketTime(), 20202020);
	}

	TEST_F(COneDealTest, TestGetType) {
		COneDeal oneDeal;
		oneDeal.SetType(30303030);
		EXPECT_EQ(oneDeal.GetType(), 30303030);
	}
	TEST_F(COneDealTest, TestGetPrice) {
		COneDeal oneDeal;
		oneDeal.SetPrice(40404040);
		EXPECT_EQ(oneDeal.GetPrice(), 40404040);
	}
	TEST_F(COneDealTest, TestGetVolume) {
		COneDeal oneDeal;
		oneDeal.SetVolume(50505050);
		EXPECT_EQ(oneDeal.GetVolume(), 50505050);
	}
}