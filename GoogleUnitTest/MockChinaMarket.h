#pragma once

#include"ChinaMarket.h"

namespace testing {
	class CMockChinaMarket final : public CChinaMarket {
	public:
		MOCK_METHOD(void, TaskCreateTask, (long), (override));
		MOCK_METHOD(void, TaskExitSystem, (long), (override));
		MOCK_METHOD(bool, TaskResetMarket, (long lCurrentTime), (override));

		MOCK_METHOD(void, CreateThreadDistributeAndCalculateRTData, (), (override));
		MOCK_METHOD(void, CreateThreadProcessTodayStock, (), (override));
		MOCK_METHOD(void, CreateThreadBuildDayLineRS, (long lStartCalculatingDate), (override));
		MOCK_METHOD(void, CreateThreadBuildDayLineRSOfDate, (long lThisDate), (override));
		MOCK_METHOD(void, CreateThreadLoadDayLine, (CChinaStockPtr pCurrentStock), (override));
		MOCK_METHOD(void, CreateThreadLoadWeekLine, (CChinaStockPtr pCurrentStock), (override));
		MOCK_METHOD(void, CreateThreadUpdateStockProfileDB, (), (override));
		MOCK_METHOD(void, CreateThreadChoice10RSStrong2StockSet, (), (override));
		MOCK_METHOD(void, CreateThreadChoice10RSStrong1StockSet, (), (override));
		MOCK_METHOD(void, CreateThreadChoice10RSStrongStockSet, (), (override));
		MOCK_METHOD(void, CreateThreadBuildWeekLine, (long lStartDate), (override));
		MOCK_METHOD(void, CreateThreadBuildWeekLineOfStock, (CChinaStockPtr pStock, long lStartDate), (override));
		MOCK_METHOD(void, CreateThreadBuildWeekLineRS, (), (override));
		MOCK_METHOD(void, CreateThreadBuildWeekLineRSOfDate, (long lThisDate), (override));
		MOCK_METHOD(void, CreateThreadBuildWeekLineOfCurrentWeek, (), (override));
		MOCK_METHOD(void, CreateThreadLoadTempRTData, (long lTheDate), (override));
		MOCK_METHOD(void, CreateThreadUpdateTempRTData, (), (override));
		MOCK_METHOD(void, CreateThreadUpdateOptionDB, (), (override));
		MOCK_METHOD(void, CreateThreadSaveStockSection, (), (override));
		MOCK_METHOD(void, CreateThreadUpdateChoseStockDB, (), (override));

		MOCK_METHOD(void, UpdateOptionDB, (), (override));
		MOCK_METHOD(void, AppendChosenStockDB, (), (override));
		MOCK_METHOD(void, LoadTempRTData, (long lTheDate), (override));
		MOCK_METHOD(bool, BuildWeekLineOfCurrentWeek, (), (override));
		MOCK_METHOD(bool, BuildCurrentWeekWeekLineTable, (), (override));

		MOCK_METHOD(bool, DeleteCurrentWeekWeekLine, (), (override));
		MOCK_METHOD(bool, DeleteWeekLine, (), (override));
		MOCK_METHOD(void, DeleteWeekLine, (long lMonday), (override));
	};

	using CMockChinaMarketPtr = shared_ptr<CMockChinaMarket>;
}
