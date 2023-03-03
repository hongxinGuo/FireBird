#pragma once

#include"ChinaMarket.h"

namespace testing {
	class CMockChinaMarket final : public CChinaMarket {
	public:
		MOCK_METHOD(void, CreatingThreadProcessTodayStock, (), (override));
		MOCK_METHOD(void, CreatingThreadBuildDayLineRS, (long lStartCalculatingDate), (override));
		MOCK_METHOD(void, CreatingThreadBuildDayLineRSOfDate, (long lThisDate), (override));
		MOCK_METHOD(void, CreatingThreadLoadDayLine, (CChinaStock* pCurrentStock), (override));
		MOCK_METHOD(void, CreatingThreadLoadWeekLine, (CChinaStock* pCurrentStock), (override));
		MOCK_METHOD(void, CreatingThreadUpdateStockProfileDB, (), (override));
		MOCK_METHOD(void, CreatingThreadChoice10RSStrong2StockSet, (), (override));
		MOCK_METHOD(void, CreatingThreadChoice10RSStrong1StockSet, (), (override));
		MOCK_METHOD(void, CreatingThreadChoice10RSStrongStockSet, (), (override));
		MOCK_METHOD(void, CreatingThreadBuildWeekLine, (long lStartDate), (override));
		MOCK_METHOD(void, CreatingThreadBuildWeekLineOfStock, (CChinaStock* pStock, long lStartDate), (override));
		MOCK_METHOD(void, CreatingThreadBuildWeekLineRS, (), (override));
		MOCK_METHOD(void, CreatingThreadBuildWeekLineRSOfDate, (long lThisDate), (override));
		MOCK_METHOD(void, CreatingThreadBuildWeekLineOfCurrentWeek, (), (override));

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
