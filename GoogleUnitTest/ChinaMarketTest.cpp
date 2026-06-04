#include"pch.h"

#include "CharSetTransfer.h"
#include"TimeConvert.h"
#include"ChinaStockCodeConverter.h"

#include"ChinaMarket.h"
#include"ChinaStock.h"
#include "dataBaseConnector.h"

#include"GeneralCheck.h"
#include "NeteaseRTDataSource.h"
#include "SinaRTDataSource.h"
#include "TengxunRTDataSource.h"

using namespace testing;
#include<fmt/core.h>
using namespace fmt;

#include<memory>

namespace FireBirdTest {
	class CChinaMarketTest : public Test {
	protected:
		static void SetUpTestSuite() {
			SCOPED_TRACE("");
			GeneralCheck();

			for (size_t i = 0; i < gl_dataContainerChinaStock.Size(); i++) {
				const CChinaStockPtr pStock = gl_dataContainerChinaStock.GetStock(i);
				EXPECT_TRUE(pStock->IsUpdateDayLine()) << pStock->GetSymbol();
			}
			EXPECT_EQ(gl_dataContainerChinaStock.GetDayLineNeedUpdateNumber(), gl_dataContainerChinaStock.Size());

			EXPECT_EQ(gl_dataContainerChinaStock.Size(), 5701) << "测试数据库中的股票代码总数为5701";
		}

		static void TearDownTestSuite() {
			for (size_t i = 0; i < gl_dataContainerChinaStock.Size(); i++) {
				const CChinaStockPtr pStock = gl_dataContainerChinaStock.GetStock(i);
				EXPECT_TRUE(pStock->IsUpdateDayLine()) << pStock->GetSymbol();
			}
			EXPECT_EQ(gl_dataContainerChinaStock.GetDayLineNeedUpdateNumber(), gl_dataContainerChinaStock.Size());

			EXPECT_EQ(gl_dataContainerChinaStock.Size(), 5701) << "测试数据库中的股票代码总数为5701";

			SCOPED_TRACE("");
			GeneralCheck();
		}

		void SetUp() override {
			SCOPED_TRACE("");
			GeneralCheck();

			ASSERT_FALSE(gl_systemConfiguration.IsWorkingMode());
			EXPECT_EQ(gl_dataContainerChinaStock.GetDayLineNeedUpdateNumber(), gl_dataContainerChinaStock.Size());
			gl_pChinaMarket->CalculateTime();
			gl_dataContainerChinaStock.SetSinaRTDataInquiringIndex(0);
			gl_dataContainerChinaStock.SetTengxunRTDataInquiringIndex(0);
			gl_pChinaMarket->SetSystemReady(true); // 测试市场时，默认系统已经准备好
			EXPECT_FALSE(gl_pChinaMarket->IsMarketOpened());
			EXPECT_EQ(gl_dataContainerChinaStock.Size(), 5701) << "测试数据库中的股票代码总数为5701";
		}

		void TearDown() override {
			// clearUp
			EXPECT_FALSE(gl_pChinaMarket->IsMarketOpened());
			gl_pChinaMarket->SetRTDataSetCleared(false);
			gl_pChinaMarket->SetUpdateOptionDB(false);
			gl_dataContainerChinaStock.SetSinaRTDataInquiringIndex(0);
			gl_dataContainerChinaStock.SetTengxunRTDataInquiringIndex(0);
			while (gl_systemMessage.InformationSize() > 0) gl_systemMessage.PopInformationMessage();
			gl_pChinaMarket->SetUpdateOptionDB(false);
			gl_pChinaMarket->SetSystemReady(true); // 离开此测试时，默认系统已准备好。
			EXPECT_TRUE(gl_pChinaMarket->IsCheckingActiveStock());

			for (size_t i = 0; i < gl_dataContainerChinaStock.Size(); i++) {
				const CChinaStockPtr pStock = gl_dataContainerChinaStock.GetStock(i);
				if (!pStock->IsUpdateDayLine()) pStock->SetUpdateDayLine(true);
				if (pStock->IsUpdateDayLineDB()) pStock->SetUpdateDayLineDB(false);
			}
			EXPECT_EQ(gl_dataContainerChinaStock.GetDayLineNeedUpdateNumber(), gl_dataContainerChinaStock.Size());
			EXPECT_EQ(gl_pChinaMarket->GetCurrentSelectedStockSet(), -1);

			EXPECT_EQ(gl_dataContainerChinaStock.Size(), 5701) << "测试数据库中的股票代码总数为5701";

			gl_pChinaMarket->SetCurrentSelectedStockSet(-1);

			while (gl_systemMessage.ErrorMessageSize() > 0) gl_systemMessage.PopErrorMessage();
			gl_systemConfiguration.SetUpdateDB(false);

			SCOPED_TRACE("");
			GeneralCheck();
		}
	};

	TEST_F(CChinaMarketTest, TestInitialize) {
		CChinaStockPtr pStock = nullptr;
		for (size_t i = 0; i < gl_dataContainerChinaStock.Size(); i++) {
			pStock = gl_dataContainerChinaStock.GetStock(i);
			EXPECT_FALSE(pStock->IsUpdateDayLineDB());
			if (IsShanghaiExchange(pStock->GetSymbol())) {
				if ((pStock->GetSymbol().substr(0, 6) >= "000000") && (pStock->GetSymbol().substr(0, 6) <= "000999")) {
					EXPECT_FALSE(pStock->IsNeedProcessRTData());
				}
			}
			else if ((pStock->GetSymbol().substr(0, 6) >= "399000") && (pStock->GetSymbol().substr(0, 6) <= "399999")) {
				EXPECT_FALSE(pStock->IsNeedProcessRTData());
			}
			else { EXPECT_TRUE(pStock->IsNeedProcessRTData()); }
		}
		EXPECT_FALSE(gl_pChinaMarket->IsSelectedStockLoaded());
		EXPECT_TRUE(gl_pChinaMarket->IsSystemReady());
		EXPECT_FALSE(gl_pChinaMarket->IsMarketOpened());
		EXPECT_FALSE(gl_pChinaMarket->IsTodayTempRTDataLoaded());
		EXPECT_GT(gl_dataContainerChinaStock.Size(), 0); // 在全局变量gl_ChinaStockMarket初始化时就生成了全部股票代码池
		EXPECT_EQ(gl_dataContainerChinaStock.Size(), 5701) << "测试数据库中的股票代码总数为5701";
		pStock = gl_dataContainerChinaStock.GetStock(0);
		EXPECT_EQ(pStock->GetSymbol(), "000001.SS");
		string s = "上证指数";
		EXPECT_EQ(pStock->GetDisplaySymbol(), s);
		EXPECT_EQ(gl_dataContainerChinaStock.GetOffset("000001.SS"), 0);

		EXPECT_EQ(gl_dataContainerChinaStock.GetDayLineNeedSaveNumber(), 0);
		EXPECT_TRUE(gl_pChinaMarket->IsUsingSinaRTDataReceiver());
		EXPECT_TRUE(gl_pChinaMarket->IsUsingTengxunRTDataReceiver());
		EXPECT_FALSE(gl_pChinaMarket->IsUsingNeteaseRTDataReceiver());

		EXPECT_EQ(gl_dataContainerChinaStock.GetSinaRTDataInquiringIndex(), 0);
		EXPECT_EQ(gl_dataContainerChinaStock.GetTengxunRTDataInquiringIndex(), 0);
	}

	TEST_F(CChinaMarketTest, TestProcessEveryDayTask1) {
		EXPECT_TRUE(gl_pChinaMarket->IsMarketTaskEmpty());

		EXPECT_FALSE(gl_pChinaMarket->ProcessTask(101010)) << "没有任务可执行";
	}

	TEST_F(CChinaMarketTest, TestProcessEveryDayTask2) {
		EXPECT_TRUE(gl_pChinaMarket->IsMarketTaskEmpty());

		EXPECT_FALSE(gl_pChinaMarket->ProcessTask(101010)) << "没有任务可执行";

		gl_pChinaMarket->AddTask(CHINA_MARKET_VALIDATE_TODAY_DATABASE__, 100101); // Note 这个任务尚未实现，可以使用
		gl_pChinaMarket->AddTask(CHINA_MARKET_VALIDATE_TODAY_DATABASE__, 110101);

		EXPECT_FALSE(gl_pChinaMarket->ProcessTask(100001)) << "有任务需要执行，但时间未到";

		EXPECT_TRUE(gl_pChinaMarket->ProcessTask(101002)) << "时间到, 执行第一个任务";

		EXPECT_FALSE(gl_pChinaMarket->IsMarketTaskEmpty());

		EXPECT_TRUE(gl_pChinaMarket->ProcessTask(111002)) << "时间到, 执行第二个任务";

		// 恢复原状
		while (!gl_pChinaMarket->IsMarketTaskEmpty()) gl_pChinaMarket->DiscardCurrentMarketTask();
	}

	TEST_F(CChinaMarketTest, TestTaskCreateTask1) {
		EXPECT_TRUE(gl_pChinaMarket->IsMarketTaskEmpty());

		gl_pChinaMarket->TaskCreateTask(10000);

		EXPECT_FALSE(gl_pChinaMarket->IsMarketTaskEmpty());

		CMarketTaskPtr pTask = gl_pChinaMarket->GetMarketTask();
		gl_pChinaMarket->DiscardCurrentMarketTask();
		EXPECT_EQ(pTask->GetType(), CHINA_MARKET_CHECK_SYSTEM_READY__);
		EXPECT_EQ(pTask->GetTime(), 1);

		pTask = gl_pChinaMarket->GetMarketTask();
		gl_pChinaMarket->DiscardCurrentMarketTask();
		EXPECT_EQ(pTask->GetType(), CHINA_MARKET_DISTRIBUTE_AND_CALCULATE_RT_DATA__);
		EXPECT_EQ(pTask->GetTime(), 1);

		pTask = gl_pChinaMarket->GetMarketTask();
		gl_pChinaMarket->DiscardCurrentMarketTask();
		EXPECT_EQ(pTask->GetType(), CHINA_MARKET_PER_MINUTE_ACCESSORY_TASK__);
		EXPECT_EQ(pTask->GetTime(), 10100);

		pTask = gl_pChinaMarket->GetMarketTask();
		gl_pChinaMarket->DiscardCurrentMarketTask();
		EXPECT_EQ(pTask->GetType(), CHINA_MARKET_UPDATE_OPTION_DB__);

		pTask = gl_pChinaMarket->GetMarketTask();
		gl_pChinaMarket->DiscardCurrentMarketTask();
		EXPECT_EQ(pTask->GetType(), CHINA_MARKET_UPDATE_STOCK_PROFILE_DB__);

		pTask = gl_pChinaMarket->GetMarketTask();
		gl_pChinaMarket->DiscardCurrentMarketTask();
		EXPECT_EQ(pTask->GetType(), CHINA_MARKET_RESET__);
		EXPECT_EQ(pTask->GetTime(), 91300);

		pTask = gl_pChinaMarket->GetMarketTask();
		gl_pChinaMarket->DiscardCurrentMarketTask();
		EXPECT_EQ(pTask->GetType(), CHINA_MARKET_CHECK_SYSTEM);
		EXPECT_EQ(pTask->GetTime(), 91800);

		pTask = gl_pChinaMarket->GetMarketTask();
		gl_pChinaMarket->DiscardCurrentMarketTask();
		EXPECT_EQ(pTask->GetType(), CHINA_MARKET_PREPARING_MARKET_OPEN__);
		EXPECT_EQ(pTask->GetTime(), 92959);

		pTask = gl_pChinaMarket->GetMarketTask();
		gl_pChinaMarket->DiscardCurrentMarketTask();
		EXPECT_EQ(pTask->GetType(), CHINA_MARKET_PROCESS_AND_SAVE_DAY_LINE__);
		EXPECT_EQ(pTask->GetTime(), 93000);

		if (gl_pChinaMarket->IsWorkingDay()) {
			pTask = gl_pChinaMarket->GetMarketTask();
			gl_pChinaMarket->DiscardCurrentMarketTask();
			EXPECT_EQ(pTask->GetType(), CHINA_MARKET_BUILD_TODAY_DATABASE__);
			EXPECT_EQ(pTask->GetTime(), 150530);
		}

		pTask = gl_pChinaMarket->GetMarketTask();
		gl_pChinaMarket->DiscardCurrentMarketTask();
		EXPECT_EQ(pTask->GetType(), CHINA_MARKET_CREATE_TASK__);
		EXPECT_EQ(pTask->GetTime(), 240000);

		EXPECT_TRUE(gl_pChinaMarket->IsMarketTaskEmpty());
	}

