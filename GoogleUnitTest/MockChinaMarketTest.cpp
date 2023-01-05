///////////////////////////////////////////////////////////////////////////////////////////
//
// 所有使用Mock类测试ChinaMarket的函数皆位于此文件中。
//
///////////////////////////////////////////////////////////////////////////////////////////
#include"pch.h"

#include"ThreadStatus.h"

#include"Thread.h"
#include"TimeConvert.h"

#include"ChinaMarket.h"
#include"ChinaStock.h"

#include"MockNeteaseDayLineWebInquiry.h"
#include"MockChinaMarket.h"

#include"GeneralCheck.h"

using namespace testing;
#include<memory>

namespace FireBirdTest {
	extern CMockChinaMarketPtr gl_pMockChinaMarket;

	static CMockNeteaseDayLineWebInquiryPtr s_pMockNeteaseDayLineWebInquiry;

	class CMockChinaMarketTest : public ::testing::Test {
	protected:
		static void SetUpTestSuite(void) {
			EXPECT_FALSE(gl_pChinaMarket->IsCurrentStockChanged());

			EXPECT_TRUE(gl_pMockChinaMarket != nullptr) << "此Mock变量在EnvironmentSetUp.h中生成";
			gl_pMockChinaMarket->SetTodayStockProcessed(false);
			EXPECT_FALSE(gl_pMockChinaMarket->IsDayLineNeedSaving());
			EXPECT_EQ(gl_pMockChinaMarket->GetDayLineNeedSaveNumber(), 0);

			ASSERT_THAT(gl_pNeteaseDayLineWebInquiry, NotNull());
			s_pMockNeteaseDayLineWebInquiry = static_pointer_cast<CMockNeteaseDayLineWebInquiry>(gl_pNeteaseDayLineWebInquiry);

			GeneralCheck();
		}

		static void TearDownTestSuite(void) {
			EXPECT_EQ(gl_pMockChinaMarket->GetDayLineNeedUpdateNumber(), gl_pMockChinaMarket->GetTotalStock());
			EXPECT_FALSE(gl_pMockChinaMarket->IsDayLineNeedSaving());
			EXPECT_EQ(gl_pMockChinaMarket->GetDayLineNeedSaveNumber(), 0);

			EXPECT_EQ(gl_pChinaMarket->GetCurrentStock(), nullptr) << gl_pChinaMarket->GetCurrentStock()->GetSymbol();
			EXPECT_FALSE(gl_pChinaMarket->IsCurrentStockChanged());

			s_pMockNeteaseDayLineWebInquiry = nullptr;

			GeneralCheck();
		}

		void SetUp(void) override {
			//EXPECT_EQ(gl_pChinaMarket->GetDayLineNeedUpdateNumber(), gl_pChinaMarket->GetTotalStock());
			EXPECT_EQ(gl_pMockChinaMarket->GetDayLineNeedSaveNumber(), 0);
			EXPECT_EQ(gl_pMockChinaMarket->GetDayLineNeedUpdateNumber(), gl_pMockChinaMarket->GetTotalStock());

			gl_pMockChinaMarket->SetTodayStockProcessed(false);
			gl_pMockChinaMarket->SetRSEndDate(19900101);

			GeneralCheck();
		}

		void TearDown(void) override {
			// clearUp
			EXPECT_EQ(gl_pMockChinaMarket->GetDayLineNeedUpdateNumber(), gl_pMockChinaMarket->GetTotalStock());
			EXPECT_EQ(gl_pMockChinaMarket->GetDayLineNeedSaveNumber(), 0);
			CChinaStockPtr pStock;
			if (gl_pMockChinaMarket->GetDayLineNeedSaveNumber() > 0) {
				for (int i = 0; i < gl_pMockChinaMarket->GetTotalStock(); i++) {
					pStock = gl_pMockChinaMarket->GetStock(i);
					if (pStock->IsDayLineNeedSaving()) { EXPECT_STREQ(pStock->GetSymbol(), _T("")); }
				}
			}
			gl_pMockChinaMarket->SetTodayStockProcessed(false);
			gl_pMockChinaMarket->SetRSEndDate(19900101);
			gl_pMockChinaMarket->SetUpdateOptionDB(false);

			GeneralCheck();
		}
	};

