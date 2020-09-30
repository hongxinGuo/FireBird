#pragma once

#include"stdafx.h"
#include"pch.h"

#include"ChinaStock.h"

namespace testing {
  class CMockChinaStock : public CChinaStock {
  public:
    MOCK_METHOD(void, ReportGuadanTransaction, (), (override));
    MOCK_METHOD(void, ReportGuadan, (), (override));
    MOCK_METHOD(bool, SaveDLBasicInfo, (), (override));
    MOCK_METHOD(bool, LoadDL, (CString strStockCode), (override));
    MOCK_METHOD(bool, SaveWeekLine, (), (override));
    MOCK_METHOD(bool, LoadWeekLine, (), (override));
  };

  typedef shared_ptr<CMockChinaStock> CMockChinaStockPtr;
}
