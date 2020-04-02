#pragma once

#include"stdafx.h"
#include"pch.h"

#include"PotenDailyBriefingWebInquiry.h"

namespace Testing {
  class CMockPotenDailyBriefingWebInquiry : public CPotenDailyBriefingWebInquiry {
  public:
    MOCK_METHOD(void, StartReadingThread, (), (override));
  };
  typedef  shared_ptr<CMockPotenDailyBriefingWebInquiry> CMockPotenDailyBriefingWebInquiryPtr;
}