#pragma once

#include"stdafx.h"
#include"pch.h"

#include"SinaStakeRTWebInquiry.h"

namespace testing {
  class CMockSinaStakeRTWebInquiry : public CSinaStakeRTWebInquiry {
  public:
    MOCK_METHOD(void, StartReadingThread, (), (override));
    MOCK_METHOD(bool, ReadWebData, (long lFirstDelayTime, long lSecondDelayTime, long lThirdDelayTime), (override));
  };
  typedef  shared_ptr<CMockSinaStakeRTWebInquiry> CMockSinaStakeRTWebInquiryPtr;
}
