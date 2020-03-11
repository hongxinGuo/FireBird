#pragma once

#include"stdafx.h"
#include"pch.h"

#include"CrweberIndexWebInquiry.h"

namespace Testing {
  class CMockCrweberIndexWebInquiry : public CCrweberIndexWebInquiry {
  public:
    MOCK_METHOD(void, StartReadingThread, (), (override));
  };
}