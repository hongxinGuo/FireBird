#include"pch.h"

#include"TimeConvert.h"
#include"ChinaStockCodeConverter.h"

#include"ChinaMarket.h"
#include"ChinaStock.h"

#include"SetOption.h"
#include"SetCurrentWeekLine.h"
#include"SetChinaChosenStock.h"

#include"NeteaseDayLineDataSource.h"

#include"GeneralCheck.h"

using namespace testing;

#include<memory>

namespace FireBirdTest {
	class CChinaMarketTest : public Test {
	protected:
		static void SetUpTestSuite() {
			GeneralCheck();

			gl_pChinaMarket->Load10DaysRSStrongStockDB(); // 装入各十日强度股票集
			EXPECT_TRUE(gl_pChinaMarket->GetChosenStockSize(10) > 0);
			EXPECT_TRUE(gl_pChinaMarket->GetChosenStockSize(11) > 0);
			EXPECT_TRUE(gl_pChinaMarket->GetChosenStockSize(12) > 0);
			EXPECT_TRUE(gl_pChinaMarket->GetChosenStockSize(13) == 0);
			EXPECT_TRUE(gl_pChinaMarket->GetChosenStockSize(14) == 0);
			EXPECT_TRUE(gl_pChinaMarket->GetChosenStockSize(15) == 0);
			EXPECT_TRUE(gl_pChinaMarket->GetChosenStockSize(16) == 0);
			EXPECT_TRUE(gl_pChinaMarket->GetChosenStockSize(17) == 0);
			EXPECT_TRUE(gl_pChinaMarket->GetChosenStockSize(18) > 0);
			EXPECT_TRUE(gl_pChinaMarket->GetChosenStockSize(19) > 0);

			for (int i = 0; i < gl_pChinaMarket->GetTotalStock(); i++) {
				const CChinaStockPtr pStock = gl_pChinaMarket->GetStock(i);
				EXPECT_TRUE(pStock->IsDayLineNeedUpdate()) << pStock->GetSymbol();
			}
			EXPECT_EQ(gl_pChinaMarket->GetCurrentStock(), nullptr) << gl_pChinaMarket->GetCurrentStock()->GetSymbol();
			EXPECT_EQ(gl_pChinaMarket->GetDayLineNeedUpdateNumber(), gl_pChinaMarket->GetTotalStock());
			EXPECT_FALSE(gl_pChinaMarket->IsCurrentStockChanged());

			EXPECT_EQ(gl_pChinaMarket->GetTotalStock(), 5040) << "测试数据库中的股票代码总数为5040";
			EXPECT_EQ(gl_pChinaMarket->GetTotalLoadedStock(), 5040) << "测试代码库中的股票代码总数为5040";
		}

		static void TearDownTestSuite() {
			EXPECT_EQ(gl_pChinaMarket->GetCurrentStock(), nullptr) << gl_pChinaMarket->GetCurrentStock()->GetSymbol();
			EXPECT_FALSE(gl_pChinaMarket->IsCurrentStockChanged());
			gl_pChinaMarket->SetCurrentStockChanged(false);
			for (int i = 0; i < gl_pChinaMarket->GetTotalStock(); i++) {
				const CChinaStockPtr pStock = gl_pChinaMarket->GetStock(i);
				EXPECT_TRUE(pStock->IsDayLineNeedUpdate()) << pStock->GetSymbol();
			}
			EXPECT_EQ(gl_pChinaMarket->GetDayLineNeedUpdateNumber(), gl_pChinaMarket->GetTotalStock());

			EXPECT_EQ(gl_pChinaMarket->GetTotalStock(), 5040) << "测试数据库中的股票代码总数为5040";

			GeneralCheck();
		}

		void SetUp() override {
			GeneralCheck();

			ASSERT_FALSE(gl_systemStatus.IsWorkingMode());
			EXPECT_EQ(gl_pChinaMarket->GetDayLineNeedUpdateNumber(), gl_pChinaMarket->GetTotalStock());
			gl_pChinaMarket->SetCurrentStockChanged(false);
			gl_pChinaMarket->CalculateTime();
			gl_pChinaMarket->SetSinaStockRTDataInquiringIndex(0);
			gl_pChinaMarket->SetTengxunRTDataInquiringIndex(0);
			gl_pChinaMarket->SetSystemReady(true); // 测试市场时，默认系统已经准备好
			gl_pChinaMarket->SetResetMarketPermission(true);
			EXPECT_TRUE(gl_pChinaMarket->IsResetMarket());
			EXPECT_FALSE(gl_pChinaMarket->IsMarketOpened());
			EXPECT_EQ(gl_pChinaMarket->GetTotalStock(), 5040) << "测试数据库中的股票代码总数为5040";
		}

		void TearDown() override {
			// clearUp
			EXPECT_FALSE(gl_pChinaMarket->IsMarketOpened());
			gl_pChinaMarket->SetRTDataSetCleared(false);
			gl_pChinaMarket->SetUpdateOptionDB(false);
			gl_pChinaMarket->SetResetMarket(true);
			gl_pChinaMarket->SetSinaStockRTDataInquiringIndex(0);
			gl_pChinaMarket->SetTengxunRTDataInquiringIndex(0);
			while (gl_systemMessage.InformationSize() > 0) gl_systemMessage.PopInformationMessage();
			gl_pChinaMarket->SetResetMarketPermission(true);
			gl_pChinaMarket->SetUpdateOptionDB(false);
			gl_pChinaMarket->SetSystemReady(true); // 离开此测试时，默认系统已准备好。
			EXPECT_TRUE(gl_pChinaMarket->IsCheckingActiveStock());

			gl_pChinaMarket->ResetCurrentStock();
			gl_pChinaMarket->SetCurrentStockChanged(false);
			for (int i = 0; i < gl_pChinaMarket->GetTotalStock(); i++) {
				const CChinaStockPtr pStock = gl_pChinaMarket->GetStock(i);
				if (!pStock->IsDayLineNeedUpdate()) pStock->SetDayLineNeedUpdate(true);
				if (pStock->IsDayLineNeedSaving()) pStock->SetDayLineNeedSaving(false);
			}
			EXPECT_EQ(gl_pChinaMarket->GetDayLineNeedUpdateNumber(), gl_pChinaMarket->GetTotalStock());
			EXPECT_EQ(gl_pChinaMarket->GetCurrentSelectedStockSet(), -1);
			EXPECT_THAT(gl_pChinaMarket->GetTotalStock(), Eq(5040));

			EXPECT_EQ(gl_pChinaMarket->GetTotalStock(), 5040) << "测试数据库中的股票代码总数为5040";

			gl_pChinaMarket->SetCurrentSelectedStockSet(-1);

			while (gl_systemMessage.ErrorMessageSize() > 0) gl_systemMessage.PopErrorMessage();

			GeneralCheck();
		}
	};

	TEST_F(CChinaMarketTest, TestInitialize) {
		CChinaStockPtr pStock = nullptr;
		for (int i = 0; i < gl_pChinaMarket->GetTotalStock(); i++) {
			pStock = gl_pChinaMarket->GetStock(i);
			EXPECT_FALSE(pStock->IsDayLineNeedSaving());
			if (IsShanghaiExchange(pStock->GetSymbol())) { if ((pStock->GetSymbol().Left(6) >= _T("000000")) && (pStock->GetSymbol().Left(6) <= _T("000999"))) { EXPECT_FALSE(pStock->IsNeedProcessRTData()); } }
			else if ((pStock->GetSymbol().Left(6) >= _T("399000")) && (pStock->GetSymbol().Left(6) <= _T("399999"))) { EXPECT_FALSE(pStock->IsNeedProcessRTData()); }
			else { EXPECT_TRUE(pStock->IsNeedProcessRTData()); }
		}
		EXPECT_FALSE(gl_pChinaMarket->IsLoadSelectedStock());
		EXPECT_TRUE(gl_pChinaMarket->IsSystemReady());
		EXPECT_FALSE(gl_pChinaMarket->IsCurrentEditStockChanged());
		EXPECT_FALSE(gl_pChinaMarket->IsMarketOpened());
		EXPECT_FALSE(gl_pChinaMarket->IsCurrentStockChanged());
		EXPECT_EQ(gl_pChinaMarket->GetTotalAttackBuyAmount(), 0);
		EXPECT_EQ(gl_pChinaMarket->GetTotalAttackSellAmount(), 0);
		EXPECT_FALSE(gl_pChinaMarket->IsTodayTempRTDataLoaded());
		EXPECT_GT(gl_pChinaMarket->GetTotalStock(), 0); // 在全局变量gl_ChinaStockMarket初始化时就生成了全部股票代码池
		EXPECT_EQ(gl_pChinaMarket->GetTotalStock(), 5040) << "测试数据库中的股票代码总数为5040";
		pStock = gl_pChinaMarket->GetStock(0);
		EXPECT_STREQ(pStock->GetSymbol(), _T("000001.SS"));
		EXPECT_EQ(gl_pChinaMarket->GetStockOffset(_T("000001.SS")), 0);

		EXPECT_TRUE(gl_pChinaMarket->HaveResetMarketPermission());
		EXPECT_EQ(gl_pChinaMarket->GetDayLineNeedSaveNumber(), 0);
		EXPECT_TRUE(gl_pChinaMarket->IsUsingSinaRTDataReceiver());
		EXPECT_TRUE(gl_pChinaMarket->IsUsingTengxunRTDataReceiver());
		EXPECT_FALSE(gl_pChinaMarket->IsUsingNeteaseRTDataReceiver());

		EXPECT_EQ(gl_pChinaMarket->GetSinaStockRTDataInquiringIndex(), 0);
		EXPECT_EQ(gl_pChinaMarket->GetTengxunRTDataInquiringIndex(), 0);
	}

	TEST_F(CChinaMarketTest, TestProcessEveryDayTask1) {
		EXPECT_TRUE(gl_pChinaMarket->IsMarketTaskEmpty());

		EXPECT_FALSE(gl_pChinaMarket->ProcessTask(101010)) << "没有任务可执行";
	}

