///////////////////////////////////////////////////////////////////////////////////////////
//
// 所有使用Mock类测试ChinaMarket的函数皆位于此文件中。
//
///////////////////////////////////////////////////////////////////////////////////////////
#include"pch.h"

#include"TimeConvert.h"

#include"ChinaStock.h"
#include "ContainerChinaStock.h"

#include"MockChinaMarket.h"

#include"GeneralCheck.h"
#include "MarketTask.h"
#include "SystemConfiguration.h"
#include "SystemMessage.h"

using namespace testing;
#include<memory>
using std::make_shared;
using std::literals::chrono_literals::operator ""s;
using std::literals::chrono_literals::operator ""min;
using std::literals::chrono_literals::operator ""h;

namespace {
	CMockChinaMarketPtr s_pMockChinaMarket;
}

namespace FireBirdTest {
	class CMockChinaMarketTest : public Test {
	protected:
		static void SetUpTestSuite() {
			SCOPED_TRACE("");
			GeneralCheck();

			s_pMockChinaMarket = make_shared<CMockChinaMarket>();
			s_pMockChinaMarket->ResetMarket();
			while (gl_systemMessage.InformationSize() > 0) gl_systemMessage.PopInformationMessage();

			EXPECT_LE(gl_dataContainerChinaStock.GetDayLineNeedUpdateNumber(), gl_dataContainerChinaStock.Size());

			for (int i = 0; i < gl_dataContainerChinaStock.Size(); i++) {
				const auto pStock = gl_dataContainerChinaStock.GetStock(i);
				if (pStock->GetDayLineEndDate() > toLocalDays(20251101)) {
					pStock->SetDayLineEndDate(toLocalDays(20250101));//Todo: 测试用，避免日线结束日期过晚,过一段时间后删除此行
					pStock->SetUpdateProfileDB(true);
				}
				pStock->SetUpdateDayLine(true);
				if (IsEarlyThen(pStock->GetDayLineEndDate(), gl_pChinaMarket->GetMarketDate(), 30)) {
					if (pStock->GetDayLineEndDate() == toLocalDays(20250101)) {
						pStock->SetUpdateProfileDB(false);
					}
				}
				EXPECT_FALSE(pStock->IsUpdateProfileDB()) << pStock->GetSymbol();
			}
			EXPECT_FALSE(gl_dataContainerChinaStock.IsUpdateProfileDB());
			EXPECT_EQ(gl_dataContainerChinaStock.GetDayLineNeedUpdateNumber(), gl_dataContainerChinaStock.Size());

			while (!s_pMockChinaMarket->IsMarketTaskEmpty()) s_pMockChinaMarket->DiscardCurrentMarketTask();

			EXPECT_TRUE(s_pMockChinaMarket != nullptr) << "此Mock变量在EnvironmentSetUp.h中生成";
			EXPECT_FALSE(gl_dataContainerChinaStock.IsUpdateDayLineDB());
			EXPECT_EQ(gl_dataContainerChinaStock.GetDayLineNeedSaveNumber(), 0);

			EXPECT_FALSE(s_pMockChinaMarket->IsSystemReady());
			s_pMockChinaMarket->SetSystemReady(true);// 测试系统默认为准备好了
		}

		static void TearDownTestSuite() {
			EXPECT_EQ(gl_dataContainerChinaStock.GetDayLineNeedUpdateNumber(), gl_dataContainerChinaStock.Size());
			EXPECT_FALSE(gl_dataContainerChinaStock.IsUpdateDayLineDB());
			EXPECT_EQ(gl_dataContainerChinaStock.GetDayLineNeedSaveNumber(), 0);

			// 重置以下指针，以测试是否存在没有配对的Mock。
			s_pMockChinaMarket = nullptr;

			SCOPED_TRACE("");
			GeneralCheck();
		}

