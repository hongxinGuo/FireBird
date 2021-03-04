#pragma once
#include"pch.h"

#include"WeekLineContainer.h"

namespace testing {
  class CMockWeekLineContainer : public CWeekLineContainer {
  public:
    MOCK_METHOD(bool, CalculateRS1, (INT64 lNumber), (override));
    MOCK_METHOD(bool, CalculateRSLogarithm1, (INT64 lNumber), (override));
    MOCK_METHOD(bool, CalculateRSIndex1, (INT64 lNumber), (override));
  };

  typedef shared_ptr<CMockWeekLineContainer> CMockWeekLineContainerPtr;
}