	TEST_F(CMockChinaMarketTest, TestTaskLoadCurrentStockDayLine1) {
		gl_pMockChinaMarket->ResetCurrentStock();
		EXPECT_CALL(*gl_pMockChinaMarket, CreatingThreadLoadDayLine(_))
			.Times(0);
		EXPECT_TRUE(gl_pMockChinaMarket->TaskLoadCurrentStockHistoryData());
	}

	TEST_F(CMockChinaMarketTest, TestTaskLoadCurrentStockDayLine2) {
		gl_pMockChinaMarket->ResetCurrentStock();
		gl_pMockChinaMarket->SetCurrentStock(_T("600000.SS"));
		gl_pMockChinaMarket->GetCurrentStock()->SetDayLineLoaded(true);
		gl_pMockChinaMarket->GetCurrentStock()->SetWeekLineLoaded(true);
		EXPECT_CALL(*gl_pMockChinaMarket, CreatingThreadLoadDayLine(_))
			.Times(0);
		EXPECT_CALL(*gl_pMockChinaMarket, CreatingThreadLoadWeekLine(_))
			.Times(0);
		EXPECT_TRUE(gl_pMockChinaMarket->TaskLoadCurrentStockHistoryData());
		EXPECT_TRUE(gl_pMockChinaMarket->GetCurrentStock()->IsDayLineLoaded());
		EXPECT_TRUE(gl_pMockChinaMarket->GetCurrentStock()->IsWeekLineLoaded());
	}

	TEST_F(CMockChinaMarketTest, TestTaskLoadCurrentStockDayLine3) {
		gl_pMockChinaMarket->ResetCurrentStock();
		gl_pMockChinaMarket->SetCurrentStock(_T("600000.SS"));
		gl_pMockChinaMarket->GetCurrentStock()->SetDayLineLoaded(false);
		gl_pMockChinaMarket->GetCurrentStock()->SetWeekLineLoaded(false);
		EXPECT_CALL(*gl_pMockChinaMarket, CreatingThreadLoadDayLine(_))
			.Times(1);
		EXPECT_CALL(*gl_pMockChinaMarket, CreatingThreadLoadWeekLine(_))
			.Times(1);
		EXPECT_TRUE(gl_pMockChinaMarket->TaskLoadCurrentStockHistoryData());
		EXPECT_TRUE(gl_pMockChinaMarket->GetCurrentStock()->IsDayLineLoaded());
		EXPECT_TRUE(gl_pMockChinaMarket->GetCurrentStock()->IsWeekLineLoaded());
	}

	TEST_F(CMockChinaMarketTest, TestTaskUpdateStockCodeDB) {
		EXPECT_CALL(*gl_pMockChinaMarket, CreatingThreadUpdateStockCodeDB())
			.Times(0);
		EXPECT_FALSE(gl_pMockChinaMarket->TaskUpdateStockCodeDB());

		gl_pMockChinaMarket->GetStock(1)->SetUpdateProfileDB(true);
		EXPECT_CALL(*gl_pMockChinaMarket, CreatingThreadUpdateStockCodeDB())
			.Times(1);
		EXPECT_TRUE(gl_pMockChinaMarket->TaskUpdateStockCodeDB());

		gl_pMockChinaMarket->GetStock(1)->SetUpdateProfileDB(false);
	}