	TEST_F(CChinaMarketTest, TestTaskCreateTask2) {
		EXPECT_TRUE(gl_pChinaMarket->IsMarketTaskEmpty());

		gl_pChinaMarket->TaskCreateTask(91500);

		EXPECT_FALSE(gl_pChinaMarket->IsMarketTaskEmpty());

		CMarketTaskPtr pTask = gl_pChinaMarket->GetMarketTask();
		gl_pChinaMarket->DiscardCurrentMarketTask();
		EXPECT_EQ(pTask->GetType(), CHINA_MARKET_CHECK_SYSTEM_READY__);
		EXPECT_EQ(pTask->GetTime(), 1);

		pTask = gl_pChinaMarket->GetMarketTask();
		gl_pChinaMarket->DiscardCurrentMarketTask();
		EXPECT_EQ(pTask->GetType(), CHINA_MARKET_DISTRIBUTE_AND_CALCULATE_RT_DATA__);
		EXPECT_EQ(pTask->GetTime(), 1);

		pTask = gl_pChinaMarket->GetMarketTask();
		gl_pChinaMarket->DiscardCurrentMarketTask();
		EXPECT_EQ(pTask->GetType(), CHINA_MARKET_PER_MINUTE_ACCESSORY_TASK__);
		EXPECT_EQ(pTask->GetTime(), 91600);

		pTask = gl_pChinaMarket->GetMarketTask();
		gl_pChinaMarket->DiscardCurrentMarketTask();
		EXPECT_EQ(pTask->GetType(), CHINA_MARKET_CHECK_SYSTEM);
		EXPECT_EQ(pTask->GetTime(), 91800);

		pTask = gl_pChinaMarket->GetMarketTask();
		gl_pChinaMarket->DiscardCurrentMarketTask();
		EXPECT_EQ(pTask->GetType(), CHINA_MARKET_UPDATE_OPTION_DB__);

		pTask = gl_pChinaMarket->GetMarketTask();
		gl_pChinaMarket->DiscardCurrentMarketTask();
		EXPECT_EQ(pTask->GetType(), CHINA_MARKET_UPDATE_STOCK_PROFILE_DB__);

		pTask = gl_pChinaMarket->GetMarketTask();
		gl_pChinaMarket->DiscardCurrentMarketTask();
		EXPECT_EQ(pTask->GetType(), CHINA_MARKET_PREPARING_MARKET_OPEN__);
		EXPECT_EQ(pTask->GetTime(), 92959);

		pTask = gl_pChinaMarket->GetMarketTask();
		gl_pChinaMarket->DiscardCurrentMarketTask();
		EXPECT_EQ(pTask->GetType(), CHINA_MARKET_PROCESS_AND_SAVE_DAY_LINE__);
		EXPECT_EQ(pTask->GetTime(), 93000);

		if (gl_pChinaMarket->IsWorkingDay()) {
			pTask = gl_pChinaMarket->GetMarketTask();
			gl_pChinaMarket->DiscardCurrentMarketTask();
			EXPECT_EQ(pTask->GetType(), CHINA_MARKET_BUILD_TODAY_DATABASE__);
			EXPECT_EQ(pTask->GetTime(), 150530);
		}

		pTask = gl_pChinaMarket->GetMarketTask();
		gl_pChinaMarket->DiscardCurrentMarketTask();
		EXPECT_EQ(pTask->GetType(), CHINA_MARKET_CREATE_TASK__);
		EXPECT_EQ(pTask->GetTime(), 240000);

		EXPECT_TRUE(gl_pChinaMarket->IsMarketTaskEmpty());
	}

	TEST_F(CChinaMarketTest, TestTaskCreateTask3) {
		EXPECT_TRUE(gl_pChinaMarket->IsMarketTaskEmpty());

		gl_pChinaMarket->TaskCreateTask(93000);

		EXPECT_FALSE(gl_pChinaMarket->IsMarketTaskEmpty());

		CMarketTaskPtr pTask = gl_pChinaMarket->GetMarketTask();
		gl_pChinaMarket->DiscardCurrentMarketTask();
		EXPECT_EQ(pTask->GetType(), CHINA_MARKET_CHECK_SYSTEM_READY__);
		EXPECT_EQ(pTask->GetTime(), 1);

		pTask = gl_pChinaMarket->GetMarketTask();
		gl_pChinaMarket->DiscardCurrentMarketTask();
		EXPECT_EQ(pTask->GetType(), CHINA_MARKET_DISTRIBUTE_AND_CALCULATE_RT_DATA__);
		EXPECT_EQ(pTask->GetTime(), 1);

		pTask = gl_pChinaMarket->GetMarketTask();
		gl_pChinaMarket->DiscardCurrentMarketTask();
		EXPECT_EQ(pTask->GetType(), CHINA_MARKET_PROCESS_AND_SAVE_DAY_LINE__);
		EXPECT_EQ(pTask->GetTime(), 93000);

		pTask = gl_pChinaMarket->GetMarketTask();
		gl_pChinaMarket->DiscardCurrentMarketTask();
		EXPECT_EQ(pTask->GetType(), CHINA_MARKET_PER_MINUTE_ACCESSORY_TASK__);
		EXPECT_EQ(pTask->GetTime(), 93100);

		pTask = gl_pChinaMarket->GetMarketTask();
		gl_pChinaMarket->DiscardCurrentMarketTask();
		EXPECT_EQ(pTask->GetType(), CHINA_MARKET_UPDATE_OPTION_DB__);
		EXPECT_EQ(pTask->GetTime(), 93305);

		pTask = gl_pChinaMarket->GetMarketTask();
		gl_pChinaMarket->DiscardCurrentMarketTask();
		EXPECT_EQ(pTask->GetType(), CHINA_MARKET_UPDATE_STOCK_PROFILE_DB__);
		EXPECT_EQ(pTask->GetTime(), 93410);

		if (gl_pChinaMarket->IsWorkingDay()) {
			pTask = gl_pChinaMarket->GetMarketTask();
			gl_pChinaMarket->DiscardCurrentMarketTask();
			EXPECT_EQ(pTask->GetType(), CHINA_MARKET_BUILD_TODAY_DATABASE__);
			EXPECT_EQ(pTask->GetTime(), 150530);
		}

		pTask = gl_pChinaMarket->GetMarketTask();
		gl_pChinaMarket->DiscardCurrentMarketTask();
		EXPECT_EQ(pTask->GetType(), CHINA_MARKET_CREATE_TASK__);
		EXPECT_EQ(pTask->GetTime(), 240000);

		EXPECT_TRUE(gl_pChinaMarket->IsMarketTaskEmpty());
	}

	TEST_F(CChinaMarketTest, TestTaskCreateTask4) {
		tm tm_{};
		tm_.tm_year = 2019 - 1900;
		tm_.tm_mon = 10;
		tm_.tm_mday = 7; // 2019年11月9日是星期四。
		tm_.tm_wday = 4;
		tm_.tm_hour = 16;
		tm_.tm_min = 0;
		tm_.tm_sec = 0;
		gl_pChinaMarket->TEST_SetMarketTM(tm_);

		EXPECT_TRUE(gl_pChinaMarket->IsMarketTaskEmpty());

		gl_pChinaMarket->TaskCreateTask(150601);

		EXPECT_FALSE(gl_pChinaMarket->IsMarketTaskEmpty());

		CMarketTaskPtr pTask = gl_pChinaMarket->GetMarketTask();
		gl_pChinaMarket->DiscardCurrentMarketTask();
		EXPECT_EQ(pTask->GetType(), CHINA_MARKET_CHECK_SYSTEM_READY__);
		EXPECT_EQ(pTask->GetTime(), 1);

		pTask = gl_pChinaMarket->GetMarketTask();
		gl_pChinaMarket->DiscardCurrentMarketTask();
		EXPECT_EQ(pTask->GetType(), CHINA_MARKET_DISTRIBUTE_AND_CALCULATE_RT_DATA__);
		EXPECT_EQ(pTask->GetTime(), 1);

		pTask = gl_pChinaMarket->GetMarketTask();
		gl_pChinaMarket->DiscardCurrentMarketTask();
		EXPECT_EQ(pTask->GetType(), CHINA_MARKET_PROCESS_AND_SAVE_DAY_LINE__);
		EXPECT_EQ(pTask->GetTime(), 93000);

		pTask = gl_pChinaMarket->GetMarketTask();
		gl_pChinaMarket->DiscardCurrentMarketTask();
		EXPECT_EQ(pTask->GetType(), CHINA_MARKET_PER_MINUTE_ACCESSORY_TASK__);
		EXPECT_EQ(pTask->GetTime(), 150700);

		pTask = gl_pChinaMarket->GetMarketTask();
		gl_pChinaMarket->DiscardCurrentMarketTask();
		EXPECT_EQ(pTask->GetType(), CHINA_MARKET_UPDATE_OPTION_DB__);
		EXPECT_EQ(pTask->GetTime(), 150905);

		pTask = gl_pChinaMarket->GetMarketTask();
		gl_pChinaMarket->DiscardCurrentMarketTask();
		EXPECT_EQ(pTask->GetType(), CHINA_MARKET_VALIDATE_TODAY_DATABASE__);
		EXPECT_EQ(pTask->GetTime(), 151000);

		pTask = gl_pChinaMarket->GetMarketTask();
		gl_pChinaMarket->DiscardCurrentMarketTask();
		EXPECT_EQ(pTask->GetType(), CHINA_MARKET_UPDATE_STOCK_PROFILE_DB__);
		EXPECT_EQ(pTask->GetTime(), 151010);

		pTask = gl_pChinaMarket->GetMarketTask();
		gl_pChinaMarket->DiscardCurrentMarketTask();
		EXPECT_EQ(pTask->GetType(), CHINA_MARKET_CREATE_TASK__);
		EXPECT_EQ(pTask->GetTime(), 240000);

		EXPECT_TRUE(gl_pChinaMarket->IsMarketTaskEmpty()) << gl_pChinaMarket->GetMarketTask()->GetTime();
	}

