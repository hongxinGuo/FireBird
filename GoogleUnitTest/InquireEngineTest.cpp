#include"pch.h"

#include"GeneralCheck.h"

#include"InquireEngine.h"

#include"WorldMarket.h"

using namespace testing;

namespace FireBirdTest {
	class CInquireEngineTest : public ::testing::Test {
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
		CInquireEngine engine;
	};

	TEST_F(CInquireEngineTest, TestInitialize) {
	}

	TEST_F(CInquireEngineTest, TestIncreaseBufferSizeIfNeeded) {
		engine.SetBufferSize(1024 * 1024);

		engine.SetByteRead(1024 * (1024 - 128));
		EXPECT_FALSE(engine.IncreaseBufferSizeIfNeeded(1));

		engine.SetByteRead(1024 * (1024 - 128) + 1);
		EXPECT_TRUE(engine.IncreaseBufferSizeIfNeeded(1));
	}

	TEST_F(CInquireEngineTest, TestCreateWebData) {
		engine.TESTSetBuffer(_T("{ \"data\": 2}"));
		const time_t tUTCTime = GetUTCTime();
		TestSetUTCTime(10);

		const auto pWebData = engine.CreateWebData();

		EXPECT_TRUE(pWebData != nullptr);
		EXPECT_EQ(pWebData->GetTime(), 10) << "设置为当前的UTCTime";
		EXPECT_TRUE(pWebData->GetDataBuffer().at(11) == '}');
		EXPECT_EQ(pWebData->GetDataBuffer().size(), 12);

		// restore
		TestSetUTCTime(tUTCTime);
	}

	TEST_F(CInquireEngineTest, TestIsWebError) {
		EXPECT_FALSE(engine.IsWebError());
		engine.SetWebError(true);
		EXPECT_TRUE(engine.IsWebError());
		engine.SetWebError(false);
		EXPECT_FALSE(engine.IsWebError());
	}
}
