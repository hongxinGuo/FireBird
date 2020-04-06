#pragma once

#include"stdafx.h"
#include"pch.h"

#include"PotenDailyBriefingMarket.h"

namespace Testing {
  class CMockPotenDailyBriefingMarket : public CPotenDailyBriefingMarket {
  public:
    MOCK_METHOD(bool, SaveCurrentData, (), (override));
  };
  typedef  shared_ptr<CMockPotenDailyBriefingMarket> CMockPotenDailyBriefingMarketPtr;
}