#include"pch.h"

#include <gtest/gtest.h>

#include"GeneralCheck.h"

#include"ChinaMarket.h"
#include "ContainerChinaStock.h"
#include"TimeConvert.h"

#include"ProductTengxunDayLine.h"
#include"DayLineWebData.h"
#include "SystemData.h"
#include"ChinaStock.h"

using namespace testing;
using std::make_shared;

namespace FireBirdTest {
	class CProductTengxunDayLineTest : public Test {
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
			tengxunDayLine.SetInquiryNumber(1);
		}

		void TearDown() override {
			// clearUp
			SCOPED_TRACE("");
			GeneralCheck();
		}

	protected:
		CProductTengxunDayLine tengxunDayLine;
	};

	TEST_F(CProductTengxunDayLineTest, TestInitialize) {
		EXPECT_EQ(tengxunDayLine.GetInquiryFunction(), "");
	}
}