		void SetUp() override {
			SCOPED_TRACE("");
			GeneralCheck();

			//EXPECT_EQ(gl_dataContainerChinaStock.GetDayLineNeedUpdateNumber(), gl_dataContainerChinaStock.Size());
			EXPECT_EQ(gl_dataContainerChinaStock.GetDayLineNeedSaveNumber(), 0);
			EXPECT_EQ(gl_dataContainerChinaStock.GetDayLineNeedUpdateNumber(), gl_dataContainerChinaStock.Size());
			EXPECT_TRUE(s_pMockChinaMarket->IsMarketTaskEmpty()) << s_pMockChinaMarket->GetMarketTask()->GetTime();
		}

		void TearDown() override {
			// clearUp
			EXPECT_EQ(gl_dataContainerChinaStock.GetDayLineNeedUpdateNumber(), gl_dataContainerChinaStock.Size());
			EXPECT_EQ(gl_dataContainerChinaStock.GetDayLineNeedSaveNumber(), 0);
			if (gl_dataContainerChinaStock.GetDayLineNeedSaveNumber() > 0) {
				for (int i = 0; i < gl_dataContainerChinaStock.Size(); i++) {
					const CChinaStockPtr pStock = gl_dataContainerChinaStock.GetStock(i);
					if (pStock->IsUpdateDayLineDB()) {
						EXPECT_EQ(pStock->GetSymbol(), "");
					}
				}
			}
			s_pMockChinaMarket->SetUpdateOptionDB(false);

			EXPECT_TRUE(s_pMockChinaMarket->IsSystemReady());
			EXPECT_TRUE(s_pMockChinaMarket->IsMarketTaskEmpty()) << s_pMockChinaMarket->GetMarketTask()->GetTime();
			EXPECT_FALSE(s_pMockChinaMarket->IsUpdateChosenStockDB());

			EXPECT_TRUE(s_pMockChinaMarket->IsSystemReady());

			SCOPED_TRACE("");
			GeneralCheck();
		}
	};

	TEST_F(CMockChinaMarketTest, TestProcessEveryDayTask1) {
		s_pMockChinaMarket->AddTask(CHINA_MARKET_CREATE_TASK_, local_seconds(1h));

		EXPECT_CALL(*s_pMockChinaMarket, TaskCreateTask()).Times(1);

		EXPECT_TRUE(s_pMockChinaMarket->ProcessTask());

		EXPECT_TRUE(s_pMockChinaMarket->IsMarketTaskEmpty()) << s_pMockChinaMarket->GetMarketTask()->GetTime();

		EXPECT_FALSE(gl_systemConfiguration.IsUpdateDB());
	}

	TEST_F(CMockChinaMarketTest, TestProcessEveryDayTask2) {
		s_pMockChinaMarket->AddTask(CHINA_MARKET_RESET_, gl_pChinaMarket->GetResetTime());

		EXPECT_CALL(*s_pMockChinaMarket, TaskResetMarket()).Times(1);

		s_pMockChinaMarket->TEST_SetMarketTime(gl_pChinaMarket->GetResetTime());
		EXPECT_TRUE(s_pMockChinaMarket->ProcessTask());
		EXPECT_TRUE(s_pMockChinaMarket->IsMarketTaskEmpty());
		EXPECT_FALSE(gl_systemConfiguration.IsUpdateDB());
	}

	TEST_F(CMockChinaMarketTest, TestProcessEveryDayTask3) {
		s_pMockChinaMarket->AddTask(CHINA_MARKET_RESET_, gl_pChinaMarket->GetResetTime());

		EXPECT_CALL(*s_pMockChinaMarket, TaskResetMarket()).Times(1);

		s_pMockChinaMarket->TEST_SetMarketTime(local_seconds(9h + 27min + 0s));
		EXPECT_TRUE(s_pMockChinaMarket->ProcessTask());

		EXPECT_TRUE(s_pMockChinaMarket->IsMarketTaskEmpty());
		EXPECT_FALSE(gl_systemConfiguration.IsUpdateDB());
	}

