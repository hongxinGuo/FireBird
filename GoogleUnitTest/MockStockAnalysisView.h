#pragma once

#include"stdafx.h"
#include"pch.h"

#include"StockAnalysisDoc.h"
#include"StockAnalysisView.h"

namespace testing {
  class CMockStockAnalysisView : public CStockAnalysisView {
  public:
    MOCK_METHOD(CSize, SysCallGetTextExtent, (CDC* pDC, CString str), (override));
    MOCK_METHOD(BOOL, SysCallTextOut, (CDC* pDC, int x, int y, CString str), (override));
    MOCK_METHOD(CPen*, SysCallSelectObject, (CDC* pDC, CPen* ppen), (override));
    MOCK_METHOD(CPoint, SysCallMoveTo, (CDC* pDC, int x, int y), (override));
    MOCK_METHOD(CPoint, SysCallMoveTo, (CDC* pDC, CPoint pt), (override));
    MOCK_METHOD(BOOL, SysCallLineTo, (CDC* pDC, int x, int y), (override));
    MOCK_METHOD(BOOL, SysCallLineTo, (CDC* pDC, CPoint pt), (override));
    MOCK_METHOD(void, SysCallFillSolidRect, (CDC* pDC, LPRECT rect, COLORREF ref), (override));
    MOCK_METHOD(void, SysCallOnSize, (UINT nType, int cx, int cy), (override));
    MOCK_METHOD(void, SysCallCmdUISetCheck, (CCmdUI* pCmdUI, int iCheck), (override));
    MOCK_METHOD(void, SysCallCmdUIEnable, (CCmdUI* pCmdUI, bool fEnable), (override));
    MOCK_METHOD(void, SysCallGetClientRect, (LPRECT lpRect), (override));
    MOCK_METHOD(void, ShowStockDayLine, (CDC* pDC), (override));
    MOCK_METHOD(void, ShowRealtimeData, (CDC* pDC), (override));
  };
}
