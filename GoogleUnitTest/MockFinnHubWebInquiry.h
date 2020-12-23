#pragma once

#include"stdafx.h"
#include"pch.h"

#include"FinnhubWebInquiry.h"

namespace testing {
  class CMockFinnhubWebInquiry : public CFinnhubWebInquiry {
  public:
    MOCK_METHOD(void, StartReadingThread, (), (override));
    MOCK_METHOD(bool, ReadWebData, ());
  };
  typedef  shared_ptr<CMockFinnhubWebInquiry> CMockFinnhubWebInquiryPtr;
}
