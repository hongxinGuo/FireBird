#include"pch.h"

#include"GeneralCheck.h"

#include"JsonParse.h"
#include "WebData.h"
#include "WebRTData.h"

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
