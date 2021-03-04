#pragma once
#include"ChinaStock.h"

namespace testing {
  class CMockChinaStock : public CChinaStock {
  public:
    MOCK_METHOD(void, ReportGuadanTransaction, (), (override));
    MOCK_METHOD(void, ReportGuadan, (), (override));
    MOCK_METHOD(bool, SaveDayLineBasicInfo, (), (override));
    MOCK_METHOD(bool, LoadDayLine, (CString strStockCode), (override));
    MOCK_METHOD(bool, SaveWeekLine, (), (override));
    MOCK_METHOD(bool, LoadWeekLine, (), (override));
  };

  typedef shared_ptr<CMockChinaStock> CMockChinaStockPtr;
}