	TEST_F(CMockChinaMarketTest, TestProcessEveryDayTask4) {
		s_pMockChinaMarket->AddTask(CHINA_MARKET_DISTRIBUTE_AND_CALCULATE_RT_DATA_, local_seconds(9h + 15min + 00s));

		s_pMockChinaMarket->TEST_SetMarketTime(local_seconds(9h + 30min + 00s));
		EXPECT_TRUE(s_pMockChinaMarket->ProcessTask());

		EXPECT_FALSE(s_pMockChinaMarket->IsMarketTaskEmpty());
		const auto pTask = s_pMockChinaMarket->GetMarketTask();
		s_pMockChinaMarket->DiscardCurrentMarketTask();
		EXPECT_EQ(pTask->GetType(), CHINA_MARKET_DISTRIBUTE_AND_CALCULATE_RT_DATA_);
		EXPECT_EQ(pTask->GetTime(), local_seconds(9h + 30min + 01s)) << "每秒一次";

		EXPECT_TRUE(s_pMockChinaMarket->IsMarketTaskEmpty());
		EXPECT_FALSE(gl_systemConfiguration.IsUpdateDB());
	}

	TEST_F(CMockChinaMarketTest, TestProcessEveryDayTask9) {
		s_pMockChinaMarket->AddTask(RELOAD_SYSTEM_, local_seconds(21h));

		EXPECT_CALL(*s_pMockChinaMarket, TaskExitSystem()).Times(1);

		s_pMockChinaMarket->TEST_SetMarketTime(local_seconds(21h));
		EXPECT_TRUE(s_pMockChinaMarket->ProcessTask());

		EXPECT_TRUE(s_pMockChinaMarket->IsMarketTaskEmpty()) << s_pMockChinaMarket->GetMarketTask()->GetTime();
		EXPECT_FALSE(gl_systemConfiguration.IsUpdateDB());

		// 重置系统准备状态
	}

	TEST_F(CMockChinaMarketTest, TestTaskAccessoryPerMinuteTask) {
		EXPECT_TRUE(s_pMockChinaMarket->IsMarketTaskEmpty());

		s_pMockChinaMarket->TEST_SetMarketTime(local_seconds(1h + 0min + 0s));
		s_pMockChinaMarket->TaskAccessoryPerMinuteTask();

		EXPECT_FALSE(s_pMockChinaMarket->IsMarketTaskEmpty());
		const auto pTask = s_pMockChinaMarket->GetMarketTask();
		s_pMockChinaMarket->DiscardCurrentMarketTask();
		EXPECT_EQ(pTask->GetType(), CHINA_MARKET_PER_MINUTE_ACCESSORY_TASK_);
		EXPECT_EQ(pTask->GetTime(), local_seconds(1h + 1min + 00s)) << "每一分钟一次";

		EXPECT_TRUE(s_pMockChinaMarket->IsMarketTaskEmpty());
		EXPECT_FALSE(gl_systemConfiguration.IsUpdateDB());
	}

	TEST_F(CMockChinaMarketTest, TestTaskDistributeAndCalculateRTData) {
		s_pMockChinaMarket->TEST_SetMarketTime(local_seconds(10h));
		s_pMockChinaMarket->TaskDistributeAndCalculateRTData();

		const auto pTask = s_pMockChinaMarket->GetMarketTask();
		s_pMockChinaMarket->DiscardCurrentMarketTask();
		EXPECT_EQ(pTask->GetType(), CHINA_MARKET_DISTRIBUTE_AND_CALCULATE_RT_DATA_);
		EXPECT_EQ(pTask->GetTime(), local_seconds(10h + 00min + 01s)) << "每秒一次";

		EXPECT_TRUE(s_pMockChinaMarket->IsMarketTaskEmpty());
		EXPECT_FALSE(gl_systemConfiguration.IsUpdateDB());
	}
}
