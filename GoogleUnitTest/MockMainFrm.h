#pragma once

#include"MainFrm.h"

namespace testing {
	class CMockMainFrame final : public CMainFrame {
	public:
		MOCK_METHOD(void, SysCallOnTimer, (UINT_PTR nIDEvent), (override));
		MOCK_METHOD(void, SysCallSetPaneText, (int iIndex, LPCTSTR lpszNewText), (override));
		MOCK_METHOD(void, SysCallSetInnerSystemPaneText, (int iIndex, LPCTSTR lpszNewText), (override));
		MOCK_METHOD(void, SysCallOnSysCommand, (UINT nID, LPARAM lParam), (override));
		MOCK_METHOD(void, ProcessChinaMarketStock, (), (override));
		MOCK_METHOD(void, CalculateTodayRS, (), (override));
		MOCK_METHOD(void, SysCallCmdUIEnable, (CCmdUI* pCmdUI, bool fFlag), (override));
		MOCK_METHOD(void, SysCallCmdUISetCheck, (CCmdUI* pCmdUI, bool fFlag), (override));
		MOCK_METHOD(LRESULT, SysCallSendMessage, (UINT message, WPARAM wParam, LPARAM lParam), (override));
		MOCK_METHOD(BOOL, SysCallPreTranslateMessage, (MSG* pMsg), (override));
		MOCK_METHOD(void, SysCallInvalidate, (), (override));
		MOCK_METHOD(void, SysCallOnChar, (UINT nChar, UINT nRepCnt, UINT nFlags), (override));
		MOCK_METHOD(void, SysCallOnKeyUp, (UINT nChar, UINT nRepCnt, UINT nFlags), (override));
	};

	using CMockMainFramePtr = shared_ptr<CMockMainFrame>;
}
