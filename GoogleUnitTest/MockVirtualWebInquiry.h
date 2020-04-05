#pragma once

#include"stdafx.h"
#include"pch.h"

#include"VirtualWebInquiry.h"

namespace Testing {
  class CMockVirtualWebInquiry : public CVirtualWebInquiry {
  public:
    MOCK_METHOD(bool, PrepareNextInquiringStr, (), (override));
    MOCK_METHOD(CString, GetNextInquiringMiddleStr, (long, bool), (override));
    MOCK_METHOD(void, StartReadingThread, (), (override));
    MOCK_METHOD(UINT, ReadWebFile, (), (override));
  };
}