	TEST_F(CChinaMarketTest, TestTaskCreateTask5) {
		tm tm_{};
		tm_.tm_year = 2019 - 1900;
		tm_.tm_mon = 10;
		tm_.tm_mday = 9; // 2019年11月9日是星期六。
		tm_.tm_wday = 6;
		tm_.tm_hour = 16;
		tm_.tm_min = 0;
		tm_.tm_sec = 0;
		gl_pChinaMarket->TEST_SetMarketTM(tm_);

		EXPECT_TRUE(gl_pChinaMarket->IsMarketTaskEmpty());

		gl_pChinaMarket->TaskCreateTask(150601);

		EXPECT_FALSE(gl_pChinaMarket->IsMarketTaskEmpty());

		CMarketTaskPtr pTask = gl_pChinaMarket->GetMarketTask();
		gl_pChinaMarket->DiscardCurrentMarketTask();
		EXPECT_EQ(pTask->GetType(), CHINA_MARKET_CHECK_SYSTEM_READY__);
		EXPECT_EQ(pTask->GetTime(), 1);

		pTask = gl_pChinaMarket->GetMarketTask();
		gl_pChinaMarket->DiscardCurrentMarketTask();
		EXPECT_EQ(pTask->GetType(), CHINA_MARKET_DISTRIBUTE_AND_CALCULATE_RT_DATA__);
		EXPECT_EQ(pTask->GetTime(), 1);

		pTask = gl_pChinaMarket->GetMarketTask();
		gl_pChinaMarket->DiscardCurrentMarketTask();
		EXPECT_EQ(pTask->GetType(), CHINA_MARKET_PROCESS_AND_SAVE_DAY_LINE__);
		EXPECT_EQ(pTask->GetTime(), 93000);

		pTask = gl_pChinaMarket->GetMarketTask();
		gl_pChinaMarket->DiscardCurrentMarketTask();
		EXPECT_EQ(pTask->GetType(), CHINA_MARKET_PER_MINUTE_ACCESSORY_TASK__);
		EXPECT_EQ(pTask->GetTime(), 150700);

		pTask = gl_pChinaMarket->GetMarketTask();
		gl_pChinaMarket->DiscardCurrentMarketTask();
		EXPECT_EQ(pTask->GetType(), CHINA_MARKET_UPDATE_OPTION_DB__);

		pTask = gl_pChinaMarket->GetMarketTask();
		gl_pChinaMarket->DiscardCurrentMarketTask();
		EXPECT_EQ(pTask->GetType(), CHINA_MARKET_UPDATE_STOCK_PROFILE_DB__);

		pTask = gl_pChinaMarket->GetMarketTask();
		gl_pChinaMarket->DiscardCurrentMarketTask();
		EXPECT_EQ(pTask->GetType(), CHINA_MARKET_CREATE_TASK__);
		EXPECT_EQ(pTask->GetTime(), 240000);

		EXPECT_TRUE(gl_pChinaMarket->IsMarketTaskEmpty()) << gl_pChinaMarket->GetMarketTask()->GetTime();
	}

	TEST_F(CChinaMarketTest, TestTaskCreateTask6) {
		tm tm_{};
		tm_.tm_year = 2019 - 1900;
		tm_.tm_mon = 10;
		tm_.tm_mday = 10; // 2019年11月10日是星期日。
		tm_.tm_wday = 0;
		tm_.tm_hour = 16;
		tm_.tm_min = 0;
		tm_.tm_sec = 0;
		gl_pChinaMarket->TEST_SetMarketTM(tm_);

		EXPECT_TRUE(gl_pChinaMarket->IsMarketTaskEmpty());
		EXPECT_FALSE(gl_systemConfiguration.IsReloadSystem()); // 不自动重启系统

		gl_pChinaMarket->TaskCreateTask(150601);

		EXPECT_FALSE(gl_pChinaMarket->IsMarketTaskEmpty());

		CMarketTaskPtr pTask = gl_pChinaMarket->GetMarketTask();
		gl_pChinaMarket->DiscardCurrentMarketTask();
		EXPECT_EQ(pTask->GetType(), CHINA_MARKET_CHECK_SYSTEM_READY__);
		EXPECT_EQ(pTask->GetTime(), 1);

		pTask = gl_pChinaMarket->GetMarketTask();
		gl_pChinaMarket->DiscardCurrentMarketTask();
		EXPECT_EQ(pTask->GetType(), CHINA_MARKET_DISTRIBUTE_AND_CALCULATE_RT_DATA__);
		EXPECT_EQ(pTask->GetTime(), 1);

		pTask = gl_pChinaMarket->GetMarketTask();
		gl_pChinaMarket->DiscardCurrentMarketTask();
		EXPECT_EQ(pTask->GetType(), CHINA_MARKET_PROCESS_AND_SAVE_DAY_LINE__);
		EXPECT_EQ(pTask->GetTime(), 93000);

		pTask = gl_pChinaMarket->GetMarketTask();
		gl_pChinaMarket->DiscardCurrentMarketTask();
		EXPECT_EQ(pTask->GetType(), CHINA_MARKET_PER_MINUTE_ACCESSORY_TASK__);
		EXPECT_EQ(pTask->GetTime(), 150700);

		pTask = gl_pChinaMarket->GetMarketTask();
		gl_pChinaMarket->DiscardCurrentMarketTask();
		EXPECT_EQ(pTask->GetType(), CHINA_MARKET_UPDATE_OPTION_DB__);

		pTask = gl_pChinaMarket->GetMarketTask();
		gl_pChinaMarket->DiscardCurrentMarketTask();
		EXPECT_EQ(pTask->GetType(), CHINA_MARKET_UPDATE_STOCK_PROFILE_DB__);

		pTask = gl_pChinaMarket->GetMarketTask();
		gl_pChinaMarket->DiscardCurrentMarketTask();
		EXPECT_EQ(pTask->GetType(), CHINA_MARKET_CREATE_TASK__);
		EXPECT_EQ(pTask->GetTime(), 240000);

		EXPECT_TRUE(gl_pChinaMarket->IsMarketTaskEmpty()) << gl_pChinaMarket->GetMarketTask()->GetTime();
	}

	TEST_F(CChinaMarketTest, TestTaskCreateTask7) {
		tm tm_{};
		tm_.tm_year = 2019 - 1900;
		tm_.tm_mon = 10;
		tm_.tm_mday = 9; // 2019年11月9日是星期六。
		tm_.tm_wday = 6;
		tm_.tm_hour = 16;
		tm_.tm_min = 0;
		tm_.tm_sec = 0;
		gl_pChinaMarket->TEST_SetMarketTM(tm_);

		EXPECT_TRUE(gl_pChinaMarket->IsMarketTaskEmpty());
		EXPECT_FALSE(gl_systemConfiguration.IsReloadSystem()); // 不自动重启系统
		gl_systemConfiguration.SetReloadSystem(true); // 每周星期日自动重启系统

		gl_pChinaMarket->TaskCreateTask(150601);

		EXPECT_FALSE(gl_pChinaMarket->IsMarketTaskEmpty());

		CMarketTaskPtr pTask = gl_pChinaMarket->GetMarketTask();
		gl_pChinaMarket->DiscardCurrentMarketTask();
		EXPECT_EQ(pTask->GetType(), CHINA_MARKET_CHECK_SYSTEM_READY__);
		EXPECT_EQ(pTask->GetTime(), 1);

		pTask = gl_pChinaMarket->GetMarketTask();
		gl_pChinaMarket->DiscardCurrentMarketTask();
		EXPECT_EQ(pTask->GetType(), CHINA_MARKET_DISTRIBUTE_AND_CALCULATE_RT_DATA__);
		EXPECT_EQ(pTask->GetTime(), 1);

		pTask = gl_pChinaMarket->GetMarketTask();
		gl_pChinaMarket->DiscardCurrentMarketTask();
		EXPECT_EQ(pTask->GetType(), CHINA_MARKET_PROCESS_AND_SAVE_DAY_LINE__);
		EXPECT_EQ(pTask->GetTime(), 93000);

		pTask = gl_pChinaMarket->GetMarketTask();
		gl_pChinaMarket->DiscardCurrentMarketTask();
		EXPECT_EQ(pTask->GetType(), CHINA_MARKET_PER_MINUTE_ACCESSORY_TASK__);
		EXPECT_EQ(pTask->GetTime(), 150700);

		pTask = gl_pChinaMarket->GetMarketTask();
		gl_pChinaMarket->DiscardCurrentMarketTask();
		EXPECT_EQ(pTask->GetType(), CHINA_MARKET_UPDATE_OPTION_DB__);

		pTask = gl_pChinaMarket->GetMarketTask();
		gl_pChinaMarket->DiscardCurrentMarketTask();
		EXPECT_EQ(pTask->GetType(), CHINA_MARKET_UPDATE_STOCK_PROFILE_DB__);

		pTask = gl_pChinaMarket->GetMarketTask();
		gl_pChinaMarket->DiscardCurrentMarketTask();
		EXPECT_EQ(pTask->GetType(), CHINA_MARKET_CREATE_TASK__);
		EXPECT_EQ(pTask->GetTime(), 240000);

		EXPECT_TRUE(gl_pChinaMarket->IsMarketTaskEmpty()) << gl_pChinaMarket->GetMarketTask()->GetTime();
		gl_systemConfiguration.SetReloadSystem(false);
	}

	TEST_F(CChinaMarketTest, TestTaskCreateTask8) {
		tm tm_{};
		tm_.tm_year = 2019 - 1900;
		tm_.tm_mon = 10;
		tm_.tm_mday = 10; // 2019年11月10日是星期日。
		tm_.tm_wday = 0;
		tm_.tm_hour = 16;
		tm_.tm_min = 0;
		tm_.tm_sec = 0;
		gl_pChinaMarket->TEST_SetMarketTM(tm_);

		EXPECT_TRUE(gl_pChinaMarket->IsMarketTaskEmpty());
		EXPECT_FALSE(gl_systemConfiguration.IsReloadSystem()); // 不自动重启系统
		gl_systemConfiguration.SetReloadSystem(true); // 每周星期日自动重启系统

		gl_pChinaMarket->TaskCreateTask(150601);

		EXPECT_FALSE(gl_pChinaMarket->IsMarketTaskEmpty());

		CMarketTaskPtr pTask = gl_pChinaMarket->GetMarketTask();
		gl_pChinaMarket->DiscardCurrentMarketTask();
		EXPECT_EQ(pTask->GetType(), CHINA_MARKET_CHECK_SYSTEM_READY__);
		EXPECT_EQ(pTask->GetTime(), 1);

		pTask = gl_pChinaMarket->GetMarketTask();
		gl_pChinaMarket->DiscardCurrentMarketTask();
		EXPECT_EQ(pTask->GetType(), CHINA_MARKET_DISTRIBUTE_AND_CALCULATE_RT_DATA__);
		EXPECT_EQ(pTask->GetTime(), 1);

		pTask = gl_pChinaMarket->GetMarketTask();
		gl_pChinaMarket->DiscardCurrentMarketTask();
		EXPECT_EQ(pTask->GetType(), CHINA_MARKET_PROCESS_AND_SAVE_DAY_LINE__);
		EXPECT_EQ(pTask->GetTime(), 93000);

		pTask = gl_pChinaMarket->GetMarketTask();
		gl_pChinaMarket->DiscardCurrentMarketTask();
		EXPECT_EQ(pTask->GetType(), CHINA_MARKET_PER_MINUTE_ACCESSORY_TASK__);
		EXPECT_EQ(pTask->GetTime(), 150700);

		pTask = gl_pChinaMarket->GetMarketTask();
		gl_pChinaMarket->DiscardCurrentMarketTask();
		EXPECT_EQ(pTask->GetType(), CHINA_MARKET_UPDATE_OPTION_DB__);
		EXPECT_EQ(pTask->GetTime(), 150905);

		pTask = gl_pChinaMarket->GetMarketTask();
		gl_pChinaMarket->DiscardCurrentMarketTask();
		EXPECT_EQ(pTask->GetType(), CHINA_MARKET_UPDATE_STOCK_PROFILE_DB__);
		EXPECT_EQ(pTask->GetTime(), 151010);

		pTask = gl_pChinaMarket->GetMarketTask();
		gl_pChinaMarket->DiscardCurrentMarketTask();
		EXPECT_EQ(pTask->GetType(), RELOAD_SYSTEM__) << "当前时间小于210000时，星期日自动重启系统";
		EXPECT_EQ(pTask->GetTime(), 210000) << "重启系统于晚上9点";

		pTask = gl_pChinaMarket->GetMarketTask();
		gl_pChinaMarket->DiscardCurrentMarketTask();
		EXPECT_EQ(pTask->GetType(), CHINA_MARKET_CREATE_TASK__);
		EXPECT_EQ(pTask->GetTime(), 240000);

		EXPECT_TRUE(gl_pChinaMarket->IsMarketTaskEmpty()) << gl_pChinaMarket->GetMarketTask()->GetTime();

		// 恢复原状
		gl_systemConfiguration.SetReloadSystem(false);
	}

