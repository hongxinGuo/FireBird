#pragma once

#include"stdafx.h"
#include"pch.h"

#include"SystemMessage.h"

namespace Testing {
  class CMockSystemDeque : public CSystemDeque {
  public:
    MOCK_METHOD(void, SysCallOutputListAddString, (COutputList* pOutputList, CString str), (override));
  };
}
