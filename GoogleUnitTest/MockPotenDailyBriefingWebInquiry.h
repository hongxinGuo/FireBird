#pragma once
#include"PotenDailyBriefingWebInquiry.h"

namespace testing {
  class CMockPotenDailyBriefingWebInquiry : public CPotenDailyBriefingWebInquiry {
  public:
    MOCK_METHOD(void, StartReadingThread, (), (override));
    MOCK_METHOD(bool, ReadWebData, ());
  };
  typedef  shared_ptr<CMockPotenDailyBriefingWebInquiry> CMockPotenDailyBriefingWebInquiryPtr;
}