	TEST_F(CChinaMarketTest, TestTaskCreateTask9) {
		tm tm_{};
		tm_.tm_year = 2019 - 1900;
		tm_.tm_mon = 10;
		tm_.tm_mday = 10; // 2019年11月10日是星期日。
		tm_.tm_wday = 0;
		tm_.tm_hour = 16;
		tm_.tm_min = 0;
		tm_.tm_sec = 0;
		gl_pChinaMarket->TEST_SetMarketTM(tm_);

		EXPECT_TRUE(gl_pChinaMarket->IsMarketTaskEmpty());
		EXPECT_FALSE(gl_systemConfiguration.IsReloadSystem()); // 不自动重启系统
		gl_systemConfiguration.SetReloadSystem(true); // 每周星期日自动重启系统

		gl_pChinaMarket->TaskCreateTask(210000); // 当前时间不早于210000，不设置自动重启系统任务

		EXPECT_FALSE(gl_pChinaMarket->IsMarketTaskEmpty());

		CMarketTaskPtr pTask = gl_pChinaMarket->GetMarketTask();
		gl_pChinaMarket->DiscardCurrentMarketTask();
		EXPECT_EQ(pTask->GetType(), CHINA_MARKET_CHECK_SYSTEM_READY__);
		EXPECT_EQ(pTask->GetTime(), 1);

		pTask = gl_pChinaMarket->GetMarketTask();
		gl_pChinaMarket->DiscardCurrentMarketTask();
		EXPECT_EQ(pTask->GetType(), CHINA_MARKET_DISTRIBUTE_AND_CALCULATE_RT_DATA__);
		EXPECT_EQ(pTask->GetTime(), 1);

		pTask = gl_pChinaMarket->GetMarketTask();
		gl_pChinaMarket->DiscardCurrentMarketTask();
		EXPECT_EQ(pTask->GetType(), CHINA_MARKET_PROCESS_AND_SAVE_DAY_LINE__);
		EXPECT_EQ(pTask->GetTime(), 93000);

		pTask = gl_pChinaMarket->GetMarketTask();
		gl_pChinaMarket->DiscardCurrentMarketTask();
		EXPECT_EQ(pTask->GetType(), CHINA_MARKET_PER_MINUTE_ACCESSORY_TASK__);
		EXPECT_EQ(pTask->GetTime(), 210100);

		pTask = gl_pChinaMarket->GetMarketTask();
		gl_pChinaMarket->DiscardCurrentMarketTask();
		EXPECT_EQ(pTask->GetType(), CHINA_MARKET_UPDATE_OPTION_DB__);
		EXPECT_EQ(pTask->GetTime(), 210305);

		pTask = gl_pChinaMarket->GetMarketTask();
		gl_pChinaMarket->DiscardCurrentMarketTask();
		EXPECT_EQ(pTask->GetType(), CHINA_MARKET_UPDATE_STOCK_PROFILE_DB__);
		EXPECT_EQ(pTask->GetTime(), 210410);

		pTask = gl_pChinaMarket->GetMarketTask();
		gl_pChinaMarket->DiscardCurrentMarketTask();
		EXPECT_EQ(pTask->GetType(), CHINA_MARKET_CREATE_TASK__) << "当前时间不早于210000，不设置重启系统任务";
		EXPECT_EQ(pTask->GetTime(), 240000);

		EXPECT_TRUE(gl_pChinaMarket->IsMarketTaskEmpty()) << gl_pChinaMarket->GetMarketTask()->GetTime();

		// 恢复原状
		gl_systemConfiguration.SetReloadSystem(false);
	}

	TEST_F(CChinaMarketTest, TestClearUpdateStockCodeDBFlag) {
		EXPECT_FALSE(gl_dataContainerChinaStock.IsDayLineDBUpdated());
		for (size_t i = 0; i < gl_dataContainerChinaStock.Size(); i++) {
			const CChinaStockPtr pStock = gl_dataContainerChinaStock.GetStock(i);
			pStock->SetDayLineDBUpdated(true);
		}
		EXPECT_TRUE(gl_dataContainerChinaStock.IsDayLineDBUpdated());
		gl_dataContainerChinaStock.ClearDayLineDBUpdatedFlag();
		EXPECT_FALSE(gl_dataContainerChinaStock.IsDayLineDBUpdated());
	}

	TEST_F(CChinaMarketTest, TestGetNeteaseDayLineInquiringStr) {
		CChinaStockPtr pStock = gl_dataContainerChinaStock.GetStock(0);
		EXPECT_TRUE(pStock->IsUpdateDayLine()) << "测试时使用testStock数据库，此数据库比较旧，最后更新时间不是昨日，故而活跃股票也需要更新日线";

		pStock->SetUpdateDayLine(false);
		pStock = gl_dataContainerChinaStock.GetStock(1);
		EXPECT_TRUE(pStock->IsUpdateDayLine());
		EXPECT_LT(pStock->GetDayLineEndDate(), gl_pChinaMarket->GetLastTradeDate()) << pStock->GetDayLineEndDate();
		pStock = gl_dataContainerChinaStock.GetStock(2);
		EXPECT_TRUE(pStock->IsUpdateDayLine());
		EXPECT_LT(pStock->GetDayLineEndDate(), gl_pChinaMarket->GetLastTradeDate());
		const long lDate = pStock->GetDayLineEndDate();
		pStock->SetDayLineEndDate(gl_pChinaMarket->GetMarketDate());
		string str = gl_dataContainerChinaStock.CreateNeteaseDayLineInquiringStr();
		EXPECT_EQ(str, "1000001");
		pStock = gl_dataContainerChinaStock.GetStock(1);
		EXPECT_FALSE(pStock->IsUpdateDayLine());
		str = gl_dataContainerChinaStock.CreateNeteaseDayLineInquiringStr();
		EXPECT_EQ(str, "1000002");

		gl_dataContainerChinaStock.GetStock(2)->SetDayLineEndDate(lDate); // 恢复原状。
	}

	TEST_F(CChinaMarketTest, TestGetMinLineOffset) {
		tm tmMarketTime;
		tmMarketTime.tm_year = 2020 - 1900;
		tmMarketTime.tm_mon = 1;
		tmMarketTime.tm_mday = 1;
		tmMarketTime.tm_hour = 9;
		tmMarketTime.tm_min = 0;
		tmMarketTime.tm_sec = 0;
		time_t tUTC = gl_pChinaMarket->TransferToUTCTime(&tmMarketTime);
		auto lOffset = gl_pChinaMarket->GetMinLineOffset(tUTC);
		EXPECT_EQ(lOffset, 0);
		tmMarketTime.tm_hour = 10;
		tmMarketTime.tm_min = 30;
		tmMarketTime.tm_sec = 59;
		tUTC = gl_pChinaMarket->TransferToUTCTime(&tmMarketTime);
		lOffset = gl_pChinaMarket->GetMinLineOffset(tUTC);
		EXPECT_EQ(lOffset, 60);
		tmMarketTime.tm_hour = 12;
		tmMarketTime.tm_min = 30;
		tmMarketTime.tm_sec = 59;
		tUTC = gl_pChinaMarket->TransferToUTCTime(&tmMarketTime);
		lOffset = gl_pChinaMarket->GetMinLineOffset(tUTC);
		EXPECT_EQ(lOffset, 119);
		tmMarketTime.tm_hour = 14;
		tmMarketTime.tm_min = 30;
		tmMarketTime.tm_sec = 59;
		tUTC = gl_pChinaMarket->TransferToUTCTime(&tmMarketTime);
		lOffset = gl_pChinaMarket->GetMinLineOffset(tUTC);
		EXPECT_EQ(lOffset, 210);
		tmMarketTime.tm_hour = 15;
		tmMarketTime.tm_min = 30;
		tmMarketTime.tm_sec = 59;
		tUTC = gl_pChinaMarket->TransferToUTCTime(&tmMarketTime);
		lOffset = gl_pChinaMarket->GetMinLineOffset(tUTC);
		EXPECT_EQ(lOffset, 239);
	}

	TEST_F(CChinaMarketTest, TestIsStock) {
		EXPECT_GT(gl_dataContainerChinaStock.Size(), 1);
		EXPECT_TRUE(gl_dataContainerChinaStock.IsSymbol("600000.SS"));
		EXPECT_FALSE(gl_dataContainerChinaStock.IsSymbol("60000.SS"));
	}

	TEST_F(CChinaMarketTest, TestGetStockName) {
		//not implemented. 由于stockName存储时使用的是UniCode制式，而本系统默认是Ansi制式，导致无法进行字符串对比。暂时不进行测试了。
		// EXPECT_STREQ(gl_pChinaMarket->GetStockName("600000.SS"), "浦发银行");
		EXPECT_EQ(gl_dataContainerChinaStock.GetStockName("60000.SS"), ""); // 没找到时返回空字符串
	}

	TEST_F(CChinaMarketTest, TestGetStockCode) {
		EXPECT_TRUE(gl_dataContainerChinaStock.IsSymbol("600001.SS"));
		EXPECT_FALSE(gl_dataContainerChinaStock.GetStock("600001.SS") == nullptr);

		EXPECT_FALSE(gl_dataContainerChinaStock.GetStock(0) == nullptr);
		EXPECT_FALSE(gl_dataContainerChinaStock.GetStock(gl_dataContainerChinaStock.Size() - 1) == nullptr);
	}

	TEST_F(CChinaMarketTest, TestMarketReady) {
		EXPECT_TRUE(gl_pChinaMarket->IsSystemReady()) << "中国股票市场默认为假, 但测试系统默认为真";
		gl_pChinaMarket->SetSystemReady(true);
		EXPECT_TRUE(gl_pChinaMarket->IsSystemReady());
		gl_pChinaMarket->SetSystemReady(false);
		EXPECT_FALSE(gl_pChinaMarket->IsSystemReady());

		gl_pChinaMarket->SetSystemReady(true); // 恢复原态
	}

	TEST_F(CChinaMarketTest, TestGetCurrentSelectedPosition) {
		EXPECT_EQ(gl_pChinaMarket->GetCurrentSelectedPosition(), 0);
		gl_pChinaMarket->SetCurrentSelectedPosition(10101010);
		EXPECT_EQ(gl_pChinaMarket->GetCurrentSelectedPosition(), 10101010);

		gl_pChinaMarket->SetCurrentSelectedPosition(0);
	}

	TEST_F(CChinaMarketTest, TestGetCurrentSelectedStockSet) {
		EXPECT_EQ(gl_pChinaMarket->GetCurrentSelectedStockSet(), -1);
		gl_pChinaMarket->SetCurrentSelectedStockSet(10);
		EXPECT_EQ(gl_pChinaMarket->GetCurrentSelectedStockSet(), 10);

		gl_pChinaMarket->SetCurrentSelectedStockSet(-1);
	}

	TEST_F(CChinaMarketTest, TestIsChosen10RSStrongStockSet) {
		gl_pChinaMarket->SetChosen10RSStrongStockSet(true);
		EXPECT_TRUE(gl_pChinaMarket->IsChosen10RSStrongStockSet());
		gl_pChinaMarket->SetChosen10RSStrongStockSet(false);
		EXPECT_FALSE(gl_pChinaMarket->IsChosen10RSStrongStockSet());
	}

	TEST_F(CChinaMarketTest, TestIsChosen10RSStrong1StockSet) {
		gl_pChinaMarket->SetChosen10RSStrong1StockSet(true);
		EXPECT_TRUE(gl_pChinaMarket->IsChosen10RSStrong1StockSet());
		gl_pChinaMarket->SetChosen10RSStrong1StockSet(false);
		EXPECT_FALSE(gl_pChinaMarket->IsChosen10RSStrong1StockSet());
	}

	TEST_F(CChinaMarketTest, TestIsUpdateDayLineDB) {
		EXPECT_FALSE(gl_dataContainerChinaStock.IsUpdateDayLineDB());
		const CChinaStockPtr pStock = gl_dataContainerChinaStock.GetStock(0);
		pStock->SetUpdateDayLineDB(true);
		EXPECT_TRUE(gl_dataContainerChinaStock.IsUpdateDayLineDB());
		pStock->SetUpdateDayLineDB(false);
	}

