#pragma once

#include"stdafx.h"
#include"pch.h"

#include"NeteaseDayLineWebInquiry.h"

namespace Testing {
  class CMockNeteaseDayLineWebInquiry : public CNeteaseDayLineWebInquiry {
  public:
    MOCK_METHOD(void, StartReadingThread, (), (override));
  };
}
