// ChildFrm.cpp : implementation of the CWatchdogChildFrame class
//

#include "pch.h"
#include "framework.h"
//#include "Watchdog.h"

#include "WatchdogChildFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CWatchdogChildFrame

IMPLEMENT_DYNCREATE(CWatchdogChildFrame, CMDIChildWndEx)

BEGIN_MESSAGE_MAP(CWatchdogChildFrame, CMDIChildWndEx)
END_MESSAGE_MAP()

// CWatchdogChildFrame construction/destruction

CWatchdogChildFrame::CWatchdogChildFrame() noexcept {}

CWatchdogChildFrame::~CWatchdogChildFrame() {}

BOOL CWatchdogChildFrame::PreCreateWindow(CREATESTRUCT& cs) {
	// TODO: Modify the Window class or styles here by modifying the CREATESTRUCT cs
	if (!CMDIChildWndEx::PreCreateWindow(cs))
		return FALSE;

	return TRUE;
}

// CWatchdogChildFrame diagnostics

#ifdef _DEBUG
void CWatchdogChildFrame::AssertValid() const {
	CMDIChildWndEx::AssertValid();
}

void CWatchdogChildFrame::Dump(CDumpContext& dc) const {
	CMDIChildWndEx::Dump(dc);
}
#endif //_DEBUG

// CWatchdogChildFrame message handlers