	TEST_F(CChinaMarketTest, TestIsUpdateDayLine) {
		CChinaStockPtr pStock;
		EXPECT_TRUE(gl_dataContainerChinaStock.IsUpdateDayLine());
		for (size_t i = 0; i < gl_dataContainerChinaStock.Size(); i++) {
			pStock = gl_dataContainerChinaStock.GetStock(i);
			if (pStock->IsUpdateDayLine()) pStock->SetUpdateDayLine(false);
		}
		EXPECT_FALSE(gl_dataContainerChinaStock.IsUpdateDayLine());
		for (size_t i = 0; i < gl_dataContainerChinaStock.Size(); i++) {
			pStock = gl_dataContainerChinaStock.GetStock(i);
			if (!pStock->IsUpdateDayLine()) pStock->SetUpdateDayLine(true);
		}
	}

	TEST_F(CChinaMarketTest, TestIsDayLineNeedProcess) {
		EXPECT_FALSE(gl_pChinaMarket->IsDayLineNeedProcess()) << "默认状态下无需处理";
		CDayLineWebDataPtr pData = make_shared<CDayLineWebData>();
		gl_qDayLine.enqueue(pData);

		EXPECT_TRUE(gl_pChinaMarket->IsDayLineNeedProcess());

		gl_qDayLine.try_dequeue(pData);
		EXPECT_FALSE(gl_pChinaMarket->IsDayLineNeedProcess());
	}

	TEST_F(CChinaMarketTest, TestProcessDayLine) {
		const CDayLineWebDataPtr pData = make_shared<CDayLineWebData>();
		CChinaStockPtr pStock = gl_dataContainerChinaStock.GetStock("600666.SS");

		pData->SetStockCode("600666.SS");
		gl_qDayLine.enqueue(pData);

		EXPECT_TRUE(gl_pChinaMarket->ProcessDayLine());
	}

	TEST_F(CChinaMarketTest, TestIsSelectedStockLoaded) {
		EXPECT_FALSE(gl_pChinaMarket->IsSelectedStockLoaded());
		gl_pChinaMarket->SetSelectedStockLoaded(true);
		EXPECT_TRUE(gl_pChinaMarket->IsSelectedStockLoaded());
		gl_pChinaMarket->SetSelectedStockLoaded(false);
		EXPECT_FALSE(gl_pChinaMarket->IsSelectedStockLoaded());
	}

	TEST_F(CChinaMarketTest, TestSetUpdateOptionDB) {
		EXPECT_FALSE(gl_pChinaMarket->IsUpdateOptionDB());
		gl_pChinaMarket->SetUpdateOptionDB(true);
		EXPECT_TRUE(gl_pChinaMarket->IsUpdateOptionDB());
		gl_pChinaMarket->SetUpdateOptionDB(false);
		EXPECT_FALSE(gl_pChinaMarket->IsUpdateOptionDB());
	}

	TEST_F(CChinaMarketTest, TestSetUpdateChosenStockDB) {
		EXPECT_FALSE(gl_pChinaMarket->IsUpdateChosenStockDB());
		gl_pChinaMarket->SetUpdateChosenStockDB(true);
		EXPECT_TRUE(gl_pChinaMarket->IsUpdateChosenStockDB());
		gl_pChinaMarket->SetUpdateChosenStockDB(false);
		EXPECT_FALSE(gl_pChinaMarket->IsUpdateChosenStockDB());
	}

	TEST_F(CChinaMarketTest, TestIsRTDataSetCleared) {
		EXPECT_FALSE(gl_pChinaMarket->IsRTDataSetCleared());
		gl_pChinaMarket->SetRTDataSetCleared(true);
		EXPECT_TRUE(gl_pChinaMarket->IsRTDataSetCleared());
		gl_pChinaMarket->SetRTDataSetCleared(false);
		EXPECT_FALSE(gl_pChinaMarket->IsRTDataSetCleared());
	}

	TEST_F(CChinaMarketTest, TestIsUpdateTempDataDB) {
		EXPECT_TRUE(gl_pChinaMarket->IsUpdateTempDataDB());
		gl_pChinaMarket->SetUpdateTempDataDB(false);
		EXPECT_FALSE(gl_pChinaMarket->IsUpdateTempDataDB());
		gl_pChinaMarket->SetUpdateTempDataDB(true);
		EXPECT_TRUE(gl_pChinaMarket->IsUpdateTempDataDB());
	}

	TEST_F(CChinaMarketTest, TestIsUpdateStockProfileDB) {
		EXPECT_THAT(gl_dataContainerChinaStock.IsUpdateProfileDB(), IsFalse());
		gl_dataContainerChinaStock.GetStock(1)->SetUpdateProfileDB(true);
		EXPECT_THAT(gl_dataContainerChinaStock.IsUpdateProfileDB(), IsTrue());

		gl_dataContainerChinaStock.GetStock(1)->SetUpdateProfileDB(false);
	}

	TEST_F(CChinaMarketTest, TestAddChosenStock) {
		auto pStock = gl_dataContainerChinaStock.GetStock(1);
		gl_pChinaMarket->AddChosenStock(pStock);
		EXPECT_EQ(gl_pChinaMarket->GetChosenStockSize(), 1);
		pStock = gl_dataContainerChinaStock.GetStock(2);
		EXPECT_TRUE(gl_pChinaMarket->AddChosenStock(pStock));
		EXPECT_EQ(gl_pChinaMarket->GetChosenStockSize(), 2);
		pStock = gl_dataContainerChinaStock.GetStock(1);
		EXPECT_FALSE(gl_pChinaMarket->AddChosenStock(pStock));
		EXPECT_EQ(gl_pChinaMarket->GetChosenStockSize(), 2);

		EXPECT_TRUE(gl_pChinaMarket->DeleteChosenStock(pStock));
		pStock = gl_dataContainerChinaStock.GetStock(5);
		EXPECT_EQ(gl_pChinaMarket->GetChosenStockSize(), 1);
		EXPECT_FALSE(gl_pChinaMarket->DeleteChosenStock(pStock));
		gl_pChinaMarket->ClearChoiceStockContainer();
	}

	TEST_F(CChinaMarketTest, TestAddChosenStock2) {
		EXPECT_EQ(gl_pChinaMarket->GetChosenStockSize(), 0);
		const CChinaStockPtr pStock = gl_dataContainerChinaStock.GetStock(0);
		gl_pChinaMarket->AddChosenStock(pStock);
		EXPECT_EQ(gl_pChinaMarket->GetChosenStockSize(), 1);
		gl_pChinaMarket->ClearChoiceStockContainer();
		EXPECT_EQ(gl_pChinaMarket->GetChosenStockSize(), 0);
	}

	TEST_F(CChinaMarketTest, TestCheckMarketOpen) {
		tm tm_{};
		tm_.tm_wday = 1;
		gl_pChinaMarket->TEST_SetMarketTM(tm_);
		EXPECT_FALSE(gl_pChinaMarket->CheckMarketOpen(92759));
		EXPECT_FALSE(gl_pChinaMarket->IsMarketOpened());
		EXPECT_TRUE(gl_pChinaMarket->CheckMarketOpen(92800));
		EXPECT_TRUE(gl_pChinaMarket->IsMarketOpened());
		EXPECT_TRUE(gl_pChinaMarket->CheckMarketOpen(150559));
		EXPECT_TRUE(gl_pChinaMarket->IsMarketOpened());
		EXPECT_FALSE(gl_pChinaMarket->CheckMarketOpen(150600));
		tm_.tm_wday = 0;
		gl_pChinaMarket->TEST_SetMarketTM(tm_);
		EXPECT_FALSE(gl_pChinaMarket->CheckMarketOpen(92859));
		EXPECT_FALSE(gl_pChinaMarket->IsMarketOpened());
		EXPECT_FALSE(gl_pChinaMarket->CheckMarketOpen(92900));
		EXPECT_FALSE(gl_pChinaMarket->IsMarketOpened());
		EXPECT_FALSE(gl_pChinaMarket->CheckMarketOpen(150559));
		EXPECT_FALSE(gl_pChinaMarket->IsMarketOpened());
		EXPECT_FALSE(gl_pChinaMarket->CheckMarketOpen(150600));
		EXPECT_FALSE(gl_pChinaMarket->IsMarketOpened());
	}

	TEST_F(CChinaMarketTest, TestCheckFastReceivingRTData) {
		EXPECT_FALSE(gl_systemConfiguration.IsFastInquiringRTData()) << "DEBUG模式时默认为假";
		tm tm_;
		tm_.tm_wday = 1;
		gl_pChinaMarket->TEST_SetMarketTM(tm_);
		gl_systemConfiguration.SetFastInquiringRTData(false);
		EXPECT_FALSE(gl_pChinaMarket->CheckFastReceivingData(91159));
		EXPECT_FALSE(gl_pChinaMarket->IsFastReceivingRTData());
		EXPECT_TRUE(gl_pChinaMarket->CheckFastReceivingData(91200));
		EXPECT_TRUE(gl_pChinaMarket->IsFastReceivingRTData());
		EXPECT_TRUE(gl_pChinaMarket->CheckFastReceivingData(114500));
		EXPECT_TRUE(gl_pChinaMarket->IsFastReceivingRTData());
		EXPECT_FALSE(gl_pChinaMarket->CheckFastReceivingData(114501));
		EXPECT_FALSE(gl_pChinaMarket->IsFastReceivingRTData());
		EXPECT_FALSE(gl_pChinaMarket->CheckFastReceivingData(124459));
		EXPECT_FALSE(gl_pChinaMarket->IsFastReceivingRTData());
		EXPECT_TRUE(gl_pChinaMarket->CheckFastReceivingData(124500));
		EXPECT_TRUE(gl_pChinaMarket->IsFastReceivingRTData());
		EXPECT_TRUE(gl_pChinaMarket->CheckFastReceivingData(150630));
		EXPECT_TRUE(gl_pChinaMarket->IsFastReceivingRTData());
		EXPECT_FALSE(gl_pChinaMarket->CheckFastReceivingData(150631));
		tm_.tm_wday = 0;
		gl_pChinaMarket->TEST_SetMarketTM(tm_);
		EXPECT_FALSE(gl_pChinaMarket->CheckFastReceivingData(91459));
		EXPECT_FALSE(gl_pChinaMarket->IsFastReceivingRTData());
		EXPECT_FALSE(gl_pChinaMarket->CheckFastReceivingData(91500));
		EXPECT_FALSE(gl_pChinaMarket->IsFastReceivingRTData());
		EXPECT_FALSE(gl_pChinaMarket->CheckFastReceivingData(113459));
		EXPECT_FALSE(gl_pChinaMarket->IsFastReceivingRTData());
		EXPECT_FALSE(gl_pChinaMarket->CheckFastReceivingData(113500));
		EXPECT_FALSE(gl_pChinaMarket->IsFastReceivingRTData());
		EXPECT_FALSE(gl_pChinaMarket->CheckFastReceivingData(125459));
		EXPECT_FALSE(gl_pChinaMarket->IsFastReceivingRTData());
		EXPECT_FALSE(gl_pChinaMarket->CheckFastReceivingData(125500));
		EXPECT_FALSE(gl_pChinaMarket->IsFastReceivingRTData());
		EXPECT_FALSE(gl_pChinaMarket->CheckFastReceivingData(150630));
		EXPECT_FALSE(gl_pChinaMarket->IsFastReceivingRTData());
		EXPECT_FALSE(gl_pChinaMarket->CheckFastReceivingData(150631));
		EXPECT_FALSE(gl_pChinaMarket->IsFastReceivingRTData());

		// 恢复原状
		gl_systemConfiguration.SetFastInquiringRTData(false);
	}