	TEST_F(CMockChinaMarketTest, TestTaskChoice10RSStrong2StockSet) {
		tm tm_;
		tm_.tm_wday = 1; // 星期一
		gl_pMockChinaMarket->TEST_SetMarketTM(tm_);
		gl_pMockChinaMarket->SetSystemReady(false);
		EXPECT_FALSE(gl_pMockChinaMarket->TaskChoice10RSStrong2StockSet(151200));
		EXPECT_FALSE(gl_pMockChinaMarket->TaskChoice10RSStrong2StockSet(151201));
		gl_pMockChinaMarket->SetSystemReady(true);
		gl_pMockChinaMarket->SetChosen10RSStrong2StockSet(true);
		EXPECT_FALSE(gl_pMockChinaMarket->TaskChoice10RSStrong2StockSet(151200));
		EXPECT_FALSE(gl_pMockChinaMarket->TaskChoice10RSStrong2StockSet(151201));
		gl_pMockChinaMarket->SetSystemReady(true);
		gl_pMockChinaMarket->SetChosen10RSStrong2StockSet(false);
		EXPECT_CALL(*gl_pMockChinaMarket, CreatingThreadChoice10RSStrong2StockSet)
			.Times(1);
		EXPECT_FALSE(gl_pMockChinaMarket->TaskChoice10RSStrong2StockSet(151200));
		EXPECT_TRUE(gl_pMockChinaMarket->TaskChoice10RSStrong2StockSet(151201));
		EXPECT_TRUE(gl_pMockChinaMarket->IsChosen10RSStrong2StockSet());

		tm_.tm_wday = 0; // 星期日
		gl_pMockChinaMarket->TEST_SetMarketTM(tm_);
		gl_pMockChinaMarket->SetTodayStockProcessed(false);
		gl_pMockChinaMarket->SetChosen10RSStrong2StockSet(false);
		EXPECT_FALSE(gl_pMockChinaMarket->TaskChoice10RSStrong2StockSet(151201));
		EXPECT_FALSE(gl_pMockChinaMarket->IsChosen10RSStrong2StockSet()) << _T("休息日不处理");
	}

	TEST_F(CMockChinaMarketTest, TestTaskChoice10RSStrong1StockSet) {
		tm tm_;
		tm_.tm_wday = 1; // 星期一
		gl_pMockChinaMarket->TEST_SetMarketTM(tm_);
		gl_pMockChinaMarket->SetSystemReady(false);
		EXPECT_FALSE(gl_pMockChinaMarket->TaskChoice10RSStrong1StockSet(151100));
		EXPECT_FALSE(gl_pMockChinaMarket->TaskChoice10RSStrong1StockSet(151101));
		gl_pMockChinaMarket->SetSystemReady(true);
		gl_pMockChinaMarket->SetChosen10RSStrong1StockSet(true);
		EXPECT_FALSE(gl_pMockChinaMarket->TaskChoice10RSStrong1StockSet(151100));
		EXPECT_FALSE(gl_pMockChinaMarket->TaskChoice10RSStrong1StockSet(151101));
		gl_pMockChinaMarket->SetSystemReady(true);
		gl_pMockChinaMarket->SetChosen10RSStrong1StockSet(false);
		EXPECT_CALL(*gl_pMockChinaMarket, CreatingThreadChoice10RSStrong1StockSet)
			.Times(1);
		EXPECT_FALSE(gl_pMockChinaMarket->TaskChoice10RSStrong1StockSet(151100));
		EXPECT_TRUE(gl_pMockChinaMarket->TaskChoice10RSStrong1StockSet(151101));
		EXPECT_TRUE(gl_pMockChinaMarket->IsChosen10RSStrong1StockSet());

		tm_.tm_wday = 0; // 星期日
		gl_pMockChinaMarket->TEST_SetMarketTM(tm_);
		gl_pMockChinaMarket->SetTodayStockProcessed(false);
		gl_pMockChinaMarket->SetChosen10RSStrong1StockSet(false);
		EXPECT_FALSE(gl_pMockChinaMarket->TaskChoice10RSStrong1StockSet(151101));
		EXPECT_FALSE(gl_pMockChinaMarket->IsChosen10RSStrong1StockSet()) << _T("休息日不处理");
	}

