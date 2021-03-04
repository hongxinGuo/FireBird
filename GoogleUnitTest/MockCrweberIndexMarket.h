#pragma once
#include"CrweberIndexMarket.h"

namespace testing {
  class CMockCrweberIndexMarket : public CCrweberIndexMarket {
  public:
    MOCK_METHOD(bool, LoadDatabase, (), (override));
    MOCK_METHOD(bool, SaveDatabase, (), (override));
    MOCK_METHOD(bool, SaveCrweberIndexData, (CCrweberIndexPtr pCrweberIndex));
  };
}
