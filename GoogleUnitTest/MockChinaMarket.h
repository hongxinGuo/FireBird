#pragma once

#include"stdafx.h"
#include"pch.h"

#include"ChinaStock.h"
#include"ChinaMarket.h"

namespace Testing {
  class CMockChinaMarket : public CChinaMarket {
  public:
    MOCK_METHOD(bool, RunningThreadSaveChoicedRTData, (), (override));
    MOCK_METHOD(bool, RunningThreadProcessTodayStock, (void), (override));
    MOCK_METHOD(bool, RunningThreadCalculateRelativeStrong, (long lStartCalculatingDay), (override));
    MOCK_METHOD(bool, RunningThreadCalculateThisDayRS, (long lThisDay), (override));
    MOCK_METHOD(bool, RunningThreadSaveTempRTData, (void), (override));
    MOCK_METHOD(bool, RunningThreadSaveDayLineOfOneStock, (CChinaStockPtr pStock), (override));
    MOCK_METHOD(bool, RunningThreadLoadDayLine, (CChinaStockPtr pCurrentStock), (override));
    MOCK_METHOD(bool, RunningThreadUpdateStockCodeDB, (void), (override));
    MOCK_METHOD(bool, RunningThreadUpdateOptionDB, (void), (override));
  };

  typedef shared_ptr<CMockChinaMarket> CMockChinaStockPtr;
}
