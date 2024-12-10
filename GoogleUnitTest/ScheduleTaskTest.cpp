#include"pch.h"

 

#include"GeneralCheck.h"

import FireBird.Accessory.JsonParse;

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
