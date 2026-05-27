#include"pch.h"

#include "GeneralCheck.h"

#include"SetChinaMarketDayLineInfo.h"

#include"SetInsiderSentiment.h"
#include"SetFinnhubStockDayLine.h"

using namespace testing;

namespace FireBirdTest {
	class SetInitializeTest : public Test {
	protected:
		static void SetUpTestSuite() {
			SCOPED_TRACE("");
			GeneralCheck();
		}

		static void TearDownTestSuite() {
			SCOPED_TRACE("");
			GeneralCheck();
		}

		void SetUp() override {
			SCOPED_TRACE("");
			GeneralCheck();
		}

		void TearDown() override {
			// clearUp
			SCOPED_TRACE("");
			GeneralCheck();
		}

	protected:
	};

	TEST_F(SetInitializeTest, TestDayLineBasicInfoInitialize) {
		CSetChinaMarketDayLineInfo setDayLineBasicInfo;

		EXPECT_FALSE(gl_systemConfiguration.IsWorkingMode());
		EXPECT_STREQ(setDayLineBasicInfo.GetDefaultConnect(), _T("DSN=stock_market_test;UID=Test;PASSWORD=test;charset=utf8mb4"));
		gl_systemConfiguration.SetWorkingMode(true);
		EXPECT_STREQ(setDayLineBasicInfo.GetDefaultConnect(), _T("DSN=stock_market;UID=FireBird;PASSWORD=firebird;charset=utf8mb4"));
		gl_systemConfiguration.SetWorkingMode(false);

		EXPECT_STREQ(setDayLineBasicInfo.GetDefaultSQL(), _T("[china_stock_dayline]"));
	}

	TEST_F(SetInitializeTest, TestFinnhubStockDayLineInitialize) {
		CSetFinnhubStockDayLine setFinnhubStockDayLine;

		EXPECT_FALSE(gl_systemConfiguration.IsWorkingMode());
		EXPECT_STREQ(setFinnhubStockDayLine.GetDefaultConnect(), _T("DSN=stock_market_test;UID=Test;PASSWORD=test;charset=utf8mb4"));
		gl_systemConfiguration.SetWorkingMode(true);
		EXPECT_STREQ(setFinnhubStockDayLine.GetDefaultConnect(), _T("DSN=stock_market;UID=FireBird;PASSWORD=firebird;charset=utf8mb4"));
		gl_systemConfiguration.SetWorkingMode(false);

		EXPECT_STREQ(setFinnhubStockDayLine.GetDefaultSQL(), _T("[finnhub_stock_dayline]"));
	}
	TEST_F(SetInitializeTest, TestInsiderSentimentInitialize) {
		CSetInsiderSentiment setInsiderSentiment;

		EXPECT_FALSE(gl_systemConfiguration.IsWorkingMode());
		EXPECT_STREQ(setInsiderSentiment.GetDefaultConnect(), _T("DSN=stock_market_test;UID=Test;PASSWORD=test;charset=utf8mb4"));
		gl_systemConfiguration.SetWorkingMode(true);
		EXPECT_STREQ(setInsiderSentiment.GetDefaultConnect(), _T("DSN=stock_market;UID=FireBird;PASSWORD=firebird;charset=utf8mb4"));
		gl_systemConfiguration.SetWorkingMode(false);

		EXPECT_STREQ(setInsiderSentiment.GetDefaultSQL(), _T("[finnhub_insider_sentiment]"));
	}
}
