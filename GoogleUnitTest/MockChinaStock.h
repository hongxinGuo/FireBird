#pragma once

#include"stdafx.h"
#include"pch.h"

#include"ChinaStock.h"

namespace testing {
  class CMockChinaStock : public CChinaStock {
  public:
    MOCK_METHOD(bool, CalculateDayLineRS, (INT64 lNumber), (override));
    MOCK_METHOD(bool, CalculateDayLineRSLogarithm, (INT64 lNumber), (override));
    MOCK_METHOD(bool, CalculateDayLineRSIndex, (INT64 lNumber), (override));
    MOCK_METHOD(void, ReportGuadanTransaction, (), (override));
    MOCK_METHOD(void, ReportGuadan, (), (override));
    MOCK_METHOD(bool, SaveDayLineBasicInfo, (), (override));
    MOCK_METHOD(bool, LoadDayLine, (), (override));
    MOCK_METHOD(bool, SaveWeekLine, (), (override));
    MOCK_METHOD(bool, LoadWeekLine, (), (override));
  };

  typedef shared_ptr<CMockChinaStock> CMockChinaStockPtr;
}