	TEST_F(CMockChinaMarketTest, TestTaskChoice10RSStrongStockSet) {
		tm tm_;
		tm_.tm_wday = 1; // 星期一
		gl_pMockChinaMarket->TEST_SetMarketTM(tm_);
		gl_pMockChinaMarket->SetSystemReady(false);
		EXPECT_FALSE(gl_pMockChinaMarket->TaskChoice10RSStrongStockSet(151000));
		EXPECT_FALSE(gl_pMockChinaMarket->TaskChoice10RSStrongStockSet(151001));
		gl_pMockChinaMarket->SetSystemReady(true);
		gl_pMockChinaMarket->SetChosen10RSStrongStockSet(true);
		EXPECT_FALSE(gl_pMockChinaMarket->TaskChoice10RSStrongStockSet(151000));
		EXPECT_FALSE(gl_pMockChinaMarket->TaskChoice10RSStrongStockSet(151001));
		gl_pMockChinaMarket->SetSystemReady(true);
		gl_pMockChinaMarket->SetChosen10RSStrongStockSet(false);
		EXPECT_CALL(*gl_pMockChinaMarket, CreatingThreadChoice10RSStrongStockSet)
			.Times(1);
		EXPECT_FALSE(gl_pMockChinaMarket->TaskChoice10RSStrongStockSet(151000));
		EXPECT_TRUE(gl_pMockChinaMarket->TaskChoice10RSStrongStockSet(151001));
		EXPECT_TRUE(gl_pMockChinaMarket->IsChosen10RSStrongStockSet());

		tm_.tm_wday = 0; // 星期日
		gl_pMockChinaMarket->TEST_SetMarketTM(tm_);
		gl_pMockChinaMarket->SetTodayStockProcessed(false);
		gl_pMockChinaMarket->SetChosen10RSStrongStockSet(false);
		EXPECT_FALSE(gl_pMockChinaMarket->TaskChoice10RSStrongStockSet(151001));
		EXPECT_FALSE(gl_pMockChinaMarket->IsChosen10RSStrongStockSet()) << _T("休息日不处理");
	}

	TEST_F(CMockChinaMarketTest, TestProcessTodayStock) {
		tm tm_;
		tm_.tm_wday = 1; // 星期一
		gl_pMockChinaMarket->TEST_SetMarketTM(tm_);
		gl_pMockChinaMarket->SetSystemReady(false);
		gl_pMockChinaMarket->SetTodayStockProcessed(false);
		EXPECT_FALSE(gl_pMockChinaMarket->TaskProcessTodayStock(150359));
		EXPECT_FALSE(gl_pMockChinaMarket->TaskProcessTodayStock(150400));
		EXPECT_FALSE(gl_pMockChinaMarket->IsTodayStockProcessed());
		gl_pMockChinaMarket->SetSystemReady(true);
		gl_pMockChinaMarket->SetTodayStockProcessed(true);
		EXPECT_FALSE(gl_pMockChinaMarket->TaskProcessTodayStock(150359));
		EXPECT_FALSE(gl_pMockChinaMarket->TaskProcessTodayStock(150400));
		EXPECT_TRUE(gl_pMockChinaMarket->IsTodayStockProcessed());
		gl_pMockChinaMarket->SetSystemReady(true);
		gl_pMockChinaMarket->SetTodayStockProcessed(false);
		EXPECT_CALL(*gl_pMockChinaMarket, CreatingThreadProcessTodayStock)
			.Times(1);
		EXPECT_FALSE(gl_pMockChinaMarket->TaskProcessTodayStock(150359));
		EXPECT_TRUE(gl_pMockChinaMarket->TaskProcessTodayStock(150400));
		EXPECT_FALSE(gl_pMockChinaMarket->IsTodayStockProcessed()) << _T("此标志由工作线程负责设置，故而此时尚未设置");

		tm_.tm_wday = 0; // 星期日
		gl_pMockChinaMarket->TEST_SetMarketTM(tm_);
		gl_pMockChinaMarket->SetTodayStockProcessed(false);
		EXPECT_FALSE(gl_pMockChinaMarket->TaskProcessTodayStock(150400));
		EXPECT_FALSE(gl_pMockChinaMarket->IsTodayStockProcessed()) << _T("休息日不处理");
	}

