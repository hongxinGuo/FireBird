#pragma once

#include"stdafx.h"
#include"pch.h"

#include"NeteaseDayLineWebInquiry.h"

namespace testing {
  class CMockNeteaseDayLineWebInquiry : public CNeteaseDayLineWebInquiry {
  public:
    MOCK_METHOD(void, StartReadingThread, (), (override));
    MOCK_METHOD(bool, ReadWebData3, (long lFirstDelayTime, long lSecondDelayTime, long lThirdDelayTime));
  };
  typedef  shared_ptr<CMockNeteaseDayLineWebInquiry> CMockNeteaseDayLineWebInquiryPtr;
}
