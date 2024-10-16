#include"pch.h"

#include"GeneralCheck.h"

#include"ThreadStatus.h"

#include "ChinaMarket.h"
#include "WorldMarket.h"

using namespace testing;

namespace FireBirdTest {
	class ThreadStatusTest : public Test {
	protected:
		static void SetUpTestSuite() {
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
	};

	TEST_F(ThreadStatusTest, TestInitialize) {
		ASSERT_FALSE(gl_systemConfiguration.IsWorkingMode());
		EXPECT_FALSE(gl_ThreadStatus.IsBackGroundThreadsWorking());
		EXPECT_EQ(gl_ThreadStatus.GetNumberOfBackGroundWorkingThread(), 0);

		const size_t l = gl_systemMessage.InformationSize();
		CThreadStatus threadStatus; // 生成第二个实例（第一个为全局变量，系统启动时就生成了）Note 不要删除
		EXPECT_EQ(gl_systemMessage.InformationSize(), l + 1) << "不允许生成第二个变量，系统报警"; // 系统报警队列
		for (int i = 0; i < l + 1; i++) {
			CString str = gl_systemMessage.PopInformationMessage(); // 清除信息队列
		}
		EXPECT_EQ(gl_systemMessage.InformationSize(), 0);
	}

	TEST_F(ThreadStatusTest, TestIsSavingThreadRunning) {
		EXPECT_FALSE(gl_ThreadStatus.IsSavingThreadRunning());

		EXPECT_TRUE(gl_UpdateChinaMarketDB.try_acquire());
		EXPECT_TRUE(gl_UpdateWorldMarketDB.try_acquire());

		EXPECT_TRUE(gl_ThreadStatus.IsSavingThreadRunning());

		gl_UpdateChinaMarketDB.release();
		EXPECT_TRUE(gl_ThreadStatus.IsSavingThreadRunning());

		gl_UpdateWorldMarketDB.release();
		EXPECT_FALSE(gl_ThreadStatus.IsSavingThreadRunning());

		gl_UpdateChinaMarketDB.acquire();
		EXPECT_TRUE(gl_ThreadStatus.IsSavingThreadRunning());

		// clear up
		gl_UpdateChinaMarketDB.release();
	}

	TEST_F(ThreadStatusTest, TestIsCalculatingRS) {
		EXPECT_FALSE(gl_ThreadStatus.IsBackGroundThreadsWorking());
		for (int i = 0; i < gl_systemConfiguration.GetBackgroundThreadPermittedNumber(); i++) {  // 目前采用最多8个线程
			gl_ThreadStatus.IncreaseBackGroundWorkingThread();
		}
		for (int i = 0; i < gl_systemConfiguration.GetBackgroundThreadPermittedNumber() - 1; i++) {
			gl_ThreadStatus.DecreaseBackGroundWorkingThread();
			EXPECT_TRUE(gl_ThreadStatus.IsBackGroundThreadsWorking());
		}
		gl_ThreadStatus.DecreaseBackGroundWorkingThread();
		EXPECT_FALSE(gl_ThreadStatus.IsBackGroundThreadsWorking());
	}
}
