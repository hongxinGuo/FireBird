///////////////////////////////////////////////////////////////////////////////////////////
//
// 所有使用Mock类测试ChinaMarket的函数皆位于此文件中。
//
///////////////////////////////////////////////////////////////////////////////////////////
#include"pch.h"

//#include"gtest/gtest.h"

#include"SystemMessage.h"

#include "Thread.h"
#include"TimeConvert.h"

#include"ChinaStock.h"

#include"MockChinaMarket.h"

#include"GeneralCheck.h"
#include "NeteaseDayLineDataSource.h"

using namespace testing;
#include<memory>

namespace FireBirdTest {
	CMockChinaMarketPtr s_pMockChinaMarket;

	class CMockChinaMarketTest : public Test {
	protected:
		static void SetUpTestSuite() {
			SCOPED_TRACE("");
			GeneralCheck();

			EXPECT_FALSE(gl_pChinaMarket->IsCurrentStockChanged());

			s_pMockChinaMarket = make_shared<CMockChinaMarket>();
			s_pMockChinaMarket->ResetMarket();
			while (gl_systemMessage.InformationSize() > 0) gl_systemMessage.PopInformationMessage();
			EXPECT_LE(gl_dataContainerChinaStock.GetDayLineNeedUpdateNumber(), gl_dataContainerChinaStock.Size());

			for (int i = 0; i < gl_dataContainerChinaStock.Size(); i++) {
				const auto pStock = gl_dataContainerChinaStock.GetStock(i);
				pStock->SetUpdateDayLine(true);
				if (pStock->GetDayLineEndDate() == 20210430) pStock->SetIPOStatus(_STOCK_IPOED_); // 修改活跃股票的IPO状态

				if (IsEarlyThen(pStock->GetDayLineEndDate(), s_pMockChinaMarket->GetMarketDate(), 30)) {
					if (pStock->GetDayLineEndDate() == 20210430) {
						EXPECT_TRUE(pStock->IsUpdateProfileDB()) << pStock->GetSymbol(); //"当股票日线结束日期早于30日时，装入股票代码数据库时要求更新代码库";
						pStock->SetUpdateProfileDB(false);
					}
				}
			}
			EXPECT_EQ(gl_dataContainerChinaStock.GetDayLineNeedUpdateNumber(), gl_dataContainerChinaStock.Size());
			while (!s_pMockChinaMarket->IsMarketTaskEmpty()) s_pMockChinaMarket->DiscardCurrentMarketTask();

			EXPECT_TRUE(s_pMockChinaMarket != nullptr) << "此Mock变量在EnvironmentSetUp.h中生成";
			EXPECT_FALSE(gl_dataContainerChinaStock.IsUpdateDayLineDB());
			EXPECT_EQ(gl_dataContainerChinaStock.GetDayLineNeedSaveNumber(), 0);

			ASSERT_THAT(gl_pNeteaseDayLineDataSource, NotNull());

			EXPECT_FALSE(s_pMockChinaMarket->IsSystemReady());
			s_pMockChinaMarket->SetSystemReady(true);// 测试系统默认为准备好了
		}

		static void TearDownTestSuite() {
			EXPECT_EQ(gl_dataContainerChinaStock.GetDayLineNeedUpdateNumber(), gl_dataContainerChinaStock.Size());
			EXPECT_FALSE(gl_dataContainerChinaStock.IsUpdateDayLineDB());
			EXPECT_EQ(gl_dataContainerChinaStock.GetDayLineNeedSaveNumber(), 0);

			EXPECT_EQ(gl_pChinaMarket->GetCurrentStock(), nullptr) << gl_pChinaMarket->GetCurrentStock()->GetSymbol();
			EXPECT_FALSE(gl_pChinaMarket->IsCurrentStockChanged());

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

			s_pMockChinaMarket->SetRSEndDate(19900101);
		}

