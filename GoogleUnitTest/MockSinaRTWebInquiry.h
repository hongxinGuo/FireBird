#pragma once

#include"stdafx.h"
#include"pch.h"

#include"SinaRTWebInquiry.h"

namespace Testing {
  class CMockSinaRTWebInquiry : public CSinaRTWebInquiry {
  public:
    MOCK_METHOD(void, StartReadingThread, (), (override));
  };
  typedef  shared_ptr<CMockSinaRTWebInquiry> CMockSinaRTWebInquiryPtr;
}
