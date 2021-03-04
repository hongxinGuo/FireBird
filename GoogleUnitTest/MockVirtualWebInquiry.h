#pragma once
#include"pch.h"

#include"VirtualWebInquiry.h"

namespace testing {
  class CMockVirtualWebInquiry : public CVirtualWebInquiry {
  public:
    MOCK_METHOD(bool, PrepareNextInquiringStr, (), (override));
    MOCK_METHOD(CString, GetNextInquiringMiddleStr, (long, bool), (override));
    MOCK_METHOD(void, StartReadingThread, (), (override));
    MOCK_METHOD(UINT, ReadWebFileOneTime, (), (override));
  };
}