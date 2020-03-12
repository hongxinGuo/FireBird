#pragma once

#include"stdafx.h"
#include"pch.h"

#include"MainFrm.h"

namespace Testing {
  class CMockMainFrame : public CMainFrame {
  public:
    MOCK_METHOD(void, ProcessTodayStock, (), (override));
    MOCK_METHOD(void, CalculateTodayRelativeStrong, (), (override));
  };
}