	TEST_F(CChinaMarketTest, TestIncreaseNeteaseDayLineInquiringIndex) {
		size_t k = 0;
		auto i = gl_pChinaMarket->IncreaseStockInquiringIndex(k, gl_dataContainerChinaStock.Size());
		EXPECT_EQ(i, 1);
		EXPECT_EQ(k, 1);
		i = gl_pChinaMarket->IncreaseStockInquiringIndex(k, gl_dataContainerChinaStock.Size());
		EXPECT_EQ(i, 2);
		EXPECT_EQ(k, 2);
		k = 11999;
		i = gl_pChinaMarket->IncreaseStockInquiringIndex(k, gl_dataContainerChinaStock.Size());
		EXPECT_EQ(k, 0);
		EXPECT_EQ(i, 0);
	}

	TEST_F(CChinaMarketTest, TestSetCheckActiveStockFlag1) {
		EXPECT_TRUE(gl_pChinaMarket->IsSystemReady()) << "单元测试时，系统状态永远为准备好了";
		gl_pChinaMarket->SetSystemReady(false);
		gl_pChinaMarket->SetCheckActiveStockFlag(93500);
		EXPECT_TRUE(gl_pChinaMarket->IsCheckingActiveStock()) << "当系统尚未准备好时，检测活跃股票的标识永远为真";
		gl_pChinaMarket->SetSystemReady(true);
	}

	TEST_F(CChinaMarketTest, TestSetCheckActiveStockFlag2) {
		EXPECT_TRUE(gl_pChinaMarket->IsSystemReady());
		EXPECT_TRUE(gl_pChinaMarket->IsCheckingActiveStock());
		gl_pChinaMarket->SetCheckActiveStockFlag(91459);
		EXPECT_FALSE(gl_pChinaMarket->IsCheckingActiveStock());
		gl_pChinaMarket->SetCheckActiveStockFlag(91500);
		EXPECT_TRUE(gl_pChinaMarket->IsCheckingActiveStock());
		gl_pChinaMarket->SetCheckActiveStockFlag(92659);
		EXPECT_TRUE(gl_pChinaMarket->IsCheckingActiveStock());
		gl_pChinaMarket->SetCheckActiveStockFlag(92700);
		EXPECT_FALSE(gl_pChinaMarket->IsCheckingActiveStock());
		gl_pChinaMarket->SetCheckActiveStockFlag(113059);
		EXPECT_FALSE(gl_pChinaMarket->IsCheckingActiveStock());
		gl_pChinaMarket->SetCheckActiveStockFlag(113300);
		EXPECT_TRUE(gl_pChinaMarket->IsCheckingActiveStock());
		gl_pChinaMarket->SetCheckActiveStockFlag(125859);
		EXPECT_TRUE(gl_pChinaMarket->IsCheckingActiveStock());
		gl_pChinaMarket->SetCheckActiveStockFlag(125900);
		EXPECT_FALSE(gl_pChinaMarket->IsCheckingActiveStock());
		gl_pChinaMarket->SetCheckActiveStockFlag(150000);
		EXPECT_FALSE(gl_pChinaMarket->IsCheckingActiveStock());

		gl_pChinaMarket->SetCheckActiveStock(true); // 恢复原态
	}

	TEST_F(CChinaMarketTest, TestLoadStockCodeDB) {
		CChinaStockPtr pStock = gl_dataContainerChinaStock.GetStock(0);
		EXPECT_EQ(pStock->GetSymbol(), "000001.SS");
		EXPECT_EQ(pStock->GetDayLineStartDate(), 19901219);
		EXPECT_FALSE(pStock->IsActive()) << "装载股票代码时永远设置为假";
		pStock = gl_dataContainerChinaStock.GetStock("600002.SS");
		EXPECT_EQ(pStock->GetDayLineStartDate(), 19980408);
		EXPECT_EQ(pStock->GetDayLineEndDate(), 20060406);
		EXPECT_FALSE(pStock->IsActive());
	}

	TEST_F(CChinaMarketTest, TestGetLastLoginDate) {
		gl_pChinaMarket->SetLastLoginDate(19900102);
		EXPECT_EQ(gl_pChinaMarket->GetLastLoginDate(), 19900102);
	}

	TEST_F(CChinaMarketTest, TestGetNewestTransactionTime) {
		gl_pChinaMarket->SetTransactionTime(10101010);
		EXPECT_EQ(gl_pChinaMarket->GetTransactionTime(), 10101010);
	}

	TEST_F(CChinaMarketTest, TesstIsTodayTempRTDataLoaded) {
		EXPECT_FALSE(gl_pChinaMarket->IsTodayTempRTDataLoaded());
		gl_pChinaMarket->SetTodayTempRTDataLoaded(true);
		EXPECT_TRUE(gl_pChinaMarket->IsTodayTempRTDataLoaded());
		gl_pChinaMarket->SetTodayTempRTDataLoaded(false);
		EXPECT_FALSE(gl_pChinaMarket->IsTodayTempRTDataLoaded());
	}

	TEST_F(CChinaMarketTest, TesstIsCheckActiveStock) {
		EXPECT_TRUE(gl_pChinaMarket->IsCheckingActiveStock());
		gl_pChinaMarket->SetCheckActiveStock(false);
		EXPECT_FALSE(gl_pChinaMarket->IsCheckingActiveStock());
		gl_pChinaMarket->SetCheckActiveStock(true);
		EXPECT_TRUE(gl_pChinaMarket->IsCheckingActiveStock());
	}

	TEST_F(CChinaMarketTest, TestGetStockPtr) {
		const CChinaStockPtr pStock = gl_dataContainerChinaStock.GetStock("600000.SS");
		EXPECT_EQ(pStock->GetSymbol(), "600000.SS");
	}

	TEST_F(CChinaMarketTest, TestClearDayLineNeedUpdaeStatus) {
		gl_dataContainerChinaStock.ClearDayLineNeedUpdateStatus();

		for (size_t i = 0; i < gl_dataContainerChinaStock.Size(); i++) {
			EXPECT_FALSE(gl_dataContainerChinaStock.GetStock(i)->IsUpdateDayLine());
		}

		for (size_t i = 0; i < gl_dataContainerChinaStock.Size(); i++) {
			gl_dataContainerChinaStock.GetStock(i)->SetUpdateDayLine(true);
		}
	}

	TEST_F(CChinaMarketTest, TestGetRTDataReceived) {
		gl_pChinaMarket->SetRTDataReceived(101010);
		EXPECT_EQ(gl_pChinaMarket->GetRTDataReceived(), 101010);
		gl_pChinaMarket->SetRTDataReceived(1010101010);
		EXPECT_EQ(gl_pChinaMarket->GetRTDataReceived(), 1010101010);
	}

	TEST_F(CChinaMarketTest, TestIsTimeToResetSystem) {
		EXPECT_FALSE(gl_pChinaMarket->IsTimeToResetSystem(91259));
		EXPECT_TRUE(gl_pChinaMarket->IsTimeToResetSystem(91300));
		EXPECT_TRUE(gl_pChinaMarket->IsTimeToResetSystem(91400));
		EXPECT_FALSE(gl_pChinaMarket->IsTimeToResetSystem(91401));
		EXPECT_FALSE(gl_pChinaMarket->IsTimeToResetSystem(92459));
		EXPECT_TRUE(gl_pChinaMarket->IsTimeToResetSystem(92500));
		EXPECT_TRUE(gl_pChinaMarket->IsTimeToResetSystem(92700));
		EXPECT_FALSE(gl_pChinaMarket->IsTimeToResetSystem(92701));
		EXPECT_FALSE(gl_pChinaMarket->IsTimeToResetSystem(93000));
	}

	TEST_F(CChinaMarketTest, TestIsOrdinaryTradeTime) {
		if (gl_pChinaMarket->IsWorkingDay()) {
			EXPECT_FALSE(gl_pChinaMarket->IsOrdinaryTradeTime(92959));
			EXPECT_TRUE(gl_pChinaMarket->IsOrdinaryTradeTime(93000));
			EXPECT_TRUE(gl_pChinaMarket->IsOrdinaryTradeTime(113000));
			EXPECT_FALSE(gl_pChinaMarket->IsOrdinaryTradeTime(113001));
			EXPECT_FALSE(gl_pChinaMarket->IsOrdinaryTradeTime(125959));
			EXPECT_TRUE(gl_pChinaMarket->IsOrdinaryTradeTime(130000));
			EXPECT_TRUE(gl_pChinaMarket->IsOrdinaryTradeTime(150000));
			EXPECT_FALSE(gl_pChinaMarket->IsOrdinaryTradeTime(150001));
		}
		else {
			EXPECT_FALSE(gl_pChinaMarket->IsOrdinaryTradeTime(92959));
			EXPECT_FALSE(gl_pChinaMarket->IsOrdinaryTradeTime(93000));
			EXPECT_FALSE(gl_pChinaMarket->IsOrdinaryTradeTime(113000));
			EXPECT_FALSE(gl_pChinaMarket->IsOrdinaryTradeTime(113001));
			EXPECT_FALSE(gl_pChinaMarket->IsOrdinaryTradeTime(125959));
			EXPECT_FALSE(gl_pChinaMarket->IsOrdinaryTradeTime(130000));
			EXPECT_FALSE(gl_pChinaMarket->IsOrdinaryTradeTime(150000));
			EXPECT_FALSE(gl_pChinaMarket->IsOrdinaryTradeTime(150001));
		}
	}

	TEST_F(CChinaMarketTest, TestIsWorkingTime) {
		if (gl_pChinaMarket->IsWorkingDay()) {
			EXPECT_FALSE(gl_pChinaMarket->IsWorkingTime(91159));
			EXPECT_TRUE(gl_pChinaMarket->IsWorkingTime(91200));
			EXPECT_TRUE(gl_pChinaMarket->IsWorkingTime(114500));
			EXPECT_FALSE(gl_pChinaMarket->IsWorkingTime(114501));
			EXPECT_FALSE(gl_pChinaMarket->IsWorkingTime(124459));
			EXPECT_TRUE(gl_pChinaMarket->IsWorkingTime(124500));
			EXPECT_TRUE(gl_pChinaMarket->IsWorkingTime(150630));
			EXPECT_FALSE(gl_pChinaMarket->IsWorkingTime(150631));
		}
		else {
			EXPECT_FALSE(gl_pChinaMarket->IsWorkingTime(91159));
			EXPECT_FALSE(gl_pChinaMarket->IsWorkingTime(91200));
			EXPECT_FALSE(gl_pChinaMarket->IsWorkingTime(114500));
			EXPECT_FALSE(gl_pChinaMarket->IsWorkingTime(114501));
			EXPECT_FALSE(gl_pChinaMarket->IsWorkingTime(124459));
			EXPECT_FALSE(gl_pChinaMarket->IsWorkingTime(124500));
			EXPECT_FALSE(gl_pChinaMarket->IsWorkingTime(150630));
			EXPECT_FALSE(gl_pChinaMarket->IsWorkingTime(150631));
		}
	}

	TEST_F(CChinaMarketTest, TestIsDummyTime) {
		if (gl_pChinaMarket->IsWorkingDay()) {
			EXPECT_TRUE(gl_pChinaMarket->IsDummyTime(91159));
			EXPECT_FALSE(gl_pChinaMarket->IsDummyTime(91200));
			EXPECT_FALSE(gl_pChinaMarket->IsDummyTime(114500));
			EXPECT_TRUE(gl_pChinaMarket->IsDummyTime(114501));
			EXPECT_TRUE(gl_pChinaMarket->IsDummyTime(124459));
			EXPECT_FALSE(gl_pChinaMarket->IsDummyTime(124500));
			EXPECT_FALSE(gl_pChinaMarket->IsDummyTime(150630));
			EXPECT_TRUE(gl_pChinaMarket->IsDummyTime(150631));
		}
		else {
			EXPECT_TRUE(gl_pChinaMarket->IsDummyTime(91159));
			EXPECT_TRUE(gl_pChinaMarket->IsDummyTime(91200));
			EXPECT_TRUE(gl_pChinaMarket->IsDummyTime(114500));
			EXPECT_TRUE(gl_pChinaMarket->IsDummyTime(114501));
			EXPECT_TRUE(gl_pChinaMarket->IsDummyTime(124459));
			EXPECT_TRUE(gl_pChinaMarket->IsDummyTime(124500));
			EXPECT_TRUE(gl_pChinaMarket->IsDummyTime(150630));
			EXPECT_TRUE(gl_pChinaMarket->IsDummyTime(150631));
		}
	}

