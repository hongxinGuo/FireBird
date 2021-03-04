#pragma once
#include"pch.h"

#include"QuandlWebInquiry.h"

namespace testing {
  class CMockQuandlWebInquiry : public CQuandlWebInquiry {
  public:
    MOCK_METHOD(void, StartReadingThread, (), (override));
    MOCK_METHOD(bool, ReadWebData, ());
  };
  typedef  shared_ptr<CMockQuandlWebInquiry> CMockQuandlWebInquiryPtr;
}