#pragma once
#include"NeteaseRTWebInquiry.h"

namespace testing {
  class CMockNeteaseRTWebInquiry : public CNeteaseRTWebInquiry {
  public:
    MOCK_METHOD(void, StartReadingThread, (), (override));
    MOCK_METHOD(bool, ReadWebData, ());
  };
  typedef  shared_ptr<CMockNeteaseRTWebInquiry> CMockNeteaseRTWebInquiryPtr;
}
