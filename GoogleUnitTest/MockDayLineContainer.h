#pragma once

#include"stdafx.h"
#include"pch.h"

#include"DayLineContainer.h"

namespace testing {
  class CMockDayLineContainer : public CDayLineContainer {
  public:
    MOCK_METHOD(bool, CalculateRS1, (INT64 lNumber), (override));
    MOCK_METHOD(bool, CalculateRSLogarithm1, (INT64 lNumber), (override));
    MOCK_METHOD(bool, CalculateRSIndex1, (INT64 lNumber), (override));
  };

  typedef shared_ptr<CMockDayLineContainer> CMockDayLineContainerPtr;
}
