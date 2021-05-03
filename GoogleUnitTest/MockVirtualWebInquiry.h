#pragma once
#include"VirtualWebInquiry.h"

namespace testing {
  class CMockVirtualWebInquiry : public CVirtualWebInquiry {
  public:
    MOCK_METHOD(bool, OpenFile, (CString strInquiring), (override));
    MOCK_METHOD(bool, PrepareNextInquiringStr, (), (override));
    MOCK_METHOD(CString, GetNextInquiringMiddleStr, (long, bool), (override));
    MOCK_METHOD(void, StartReadingThread, (), (override));
    MOCK_METHOD(UINT, ReadWebFileOneTime, (), (override));
  };
}