	TEST_F(CChinaMarketTest, TestProcessEveryDayTask2) {
		EXPECT_TRUE(gl_pChinaMarket->IsMarketTaskEmpty());

		EXPECT_FALSE(gl_pChinaMarket->ProcessTask(101010)) << "没有任务可执行";

		gl_pChinaMarket->AddTask(CHINA_MARKET_RESET__, 100101);
		gl_pChinaMarket->AddTask(CHINA_MARKET_RESET__, 110101);

		EXPECT_FALSE(gl_pChinaMarket->ProcessTask(100001)) << "有任务需要执行，但时间未到";

		EXPECT_TRUE(gl_pChinaMarket->ProcessTask(101002)) << "时间到, 执行第一个任务";

		EXPECT_FALSE(gl_pChinaMarket->IsMarketTaskEmpty());

		EXPECT_TRUE(gl_pChinaMarket->ProcessTask(111002)) << "时间到, 执行第二个任务";

		// 恢复原状
		while (!gl_pChinaMarket->IsMarketTaskEmpty()) gl_pChinaMarket->DiscardMarketTask();
	}

	TEST_F(CChinaMarketTest, TestTaskCreateTask1) {
		EXPECT_TRUE(gl_pChinaMarket->IsMarketTaskEmpty());

		gl_pChinaMarket->TaskCreateTask(10000);

		EXPECT_FALSE(gl_pChinaMarket->IsMarketTaskEmpty());

		CMarketTaskPtr pTask = gl_pChinaMarket->GetMarketTask();
		gl_pChinaMarket->DiscardMarketTask();
		EXPECT_EQ(pTask->GetType(), CHINA_MARKET_CHECK_SYSTEM_READY__);
		EXPECT_EQ(pTask->GetTime(), 1);

		pTask = gl_pChinaMarket->GetMarketTask();
		gl_pChinaMarket->DiscardMarketTask();
		EXPECT_EQ(pTask->GetType(), CHINA_MARKET_DISTRIBUTE_AND_CALCULATE_RT_DATA__);
		EXPECT_EQ(pTask->GetTime(), 1);

		pTask = gl_pChinaMarket->GetMarketTask();
		gl_pChinaMarket->DiscardMarketTask();
		EXPECT_EQ(pTask->GetType(), CHINA_MARKET_ACCESSORY_TASK__);
		EXPECT_EQ(pTask->GetTime(), 10100);

		pTask = gl_pChinaMarket->GetMarketTask();
		gl_pChinaMarket->DiscardMarketTask();
		EXPECT_EQ(pTask->GetType(), CHINA_MARKET_UPDATE_OPTION_DB__);

		pTask = gl_pChinaMarket->GetMarketTask();
		gl_pChinaMarket->DiscardMarketTask();
		EXPECT_EQ(pTask->GetType(), CHINA_MARKET_UPDATE_STOCK_PROFILE_DB__);

		pTask = gl_pChinaMarket->GetMarketTask();
		gl_pChinaMarket->DiscardMarketTask();
		EXPECT_EQ(pTask->GetType(), CHINA_MARKET_RESET__);
		EXPECT_EQ(pTask->GetTime(), 91300);

		pTask = gl_pChinaMarket->GetMarketTask();
		gl_pChinaMarket->DiscardMarketTask();
		EXPECT_EQ(pTask->GetType(), CHINA_MARKET_RESET__);
		EXPECT_EQ(pTask->GetTime(), 92600);

		pTask = gl_pChinaMarket->GetMarketTask();
		gl_pChinaMarket->DiscardMarketTask();
		EXPECT_EQ(pTask->GetType(), CHINA_MARKET_LOAD_TEMP_RT_DATA__);
		EXPECT_EQ(pTask->GetTime(), 92700);

		if (gl_pChinaMarket->IsWorkingDay()) {
			pTask = gl_pChinaMarket->GetMarketTask();
			gl_pChinaMarket->DiscardMarketTask();
			EXPECT_EQ(pTask->GetType(), CHINA_MARKET_SAVE_TEMP_RT_DATA__);
			EXPECT_EQ(pTask->GetTime(), 92730);
		}

		pTask = gl_pChinaMarket->GetMarketTask();
		gl_pChinaMarket->DiscardMarketTask();
		EXPECT_EQ(pTask->GetType(), CHINA_MARKET_PROCESS_AND_SAVE_DAY_LINE__);
		EXPECT_EQ(pTask->GetTime(), 113510);

		if (gl_pChinaMarket->IsWorkingDay()) {
			pTask = gl_pChinaMarket->GetMarketTask();
			gl_pChinaMarket->DiscardMarketTask();
			EXPECT_EQ(pTask->GetType(), CHINA_MARKET_BUILD_TODAY_DATABASE__);
			EXPECT_EQ(pTask->GetTime(), 150530);

			pTask = gl_pChinaMarket->GetMarketTask();
			gl_pChinaMarket->DiscardMarketTask();
			EXPECT_EQ(pTask->GetType(), CHINA_MARKET_CHOICE_10_RS_STRONG_STOCK_SET__);
			EXPECT_EQ(pTask->GetTime(), 150700);
		}

		EXPECT_TRUE(gl_pChinaMarket->IsMarketTaskEmpty());
	}

	TEST_F(CChinaMarketTest, TestTaskCreateTask2) {
		EXPECT_TRUE(gl_pChinaMarket->IsMarketTaskEmpty());

		gl_pChinaMarket->TaskCreateTask(91500);

		EXPECT_FALSE(gl_pChinaMarket->IsMarketTaskEmpty());

		CMarketTaskPtr pTask = gl_pChinaMarket->GetMarketTask();
		gl_pChinaMarket->DiscardMarketTask();
		EXPECT_EQ(pTask->GetType(), CHINA_MARKET_CHECK_SYSTEM_READY__);
		EXPECT_EQ(pTask->GetTime(), 1);

		pTask = gl_pChinaMarket->GetMarketTask();
		gl_pChinaMarket->DiscardMarketTask();
		EXPECT_EQ(pTask->GetType(), CHINA_MARKET_DISTRIBUTE_AND_CALCULATE_RT_DATA__);
		EXPECT_EQ(pTask->GetTime(), 1);

		pTask = gl_pChinaMarket->GetMarketTask();
		gl_pChinaMarket->DiscardMarketTask();
		EXPECT_EQ(pTask->GetType(), CHINA_MARKET_ACCESSORY_TASK__);
		EXPECT_EQ(pTask->GetTime(), 91600);

		pTask = gl_pChinaMarket->GetMarketTask();
		gl_pChinaMarket->DiscardMarketTask();
		EXPECT_EQ(pTask->GetType(), CHINA_MARKET_UPDATE_OPTION_DB__);

		pTask = gl_pChinaMarket->GetMarketTask();
		gl_pChinaMarket->DiscardMarketTask();
		EXPECT_EQ(pTask->GetType(), CHINA_MARKET_UPDATE_STOCK_PROFILE_DB__);

		pTask = gl_pChinaMarket->GetMarketTask();
		gl_pChinaMarket->DiscardMarketTask();
		EXPECT_EQ(pTask->GetType(), CHINA_MARKET_RESET__);
		EXPECT_EQ(pTask->GetTime(), 92600);

		pTask = gl_pChinaMarket->GetMarketTask();
		gl_pChinaMarket->DiscardMarketTask();
		EXPECT_EQ(pTask->GetType(), CHINA_MARKET_LOAD_TEMP_RT_DATA__);
		EXPECT_EQ(pTask->GetTime(), 92700);

		if (gl_pChinaMarket->IsWorkingDay()) {
			pTask = gl_pChinaMarket->GetMarketTask();
			gl_pChinaMarket->DiscardMarketTask();
			EXPECT_EQ(pTask->GetType(), CHINA_MARKET_SAVE_TEMP_RT_DATA__);
			EXPECT_EQ(pTask->GetTime(), 92730);
		}

		pTask = gl_pChinaMarket->GetMarketTask();
		gl_pChinaMarket->DiscardMarketTask();
		EXPECT_EQ(pTask->GetType(), CHINA_MARKET_PROCESS_AND_SAVE_DAY_LINE__);
		EXPECT_EQ(pTask->GetTime(), 113510);

		if (gl_pChinaMarket->IsWorkingDay()) {
			pTask = gl_pChinaMarket->GetMarketTask();
			gl_pChinaMarket->DiscardMarketTask();
			EXPECT_EQ(pTask->GetType(), CHINA_MARKET_BUILD_TODAY_DATABASE__);
			EXPECT_EQ(pTask->GetTime(), 150530);

			pTask = gl_pChinaMarket->GetMarketTask();
			gl_pChinaMarket->DiscardMarketTask();
			EXPECT_EQ(pTask->GetType(), CHINA_MARKET_CHOICE_10_RS_STRONG_STOCK_SET__);
			EXPECT_EQ(pTask->GetTime(), 150700);
		}

		EXPECT_TRUE(gl_pChinaMarket->IsMarketTaskEmpty());
	}

