#pragma once

#include"stdafx.h"
#include"pch.h"

#include"ChinaStock.h"

namespace Testing {
  class CMockChinaStock : public CChinaStock {
  public:
    MOCK_METHOD(bool, CalculateDayLineRS, (INT64 lNumber), (override));
  };

  typedef shared_ptr<CMockChinaStock> CMockChinaStockPtr;
}
