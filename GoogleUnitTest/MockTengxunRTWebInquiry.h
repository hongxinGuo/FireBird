#pragma once

#include"stdafx.h"
#include"pch.h"

#include"NeteaseRTWebInquiry.h"

namespace Testing {
  class CMockTengxunRTWebInquiry : public CTengxunRTWebInquiry {
  public:
    MOCK_METHOD(void, StartReadingThread, (), (override));
    MOCK_METHOD(bool, ReadWebData, (long lFirstDelayTime, long lSecondDelayTime, long lThirdDelayTime));
  };
  typedef  shared_ptr<CMockTengxunRTWebInquiry> CMockTengxunRTWebInquiryPtr;
}
