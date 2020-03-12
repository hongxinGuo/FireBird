#pragma once

#include"stdafx.h"
#include"pch.h"

#include"MainFrm.h"

namespace Testing {
  class CMockMainFrame : public CMainFrame {
  public:
    MOCK_METHOD(void, SysCallOnSysCommand, (UINT nID, LPARAM lParam), (override));
    MOCK_METHOD(void, ProcessTodayStock, (), (override));
    MOCK_METHOD(void, CalculateTodayRelativeStrong, (), (override));
    MOCK_METHOD(void, SysCallCmdUIEnable, (CCmdUI* pCmdUI, bool fFlag), (override));
    MOCK_METHOD(void, SysCallCmdUISetCheck, (CCmdUI* pCmdUI, bool fFlag), (override));
  };
}