		void TearDown() override {
			// clearUp
			EXPECT_EQ(gl_dataContainerChinaStock.GetDayLineNeedUpdateNumber(), gl_dataContainerChinaStock.Size());
			EXPECT_EQ(gl_dataContainerChinaStock.GetDayLineNeedSaveNumber(), 0);
			if (gl_dataContainerChinaStock.GetDayLineNeedSaveNumber() > 0) {
				for (int i = 0; i < gl_dataContainerChinaStock.Size(); i++) {
					const CChinaStockPtr pStock = gl_dataContainerChinaStock.GetStock(i);
					if (pStock->IsUpdateDayLineDB()) { EXPECT_STREQ(pStock->GetSymbol(), _T("")); }
				}
			}
			s_pMockChinaMarket->SetRSEndDate(19900101);
			s_pMockChinaMarket->SetUpdateOptionDB(false);

			EXPECT_TRUE(s_pMockChinaMarket->IsSystemReady());
			EXPECT_TRUE(s_pMockChinaMarket->IsMarketTaskEmpty()) << s_pMockChinaMarket->GetMarketTask()->GetTime();
			EXPECT_FALSE(s_pMockChinaMarket->IsUpdateChosenStockDB());

			SCOPED_TRACE("");
			GeneralCheck();
		}
	};

	TEST_F(CMockChinaMarketTest, TestProcessEveryDayTask1) {
		s_pMockChinaMarket->AddTask(CHINA_MARKET_CREATE_TASK__, 10000);

		EXPECT_CALL(*s_pMockChinaMarket, TaskCreateTask(10000)).Times(1);

		EXPECT_TRUE(s_pMockChinaMarket->ProcessTask(10000));

		EXPECT_TRUE(s_pMockChinaMarket->IsMarketTaskEmpty()) << s_pMockChinaMarket->GetMarketTask()->GetTime();
	}

	TEST_F(CMockChinaMarketTest, TestProcessEveryDayTask2) {
		s_pMockChinaMarket->AddTask(CHINA_MARKET_RESET__, 91300);

		EXPECT_CALL(*s_pMockChinaMarket, TaskResetMarket(91300)).Times(1);

		EXPECT_TRUE(s_pMockChinaMarket->ProcessTask(91300));

		EXPECT_TRUE(s_pMockChinaMarket->IsMarketTaskEmpty());
	}

	TEST_F(CMockChinaMarketTest, TestProcessEveryDayTask3) {
		s_pMockChinaMarket->AddTask(CHINA_MARKET_RESET__, 92600);

		EXPECT_CALL(*s_pMockChinaMarket, TaskResetMarket(92700)).Times(1);

		EXPECT_TRUE(s_pMockChinaMarket->ProcessTask(92700));

		EXPECT_TRUE(s_pMockChinaMarket->IsMarketTaskEmpty());
	}

	TEST_F(CMockChinaMarketTest, TestProcessEveryDayTask4) {
		s_pMockChinaMarket->AddTask(CHINA_MARKET_DISTRIBUTE_AND_CALCULATE_RT_DATA__, 91500);

		EXPECT_TRUE(s_pMockChinaMarket->ProcessTask(93000));

		EXPECT_FALSE(s_pMockChinaMarket->IsMarketTaskEmpty());
		const auto pTask = s_pMockChinaMarket->GetMarketTask();
		s_pMockChinaMarket->DiscardCurrentMarketTask();
		EXPECT_EQ(pTask->GetType(), CHINA_MARKET_DISTRIBUTE_AND_CALCULATE_RT_DATA__);
		EXPECT_EQ(pTask->GetTime(), 93001) << "每秒一次";

		EXPECT_TRUE(s_pMockChinaMarket->IsMarketTaskEmpty());
	}

