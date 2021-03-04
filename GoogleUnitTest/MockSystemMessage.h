#pragma once
#include"pch.h"

#include"SystemMessage.h"

namespace testing {
  class CMockSystemDeque : public CSystemDeque {
  public:
    MOCK_METHOD(void, SysCallOutputListAddString, (COutputList* pOutputList, CString str), (override));
  };
}