	TEST_F(CMockChinaMarketTest, TestThreadProcessTodayStock) {
		gl_pMockChinaMarket->SetTodayStockProcessed(false);
		gl_pMockChinaMarket->CalculateTime();
		gl_pMockChinaMarket->SetNewestTransactionTime(gl_pMockChinaMarket->GetUTCTime());
		long lDate = ConvertToDate(gl_pMockChinaMarket->GetNewestTransactionTime());
		gl_pMockChinaMarket->TEST_SetFormattedMarketTime(130000); // 设置市场时间为小于150400，
		EXPECT_CALL(*gl_pMockChinaMarket, BuildDayLine(lDate))
			.Times(1)
			.WillOnce(Return(4000));
		EXPECT_CALL(*gl_pMockChinaMarket, BuildDayLineRS(lDate))
			.Times(1)
			.WillOnce(Return(true));
		EXPECT_CALL(*gl_pMockChinaMarket, BuildWeekLineOfCurrentWeek)
			.Times(1);
		EXPECT_CALL(*gl_pMockChinaMarket, BuildWeekLineRS(GetCurrentMonday(lDate)))
			.Times(1)
			.WillOnce(Return(true));
		EXPECT_CALL(*gl_pMockChinaMarket, UpdateStockCodeDB)
			.Times(1)
			.WillOnce(Return(true));
		gl_pMockChinaMarket->SetSystemReady(true);
		EXPECT_EQ(ThreadProcessTodayStock(gl_pMockChinaMarket.get()), static_cast<UINT>(14));
		// 市场时间小于150400时
		EXPECT_EQ(gl_pMockChinaMarket->GetRSEndDate(), 19900101) << "没有执行修改最新相对强度日的动作";
		EXPECT_FALSE(gl_pMockChinaMarket->IsUpdateStockCodeDB());
		EXPECT_FALSE(gl_pMockChinaMarket->IsUpdateOptionDB());
		EXPECT_FALSE(gl_pMockChinaMarket->IsTodayStockProcessed());

		gl_pMockChinaMarket->TEST_SetFormattedMarketTime(150500); // 设置市场时间为大于150400，
		EXPECT_CALL(*gl_pMockChinaMarket, BuildDayLine(lDate))
			.Times(1)
			.WillOnce(Return(4000));
		EXPECT_CALL(*gl_pMockChinaMarket, BuildDayLineRS(lDate))
			.Times(1)
			.WillOnce(Return(true));
		EXPECT_CALL(*gl_pMockChinaMarket, BuildWeekLineOfCurrentWeek)
			.Times(1);
		EXPECT_CALL(*gl_pMockChinaMarket, BuildWeekLineRS(GetCurrentMonday(lDate)))
			.Times(1)
			.WillOnce(Return(true));
		EXPECT_CALL(*gl_pMockChinaMarket, UpdateStockCodeDB)
			.Times(1)
			.WillOnce(Return(true));
		gl_pMockChinaMarket->SetSystemReady(true);
		EXPECT_EQ(ThreadProcessTodayStock(gl_pMockChinaMarket.get()), static_cast<UINT>(14));
		// 市场时间大于150400时
		EXPECT_EQ(gl_pMockChinaMarket->GetRSEndDate(), lDate);
		EXPECT_TRUE(gl_pMockChinaMarket->IsUpdateOptionDB());
		EXPECT_TRUE(gl_pMockChinaMarket->IsTodayStockProcessed());

		// 恢复原状
		EXPECT_EQ(gl_systemMessage.InformationSize(), 2);
		while (gl_systemMessage.InformationSize() > 0) gl_systemMessage.PopInformationMessage();
	}