	TEST_F(CMockChinaMarketTest, TestProcessEveryDayTask9) {
		s_pMockChinaMarket->AddTask(RELOAD_SYSTEM__, 210000);

		EXPECT_CALL(*s_pMockChinaMarket, TaskExitSystem(210000)).Times(1);

		EXPECT_TRUE(s_pMockChinaMarket->ProcessTask(210000));

		EXPECT_TRUE(s_pMockChinaMarket->IsMarketTaskEmpty()) << s_pMockChinaMarket->GetMarketTask()->GetTime();
	}

	TEST_F(CMockChinaMarketTest, TestTaskAccessoryPerMinuteTask) {
		EXPECT_TRUE(s_pMockChinaMarket->IsMarketTaskEmpty());

		s_pMockChinaMarket->TaskAccessoryPerMinuteTask(10000);

		EXPECT_FALSE(s_pMockChinaMarket->IsMarketTaskEmpty());
		const auto pTask = s_pMockChinaMarket->GetMarketTask();
		s_pMockChinaMarket->DiscardCurrentMarketTask();
		EXPECT_EQ(pTask->GetType(), CHINA_MARKET_PER_MINUTE_ACCESSORY_TASK__);
		EXPECT_EQ(pTask->GetTime(), 10100) << "每一分钟一次";

		EXPECT_TRUE(s_pMockChinaMarket->IsMarketTaskEmpty());
	}

	TEST_F(CMockChinaMarketTest, TestTaskDistributeAndCalculateRTData) {
		s_pMockChinaMarket->TaskDistributeAndCalculateRTData(100000);

		const auto pTask = s_pMockChinaMarket->GetMarketTask();
		s_pMockChinaMarket->DiscardCurrentMarketTask();
		EXPECT_EQ(pTask->GetType(), CHINA_MARKET_DISTRIBUTE_AND_CALCULATE_RT_DATA__);
		EXPECT_EQ(pTask->GetTime(), 100001) << "每秒一次";

		EXPECT_TRUE(s_pMockChinaMarket->IsMarketTaskEmpty());
	}

	TEST_F(CMockChinaMarketTest, TestTaskChoice10RSStrongStockSet) {
		tm tm_{};
		tm_.tm_wday = 1; // 星期一
		s_pMockChinaMarket->TEST_SetMarketTM(tm_);
		s_pMockChinaMarket->SetSystemReady(false);
		EXPECT_FALSE(s_pMockChinaMarket->TaskChoice10RSStrongStockSet(151000));
		EXPECT_FALSE(s_pMockChinaMarket->TaskChoice10RSStrongStockSet(151001));
		s_pMockChinaMarket->SetSystemReady(true);
		s_pMockChinaMarket->SetChosen10RSStrongStockSet(true);
		EXPECT_FALSE(s_pMockChinaMarket->TaskChoice10RSStrongStockSet(151000));
		EXPECT_FALSE(s_pMockChinaMarket->TaskChoice10RSStrongStockSet(151001));
		s_pMockChinaMarket->SetSystemReady(true);
		s_pMockChinaMarket->SetChosen10RSStrongStockSet(false);
		EXPECT_CALL(*s_pMockChinaMarket, Choice10RSStrongStockSet)
		.Times(1);
		EXPECT_FALSE(s_pMockChinaMarket->TaskChoice10RSStrongStockSet(151000));
		EXPECT_TRUE(s_pMockChinaMarket->TaskChoice10RSStrongStockSet(151001));
		EXPECT_TRUE(s_pMockChinaMarket->IsChosen10RSStrongStockSet());

		tm_.tm_wday = 0; // 星期日
		s_pMockChinaMarket->TEST_SetMarketTM(tm_);
		s_pMockChinaMarket->SetChosen10RSStrongStockSet(false);
		EXPECT_FALSE(s_pMockChinaMarket->TaskChoice10RSStrongStockSet(151001));
		EXPECT_FALSE(s_pMockChinaMarket->IsChosen10RSStrongStockSet()) << _T("休息日不处理");
	}
}