	TEST_F(CChinaMarketTest, TestCheckMarketReady) {
		gl_pChinaMarket->SetSystemReady(true);
		gl_pChinaMarket->SetRTDataReceived(0);
		EXPECT_TRUE(gl_pChinaMarket->TaskCheckMarketReady(0));
		EXPECT_TRUE(gl_pChinaMarket->IsSystemReady());
		gl_pChinaMarket->SetSystemReady(false);
		gl_pChinaMarket->SetRTDataReceived(0);
		gl_pChinaMarket->SetSystemReady(false);
		EXPECT_FALSE(gl_pChinaMarket->TaskCheckMarketReady(0));
		EXPECT_FALSE(gl_pChinaMarket->IsSystemReady());
		gl_pChinaMarket->SetSystemReady(false);
		gl_pChinaMarket->SetRTDataReceived(0);
		gl_pChinaMarket->SetSystemReady(false);
		EXPECT_FALSE(gl_pChinaMarket->TaskCheckMarketReady(0));
		EXPECT_FALSE(gl_pChinaMarket->IsSystemReady());
		gl_pChinaMarket->SetRTDataReceived(24000);
		EXPECT_FALSE(gl_pChinaMarket->TaskCheckMarketReady(0));
		EXPECT_FALSE(gl_pChinaMarket->IsSystemReady());
		gl_pChinaMarket->SetRTDataReceived(24000 + 1);
		EXPECT_TRUE(gl_pChinaMarket->TaskCheckMarketReady(0));
		EXPECT_TRUE(gl_pChinaMarket->IsSystemReady());
		gl_pChinaMarket->SetSystemReady(false);

		// 恢复原状
		while (!gl_pChinaMarket->IsMarketTaskEmpty()) gl_pChinaMarket->DiscardCurrentMarketTask();
	}

	TEST_F(CChinaMarketTest, TestIsTotalStockSetSelected) {
		EXPECT_EQ(gl_pChinaMarket->GetCurrentSelectedStockSet(), -1);
		EXPECT_TRUE(gl_pChinaMarket->IsTotalStockSetSelected());
		gl_pChinaMarket->SetCurrentSelectedStockSet(0);
		EXPECT_FALSE(gl_pChinaMarket->IsTotalStockSetSelected());

		gl_pChinaMarket->SetCurrentSelectedStockSet(-1);
	}

	TEST_F(CChinaMarketTest, TestIsUsingNeteaseRTDataReceiver) {
		EXPECT_FALSE(gl_pChinaMarket->IsUsingNeteaseRTDataReceiver());
		gl_pChinaMarket->SetUsingNeteaseRTDataReceiver(true);
		EXPECT_TRUE(gl_pChinaMarket->IsUsingNeteaseRTDataReceiver());
		gl_pChinaMarket->SetUsingNeteaseRTDataReceiver(false);
		EXPECT_FALSE(gl_pChinaMarket->IsUsingNeteaseRTDataReceiver());
	}

	TEST_F(CChinaMarketTest, TestIsUsingTengxunRTDataReceiver) {
		EXPECT_TRUE(gl_pChinaMarket->IsUsingTengxunRTDataReceiver());
		gl_pChinaMarket->SetUsingTengxunRTDataReceiver(false);
		EXPECT_FALSE(gl_pChinaMarket->IsUsingTengxunRTDataReceiver());
		gl_pChinaMarket->SetUsingTengxunRTDataReceiver(true);
		EXPECT_TRUE(gl_pChinaMarket->IsUsingTengxunRTDataReceiver());
	}

	TEST_F(CChinaMarketTest, TestGetCountDownTengxunNumber) {
		EXPECT_EQ(gl_pChinaMarket->GetCountDownTengxunNumber(), 10);
		gl_pChinaMarket->SetCountDownTengxunNumber(100);
		EXPECT_EQ(gl_pChinaMarket->GetCountDownTengxunNumber(), 100);
		gl_pChinaMarket->SetCountDownTengxunNumber(10);
	}

	TEST_F(CChinaMarketTest, TestLoadOptionDB) {
		using namespace StockMarket;
		const auto& t = ChinaMarketOptions{};

		{
			auto db = gl_dbStockMarket.get();
			auto tx = sqlpp::start_transaction(db);
			db(remove_from(t).unconditionally());
			tx.commit();
		}

		gl_pChinaMarket->SetLastLoginDate(gl_pChinaMarket->GetMarketDate());
		gl_pChinaMarket->UpdateOptionDB();
		gl_pChinaMarket->SetLastLoginDate(1);

		gl_pChinaMarket->LoadOptionDB();

		EXPECT_EQ(gl_pChinaMarket->GetLastLoginDate(), gl_pChinaMarket->GetMarketDate());

		gl_pChinaMarket->SetLastLoginDate(20200303);

		gl_pChinaMarket->UpdateOptionDB();

		gl_pChinaMarket->SetLastLoginDate(1);

		gl_pChinaMarket->LoadOptionDB();

		EXPECT_EQ(gl_pChinaMarket->GetLastLoginDate(), gl_pChinaMarket->GetMarketDate()) << "永远是当前日期\n";

		{
			auto db = gl_dbStockMarket.get();
			auto tx = sqlpp::start_transaction(db);
			db(remove_from(t).unconditionally());
			tx.commit();
		}

		gl_pChinaMarket->LoadOptionDB();

		EXPECT_EQ(gl_pChinaMarket->GetLastLoginDate(), CHINA_MARKET_BEGIN_DATE_);
	}

	TEST_F(CChinaMarketTest, TestCreateStockCodeSet) {
		vector<CVirtualHistoryCandle> vData;
		CVirtualHistoryCandle data;
		data.SetStockSymbol("600000.SS");
		vData.resize(2);
		vData[0] = data;
		data.SetStockSymbol("600004.SS");
		vData[1] = data;

		set<string> setStockCode;
		gl_pChinaMarket->CreateStockCodeSet(setStockCode, &vData);

		EXPECT_TRUE(setStockCode.contains("600000.SS"));
		EXPECT_TRUE(setStockCode.contains("600004.SS"));
		EXPECT_FALSE(setStockCode.contains("600001.SS"));
	}

	TEST_F(CChinaMarketTest, TestUpdateStockProfileDB) {
		ASSERT_THAT(gl_dataContainerChinaStock.IsUpdateProfileDB(), IsFalse()) << "此测试开始时，必须保证没有设置更新代码库的标识，否则会真正更新了测试代码库";
		EXPECT_EQ(gl_dataContainerChinaStock.Size(), 5701) << "测试代码库中的股票代码总数为5701";

		auto pStock = make_shared<CChinaStock>();
		pStock->SetSymbol("Test");
		pStock->SetExchange("Test");
		pStock->SetNewStock(true);
		pStock->SetUpdateProfileDB(true);
		EXPECT_FALSE(gl_dataContainerChinaStock.IsSymbol(pStock->GetSymbol())); // 确保是一个新股票代码
		gl_dataContainerChinaStock.Add(pStock);
		EXPECT_EQ(gl_dataContainerChinaStock.Size(), 5702) << "测试代码库中的股票代码总数为5701";
		pStock = gl_dataContainerChinaStock.GetStock("000001.SS");
		auto exchange = pStock->GetExchange();
		pStock->SetExchange("CN");
		pStock->SetUpdateProfileDB(true);

		gl_dataContainerChinaStock.UpdateProfileDB();

		using namespace StockMarket;
		const auto& t = ChinaStockProfile{};
		auto db = gl_dbStockMarket.get();
		auto tx = sqlpp::start_transaction(db);
		auto result = db(select(all_of(t)).from(t).where(t.Symbol == "000001.SS"));
		int rows = result.size();
		EXPECT_EQ(rows, 1);
		auto& row = result.front();
		EXPECT_EQ(row.Exchange, "CN");

		db(update(t).set(t.Exchange = exchange).where(t.Symbol == "000001.SS"));

		EXPECT_EQ(gl_dataContainerChinaStock.Size(), 5702) << "测试代码库中的股票代码总数为5701,增加了一个";

		auto result1 = db(select(all_of(t)).from(t).where(t.Symbol == "Test"));
		int rows1 = result.size();
		EXPECT_EQ(rows1, 1);

		db(remove_from(t).where(t.Symbol == "Test"));
		tx.commit();

		EXPECT_EQ(gl_dataContainerChinaStock.Size(), 5702) << "测试代码库中的股票代码总数为5701，内存中的尚未删除";

		pStock = gl_dataContainerChinaStock.GetStock("Test");
		EXPECT_TRUE(pStock != nullptr);
		gl_dataContainerChinaStock.Delete(pStock); // 恢复原状
		EXPECT_EQ(gl_dataContainerChinaStock.Size(), 5701) << "测试代码库中的股票代码总数为5701";

		EXPECT_THAT(gl_dataContainerChinaStock.IsUpdateProfileDB(), IsFalse()) << "此测试结束时，必须保证没有设置更新代码库的标识，否则会真正更新了测试代码库";

		for (size_t i = 0; i < gl_dataContainerChinaStock.Size(); i++) {
			pStock = gl_dataContainerChinaStock.GetStock(i);
			pStock->SetUpdateProfileDB(false);
		}
	}

	TEST_F(CChinaMarketTest, TestAddStock) {
		CChinaStockPtr pStock = nullptr;
		gl_dataContainerChinaStock.Add(pStock);

		pStock = gl_dataContainerChinaStock.GetStock(1);
		gl_dataContainerChinaStock.Add(pStock);

		pStock = make_shared<CChinaStock>();
		pStock->SetSymbol("SS.SS.SS");
		const auto lTotal = gl_dataContainerChinaStock.Size();
		EXPECT_FALSE(gl_dataContainerChinaStock.IsSymbol(pStock->GetSymbol()));
		gl_dataContainerChinaStock.Add(pStock);
		EXPECT_EQ(gl_dataContainerChinaStock.Size(), lTotal + 1);

		gl_dataContainerChinaStock.Delete(pStock);
	}

	TEST_F(CChinaMarketTest, TestDeleteStock) {
		CChinaStockPtr pStock = nullptr;
		gl_dataContainerChinaStock.Delete(pStock);

		pStock = make_shared<CChinaStock>();
		pStock->SetSymbol("SS.SS.SS");
		gl_dataContainerChinaStock.Delete(pStock);

		gl_dataContainerChinaStock.Add(pStock);
		gl_dataContainerChinaStock.Delete(pStock);
	}

