#pragma once

#include"stdafx.h"
#include"pch.h"

#include"WeekLineContainer.h"

namespace testing {
  class CMockWeekLineContainer : public CWeekLineContainer {
  public:
    MOCK_METHOD(bool, CalculateRS, (INT64 lNumber), (override));
    MOCK_METHOD(bool, CalculateRSLogarithm, (INT64 lNumber), (override));
    MOCK_METHOD(bool, CalculateRSIndex, (INT64 lNumber), (override));
  };

  typedef shared_ptr<CMockWeekLineContainer> CMockWeekLineContainerPtr;
}