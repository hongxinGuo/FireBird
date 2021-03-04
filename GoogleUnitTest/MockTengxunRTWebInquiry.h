#pragma once
#include"pch.h"

#include"NeteaseRTWebInquiry.h"

namespace testing {
  class CMockTengxunRTWebInquiry : public CTengxunRTWebInquiry {
  public:
    MOCK_METHOD(void, StartReadingThread, (), (override));
    MOCK_METHOD(bool, ReadWebData, ());
  };
  typedef  shared_ptr<CMockTengxunRTWebInquiry> CMockTengxunRTWebInquiryPtr;
}
