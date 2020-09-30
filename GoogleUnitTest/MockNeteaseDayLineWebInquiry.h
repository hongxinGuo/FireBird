#pragma once

#include"stdafx.h"
#include"pch.h"

#include"NeteaseDayLineWebInquiry.h"

namespace testing {
  class CMockNeteaseDLWebInquiry : public CNeteaseDLWebInquiry {
  public:
    MOCK_METHOD(void, StartReadingThread, (), (override));
    MOCK_METHOD(bool, ReadWebData, (long lFirstDelayTime, long lSecondDelayTime, long lThirdDelayTime));
  };
  typedef  shared_ptr<CMockNeteaseDLWebInquiry> CMockNeteaseDLWebInquiryPtr;
}
