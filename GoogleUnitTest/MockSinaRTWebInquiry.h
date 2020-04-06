#pragma once

#include"stdafx.h"
#include"pch.h"

#include"SinaRTWebInquiry.h"

namespace Testing {
  class CMockSinaRTWebInquiry : public CSinaRTWebInquiry {
  public:
    MOCK_METHOD(void, StartReadingThread, (), (override));
    MOCK_METHOD(bool, ReadWebData, (long lFirstDelayTime, long lSecondDelayTime, long lThirdDelayTime));
  };
  typedef  shared_ptr<CMockSinaRTWebInquiry> CMockSinaRTWebInquiryPtr;
}