	TEST_F(CChinaMarketTest, TestTaskCheckDayLineDB) {
		EXPECT_TRUE(gl_dataContainerChinaStock.IsUpdateDayLine());
		EXPECT_FALSE(gl_pChinaMarket->IsDayLineNeedProcess());
		EXPECT_FALSE(gl_dataContainerChinaStock.IsUpdateDayLineDB());
		EXPECT_FALSE(gl_dataContainerChinaStock.IsDayLineDBUpdated());

		EXPECT_FALSE(gl_pChinaMarket->IsSavingDayLineDBTaskFinished()) << "IsSaveDayLine为假";

		const CChinaStockPtr pStock = gl_dataContainerChinaStock.GetStock(0);
		pStock->SetUpdateDayLineDB(true);
		pStock->SetDayLineDBUpdated(true);
		EXPECT_FALSE(gl_pChinaMarket->IsSavingDayLineDBTaskFinished()) << "IsUpdateDayLine等皆为真";
		EXPECT_TRUE(gl_dataContainerChinaStock.IsDayLineDBUpdated());

		EXPECT_FALSE(gl_pChinaMarket->IsSavingDayLineDBTaskFinished()) << "IsUpdateDayLine和IsUpdateDayLineDB为真";
		EXPECT_TRUE(gl_dataContainerChinaStock.IsDayLineDBUpdated());

		pStock->SetUpdateDayLineDB(false);
		EXPECT_FALSE(gl_pChinaMarket->IsSavingDayLineDBTaskFinished()) << "IsUpdateDayLine为真";
		EXPECT_TRUE(gl_dataContainerChinaStock.IsDayLineDBUpdated());

		for (size_t i = 0; i < gl_dataContainerChinaStock.Size(); i++) {
			const auto china_stock_ptr = gl_dataContainerChinaStock.GetStock(i);
			china_stock_ptr->SetUpdateDayLine(false);
		}
		EXPECT_TRUE(gl_pChinaMarket->IsSavingDayLineDBTaskFinished()) << "条件满足了";
		EXPECT_FALSE(pStock->IsDayLineDBUpdated());
		EXPECT_FALSE(gl_dataContainerChinaStock.IsDayLineDBUpdated());
		EXPECT_EQ(gl_systemMessage.InformationSize(), 1);
		EXPECT_EQ(gl_systemMessage.PopInformationMessage(), "中国市场日线历史数据更新完毕");

		for (size_t i = 0; i < gl_dataContainerChinaStock.Size(); i++) {
			const auto china_stock_ptr = gl_dataContainerChinaStock.GetStock(i);
			china_stock_ptr->SetUpdateDayLine(true);
		}
	}

	TEST_F(CChinaMarketTest, TestAppendChosenStock) {
		CChinaStockPtr pStock = gl_dataContainerChinaStock.GetStock("600601.SS");
		pStock->SetChosen(true);
		gl_pChinaMarket->AddChosenStock(pStock);
		pStock = gl_dataContainerChinaStock.GetStock("000001.SZ");
		pStock->SetChosen(true);
		gl_pChinaMarket->AddChosenStock(pStock);

		gl_pChinaMarket->AppendChosenStockDB();

		pStock = gl_dataContainerChinaStock.GetStock("600601.SS");
		gl_pChinaMarket->DeleteChosenStock(pStock);
		pStock = gl_dataContainerChinaStock.GetStock("000001.SZ");
		gl_pChinaMarket->DeleteChosenStock(pStock);
		EXPECT_FALSE(gl_dataContainerChinaStock.GetStock("600601.SS")->IsChosen());
		EXPECT_FALSE(gl_dataContainerChinaStock.GetStock("000001.SZ")->IsChosen());

		gl_pChinaMarket->LoadChosenStockDB();
		EXPECT_TRUE(gl_dataContainerChinaStock.GetStock("600601.SS")->IsChosen());
		EXPECT_TRUE(gl_dataContainerChinaStock.GetStock("000001.SZ")->IsChosen());

		pStock = gl_dataContainerChinaStock.GetStock("600601.SS");
		gl_pChinaMarket->DeleteChosenStock(pStock);
		pStock = gl_dataContainerChinaStock.GetStock("000001.SZ");
		gl_pChinaMarket->DeleteChosenStock(pStock);

		pStock = gl_dataContainerChinaStock.GetStock("600000.SS");
		pStock->SetChosen(true);
		gl_pChinaMarket->AddChosenStock(pStock);
		pStock = gl_dataContainerChinaStock.GetStock("000002.SZ");
		pStock->SetChosen(true);
		gl_pChinaMarket->AddChosenStock(pStock);

		gl_pChinaMarket->UpdateChosenStockDB();

		// 恢复原状
		pStock = gl_dataContainerChinaStock.GetStock("600000.SS");
		gl_pChinaMarket->DeleteChosenStock(pStock);
		pStock = gl_dataContainerChinaStock.GetStock("000002.SZ");
		gl_pChinaMarket->DeleteChosenStock(pStock);

		using namespace StockMarket;
		const auto& t = ChinaChoiceStock{};
		auto db = gl_dbStockMarket.get();
		auto tx = sqlpp::start_transaction(db);

		db(remove_from(t).where(t.Symbol == "600000.SS" || t.Symbol == "000002.SZ"));
		tx.commit();
	}

	TEST_F(CChinaMarketTest, TestIsRTDataNeedCalculate) {
		EXPECT_FALSE(gl_pChinaMarket->IsRTDataNeedCalculate());
		gl_pChinaMarket->SetRTDataNeedCalculate(true);
		EXPECT_TRUE(gl_pChinaMarket->IsRTDataNeedCalculate());
		gl_pChinaMarket->SetRTDataNeedCalculate(false);
		EXPECT_FALSE(gl_pChinaMarket->IsRTDataNeedCalculate());
	}
	TEST_F(CChinaMarketTest, TestGetHTTPStatus_IsWebError_GetWebErrorCode_Routing) {
		// Save and restore current server selection
		const EChinaMarketDataSourceServer oldServer = gl_systemConfiguration.GetChinaMarketRealtimeServer();

		// Validate routing for each configured realtime server
		gl_systemConfiguration.SetChinaMarketRealtimeServer(SinaRealTime_);
		EXPECT_NE(gl_pSinaRTDataSource, nullptr);
		EXPECT_EQ(gl_pChinaMarket->GetHTTPStatus(), gl_pSinaRTDataSource->GetHTTPStatusCode());
		EXPECT_EQ(gl_pChinaMarket->IsWebError(), gl_pSinaRTDataSource->IsWebError());
		EXPECT_EQ(gl_pChinaMarket->GetWebErrorCode(), gl_pSinaRTDataSource->GetWebErrorCode());

		gl_systemConfiguration.SetChinaMarketRealtimeServer(NeteaseRealTime_);
		EXPECT_NE(gl_pNeteaseRTDataSource, nullptr);
		EXPECT_EQ(gl_pChinaMarket->GetHTTPStatus(), gl_pNeteaseRTDataSource->GetHTTPStatusCode());
		EXPECT_EQ(gl_pChinaMarket->IsWebError(), gl_pNeteaseRTDataSource->IsWebError());
		EXPECT_EQ(gl_pChinaMarket->GetWebErrorCode(), gl_pNeteaseRTDataSource->GetWebErrorCode());

		gl_systemConfiguration.SetChinaMarketRealtimeServer(TengxunRealTime_);
		EXPECT_NE(gl_pTengxunRTDataSource, nullptr);
		EXPECT_EQ(gl_pChinaMarket->GetHTTPStatus(), gl_pTengxunRTDataSource->GetHTTPStatusCode());
		EXPECT_EQ(gl_pChinaMarket->IsWebError(), gl_pTengxunRTDataSource->IsWebError());
		EXPECT_EQ(gl_pChinaMarket->GetWebErrorCode(), gl_pTengxunRTDataSource->GetWebErrorCode());

		// Restore original server selection
		gl_systemConfiguration.SetChinaMarketRealtimeServer(oldServer);
	}

	TEST_F(CChinaMarketTest, TestEnableRealTimeDataSource_Routing_NoThrow) {
		// Save and restore current server selection
		const EChinaMarketDataSourceServer oldServer = gl_systemConfiguration.GetChinaMarketRealtimeServer();
		EXPECT_TRUE(gl_pSinaRTDataSource->IsEnable());
		EXPECT_TRUE(gl_pNeteaseRTDataSource->IsEnable());
		EXPECT_TRUE(gl_pTengxunRTDataSource->IsEnable());

		// Test routing for each configured realtime server: enabling and disabling should not throw
		gl_systemConfiguration.SetChinaMarketRealtimeServer(SinaRealTime_);
		EXPECT_NO_THROW(gl_pChinaMarket->EnableRealTimeDataSource(true));
		EXPECT_NO_THROW(gl_pChinaMarket->EnableRealTimeDataSource(false));
		EXPECT_NO_THROW(gl_pChinaMarket->GetHTTPStatus());
		EXPECT_NO_THROW(gl_pChinaMarket->IsWebError());
		EXPECT_NO_THROW(gl_pChinaMarket->GetWebErrorCode());

		gl_systemConfiguration.SetChinaMarketRealtimeServer(NeteaseRealTime_);
		EXPECT_NO_THROW(gl_pChinaMarket->EnableRealTimeDataSource(true));
		EXPECT_NO_THROW(gl_pChinaMarket->EnableRealTimeDataSource(false));
		EXPECT_NO_THROW(gl_pChinaMarket->GetHTTPStatus());
		EXPECT_NO_THROW(gl_pChinaMarket->IsWebError());
		EXPECT_NO_THROW(gl_pChinaMarket->GetWebErrorCode());

		gl_systemConfiguration.SetChinaMarketRealtimeServer(TengxunRealTime_);
		EXPECT_NO_THROW(gl_pChinaMarket->EnableRealTimeDataSource(true));
		EXPECT_NO_THROW(gl_pChinaMarket->EnableRealTimeDataSource(false));
		EXPECT_NO_THROW(gl_pChinaMarket->GetHTTPStatus());
		EXPECT_NO_THROW(gl_pChinaMarket->IsWebError());
		EXPECT_NO_THROW(gl_pChinaMarket->GetWebErrorCode());

		// Restore original server selection
		gl_systemConfiguration.SetChinaMarketRealtimeServer(oldServer);
		gl_pSinaRTDataSource->Enable(true);
		gl_pNeteaseRTDataSource->Enable(true);
		gl_pTengxunRTDataSource->Enable(true);
	}

	TEST_F(CChinaMarketTest, TestIsRealTimeDataSourceEnable_Routing) {
		// Save and restore current server selection and enable states
		const EChinaMarketDataSourceServer oldServer = gl_systemConfiguration.GetChinaMarketRealtimeServer();
		const bool origSinaEnabled = gl_pSinaRTDataSource->IsEnable();
		const bool origNeteaseEnabled = gl_pNeteaseRTDataSource->IsEnable();
		const bool origTengxunEnabled = gl_pTengxunRTDataSource->IsEnable();

		// Sina
		gl_systemConfiguration.SetChinaMarketRealtimeServer(SinaRealTime_);
		EXPECT_NE(gl_pSinaRTDataSource, nullptr);
		gl_pSinaRTDataSource->Enable(true);
		EXPECT_TRUE(gl_pChinaMarket->IsRealTimeDataSourceEnable());
		gl_pSinaRTDataSource->Enable(false);
		EXPECT_FALSE(gl_pChinaMarket->IsRealTimeDataSourceEnable());

		// Netease
		gl_systemConfiguration.SetChinaMarketRealtimeServer(NeteaseRealTime_);
		EXPECT_NE(gl_pNeteaseRTDataSource, nullptr);
		gl_pNeteaseRTDataSource->Enable(true);
		EXPECT_TRUE(gl_pChinaMarket->IsRealTimeDataSourceEnable());
		gl_pNeteaseRTDataSource->Enable(false);
		EXPECT_FALSE(gl_pChinaMarket->IsRealTimeDataSourceEnable());

		// Tengxun
		gl_systemConfiguration.SetChinaMarketRealtimeServer(TengxunRealTime_);
		EXPECT_NE(gl_pTengxunRTDataSource, nullptr);
		gl_pTengxunRTDataSource->Enable(true);
		EXPECT_TRUE(gl_pChinaMarket->IsRealTimeDataSourceEnable());
		gl_pTengxunRTDataSource->Enable(false);
		EXPECT_FALSE(gl_pChinaMarket->IsRealTimeDataSourceEnable());

		// Restore original states
		gl_pSinaRTDataSource->Enable(origSinaEnabled);
		gl_pNeteaseRTDataSource->Enable(origNeteaseEnabled);
		gl_pTengxunRTDataSource->Enable(origTengxunEnabled);
		gl_systemConfiguration.SetChinaMarketRealtimeServer(oldServer);
	}
}