	TEST_F(CChinaMarketTest, TestTaskCreateTask3) {
		EXPECT_TRUE(gl_pChinaMarket->IsMarketTaskEmpty());

		gl_pChinaMarket->TaskCreateTask(93000);

		EXPECT_FALSE(gl_pChinaMarket->IsMarketTaskEmpty());

		CMarketTaskPtr pTask = gl_pChinaMarket->GetMarketTask();
		gl_pChinaMarket->DiscardMarketTask();
		EXPECT_EQ(pTask->GetType(), CHINA_MARKET_CHECK_SYSTEM_READY__);
		EXPECT_EQ(pTask->GetTime(), 1);

		pTask = gl_pChinaMarket->GetMarketTask();
		gl_pChinaMarket->DiscardMarketTask();
		EXPECT_EQ(pTask->GetType(), CHINA_MARKET_DISTRIBUTE_AND_CALCULATE_RT_DATA__);
		EXPECT_EQ(pTask->GetTime(), 1);

		pTask = gl_pChinaMarket->GetMarketTask();
		gl_pChinaMarket->DiscardMarketTask();
		EXPECT_EQ(pTask->GetType(), CHINA_MARKET_LOAD_TEMP_RT_DATA__);
		EXPECT_EQ(pTask->GetTime(), 92700);

		if (gl_pChinaMarket->IsWorkingDay()) {
			pTask = gl_pChinaMarket->GetMarketTask();
			gl_pChinaMarket->DiscardMarketTask();
			EXPECT_EQ(pTask->GetType(), CHINA_MARKET_SAVE_TEMP_RT_DATA__);
			EXPECT_EQ(pTask->GetTime(), 92730);
		}

		pTask = gl_pChinaMarket->GetMarketTask();
		gl_pChinaMarket->DiscardMarketTask();
		EXPECT_EQ(pTask->GetType(), CHINA_MARKET_ACCESSORY_TASK__);
		EXPECT_EQ(pTask->GetTime(), 93100);

		pTask = gl_pChinaMarket->GetMarketTask();
		gl_pChinaMarket->DiscardMarketTask();
		EXPECT_EQ(pTask->GetType(), CHINA_MARKET_UPDATE_OPTION_DB__);
		EXPECT_EQ(pTask->GetTime(), 93305);

		pTask = gl_pChinaMarket->GetMarketTask();
		gl_pChinaMarket->DiscardMarketTask();
		EXPECT_EQ(pTask->GetType(), CHINA_MARKET_UPDATE_STOCK_PROFILE_DB__);
		EXPECT_EQ(pTask->GetTime(), 93410);

		pTask = gl_pChinaMarket->GetMarketTask();
		gl_pChinaMarket->DiscardMarketTask();
		EXPECT_EQ(pTask->GetType(), CHINA_MARKET_PROCESS_AND_SAVE_DAY_LINE__);
		EXPECT_EQ(pTask->GetTime(), 113510);

		if (gl_pChinaMarket->IsWorkingDay()) {
			pTask = gl_pChinaMarket->GetMarketTask();
			gl_pChinaMarket->DiscardMarketTask();
			EXPECT_EQ(pTask->GetType(), CHINA_MARKET_BUILD_TODAY_DATABASE__);
			EXPECT_EQ(pTask->GetTime(), 150530);

			pTask = gl_pChinaMarket->GetMarketTask();
			gl_pChinaMarket->DiscardMarketTask();
			EXPECT_EQ(pTask->GetType(), CHINA_MARKET_CHOICE_10_RS_STRONG_STOCK_SET__);
			EXPECT_EQ(pTask->GetTime(), 150700);
		}

		EXPECT_TRUE(gl_pChinaMarket->IsMarketTaskEmpty());
	}

	TEST_F(CChinaMarketTest, TestTaskCreateTask4) {
		tm tm_{0};
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
		gl_pChinaMarket->DiscardMarketTask();
		EXPECT_EQ(pTask->GetType(), CHINA_MARKET_CHECK_SYSTEM_READY__);
		EXPECT_EQ(pTask->GetTime(), 1);

		pTask = gl_pChinaMarket->GetMarketTask();
		gl_pChinaMarket->DiscardMarketTask();
		EXPECT_EQ(pTask->GetType(), CHINA_MARKET_DISTRIBUTE_AND_CALCULATE_RT_DATA__);
		EXPECT_EQ(pTask->GetTime(), 1);

		pTask = gl_pChinaMarket->GetMarketTask();
		gl_pChinaMarket->DiscardMarketTask();
		EXPECT_EQ(pTask->GetType(), CHINA_MARKET_LOAD_TEMP_RT_DATA__);
		EXPECT_EQ(pTask->GetTime(), 92700);

		pTask = gl_pChinaMarket->GetMarketTask();
		gl_pChinaMarket->DiscardMarketTask();
		EXPECT_EQ(pTask->GetType(), CHINA_MARKET_SAVE_TEMP_RT_DATA__);
		EXPECT_EQ(pTask->GetTime(), 92730);

		pTask = gl_pChinaMarket->GetMarketTask();
		gl_pChinaMarket->DiscardMarketTask();
		EXPECT_EQ(pTask->GetType(), CHINA_MARKET_PROCESS_AND_SAVE_DAY_LINE__);
		EXPECT_EQ(pTask->GetTime(), 113510);

		pTask = gl_pChinaMarket->GetMarketTask();
		gl_pChinaMarket->DiscardMarketTask();
		EXPECT_EQ(pTask->GetType(), CHINA_MARKET_ACCESSORY_TASK__);
		EXPECT_EQ(pTask->GetTime(), 150700);

		pTask = gl_pChinaMarket->GetMarketTask();
		gl_pChinaMarket->DiscardMarketTask();
		EXPECT_EQ(pTask->GetType(), CHINA_MARKET_CHOICE_10_RS_STRONG_STOCK_SET__);
		EXPECT_EQ(pTask->GetTime(), 150700);

		pTask = gl_pChinaMarket->GetMarketTask();
		gl_pChinaMarket->DiscardMarketTask();
		EXPECT_EQ(pTask->GetType(), CHINA_MARKET_UPDATE_OPTION_DB__);
		EXPECT_EQ(pTask->GetTime(), 150905);

		pTask = gl_pChinaMarket->GetMarketTask();
		gl_pChinaMarket->DiscardMarketTask();
		EXPECT_EQ(pTask->GetType(), CHINA_MARKET_VALIDATE_TODAY_DATABASE__);
		EXPECT_EQ(pTask->GetTime(), 151000);

		pTask = gl_pChinaMarket->GetMarketTask();
		gl_pChinaMarket->DiscardMarketTask();
		EXPECT_EQ(pTask->GetType(), CHINA_MARKET_UPDATE_STOCK_PROFILE_DB__);
		EXPECT_EQ(pTask->GetTime(), 151010);

		EXPECT_TRUE(gl_pChinaMarket->IsMarketTaskEmpty()) << gl_pChinaMarket->GetMarketTask()->GetTime();
	}

	TEST_F(CChinaMarketTest, TestTaskCreateTask5) {
		tm tm_{0};
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
		gl_pChinaMarket->DiscardMarketTask();
		EXPECT_EQ(pTask->GetType(), CHINA_MARKET_CHECK_SYSTEM_READY__);
		EXPECT_EQ(pTask->GetTime(), 1);

		pTask = gl_pChinaMarket->GetMarketTask();
		gl_pChinaMarket->DiscardMarketTask();
		EXPECT_EQ(pTask->GetType(), CHINA_MARKET_DISTRIBUTE_AND_CALCULATE_RT_DATA__);
		EXPECT_EQ(pTask->GetTime(), 1);

		pTask = gl_pChinaMarket->GetMarketTask();
		gl_pChinaMarket->DiscardMarketTask();
		EXPECT_EQ(pTask->GetType(), CHINA_MARKET_LOAD_TEMP_RT_DATA__);
		EXPECT_EQ(pTask->GetTime(), 92700);

		pTask = gl_pChinaMarket->GetMarketTask();
		gl_pChinaMarket->DiscardMarketTask();
		EXPECT_EQ(pTask->GetType(), CHINA_MARKET_PROCESS_AND_SAVE_DAY_LINE__);
		EXPECT_EQ(pTask->GetTime(), 113510);

		pTask = gl_pChinaMarket->GetMarketTask();
		gl_pChinaMarket->DiscardMarketTask();
		EXPECT_EQ(pTask->GetType(), CHINA_MARKET_ACCESSORY_TASK__);
		EXPECT_EQ(pTask->GetTime(), 150700);

		pTask = gl_pChinaMarket->GetMarketTask();
		gl_pChinaMarket->DiscardMarketTask();
		EXPECT_EQ(pTask->GetType(), CHINA_MARKET_UPDATE_OPTION_DB__);

		pTask = gl_pChinaMarket->GetMarketTask();
		gl_pChinaMarket->DiscardMarketTask();
		EXPECT_EQ(pTask->GetType(), CHINA_MARKET_UPDATE_STOCK_PROFILE_DB__);

		EXPECT_TRUE(gl_pChinaMarket->IsMarketTaskEmpty()) << gl_pChinaMarket->GetMarketTask()->GetTime();
	}

	TEST_F(CChinaMarketTest, TestClearUpdateStockCodeDBFlag) {
		EXPECT_FALSE(gl_pChinaMarket->IsDayLineDBUpdated());
		for (int i = 0; i < gl_pChinaMarket->GetTotalStock(); i++) {
			const CChinaStockPtr pStock = gl_pChinaMarket->GetStock(i);
			pStock->SetDayLineDBUpdated(true);
		}
		EXPECT_TRUE(gl_pChinaMarket->IsDayLineDBUpdated());
		gl_pChinaMarket->ClearDayLineDBUpdatedFlag();
		EXPECT_FALSE(gl_pChinaMarket->IsDayLineDBUpdated());
	}

