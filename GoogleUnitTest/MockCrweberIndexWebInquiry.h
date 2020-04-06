#pragma once

#include"stdafx.h"
#include"pch.h"

#include"CrweberIndexWebInquiry.h"

namespace Testing {
  class CMockCrweberIndexWebInquiry : public CCrweberIndexWebInquiry {
  public:
    MOCK_METHOD(void, StartReadingThread, (), (override));
    MOCK_METHOD(bool, ReadWebData, (long lFirstDelayTime, long lSecondDelayTime, long lThirdDelayTime));
  };
  typedef shared_ptr<CMockCrweberIndexWebInquiry> CMockCrweberIndexWebInquiryPtr;
}