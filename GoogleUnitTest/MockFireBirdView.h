#pragma once

#include"gmock/gmock.h"

#include"FireBirdView.h"

namespace testing {
	class CMockFireBirdView final : public CFireBirdView {
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
		MOCK_METHOD(BOOL, SysCallBitBlt, (CDC* pdc, int x, int y, int nWidth, int nHeight, CDC* pSrcDC, int xSrc, int ySrc, DWORD dwRop), (override));
		MOCK_METHOD(void, ShowStockHistoryDataLine, (CDC* pDC), (override));
		MOCK_METHOD(void, ShowRealtimeData, (CDC* pDC), (override));
	};

	using CMockFireBirdViewPtr = shared_ptr<CMockFireBirdView>;
}