	TEST_F(CChinaMarketTest, TestGetNeteaseDayLineInquiringStr) {
		CChinaStockPtr pStock = gl_pChinaMarket->GetStock(0);
		EXPECT_TRUE(pStock->IsDayLineNeedUpdate()) << _T("测试时使用teststock数据库，此数据库比较旧，最后更新时间不是昨日，故而活跃股票也需要更新日线");

		pStock->SetDayLineNeedUpdate(false);
		pStock = gl_pChinaMarket->GetStock(1);
		EXPECT_TRUE(pStock->IsDayLineNeedUpdate());
		EXPECT_LT(pStock->GetDayLineEndDate(), gl_pChinaMarket->GetLastTradeDate()) << pStock->GetDayLineEndDate();
		pStock = gl_pChinaMarket->GetStock(2);
		EXPECT_TRUE(pStock->IsDayLineNeedUpdate());
		EXPECT_LT(pStock->GetDayLineEndDate(), gl_pChinaMarket->GetLastTradeDate());
		const long lDate = pStock->GetDayLineEndDate();
		pStock->SetDayLineEndDate(gl_pChinaMarket->GetMarketDate());
		CString str = gl_pChinaMarket->CreateNeteaseDayLineInquiringStr();
		EXPECT_STREQ(str, _T("1000001"));
		pStock = gl_pChinaMarket->GetStock(1);
		EXPECT_FALSE(pStock->IsDayLineNeedUpdate());
		str = gl_pChinaMarket->CreateNeteaseDayLineInquiringStr();
		EXPECT_STREQ(str, _T("1000002"));

		gl_pChinaMarket->GetStock(2)->SetDayLineEndDate(lDate); // 恢复原状。
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
		long lOffset = gl_pChinaMarket->GetMinLineOffset(tUTC);
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

	TEST_F(CChinaMarketTest, TestIsAStock) {
		const auto pStock = make_shared<CChinaStock>();
		pStock->SetSymbol(_T("600000.SS"));
		EXPECT_TRUE(gl_pChinaMarket->IsAStock(pStock));
		pStock->SetSymbol(_T("600000.SA"));
		EXPECT_FALSE(gl_pChinaMarket->IsAStock(pStock));
		pStock->SetSymbol(_T("000001.SZ"));
		EXPECT_TRUE(gl_pChinaMarket->IsAStock(pStock));
		pStock->SetSymbol(_T("10001.SZ"));
		EXPECT_FALSE(gl_pChinaMarket->IsAStock(pStock));
	}

	TEST_F(CChinaMarketTest, TestIsAStock2) {
		EXPECT_TRUE(gl_pChinaMarket->IsAStock(_T("600000.SS")));
		EXPECT_TRUE(gl_pChinaMarket->IsAStock(_T("601198.SS")));
		EXPECT_FALSE(gl_pChinaMarket->IsAStock(_T("602102.SS")));
		EXPECT_FALSE(gl_pChinaMarket->IsAStock(_T("603604.SS")));
		EXPECT_FALSE(gl_pChinaMarket->IsAStock(_T("604604.SS")));
		EXPECT_FALSE(gl_pChinaMarket->IsAStock(_T("605604.SS")));
		EXPECT_FALSE(gl_pChinaMarket->IsAStock(_T("606604.SS")));
		EXPECT_FALSE(gl_pChinaMarket->IsAStock(_T("607604.SS")));
		EXPECT_FALSE(gl_pChinaMarket->IsAStock(_T("608604.SS")));
		EXPECT_FALSE(gl_pChinaMarket->IsAStock(_T("609604.SS")));

		EXPECT_FALSE(gl_pChinaMarket->IsAStock(_T("600000.SZ")));
		EXPECT_FALSE(gl_pChinaMarket->IsAStock(_T("000001.SA")));
		EXPECT_FALSE(gl_pChinaMarket->IsAStock(_T("000001.AZ")));
		EXPECT_FALSE(gl_pChinaMarket->IsAStock(_T("200001.SZ")));
		EXPECT_TRUE(gl_pChinaMarket->IsAStock(_T("000001.SZ")));
		EXPECT_TRUE(gl_pChinaMarket->IsAStock(_T("002389.SZ")));
		EXPECT_FALSE(gl_pChinaMarket->IsAStock(_T("003389.SZ")));
		EXPECT_FALSE(gl_pChinaMarket->IsAStock(_T("004389.SZ")));
		EXPECT_FALSE(gl_pChinaMarket->IsAStock(_T("005389.SZ")));
		EXPECT_FALSE(gl_pChinaMarket->IsAStock(_T("006389.SZ")));
		EXPECT_FALSE(gl_pChinaMarket->IsAStock(_T("007389.SZ")));
		EXPECT_FALSE(gl_pChinaMarket->IsAStock(_T("008389.SZ")));
		EXPECT_FALSE(gl_pChinaMarket->IsAStock(_T("009389.SZ")));
		EXPECT_FALSE(gl_pChinaMarket->IsAStock(_T("001389.SZ")));
	}

	TEST_F(CChinaMarketTest, TestIsStock) {
		EXPECT_GT(gl_pChinaMarket->GetTotalStock(), 1);
		EXPECT_TRUE(gl_pChinaMarket->IsStock(_T("600000.SS")));
		EXPECT_FALSE(gl_pChinaMarket->IsStock(_T("60000.SS")));
	}

	TEST_F(CChinaMarketTest, TestGetStockName) {
		//not implemented. 由于stockName存储时使用的是UniCode制式，而本系统默认是Ansi制式，导致无法进行字符串对比。暂时不进行测试了。
		// EXPECT_STREQ(gl_pChinaMarket->GetStockName(_T("600000.SS")), _T("浦发银行"));
		EXPECT_STREQ(gl_pChinaMarket->GetStockName(_T("60000.SS")), _T("")); // 没找到时返回空字符串
	}

	TEST_F(CChinaMarketTest, TestGetStockCode) {
		EXPECT_FALSE(gl_pChinaMarket->GetStock(_T("600001.SS")) == nullptr);

		EXPECT_FALSE(gl_pChinaMarket->GetStock(0) == nullptr);
		EXPECT_FALSE(gl_pChinaMarket->GetStock(gl_pChinaMarket->GetTotalStock() - 1) == nullptr);
	}

	TEST_F(CChinaMarketTest, TestGetCurrentStock) {
		CChinaStockPtr pStock = gl_pChinaMarket->GetStock(7);
		const CChinaStockPtr pStock2 = gl_pChinaMarket->GetStock(4);

		gl_pChinaMarket->SetCurrentStock(pStock);
		EXPECT_EQ(gl_pChinaMarket->GetCurrentStock(), pStock);
		EXPECT_TRUE(gl_pChinaMarket->IsCurrentStockChanged());
		gl_pChinaMarket->SetCurrentStockChanged(false);
		gl_pChinaMarket->SetCurrentStock(pStock);
		EXPECT_FALSE(gl_pChinaMarket->IsCurrentStockChanged());
		gl_pChinaMarket->SetCurrentStock(pStock2);
		EXPECT_TRUE(gl_pChinaMarket->IsCurrentStockChanged());
		gl_pChinaMarket->SetCurrentStock(_T("600000.SS"));
		pStock = gl_pChinaMarket->GetCurrentStock();
		EXPECT_STREQ(pStock->GetSymbol(), _T("600000.SS"));
		gl_pChinaMarket->ResetCurrentStock();
		EXPECT_EQ(gl_pChinaMarket->GetCurrentStock(), nullptr);

		//恢复原状
		while (!gl_pChinaMarket->IsMarketTaskEmpty()) gl_pChinaMarket->DiscardMarketTask();
		gl_pChinaMarket->SetCurrentStockChanged(false);
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

	TEST_F(CChinaMarketTest, TestIsChosen10RSStrong2StockSet) {
		gl_pChinaMarket->SetChosen10RSStrong2StockSet(true);
		EXPECT_TRUE(gl_pChinaMarket->IsChosen10RSStrong2StockSet());
		gl_pChinaMarket->SetChosen10RSStrong2StockSet(false);
		EXPECT_FALSE(gl_pChinaMarket->IsChosen10RSStrong2StockSet());
	}

	TEST_F(CChinaMarketTest, TestIsDayLineNeedSaving) {
		EXPECT_FALSE(gl_pChinaMarket->IsDayLineNeedSaving());
		const CChinaStockPtr pStock = gl_pChinaMarket->GetStock(0);
		pStock->SetDayLineNeedSaving(true);
		EXPECT_TRUE(gl_pChinaMarket->IsDayLineNeedSaving());
		pStock->SetDayLineNeedSaving(false);
	}

	TEST_F(CChinaMarketTest, TestIsDayLineNeedUpdate) {
		CChinaStockPtr pStock;
		EXPECT_TRUE(gl_pChinaMarket->IsDayLineNeedUpdate());
		for (int i = 0; i < gl_pChinaMarket->GetTotalStock(); i++) {
			pStock = gl_pChinaMarket->GetStock(i);
			if (pStock->IsDayLineNeedUpdate()) pStock->SetDayLineNeedUpdate(false);
		}
		EXPECT_FALSE(gl_pChinaMarket->IsDayLineNeedUpdate());
		for (int i = 0; i < gl_pChinaMarket->GetTotalStock(); i++) {
			pStock = gl_pChinaMarket->GetStock(i);
			if (!pStock->IsDayLineNeedUpdate()) pStock->SetDayLineNeedUpdate(true);
		}
	}

	TEST_F(CChinaMarketTest, TestIsDayLineNeedProcess) {
		EXPECT_FALSE(gl_pChinaMarket->IsDayLineNeedProcess()) << "默认状态下无需处理";
		const auto pData = make_shared<CDayLineWebData>();
		gl_pChinaMarket->PushDayLine(pData);

		EXPECT_TRUE(gl_pChinaMarket->IsDayLineNeedProcess());

		gl_pChinaMarket->PopDayLine();
		EXPECT_FALSE(gl_pChinaMarket->IsDayLineNeedProcess());
	}

	TEST_F(CChinaMarketTest, TestProcessDayLine) {
		const auto pData = make_shared<CDayLineWebData>();
		CChinaStockPtr pStock = gl_pChinaMarket->GetStock(_T("600666.SS"));
		CString strTest = _T("");

		pData->SetStockCode(_T("600666.SS"));
		gl_pChinaMarket->PushDayLine(pData);

		EXPECT_TRUE(gl_pChinaMarket->ProcessDayLine());
	}

	TEST_F(CChinaMarketTest, TestIsLoadSelectedStock) {
		EXPECT_FALSE(gl_pChinaMarket->IsLoadSelectedStock());
		gl_pChinaMarket->SetLoadSelectedStock(true);
		EXPECT_TRUE(gl_pChinaMarket->IsLoadSelectedStock());
		gl_pChinaMarket->SetLoadSelectedStock(false);
		EXPECT_FALSE(gl_pChinaMarket->IsLoadSelectedStock());
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

	TEST_F(CChinaMarketTest, TestIsSavingTempData) {
		EXPECT_TRUE(gl_pChinaMarket->IsSavingTempData());
		gl_pChinaMarket->SetSavingTempData(false);
		EXPECT_FALSE(gl_pChinaMarket->IsSavingTempData());
		gl_pChinaMarket->SetSavingTempData(true);
		EXPECT_TRUE(gl_pChinaMarket->IsSavingTempData());
	}

	TEST_F(CChinaMarketTest, TestIsUpdateStockProfileDB) {
		EXPECT_THAT(gl_pChinaMarket->IsUpdateStockProfileDB(), IsFalse());
		gl_pChinaMarket->GetStock(1)->SetUpdateProfileDB(true);
		EXPECT_THAT(gl_pChinaMarket->IsUpdateStockProfileDB(), IsTrue());

		gl_pChinaMarket->GetStock(1)->SetUpdateProfileDB(false);
	}

	TEST_F(CChinaMarketTest, TestTaskResetMarket) {
		gl_pChinaMarket->SetResetMarket(false);
		gl_pChinaMarket->SetSystemReady(true);

		EXPECT_TRUE(gl_pChinaMarket->TaskResetMarket(92500));

		EXPECT_FALSE(gl_pChinaMarket->IsSystemReady());
		EXPECT_TRUE(gl_pChinaMarket->IsResetMarket());
		EXPECT_FALSE(gl_pChinaMarket->IsMarketTaskEmpty());
		auto pTask = gl_pChinaMarket->GetMarketTask();
		gl_pChinaMarket->DiscardMarketTask();
		EXPECT_EQ(pTask->GetType(), CHINA_MARKET_CHECK_SYSTEM_READY__); // 重置系统时，也同时生成检查系统初始化完成与否的任务
		EXPECT_EQ(pTask->GetTime(), 92500);
		pTask = gl_pChinaMarket->GetMarketTask();
		gl_pChinaMarket->DiscardMarketTask();
		EXPECT_EQ(pTask->GetType(), CHINA_MARKET_UPDATE_STOCK_SECTION__);
		EXPECT_EQ(pTask->GetTime(), 93000);

		// 恢复原状
		while (!gl_pChinaMarket->IsMarketTaskEmpty()) gl_pChinaMarket->DiscardMarketTask();
	}

	TEST_F(CChinaMarketTest, TestIsCurrentEditStockChanged) {
		EXPECT_FALSE(gl_pChinaMarket->IsCurrentEditStockChanged());
		gl_pChinaMarket->SetCurrentEditStockChanged(true);
		EXPECT_TRUE(gl_pChinaMarket->IsCurrentEditStockChanged());
		gl_pChinaMarket->SetCurrentEditStockChanged(false);
		EXPECT_FALSE(gl_pChinaMarket->IsCurrentEditStockChanged());
	}

	TEST_F(CChinaMarketTest, TestAddChosenStock) {
		auto pStock = gl_pChinaMarket->GetStock(1);
		gl_pChinaMarket->AddChosenStock(pStock);
		EXPECT_EQ(gl_pChinaMarket->GetChosenStockSize(), 1);
		pStock = gl_pChinaMarket->GetStock(2);
		EXPECT_TRUE(gl_pChinaMarket->AddChosenStock(pStock));
		EXPECT_EQ(gl_pChinaMarket->GetChosenStockSize(), 2);
		pStock = gl_pChinaMarket->GetStock(1);
		EXPECT_FALSE(gl_pChinaMarket->AddChosenStock(pStock));
		EXPECT_EQ(gl_pChinaMarket->GetChosenStockSize(), 2);

		EXPECT_TRUE(gl_pChinaMarket->DeleteChosenStock(pStock));
		pStock = gl_pChinaMarket->GetStock(5);
		EXPECT_EQ(gl_pChinaMarket->GetChosenStockSize(), 1);
		EXPECT_FALSE(gl_pChinaMarket->DeleteChosenStock(pStock));
		gl_pChinaMarket->ClearChoiceStockContainer();
	}

	TEST_F(CChinaMarketTest, TestAddChosenStock2) {
		EXPECT_EQ(gl_pChinaMarket->GetChosenStockSize(), 0);
		const CChinaStockPtr pStock = gl_pChinaMarket->GetStock(0);
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
		long k = 0;
		int i = gl_pChinaMarket->IncreaseStockInquiringIndex(k, gl_pChinaMarket->GetTotalStock());
		EXPECT_EQ(i, 1);
		EXPECT_EQ(k, 1);
		i = gl_pChinaMarket->IncreaseStockInquiringIndex(k, gl_pChinaMarket->GetTotalStock());
		EXPECT_EQ(i, 2);
		EXPECT_EQ(k, 2);
		k = 11999;
		i = gl_pChinaMarket->IncreaseStockInquiringIndex(k, gl_pChinaMarket->GetTotalStock());
		EXPECT_EQ(k, 0);
		EXPECT_EQ(i, 0);
	}

	TEST_F(CChinaMarketTest, TestProcessRTData) {
		gl_pChinaMarket->SetRTDataNeedCalculate(true);
		EXPECT_TRUE(gl_pChinaMarket->ProcessRTData());
		gl_pChinaMarket->SetRTDataNeedCalculate(false);
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
		CChinaStockPtr pStock = gl_pChinaMarket->GetStock(0);
		EXPECT_THAT(pStock->IsIPOed(), IsTrue());
		EXPECT_STREQ(pStock->GetSymbol(), _T("000001.SS"));
		EXPECT_EQ(pStock->GetDayLineStartDate(), 19901220);
		EXPECT_FALSE(pStock->IsActive()) << "装载股票代码时永远设置为假";
		pStock = gl_pChinaMarket->GetStock(_T("600002.SS"));
		EXPECT_TRUE(pStock->IsDelisted());
		EXPECT_EQ(pStock->GetDayLineStartDate(), 19980409);
		EXPECT_EQ(pStock->GetDayLineEndDate(), 20060406);
		EXPECT_FALSE(pStock->IsActive());
	}

	TEST_F(CChinaMarketTest, TestGetLastLoginDate) {
		gl_pChinaMarket->SetLastLoginDate(19900102);
		EXPECT_EQ(gl_pChinaMarket->GetLastLoginDate(), 19900102);
	}

	TEST_F(CChinaMarketTest, TestGetRSStartDate) {
		gl_pChinaMarket->SetRSStartDate(19900202);
		EXPECT_EQ(gl_pChinaMarket->GetRSStartDate(), 19900202);
	}

	TEST_F(CChinaMarketTest, TestGetRSEndDate) {
		gl_pChinaMarket->SetRSEndDate(19900302);
		EXPECT_EQ(gl_pChinaMarket->GetRSEndDate(), 19900302);
	}

	TEST_F(CChinaMarketTest, TestGetNewestTransactionTime) {
		gl_pChinaMarket->SetNewestTransactionTime(10101010);
		EXPECT_EQ(gl_pChinaMarket->GetNewestTransactionTime(), 10101010);
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
		const CChinaStockPtr pStock = gl_pChinaMarket->GetStock(_T("600000.SS"));
		EXPECT_STREQ(pStock->GetSymbol(), _T("600000.SS"));
	}

	TEST_F(CChinaMarketTest, TestClearDayLineNeedUpdaeStatus) {
		gl_pChinaMarket->ClearDayLineNeedUpdateStatus();

		for (int i = 0; i < gl_pChinaMarket->GetTotalStock(); i++) {
			EXPECT_FALSE(gl_pChinaMarket->GetStock(i)->IsDayLineNeedUpdate());
		}

		for (int i = 0; i < gl_pChinaMarket->GetTotalStock(); i++) {
			gl_pChinaMarket->GetStock(i)->SetDayLineNeedUpdate(true);
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
		gl_pChinaMarket->SetResetMarket(false);
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
		while (!gl_pChinaMarket->IsMarketTaskEmpty()) gl_pChinaMarket->DiscardMarketTask();
	}

	TEST_F(CChinaMarketTest, TestChangeCurrentStockToNextStock1) {
		EXPECT_EQ(gl_pChinaMarket->GetCurrentSelectedStockSet(), -1);
		gl_pChinaMarket->SetCurrentStock(gl_pChinaMarket->GetStock(0));
		gl_pChinaMarket->ChangeToNextStock();
		EXPECT_EQ(gl_pChinaMarket->GetStockIndex(gl_pChinaMarket->GetCurrentStock()), 1);
		gl_pChinaMarket->SetCurrentStockChanged(false);

		gl_pChinaMarket->SetCurrentSelectedPosition(0);
		gl_pChinaMarket->SetCurrentSelectedStockSet(-1);

		//恢复原状
		while (!gl_pChinaMarket->IsMarketTaskEmpty()) gl_pChinaMarket->DiscardMarketTask();
	}

	TEST_F(CChinaMarketTest, TestChangeCurrentStockToNextStock2) {
		EXPECT_EQ(gl_pChinaMarket->GetCurrentSelectedStockSet(), -1);
		gl_pChinaMarket->SetCurrentSelectedStockSet(10); // 10、11、12股票集中有股票
		EXPECT_TRUE(gl_pChinaMarket->GetCurrentSelectedStock() != nullptr);
		gl_pChinaMarket->SetCurrentStock(gl_pChinaMarket->GetCurrentSelectedStock());

		gl_pChinaMarket->ChangeToPrevStock();
		gl_pChinaMarket->ChangeToNextStock();
		EXPECT_EQ(gl_pChinaMarket->GetCurrentSelectedPosition(), 0);
		gl_pChinaMarket->ChangeToNextStock();
		EXPECT_EQ(gl_pChinaMarket->GetCurrentSelectedPosition(), 1);
		gl_pChinaMarket->ChangeToPrevStock();
		gl_pChinaMarket->ChangeToPrevStock();
		EXPECT_EQ(gl_pChinaMarket->GetCurrentStockSetSize(), gl_pChinaMarket->GetCurrentSelectedPosition() + 1);
		gl_pChinaMarket->SetCurrentStockChanged(false);

		gl_pChinaMarket->SetCurrentSelectedPosition(0);
		gl_pChinaMarket->SetCurrentSelectedStockSet(-1);

		//恢复原状
		while (!gl_pChinaMarket->IsMarketTaskEmpty()) gl_pChinaMarket->DiscardMarketTask();
	}

	TEST_F(CChinaMarketTest, TestChangeCurrentStockToPrevStock1) {
		EXPECT_EQ(gl_pChinaMarket->GetCurrentSelectedStockSet(), -1);
		gl_pChinaMarket->SetCurrentStock(gl_pChinaMarket->GetStock(1)); // 选取A股指数
		gl_pChinaMarket->ChangeToPrevStock();
		gl_pChinaMarket->ChangeToPrevStock();
		EXPECT_EQ(gl_pChinaMarket->GetStockIndex(gl_pChinaMarket->GetCurrentStock()), gl_pChinaMarket->GetTotalStock() - 1) << _T("上证指数前的为空，然后就转到最后面的中证煤炭了");
		gl_pChinaMarket->SetCurrentStockChanged(false);
		gl_pChinaMarket->SetCurrentSelectedPosition(0);

		//恢复原状
		while (!gl_pChinaMarket->IsMarketTaskEmpty()) gl_pChinaMarket->DiscardMarketTask();
	}

	TEST_F(CChinaMarketTest, TestChangeCurrentStockToPrevStock2) {
		EXPECT_EQ(gl_pChinaMarket->GetCurrentSelectedStockSet(), -1);
		gl_pChinaMarket->SetCurrentSelectedStockSet(10); // 10、11、12股票集中有股票
		EXPECT_TRUE(gl_pChinaMarket->GetCurrentSelectedStock() != nullptr);
		gl_pChinaMarket->SetCurrentStock(gl_pChinaMarket->GetCurrentSelectedStock());

		gl_pChinaMarket->ChangeToNextStock();
		EXPECT_EQ(gl_pChinaMarket->GetCurrentSelectedPosition(), 1);
		gl_pChinaMarket->ChangeToPrevStock();
		EXPECT_EQ(gl_pChinaMarket->GetCurrentSelectedPosition(), 0);
		gl_pChinaMarket->ChangeToPrevStock();
		EXPECT_EQ(gl_pChinaMarket->GetCurrentSelectedPosition() + 1, gl_pChinaMarket->GetCurrentStockSetSize());
		gl_pChinaMarket->SetCurrentStockChanged(false);

		gl_pChinaMarket->SetCurrentSelectedPosition(0);
		gl_pChinaMarket->SetCurrentSelectedStockSet(-1);

		//恢复原状
		while (!gl_pChinaMarket->IsMarketTaskEmpty()) gl_pChinaMarket->DiscardMarketTask();
	}

	TEST_F(CChinaMarketTest, TestChangeToPrevStockSet) {
		EXPECT_EQ(gl_pChinaMarket->GetCurrentSelectedStockSet(), -1);
		gl_pChinaMarket->ChangeToPrevStockSet();
		EXPECT_EQ(gl_pChinaMarket->GetCurrentSelectedStockSet(), 19);
		gl_pChinaMarket->ChangeToPrevStockSet();
		EXPECT_EQ(gl_pChinaMarket->GetCurrentSelectedStockSet(), 18);
		gl_pChinaMarket->ChangeToPrevStockSet();
		EXPECT_EQ(gl_pChinaMarket->GetCurrentSelectedStockSet(), 12);
		gl_pChinaMarket->ChangeToPrevStockSet();
		EXPECT_EQ(gl_pChinaMarket->GetCurrentSelectedStockSet(), 11);
		gl_pChinaMarket->ChangeToPrevStockSet();
		EXPECT_EQ(gl_pChinaMarket->GetCurrentSelectedStockSet(), 10);

		gl_pChinaMarket->SetCurrentSelectedStockSet(-1);
	}

	TEST_F(CChinaMarketTest, TestChangeToNextStockSet) {
		EXPECT_EQ(gl_pChinaMarket->GetCurrentSelectedStockSet(), -1);
		gl_pChinaMarket->ChangeToNextStockSet();
		EXPECT_EQ(gl_pChinaMarket->GetCurrentSelectedStockSet(), 10);
		gl_pChinaMarket->ChangeToNextStockSet();
		EXPECT_EQ(gl_pChinaMarket->GetCurrentSelectedStockSet(), 11);
		gl_pChinaMarket->ChangeToNextStockSet();
		EXPECT_EQ(gl_pChinaMarket->GetCurrentSelectedStockSet(), 12);
		gl_pChinaMarket->ChangeToNextStockSet();
		EXPECT_EQ(gl_pChinaMarket->GetCurrentSelectedStockSet(), 18);
		gl_pChinaMarket->ChangeToNextStockSet();
		EXPECT_EQ(gl_pChinaMarket->GetCurrentSelectedStockSet(), 19);

		gl_pChinaMarket->SetCurrentSelectedStockSet(-1);
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
		CSetOption setOption;

		setOption.Open();
		setOption.m_pDatabase->BeginTrans();
		while (!setOption.IsEOF()) {
			setOption.Delete();
			setOption.MoveNext();
		}
		setOption.m_pDatabase->CommitTrans();
		setOption.Close();

		gl_pChinaMarket->SetRSStartDate(20200101);
		gl_pChinaMarket->SetRSEndDate(20200202);
		gl_pChinaMarket->SetLastLoginDate(gl_pChinaMarket->GetMarketDate());
		gl_pChinaMarket->SetUpdatedDateFor10DaysRS1(19990101);
		gl_pChinaMarket->SetUpdatedDateFor10DaysRS2(19990202);

		gl_pChinaMarket->UpdateOptionDB();

		gl_pChinaMarket->SetRSStartDate(1);
		gl_pChinaMarket->SetRSEndDate(1);
		gl_pChinaMarket->SetLastLoginDate(1);
		gl_pChinaMarket->SetUpdatedDateFor10DaysRS1(1);
		gl_pChinaMarket->SetUpdatedDateFor10DaysRS2(1);

		gl_pChinaMarket->LoadOptionDB();

		EXPECT_EQ(gl_pChinaMarket->GetRSStartDate(), 20200101);
		EXPECT_EQ(gl_pChinaMarket->GetRSEndDate(), 20200202);
		EXPECT_EQ(gl_pChinaMarket->GetLastLoginDate(), gl_pChinaMarket->GetMarketDate());
		EXPECT_EQ(gl_pChinaMarket->GetUpdatedDateFor10DaysRS1(), 19990101);
		EXPECT_FALSE(gl_pChinaMarket->IsChosen10RSStrong1StockSet());
		EXPECT_EQ(gl_pChinaMarket->GetUpdatedDateFor10DaysRS2(), 19990202);
		EXPECT_FALSE(gl_pChinaMarket->IsChosen10RSStrong2StockSet());

		gl_pChinaMarket->SetRSStartDate(20100101);
		gl_pChinaMarket->SetRSEndDate(20100202);
		gl_pChinaMarket->SetLastLoginDate(20200303);
		gl_pChinaMarket->SetUpdatedDateFor10DaysRS1(19980101);
		gl_pChinaMarket->SetUpdatedDateFor10DaysRS2(19980202);

		gl_pChinaMarket->UpdateOptionDB();

		gl_pChinaMarket->SetRSStartDate(1);
		gl_pChinaMarket->SetRSEndDate(1);
		gl_pChinaMarket->SetLastLoginDate(1);
		gl_pChinaMarket->SetUpdatedDateFor10DaysRS1(1);
		gl_pChinaMarket->SetUpdatedDateFor10DaysRS2(1);

		gl_pChinaMarket->LoadOptionDB();

		EXPECT_EQ(gl_pChinaMarket->GetRSStartDate(), 20100101);
		EXPECT_EQ(gl_pChinaMarket->GetRSEndDate(), 20100202);
		EXPECT_EQ(gl_pChinaMarket->GetLastLoginDate(), gl_pChinaMarket->GetMarketDate()) << _T("永远是当前日期\n");
		EXPECT_EQ(gl_pChinaMarket->GetUpdatedDateFor10DaysRS1(), 19980101);
		EXPECT_FALSE(gl_pChinaMarket->IsChosen10RSStrong1StockSet());
		EXPECT_EQ(gl_pChinaMarket->GetUpdatedDateFor10DaysRS2(), 19980202);
		EXPECT_FALSE(gl_pChinaMarket->IsChosen10RSStrong2StockSet());

		setOption.Open();
		setOption.m_pDatabase->BeginTrans();
		while (!setOption.IsEOF()) {
			setOption.Delete();
			setOption.MoveNext();
		}
		setOption.m_pDatabase->CommitTrans();
		setOption.Close();
		gl_pChinaMarket->LoadOptionDB();

		EXPECT_EQ(gl_pChinaMarket->GetRSStartDate(), _CHINA_MARKET_BEGIN_DATE_);
		EXPECT_EQ(gl_pChinaMarket->GetRSEndDate(), _CHINA_MARKET_BEGIN_DATE_);
		EXPECT_EQ(gl_pChinaMarket->GetLastLoginDate(), _CHINA_MARKET_BEGIN_DATE_);
		EXPECT_EQ(gl_pChinaMarket->GetUpdatedDateFor10DaysRS1(), _CHINA_MARKET_BEGIN_DATE_);
		EXPECT_FALSE(gl_pChinaMarket->IsChosen10RSStrong1StockSet());
		EXPECT_EQ(gl_pChinaMarket->GetUpdatedDateFor10DaysRS2(), _CHINA_MARKET_BEGIN_DATE_);
		EXPECT_FALSE(gl_pChinaMarket->IsChosen10RSStrong2StockSet());
	}

	TEST_F(CChinaMarketTest, TestDeleteDayLineBasicInfo) {
		char buffer[20];

		CSetDayLineBasicInfo setDayLine, setDayLine2;
		const auto pDayLine = make_shared<CDayLine>();

		pDayLine->SetStockSymbol(_T("600000.SS"));
		pDayLine->SetDate(19900101);

		_ltoa_s(19900101, buffer, 10);
		const CString strDate = buffer;
		setDayLine.m_strFilter = _T("[Date] =");
		setDayLine.m_strFilter += strDate;
		setDayLine.Open();
		setDayLine.m_pDatabase->BeginTrans();
		pDayLine->AppendBasicData(&setDayLine);
		setDayLine.m_pDatabase->CommitTrans();
		setDayLine.Close();

		setDayLine.m_strFilter = _T("[Date] =");
		setDayLine.m_strFilter += strDate;
		setDayLine.Open();
		EXPECT_FALSE(setDayLine.IsEOF());
		setDayLine.Close();

		gl_pChinaMarket->DeleteDayLineBasicInfo(19900101);

		setDayLine2.m_strFilter = _T("[Date] =");
		setDayLine2.m_strFilter += strDate;
		setDayLine2.Open();
		EXPECT_TRUE(setDayLine2.IsEOF());
		setDayLine2.Close();
	}

	TEST_F(CChinaMarketTest, TestDeleteDayLineExtendInfo) {
		char buffer[20];

		CSetDayLineExtendInfo setDayLine, setDayLine2;
		const auto pDayLine = make_shared<CDayLine>();

		pDayLine->SetStockSymbol(_T("600000.SS"));
		pDayLine->SetDate(19900101);

		_ltoa_s(19900101, buffer, 10);
		const CString strDate = buffer;
		setDayLine.m_strFilter = _T("[ID] = 1");
		setDayLine.Open();
		setDayLine.m_pDatabase->BeginTrans();
		setDayLine.AddNew();
		setDayLine.m_Symbol = _T("600000.SS");
		setDayLine.m_Date = 19900101;
		setDayLine.Update();
		setDayLine.m_pDatabase->CommitTrans();
		setDayLine.Close();

		setDayLine.m_strFilter = _T("[Date] =");
		setDayLine.m_strFilter += strDate;
		setDayLine.Open();
		EXPECT_FALSE(setDayLine.IsEOF());
		setDayLine.Close();

		gl_pChinaMarket->DeleteDayLineExtendInfo(19900101);

		setDayLine2.m_strFilter = _T("[Date] =");
		setDayLine2.m_strFilter += strDate;
		setDayLine2.Open();
		EXPECT_TRUE(setDayLine2.IsEOF());
		setDayLine2.Close();
	}

	TEST_F(CChinaMarketTest, TestDeleteCurrentWeekLine) {
		CSetCurrentWeekLine setCurrentWeekLine, setCurrentWeekLine2;
		const auto pWeekLine = make_shared<CWeekLine>();

		pWeekLine->SetStockSymbol(_T("600000.SS"));
		pWeekLine->SetDate(GetCurrentMonday(20200101));
		setCurrentWeekLine.m_strFilter = _T("[ID] = 1");
		setCurrentWeekLine.Open();
		setCurrentWeekLine.m_pDatabase->BeginTrans();
		pWeekLine->Append(&setCurrentWeekLine);
		setCurrentWeekLine.m_pDatabase->CommitTrans();
		setCurrentWeekLine.Close();

		setCurrentWeekLine.m_strFilter = _T("");
		setCurrentWeekLine.Open();
		EXPECT_FALSE(setCurrentWeekLine.IsEOF());
		setCurrentWeekLine.Close();

		gl_pChinaMarket->DeleteCurrentWeekWeekLine();

		setCurrentWeekLine2.Open();
		EXPECT_TRUE(setCurrentWeekLine2.IsEOF());
		setCurrentWeekLine2.Close();
	}

	TEST_F(CChinaMarketTest, TestLoadDayLine) {
		CContainerChinaDayLine dataChinaDayLine;
		const long lDate = GetCurrentMonday(20200101);

		gl_pChinaMarket->LoadDayLine(dataChinaDayLine, lDate);

		CString strSQL;
		char pch[30];
		CSetDayLineBasicInfo setDayLineBasicInfo;
		CSetDayLineExtendInfo setDayLineExtendInfo;
		long i = 0;

		sprintf_s(pch, _T("%08d"), lDate);
		const CString strDate = pch;
		setDayLineBasicInfo.m_strSort = _T("[Symbol]");
		setDayLineBasicInfo.m_strFilter = _T("[Date] =");
		setDayLineBasicInfo.m_strFilter += strDate;
		setDayLineBasicInfo.Open();
		while (!setDayLineBasicInfo.IsEOF()) {
			const CDayLinePtr pDayLine = dynamic_pointer_cast<CDayLine>(dataChinaDayLine.GetData(i++));
			EXPECT_STREQ(setDayLineBasicInfo.m_Symbol, pDayLine->GetStockSymbol());
			setDayLineBasicInfo.MoveNext();
		}
		EXPECT_EQ(i, dataChinaDayLine.Size());
		setDayLineBasicInfo.Close();
	}

	TEST_F(CChinaMarketTest, TestCreateStockCodeSet) {
		vector<CVirtualHistoryCandleExtendPtr> vData;
		auto pData = make_shared<CVirtualHistoryCandleExtend>();
		pData->SetStockSymbol(_T("600000.SS"));
		vData.resize(2);
		vData[0] = pData;
		pData = make_shared<CVirtualHistoryCandleExtend>();
		pData->SetStockSymbol(_T("600004.SS"));
		vData[1] = pData;

		set<CString> setStockCode;
		gl_pChinaMarket->CreateStockCodeSet(setStockCode, &vData);

		EXPECT_TRUE(setStockCode.contains(_T("600000.SS")));
		EXPECT_TRUE(setStockCode.contains(_T("600004.SS")));
		EXPECT_FALSE(setStockCode.contains(_T("600001.SS")));
	}

	TEST_F(CChinaMarketTest, TestUpdateStockProfileDB) {
		ASSERT_THAT(gl_pChinaMarket->IsUpdateStockProfileDB(), IsFalse()) << "此测试开始时，必须保证没有设置更新代码库的标识，否则会真正更新了测试代码库";

		auto pStock = make_shared<CChinaStock>();
		pStock->SetSymbol(_T("SS.SS.SS"));
		pStock->SetTodayNewStock(true);
		pStock->SetUpdateProfileDB(true);
		EXPECT_FALSE(gl_pChinaMarket->IsStock(pStock->GetSymbol())); // 确保是一个新股票代码
		gl_pChinaMarket->AddStock(pStock);
		pStock = gl_pChinaMarket->GetStock(_T("000001.SS"));
		EXPECT_EQ(pStock->GetIPOStatus(), _STOCK_IPOED_);
		pStock->SetUpdateProfileDB(true);
		pStock->SetIPOStatus(_STOCK_DELISTED_);

		gl_pChinaMarket->UpdateStockProfileDB();

		CSetChinaStockSymbol setChinaStock;
		setChinaStock.m_strFilter = _T("[Symbol] = '000001.SS'");
		setChinaStock.Open();
		EXPECT_EQ(setChinaStock.m_IPOStatus, _STOCK_DELISTED_);
		setChinaStock.m_pDatabase->BeginTrans();
		setChinaStock.Edit();
		setChinaStock.m_IPOStatus = _STOCK_IPOED_;
		setChinaStock.Update();
		setChinaStock.m_pDatabase->CommitTrans();
		setChinaStock.Close();

		setChinaStock.m_strFilter = _T("[Symbol] = 'SS.SS.SS'");
		setChinaStock.Open();
		EXPECT_FALSE(setChinaStock.IsEOF());
		setChinaStock.m_pDatabase->BeginTrans();
		while (!setChinaStock.IsEOF()) {
			setChinaStock.Delete();
			setChinaStock.MoveNext();
		}
		setChinaStock.m_pDatabase->CommitTrans();
		setChinaStock.Close();

		pStock = gl_pChinaMarket->GetStock(_T("000001.SS"));
		pStock->SetIPOStatus(_STOCK_IPOED_); // 恢复原状
		pStock = gl_pChinaMarket->GetStock(_T("SS.SS.SS"));
		EXPECT_TRUE(pStock != nullptr);
		gl_pChinaMarket->DeleteStock(pStock); // 恢复原状

		EXPECT_THAT(gl_pChinaMarket->IsUpdateStockProfileDB(), IsFalse()) << "此测试结束时，必须保证没有设置更新代码库的标识，否则会真正更新了测试代码库";

		for (int i = 0; i < gl_pChinaMarket->GetTotalStock(); i++) {
			pStock = gl_pChinaMarket->GetStock(i);
			pStock->SetUpdateProfileDB(false);
		}
	}

	TEST_F(CChinaMarketTest, TestAddStock) {
		CChinaStockPtr pStock = nullptr;
		EXPECT_FALSE(gl_pChinaMarket->AddStock(pStock));

		pStock = gl_pChinaMarket->GetStock(1);
		EXPECT_FALSE(gl_pChinaMarket->AddStock(pStock));

		pStock = make_shared<CChinaStock>();
		pStock->SetSymbol(_T("SS.SS.SS"));
		const auto lTotal = gl_pChinaMarket->GetTotalStock();
		EXPECT_FALSE(gl_pChinaMarket->IsStock(pStock->GetSymbol()));
		EXPECT_TRUE(gl_pChinaMarket->AddStock(pStock));
		EXPECT_EQ(gl_pChinaMarket->GetTotalStock(), lTotal + 1);

		gl_pChinaMarket->DeleteStock(pStock);
	}

	TEST_F(CChinaMarketTest, TestDeleteStock) {
		CChinaStockPtr pStock = nullptr;
		EXPECT_FALSE(gl_pChinaMarket->DeleteStock(pStock));

		pStock = make_shared<CChinaStock>();
		pStock->SetSymbol(_T("SS.SS.SS"));
		EXPECT_FALSE(gl_pChinaMarket->DeleteStock(pStock));

		gl_pChinaMarket->AddStock(pStock);
		EXPECT_TRUE(gl_pChinaMarket->DeleteStock(pStock));
	}

	TEST_F(CChinaMarketTest, TestTaskProcessTengxunRTData) {
		auto pRTData = make_shared<CWebRTData>();

		EXPECT_THAT(gl_pChinaMarket->TengxunRTSize(), Eq(0));

		pRTData->SetActive(true);
		pRTData->SetSymbol(_T("600000.SS"));
		pRTData->SetTotalValue(101010101010);
		pRTData->SetCurrentValue(2020202020202020);
		pRTData->SetHighLimit(10101010);
		pRTData->SetLowLimit(1010);

		gl_pChinaMarket->PushTengxunRT(pRTData);

		pRTData = make_shared<CWebRTData>();

		pRTData->SetActive(false);
		pRTData->SetSymbol(_T("600001.SS"));
		pRTData->SetTotalValue(101010101010);
		pRTData->SetCurrentValue(2020202020202020);
		pRTData->SetHighLimit(10101010);
		pRTData->SetLowLimit(1010);

		gl_pChinaMarket->PushTengxunRT(pRTData);

		CChinaStockPtr pStock = gl_pChinaMarket->GetStock(_T("600000.SS"));
		EXPECT_THAT(pStock->GetTotalValue(), 0);
		EXPECT_THAT(pStock->GetCurrentValue(), 0);
		EXPECT_THAT(pStock->GetHighLimit(), 0);
		EXPECT_THAT(pStock->GetLowLimit(), 0);

		pStock = gl_pChinaMarket->GetStock(_T("600001.SS"));
		EXPECT_THAT(pStock->GetTotalValue(), 0);
		EXPECT_THAT(pStock->GetCurrentValue(), 0);
		EXPECT_THAT(pStock->GetHighLimit(), 0);
		EXPECT_THAT(pStock->GetLowLimit(), 0);

		EXPECT_TRUE(gl_pChinaMarket->ProcessTengxunRTData());

		pStock = gl_pChinaMarket->GetStock(_T("600000.SS"));
		EXPECT_THAT(pStock->GetTotalValue(), 101010101010);
		EXPECT_THAT(pStock->GetCurrentValue(), 2020202020202020);
		EXPECT_THAT(pStock->GetHighLimit(), 10101010);
		EXPECT_THAT(pStock->GetLowLimit(), 1010);

		pStock = gl_pChinaMarket->GetStock(_T("600001.SS"));
		EXPECT_THAT(pStock->GetTotalValue(), 0);
		EXPECT_THAT(pStock->GetCurrentValue(), 0);
		EXPECT_THAT(pStock->GetHighLimit(), 0);
		EXPECT_THAT(pStock->GetLowLimit(), 0);

		EXPECT_THAT(gl_pChinaMarket->TengxunRTSize(), Eq(0));
	}

	TEST_F(CChinaMarketTest, TestTaskCheckDayLineDB) {
		EXPECT_TRUE(gl_pChinaMarket->IsDayLineNeedUpdate());
		EXPECT_FALSE(gl_pChinaMarket->IsDayLineNeedProcess());
		EXPECT_FALSE(gl_pChinaMarket->IsDayLineNeedSaving());
		EXPECT_FALSE(gl_pChinaMarket->IsDayLineDBUpdated());

		EXPECT_FALSE(gl_pChinaMarket->IsFinishedSavingDayLineDB()) << "IsSaveDayLine为假";

		const CChinaStockPtr pStock = gl_pChinaMarket->GetStock(0);
		pStock->SetDayLineNeedSaving(true);
		pStock->SetDayLineDBUpdated(true);
		EXPECT_FALSE(gl_pChinaMarket->IsFinishedSavingDayLineDB()) << "IsDayLineNeedUpdate等皆为真";
		EXPECT_TRUE(gl_pChinaMarket->IsDayLineDBUpdated());

		EXPECT_FALSE(gl_pChinaMarket->IsFinishedSavingDayLineDB()) << "IsDayLineNeedUpdate和IsDayLineNeedSaving为真";
		EXPECT_TRUE(gl_pChinaMarket->IsDayLineDBUpdated());

		pStock->SetDayLineNeedSaving(false);
		EXPECT_FALSE(gl_pChinaMarket->IsFinishedSavingDayLineDB()) << "IsDayLineNeedUpdate为真";
		EXPECT_TRUE(gl_pChinaMarket->IsDayLineDBUpdated());

		for (int i = 0; i < gl_pChinaMarket->GetTotalStock(); i++) {
			const auto china_stock_ptr = gl_pChinaMarket->GetStock(i);
			china_stock_ptr->SetDayLineNeedUpdate(false);
		}
		EXPECT_TRUE(gl_pChinaMarket->IsFinishedSavingDayLineDB()) << "条件满足了";
		EXPECT_FALSE(pStock->IsDayLineDBUpdated());
		EXPECT_FALSE(gl_pChinaMarket->IsDayLineDBUpdated());
		EXPECT_EQ(gl_systemMessage.InformationSize(), 1);
		EXPECT_STREQ(gl_systemMessage.PopInformationMessage(), _T("中国市场日线历史数据更新完毕"));

		for (int i = 0; i < gl_pChinaMarket->GetTotalStock(); i++) {
			const auto china_stock_ptr = gl_pChinaMarket->GetStock(i);
			china_stock_ptr->SetDayLineNeedUpdate(true);
		}
	}

	TEST_F(CChinaMarketTest, TestAppendChosenStock) {
		CChinaStockPtr pStock = gl_pChinaMarket->GetStock(_T("600601.SS"));
		pStock->SetChosen(true);
		gl_pChinaMarket->AddChosenStock(pStock);
		pStock = gl_pChinaMarket->GetStock(_T("000001.SZ"));
		pStock->SetChosen(true);
		gl_pChinaMarket->AddChosenStock(pStock);

		gl_pChinaMarket->AppendChosenStockDB();

		pStock = gl_pChinaMarket->GetStock(_T("600601.SS"));
		gl_pChinaMarket->DeleteChosenStock(pStock);
		pStock = gl_pChinaMarket->GetStock(_T("000001.SZ"));
		gl_pChinaMarket->DeleteChosenStock(pStock);
		EXPECT_FALSE(gl_pChinaMarket->GetStock(_T("600601.SS"))->IsChosen());
		EXPECT_FALSE(gl_pChinaMarket->GetStock(_T("000001.SZ"))->IsChosen());

		gl_pChinaMarket->LoadChosenStockDB();
		EXPECT_TRUE(gl_pChinaMarket->GetStock(_T("600601.SS"))->IsChosen());
		EXPECT_TRUE(gl_pChinaMarket->GetStock(_T("000001.SZ"))->IsChosen());

		pStock = gl_pChinaMarket->GetStock(_T("600601.SS"));
		gl_pChinaMarket->DeleteChosenStock(pStock);
		pStock = gl_pChinaMarket->GetStock(_T("000001.SZ"));
		gl_pChinaMarket->DeleteChosenStock(pStock);

		pStock = gl_pChinaMarket->GetStock(_T("600000.SS"));
		pStock->SetChosen(true);
		gl_pChinaMarket->AddChosenStock(pStock);
		pStock = gl_pChinaMarket->GetStock(_T("000002.SZ"));
		pStock->SetChosen(true);
		gl_pChinaMarket->AddChosenStock(pStock);

		gl_pChinaMarket->UpdateChosenStockDB();

		// 恢复原状
		pStock = gl_pChinaMarket->GetStock(_T("600000.SS"));
		gl_pChinaMarket->DeleteChosenStock(pStock);
		pStock = gl_pChinaMarket->GetStock(_T("000002.SZ"));
		gl_pChinaMarket->DeleteChosenStock(pStock);

		CSetChinaChosenStock setChinaChosenStock;
		setChinaChosenStock.Open();
		setChinaChosenStock.m_pDatabase->BeginTrans();
		setChinaChosenStock.MoveLast();
		EXPECT_STREQ(setChinaChosenStock.m_Symbol, _T("000002.SZ"));
		setChinaChosenStock.Delete();
		setChinaChosenStock.MovePrev();
		EXPECT_STREQ(setChinaChosenStock.m_Symbol, _T("600000.SS"));
		setChinaChosenStock.Delete();
		setChinaChosenStock.m_pDatabase->CommitTrans();
		setChinaChosenStock.Close();
	}

	TEST_F(CChinaMarketTest, TestLoadTodayTempDB1) {
		EXPECT_FALSE(gl_pChinaMarket->IsTodayTempRTDataLoaded());
		EXPECT_TRUE(gl_pChinaMarket->IsSystemReady());
		gl_pChinaMarket->SetSystemReady(false);

		EXPECT_FALSE(gl_pChinaMarket->TaskLoadTempRTData(20210715, 10000));

		EXPECT_FALSE(gl_pChinaMarket->IsMarketTaskEmpty());
		const auto pTask = gl_pChinaMarket->GetMarketTask();
		EXPECT_EQ(pTask->GetType(), CHINA_MARKET_LOAD_TEMP_RT_DATA__) << "系统初始化尚未完成时，继续生成装载任务";
		EXPECT_EQ(pTask->GetTime(), 10001);
		gl_pChinaMarket->DiscardMarketTask();

		EXPECT_TRUE(gl_pChinaMarket->IsMarketTaskEmpty());
		EXPECT_FALSE(gl_pChinaMarket->IsTodayTempRTDataLoaded());
	}

	TEST_F(CChinaMarketTest, TestLoadTempRTData) {
		const CChinaStockPtr pStock = gl_pChinaMarket->GetStock(_T("000001.SZ"));
		pStock->SetUnknownVolume(0);
		pStock->SetTransactionNumber(0);

		gl_pChinaMarket->LoadTempRTData(20210715); // 测试库today表中的日期为20210715。

		EXPECT_TRUE(gl_pChinaMarket->IsTodayTempRTDataLoaded());
		EXPECT_EQ(pStock->GetUnknownVolume(), 10517770);
		EXPECT_EQ(pStock->GetTransactionNumber(), 2183);

		// 恢复原状
		gl_pChinaMarket->SetTodayTempRTDataLoaded(false);
	}

	TEST_F(CChinaMarketTest, TestIsRTDataNeedCalculate) {
		EXPECT_FALSE(gl_pChinaMarket->IsRTDataNeedCalculate());
		gl_pChinaMarket->SetRTDataNeedCalculate(true);
		EXPECT_TRUE(gl_pChinaMarket->IsRTDataNeedCalculate());
		gl_pChinaMarket->SetRTDataNeedCalculate(false);
		EXPECT_FALSE(gl_pChinaMarket->IsRTDataNeedCalculate());
	}

	TEST_F(CChinaMarketTest, TestIsCalculatingDayLineRS) {
		EXPECT_FALSE(gl_pChinaMarket->IsCalculatingDayLineRS());
		gl_pChinaMarket->SetCalculatingDayLineRS(true);
		EXPECT_TRUE(gl_pChinaMarket->IsCalculatingDayLineRS());
		gl_pChinaMarket->SetCalculatingDayLineRS(false);
		EXPECT_FALSE(gl_pChinaMarket->IsCalculatingDayLineRS());
	}

	TEST_F(CChinaMarketTest, TestIsCalculatingWeekLineRS) {
		EXPECT_FALSE(gl_pChinaMarket->IsCalculatingWeekLineRS());
		gl_pChinaMarket->SetCalculatingWeekLineRS(true);
		EXPECT_TRUE(gl_pChinaMarket->IsCalculatingWeekLineRS());
		gl_pChinaMarket->SetCalculatingWeekLineRS(false);
		EXPECT_FALSE(gl_pChinaMarket->IsCalculatingWeekLineRS());
	}
}
