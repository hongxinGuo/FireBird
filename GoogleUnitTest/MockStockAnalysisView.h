#pragma once

#include"stdafx.h"
#include"pch.h"

#include"StockAnalysisDoc.h"
#include"StockAnalysisView.h"

namespace testing {
  class CMockStockAnalysisView : public CStockAnalysisView {
  public:
    MOCK_METHOD(void, SysCallOnSize, (UINT nType, int cx, int cy), (override));
    MOCK_METHOD(void, SysCallCmdUISetCheck, (CCmdUI* pCmdUI, int iCheck), (override));
    MOCK_METHOD(void, SysCallCmdUIEnable, (CCmdUI* pCmdUI, bool fEnable), (override));
  };
}
