#pragma once

#include"stdafx.h"
#include"pch.h"

#include"ChinaStock.h"

namespace Testing {
  class CMockChinaStock : public CChinaStock {
  public:
    MOCK_METHOD(bool, CalculateDayLineRS, (INT64 lNumber), (override));
    MOCK_METHOD(bool, CalculateDayLineRSLogarithm, (INT64 lNumber), (override));
    MOCK_METHOD(void, ReportGuadanTransaction, (), (override));
    MOCK_METHOD(bool, SaveDayLine, (), (override));
    MOCK_METHOD(bool, LoadDayLineAndDayLineInfo, (), (override));
  };

  typedef shared_ptr<CMockChinaStock> CMockChinaStockPtr;
}
