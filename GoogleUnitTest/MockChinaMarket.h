#pragma once
#include"ChinaStock.h"
#include"ChinaMarket.h"

namespace testing {
	class CMockChinaMarket : public CChinaMarket {
	public:
		MOCK_METHOD(bool, CreatingThreadProcessTodayStock, (), (override));
		MOCK_METHOD(bool, CreatingThreadBuildDayLineRS, (long lStartCalculatingDate), (override));
		MOCK_METHOD(bool, CreatingThreadBuildDayLineRSOfDate, (long lThisDate), (override));
		MOCK_METHOD(bool, CreatingThreadSaveDayLineBasicInfoOfStock, (CChinaStock* pStock), (override));
		MOCK_METHOD(bool, CreatingThreadLoadDayLine, (CChinaStock* pCurrentStock), (override));
		MOCK_METHOD(bool, CreatingThreadLoadWeekLine, (CChinaStock* pCurrentStock), (override));
		MOCK_METHOD(bool, CreatingThreadUpdateStockCodeDB, (), (override));
		MOCK_METHOD(bool, CreatingThreadChoice10RSStrong2StockSet, (), (override));
		MOCK_METHOD(bool, CreatingThreadChoice10RSStrong1StockSet, (), (override));
		MOCK_METHOD(bool, CreatingThreadChoice10RSStrongStockSet, (), (override));
		MOCK_METHOD(bool, CreatingThreadCalculate10RSStrongStock, (vector<CChinaStockPtr>* pv10RSStrongStock, CRSReference* pRef, CChinaStockPtr pStock), (override));
		MOCK_METHOD(bool, CreatingThreadCalculate10RSStrong1Stock, (vector<CChinaStockPtr>* pv10RSStrongStock, CChinaStockPtr pStock), (override));
		MOCK_METHOD(bool, CreatingThreadCalculate10RSStrong2Stock, (vector<CChinaStockPtr>* pv10RSStrongStock, CChinaStockPtr pStock), (override));
		MOCK_METHOD(bool, CreatingThreadBuildWeekLine, (long lStartDate), (override));
		MOCK_METHOD(bool, CreatingThreadBuildWeekLineOfStock, (CChinaStock* pStock, long lStartDate), (override));
		MOCK_METHOD(bool, CreatingThreadBuildWeekLineRS, (), (override));
		MOCK_METHOD(bool, CreatingThreadBuildWeekLineRSOfDate, (long lThisDate), (override));
		MOCK_METHOD(bool, CreatingThreadBuildWeekLineOfCurrentWeek, (), (override));

		MOCK_METHOD(bool, UpdateOptionDB, (), (override));
		MOCK_METHOD(bool, UpdateStockCodeDB, (), (override));
		MOCK_METHOD(bool, AppendChoicedStockDB, (), (override));
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

	typedef shared_ptr<CMockChinaMarket> CMockChinaMarketPtr;
}
