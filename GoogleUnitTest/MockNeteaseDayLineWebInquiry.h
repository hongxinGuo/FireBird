#pragma once
#include"NeteaseDayLineWebInquiry.h"

namespace testing {
  class CMockNeteaseDayLineWebInquiry : public CNeteaseDayLineWebInquiry {
  public:
    MOCK_METHOD(void, StartReadingThread, (), (override));
    MOCK_METHOD(bool, ReadWebDataTimeLimit, (long lFirstDelayTime, long lSecondDelayTime, long lThirdDelayTime), (override));
  };
  typedef  shared_ptr<CMockNeteaseDayLineWebInquiry> CMockNeteaseDayLineWebInquiryPtr;
}
