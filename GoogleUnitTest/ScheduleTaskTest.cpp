#include"pch.h"

//#include"gtest/gtest.h"

#include"GeneralCheck.h"

#include"JsonParse.h"

namespace FireBirdTest {
	class ScheduleTaskTest : public testing::Test {
		void SetUp() override {
			SCOPED_TRACE("");
			GeneralCheck();
		}

		void TearDown() override {
			SCOPED_TRACE("");
			GeneralCheck();
		}
	};

	TEST_F(ScheduleTaskTest, TestTaskCheckWorldMarketReady) {
	}
}
