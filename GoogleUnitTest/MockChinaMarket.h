#pragma once

#include"stdafx.h"
#include"pch.h"

#include"ChinaStake.h"
#include"ChinaMarket.h"

namespace testing {
  class CMockChinaMarket : public CChinaMarket {
  public:
    MOCK_METHOD(bool, RunningThreadSaveChoicedRTData, (), (override));
    MOCK_METHOD(bool, RunningThreadProcessTodayStock, (), (override));
    MOCK_METHOD(bool, RunningThreadBuildDayLineRS, (long lStartCalculatingDate), (override));
    MOCK_METHOD(bool, RunningThreadBuildDayLineRSOfDate, (long lThisDate), (override));
    MOCK_METHOD(bool, RunningThreadSaveTempRTData, (), (override));
    MOCK_METHOD(bool, RunningThreadSaveDayLineBasicInfoOfStock, (CChinaStakePtr pStake), (override));
    MOCK_METHOD(bool, RunningThreadLoadDayLine, (CChinaStakePtr pCurrentStock), (override));
    MOCK_METHOD(bool, RunningThreadUpdateStakeCodeDB, (), (override));
    MOCK_METHOD(bool, RunningThreadUpdateOptionDB, (), (override));
    MOCK_METHOD(bool, RunningThreadAppendChoicedStockDB, (), (override));
    MOCK_METHOD(bool, RunningThreadChoice10RSStrong2StockSet, (), (override));
    MOCK_METHOD(bool, RunningThreadChoice10RSStrong1StockSet, (), (override));
    MOCK_METHOD(bool, RunningThreadChoice10RSStrongStockSet, (), (override));
    MOCK_METHOD(bool, RunningThreadCalculate10RSStrongStock, (vector<CChinaStakePtr>* pv10RSStrongStock, CRSReference* pRef, CChinaStakePtr pStake), (override));
    MOCK_METHOD(bool, RunningThreadCalculate10RSStrong1Stock, (vector<CChinaStakePtr>* pv10RSStrongStock, CChinaStakePtr pStake), (override));
    MOCK_METHOD(bool, RunningThreadCalculate10RSStrong2Stock, (vector<CChinaStakePtr>* pv10RSStrongStock, CChinaStakePtr pStake), (override));
    MOCK_METHOD(bool, RunningThreadBuildWeekLine, (long lStartDate), (override));
    MOCK_METHOD(bool, RunningThreadBuildWeekLineOfStock, (CChinaStakePtr pStake, long lStartDate), (override));
    MOCK_METHOD(bool, RunningThreadBuildWeekLineRS, (), (override));
    MOCK_METHOD(bool, RunningThreadBuildWeekLineRSOfDate, (long lThisDate), (override));
    MOCK_METHOD(bool, RunningThreadBuildWeekLineOfCurrentWeek, (), (override));
    MOCK_METHOD(bool, RunningThreadSaveActiveStakeCode, (), (override));
    MOCK_METHOD(bool, RunningThreadSaveStakeSection, (), (override));

    MOCK_METHOD(bool, UpdateOptionDB, (), (override));
    MOCK_METHOD(bool, UpdateStakeCodeDB, (), (override));
    MOCK_METHOD(bool, AppendChoicedStockDB, (), (override));
    MOCK_METHOD(bool, UpdateTodayTempDB, (), (override));
    MOCK_METHOD(bool, SaveRTData, (), (override));
    MOCK_METHOD(long, BuildDayLineOfDate, (long lCurrentTradeDate), (override));
    MOCK_METHOD(bool, Choice10RSStrong2StockSet, (), (override));
    MOCK_METHOD(bool, Choice10RSStrong1StockSet, (), (override));
    MOCK_METHOD(bool, Choice10RSStrongStockSet, (CRSReference* pRef, int iIndex), (override));
    MOCK_METHOD(bool, BuildDayLineRSOfDate, (long lDate), (override));
    MOCK_METHOD(bool, BuildWeekLineOfCurrentWeek, (), (override));
    MOCK_METHOD(bool, BuildCurrentWeekWeekLineTable, (), (override));
    MOCK_METHOD(bool, BuildWeekLineRSOfDate, (long lDate), (override));
    MOCK_METHOD(bool, SaveActiveStakeCode, (), (override));
    MOCK_METHOD(bool, SaveStakeSection, (), (override));
  };

  typedef shared_ptr<CMockChinaMarket> CMockChinaMarketPtr;
}
