#pragma once

#include"stdafx.h"
#include"pch.h"

#include"FinnHubWebInquiry.h"

namespace testing {
  class CMockFinnhubWebInquiry : public CFinnhubWebInquiry {
  public:
    MOCK_METHOD(void, StartReadingThread, (), (override));
    MOCK_METHOD(bool, ReadWebData, (long lFirstDelayTime, long lSecondDelayTime, long lThirdDelayTime));
  };
  typedef  shared_ptr<CMockFinnhubWebInquiry> CMockFinnhubWebInquiryPtr;
}
