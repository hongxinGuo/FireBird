#pragma once

#include"stdafx.h"
#include"pch.h"

#include"NeteaseRTWebInquiry.h"

namespace Testing {
  class CMockNeteaseRTWebInquiry : public CNeteaseRTWebInquiry {
  public:
    MOCK_METHOD(void, StartReadingThread, (), (override));
  };
  typedef  shared_ptr<CMockNeteaseRTWebInquiry> CMockNeteaseRTWebInquiryPtr;
}