	TEST_F(CMockChinaMarketTest, TestThreadCalculateDayLineRS) {
		gl_pMockChinaMarket->CalculateTime();
		time_t tStart = gl_pMockChinaMarket->GetUTCTime() - 3600 * 24 * 6; // 从一周前开始计算
		tm _tm;
		GetMarketTimeStruct(&_tm, tStart, gl_pMockChinaMarket->GetMarketTimeZone());
		long lStartDate = (_tm.tm_year + 1900) * 10000 + (_tm.tm_mon + 1) * 100 + _tm.tm_mday;
		gl_systemStatus.SetExitingCalculatingRS(true); // 中间被打断
		gl_pMockChinaMarket->SetCalculatingDayLineRS(true);
		gl_pMockChinaMarket->SetRSEndDate(0);
		gl_pMockChinaMarket->SetUpdateOptionDB(false);
		EXPECT_CALL(*gl_pMockChinaMarket, CreatingThreadBuildDayLineRSOfDate(_))
			.Times(5)
			.WillRepeatedly(Return(true));
		EXPECT_EQ(ThreadBuildDayLineRS(gl_pMockChinaMarket.get(), lStartDate), static_cast<UINT>(11));
		EXPECT_FALSE(gl_pMockChinaMarket->IsUpdateOptionDB()) << _T("被打断后不设置此标识");
		EXPECT_EQ(gl_pMockChinaMarket->GetRSEndDate(), 0);
		EXPECT_FALSE(gl_systemStatus.IsExitingCalculatingRS());
		EXPECT_FALSE(gl_pMockChinaMarket->IsCalculatingDayLineRS());

		tStart = gl_pMockChinaMarket->GetUTCTime() - 3600 * 24 * 6; // 从一周前开始计算
		GetMarketTimeStruct(&_tm, tStart, gl_pMockChinaMarket->GetMarketTimeZone());
		lStartDate = (_tm.tm_year + 1900) * 10000 + (_tm.tm_mon + 1) * 100 + _tm.tm_mday;
		gl_systemStatus.SetExitingCalculatingRS(false);
		gl_pMockChinaMarket->SetCalculatingDayLineRS(true);
		EXPECT_CALL(*gl_pMockChinaMarket, CreatingThreadBuildDayLineRSOfDate(_))
			.Times(5);
		EXPECT_EQ(ThreadBuildDayLineRS(gl_pMockChinaMarket.get(), lStartDate), static_cast<UINT>(11));
		EXPECT_TRUE(gl_pMockChinaMarket->IsUpdateOptionDB());
		EXPECT_EQ(gl_pMockChinaMarket->GetRSEndDate(), gl_pMockChinaMarket->GetMarketDate());
		EXPECT_FALSE(gl_pMockChinaMarket->IsCalculatingDayLineRS());

		EXPECT_THAT(gl_systemMessage.InformationSize(), 4); // 共两次调用，有四个信息
		gl_systemMessage.PopInformationMessage();
		gl_systemMessage.PopInformationMessage();
		gl_systemMessage.PopInformationMessage();
		gl_systemMessage.PopInformationMessage();
	}

	TEST_F(CMockChinaMarketTest, TestThreadUpdateOptionDB) {
		EXPECT_CALL(*gl_pMockChinaMarket, UpdateOptionDB)
			.Times(1);
		EXPECT_EQ(ThreadUpdateOptionDB(gl_pMockChinaMarket.get()), static_cast<UINT>(20));
	}

	TEST_F(CMockChinaMarketTest, TestThreadUpdateStockCodeDB) {
		ASSERT_THAT(gl_pMockChinaMarket->IsUpdateStockCodeDB(), IsFalse()) << "此测试开始时，必须保证没有设置更新代码库的标识，否则会真正更新了测试代码库";

		EXPECT_CALL(*gl_pMockChinaMarket, UpdateStockCodeDB)
			.Times(1);
		gl_pMockChinaMarket->SetSystemReady(true);
		EXPECT_EQ(ThreadUpdateStockCodeDB(gl_pMockChinaMarket.get()), static_cast<UINT>(18));
	}

