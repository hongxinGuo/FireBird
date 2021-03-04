#pragma once
#include"pch.h"

#include"PotenDailyBriefingMarket.h"

namespace testing {
  class CMockPotenDailyBriefingMarket : public CPotenDailyBriefingMarket {
  public:
    MOCK_METHOD(bool, SaveCurrentData, (), (override));
  };
  typedef  shared_ptr<CMockPotenDailyBriefingMarket> CMockPotenDailyBriefingMarketPtr;
}
