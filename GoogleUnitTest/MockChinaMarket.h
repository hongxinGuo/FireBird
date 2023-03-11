#pragma once

#include"ChinaMarket.h"

namespace testing {
	class CMockChinaMarket final : public CChinaMarket {
	public:
		MOCK_METHOD(bool, TaskCreateTask, (long), (override));
		MOCK_METHOD(bool, TaskResetMarket, (long lCurrentTime), (override));
		MOCK_METHOD(bool, TaskResetMarketAgain, (long lCurrentTime), (override));
		MOCK_METHOD(void, TaskDistributeAndCalculateRTData, (long), (override));

		MOCK_METHOD(void, CreateThreadProcessRTData, (), (override));
		MOCK_METHOD(void, CreateThreadProcessTodayStock, (), (override));
		MOCK_METHOD(void, CreateThreadBuildDayLineRS, (long lStartCalculatingDate), (override));
		MOCK_METHOD(void, CreateThreadBuildDayLineRSOfDate, (long lThisDate), (override));
		MOCK_METHOD(void, CreateThreadLoadDayLine, (CChinaStock* pCurrentStock), (override));
		MOCK_METHOD(void, CreateThreadLoadWeekLine, (CChinaStock* pCurrentStock), (override));
		MOCK_METHOD(void, CreateThreadUpdateStockProfileDB, (), (override));
		MOCK_METHOD(void, CreateThreadChoice10RSStrong2StockSet, (), (override));
		MOCK_METHOD(void, CreateThreadChoice10RSStrong1StockSet, (), (override));
		MOCK_METHOD(void, CreateThreadChoice10RSStrongStockSet, (), (override));
		MOCK_METHOD(void, CreateThreadBuildWeekLine, (long lStartDate), (override));
		MOCK_METHOD(void, CreateThreadBuildWeekLineOfStock, (CChinaStock* pStock, long lStartDate), (override));
		MOCK_METHOD(void, CreateThreadBuildWeekLineRS, (), (override));
		MOCK_METHOD(void, CreateThreadBuildWeekLineRSOfDate, (long lThisDate), (override));
		MOCK_METHOD(void, CreateThreadBuildWeekLineOfCurrentWeek, (), (override));
		MOCK_METHOD(bool, CreateThreadUpdateTempRTData, (), (override));

		MOCK_METHOD(bool, UpdateOptionDB, (), (override));
		MOCK_METHOD(bool, UpdateStockProfileDB, (), (override));
		MOCK_METHOD(bool, AppendChosenStockDB, (), (override));
		MOCK_METHOD(bool, UpdateTodayTempDB, (), (override));
		MOCK_METHOD(bool, SaveRTData, (), (override));
		MOCK_METHOD(long, BuildDayLine, (long lCurrentTradeDate), (override));
		MOCK_METHOD(bool, Choice10RSStrong2StockSet, (), (override));
		MOCK_METHOD(bool, Choice10RSStrong1StockSet, (), (override));
		MOCK_METHOD(bool, Choice10RSStrongStockSet, (CRSReference* pRef, int iIndex), (override));
		MOCK_METHOD(bool, BuildDayLineRS, (long lDate), (override));
		MOCK_METHOD(bool, BuildWeekLineOfCurrentWeek, (), (override));
		MOCK_METHOD(bool, BuildCurrentWeekWeekLineTable, (), (override));
		MOCK_METHOD(bool, BuildWeekLineRS, (long lDate), (override));
		MOCK_METHOD(bool, SaveStockSection, (), (override));

		MOCK_METHOD(bool, DeleteCurrentWeekWeekLine, (), (override));
		MOCK_METHOD(bool, BuildWeekLine, (long lStartDate), (override));
		MOCK_METHOD(bool, DeleteWeekLine, (), (override));
		MOCK_METHOD(bool, DeleteWeekLine, (long lMonday), (override));
	};

	using CMockChinaMarketPtr = shared_ptr<CMockChinaMarket>;
}