	TEST_F(CMockChinaMarketTest, TestThreadUpdateChosenStockDB) {
		EXPECT_CALL(*gl_pMockChinaMarket, AppendChosenStockDB)
			.Times(1);
		gl_pMockChinaMarket->SetSystemReady(true);
		EXPECT_EQ(ThreadAppendChosenStockDB(gl_pMockChinaMarket.get()), static_cast<UINT>(22));
	}

	TEST_F(CMockChinaMarketTest, TestThreadSaveTempRTData) {
		EXPECT_CALL(*gl_pMockChinaMarket, UpdateTodayTempDB)
			.Times(1);
		gl_pMockChinaMarket->SetSystemReady(true);
		EXPECT_EQ(ThreadSaveTempRTData(gl_pMockChinaMarket.get()), static_cast<UINT>(13));
	}

	TEST_F(CMockChinaMarketTest, TestThreadSaveRTData) {
		EXPECT_CALL(*gl_pMockChinaMarket, SaveRTData)
			.Times(1);
		gl_pMockChinaMarket->SetSystemReady(true);
		EXPECT_EQ(ThreadSaveRTData(gl_pMockChinaMarket.get()), static_cast<UINT>(19));
	}

	TEST_F(CMockChinaMarketTest, TestThreadMaintainDayLineDatabase) {
		EXPECT_EQ(ThreadMaintainDayLineDataBase(), static_cast<UINT>(17)) << "此工作线程目前没有实际动作";
	}

	TEST_F(CMockChinaMarketTest, TestThreadBuildWeekLineOfCurrentWeek) {
		EXPECT_CALL(*gl_pMockChinaMarket, BuildWeekLineOfCurrentWeek)
			.Times(1);
		EXPECT_EQ(ThreadBuildWeekLineOfCurrentWeek(gl_pMockChinaMarket.get()), static_cast<UINT>(32));
	}

	TEST_F(CMockChinaMarketTest, TestThreadBuildCurrentWeekWeekLineTable) {
		EXPECT_CALL(*gl_pMockChinaMarket, DeleteCurrentWeekWeekLine).Times(1);
		EXPECT_CALL(*gl_pMockChinaMarket, BuildCurrentWeekWeekLineTable).Times(1);
		EXPECT_EQ(ThreadBuildCurrentWeekWeekLineTable(gl_pMockChinaMarket.get()), static_cast<UINT>(33));
	}

	TEST_F(CMockChinaMarketTest, TestThreadSaveStockSection) {
		EXPECT_CALL(*gl_pMockChinaMarket, SaveStockSection)
			.Times(1);
		EXPECT_EQ(ThreadSaveStockSection(gl_pMockChinaMarket.get()), static_cast<UINT>(35));
	}

	TEST_F(CMockChinaMarketTest, TestThreadBuildWeekLineRSOfDate) {
		long lPrevMonday = GetPrevMonday(20200101);
		gl_systemStatus.SetExitingSystem(true);
		EXPECT_CALL(*gl_pMockChinaMarket, BuildWeekLineRS(_)).Times(0);
		EXPECT_EQ(ThreadBuildWeekLineRSOfDate(gl_pMockChinaMarket.get(), lPrevMonday), static_cast<UINT>(31));

		gl_systemStatus.SetExitingSystem(false);
		gl_systemStatus.SetExitingCalculatingRS(true);
		EXPECT_CALL(*gl_pMockChinaMarket, BuildWeekLineRS(_)).Times(0);
		EXPECT_EQ(ThreadBuildWeekLineRSOfDate(gl_pMockChinaMarket.get(), lPrevMonday), static_cast<UINT>(31));

		gl_systemStatus.SetExitingCalculatingRS(false);
		EXPECT_CALL(*gl_pMockChinaMarket, BuildWeekLineRS(lPrevMonday)).Times(1);
		EXPECT_EQ(ThreadBuildWeekLineRSOfDate(gl_pMockChinaMarket.get(), lPrevMonday), static_cast<UINT>(31));
	}

