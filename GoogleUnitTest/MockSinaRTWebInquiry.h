#pragma once

#include"stdafx.h"
#include"pch.h"

#include"SinaRTWebInquiry.h"

namespace testing {
  class CMockSinaRTWebInquiry : public CSinaRTWebInquiry {
  public:
    MOCK_METHOD(void, StartReadingThread, (), (override));
    MOCK_METHOD(bool, ReadWebData, (), (override));
  };
  typedef  shared_ptr<CMockSinaRTWebInquiry> CMockSinaRTWebInquiryPtr;
}