	TEST_F(CMockChinaMarketTest, TestThreadBuildWeekLineRS) {
		gl_pMockChinaMarket->CalculateTime();
		const long lPrevMonday4 = GetPrevMonday(gl_pMockChinaMarket->GetMarketDate());
		const long lPrevMonday3 = GetPrevMonday(lPrevMonday4);
		const long lPrevMonday2 = GetPrevMonday(lPrevMonday3);
		const long lPrevMonday1 = GetPrevMonday(lPrevMonday2);
		const long lPrevMonday = GetPrevMonday(lPrevMonday1);
		InSequence seq;
		EXPECT_CALL(*gl_pMockChinaMarket, CreatingThreadBuildWeekLineRSOfDate(lPrevMonday)).Times(1);
		EXPECT_CALL(*gl_pMockChinaMarket, CreatingThreadBuildWeekLineRSOfDate(lPrevMonday1)).Times(1);
		EXPECT_CALL(*gl_pMockChinaMarket, CreatingThreadBuildWeekLineRSOfDate(lPrevMonday2)).Times(1);
		EXPECT_CALL(*gl_pMockChinaMarket, CreatingThreadBuildWeekLineRSOfDate(lPrevMonday3)).Times(1);
		EXPECT_CALL(*gl_pMockChinaMarket, CreatingThreadBuildWeekLineRSOfDate(lPrevMonday4)).Times(1);
		if (gl_pMockChinaMarket->GetDayOfWeek() == 1) {
			// 本日是星期一
			EXPECT_CALL(*gl_pMockChinaMarket, CreatingThreadBuildWeekLineRSOfDate(gl_pMockChinaMarket->GetMarketDate())).Times(1); // 当前日为星期一时，要计算当前日
		}
		EXPECT_EQ(ThreadBuildWeekLineRS(gl_pMockChinaMarket.get(), lPrevMonday1), static_cast<UINT>(30));

		EXPECT_THAT(gl_systemMessage.InformationSize(), 1);
		gl_systemMessage.PopInformationMessage();
	}

	TEST_F(CMockChinaMarketTest, TestThreadBuildWeekLine1) {
		EXPECT_CALL(*gl_pMockChinaMarket, DeleteWeekLine()).Times(1).RetiresOnSaturation();
		EXPECT_CALL(*gl_pMockChinaMarket, BuildWeekLine(19900101)).Times(1).RetiresOnSaturation();
		EXPECT_CALL(*gl_pMockChinaMarket, DeleteCurrentWeekWeekLine()).Times(1);
		EXPECT_CALL(*gl_pMockChinaMarket, BuildCurrentWeekWeekLineTable()).Times(1).RetiresOnSaturation();
		EXPECT_EQ(ThreadBuildWeekLine(gl_pMockChinaMarket.get(), 19900101), static_cast<UINT>(25));
	}

	TEST_F(CMockChinaMarketTest, TestThreadBuildWeekLine2) {
		gl_pMockChinaMarket->CalculateTime();
		long lCurrentMonday = GetCurrentMonday(gl_pMockChinaMarket->GetMarketDate());
		EXPECT_CALL(*gl_pMockChinaMarket, DeleteWeekLine(lCurrentMonday)).Times(1);
		EXPECT_CALL(*gl_pMockChinaMarket, BuildWeekLine(lCurrentMonday)).Times(1);
		EXPECT_CALL(*gl_pMockChinaMarket, DeleteCurrentWeekWeekLine()).Times(1);
		EXPECT_CALL(*gl_pMockChinaMarket, BuildCurrentWeekWeekLineTable()).Times(1);
		EXPECT_EQ(ThreadBuildWeekLine(gl_pMockChinaMarket.get(), lCurrentMonday), static_cast<UINT>(25));
	}
}
