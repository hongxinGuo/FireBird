#include "pch.h"

#include"afxwin.h"

#include "framework.h"

#include "WatchdogOutputWnd.h"
#include "Resource.h"
//#include "WatchdogMainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CString GetStringOfLocalDateTime() {
	char buffer[100];
	tm tmLocal;
	time_t tUTC;
	time(&tUTC);
	localtime_s(&tmLocal, &tUTC);
	sprintf_s(buffer, _T("%04d年%02d月%02d日 %02d:%02d:%02d "), tmLocal.tm_year + 1900, tmLocal.tm_mon + 1, tmLocal.tm_mday, tmLocal.tm_hour, tmLocal.tm_min, tmLocal.tm_sec);
	CString str = buffer;
	return (str);
}

/////////////////////////////////////////////////////////////////////////////
// COutputBar

CWatchdogOutputWnd::CWatchdogOutputWnd() noexcept {
	m_uIdTimer = 0;
}

CWatchdogOutputWnd::~CWatchdogOutputWnd() {}

void CWatchdogOutputWnd::ReportInfo(const CString& strInfo) {
	CString str = GetStringOfLocalDateTime();
	str += _T("  ");
	str += strInfo;
	m_wndOutputBuild.AddString(str);
}

BEGIN_MESSAGE_MAP(CWatchdogOutputWnd, CDockablePane)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_TIMER()
END_MESSAGE_MAP()

int CWatchdogOutputWnd::OnCreate(LPCREATESTRUCT lpCreateStruct) {
	if (CDockablePane::OnCreate(lpCreateStruct) == -1)
		return -1;

	CRect rectDummy;
	rectDummy.SetRectEmpty();

	// Create tabs window:
	if (!m_wndTabs.Create(CMFCTabCtrl::STYLE_FLAT, rectDummy, this, 1)) {
		TRACE0("Failed to create output tab window\n");
		return -1;      // fail to create
	}

	// Create output panes:
	constexpr DWORD dwStyle = LBS_NOINTEGRALHEIGHT | WS_CHILD | WS_VISIBLE | WS_HSCROLL | WS_VSCROLL;

	if (!m_wndOutputBuild.Create(dwStyle, rectDummy, &m_wndTabs, 2) ||
		!m_wndOutputDebug.Create(dwStyle, rectDummy, &m_wndTabs, 3) ||
		!m_wndOutputFind.Create(dwStyle, rectDummy, &m_wndTabs, 4)) {
		TRACE0("Failed to create output windows\n");
		return -1;      // fail to create
	}

	UpdateFonts();

	CString strTabName;

	// Attach list windows to tab:
	BOOL bNameValid = strTabName.LoadString(IDS_BUILD_TAB);
	ASSERT(bNameValid);
	m_wndTabs.AddTab(&m_wndOutputBuild, strTabName, (UINT)0);
	bNameValid = strTabName.LoadString(IDS_DEBUG_TAB);
	ASSERT(bNameValid);
	m_wndTabs.AddTab(&m_wndOutputDebug, strTabName, (UINT)1);
	bNameValid = strTabName.LoadString(IDS_FIND_TAB);
	ASSERT(bNameValid);
	m_wndTabs.AddTab(&m_wndOutputFind, strTabName, (UINT)2);

	// Fill output tabs with some dummy text (nothing magic here)
	FillDebugWindow();
	FillFindWindow();

	// 设置10秒每次的软调度
	m_uIdTimer = SetTimer(static_cast<UINT_PTR>(3), 10000, nullptr);
	if (m_uIdTimer == 0) {
		CString str;
	}

	return 0;
}

void CWatchdogOutputWnd::OnSize(UINT nType, int cx, int cy) {
	CDockablePane::OnSize(nType, cx, cy);

	// Tab control should cover the whole client area:
	m_wndTabs.SetWindowPos(nullptr, -1, -1, cx, cy, SWP_NOMOVE | SWP_NOACTIVATE | SWP_NOZORDER);
}

void CWatchdogOutputWnd::AdjustHorzScroll(CListBox& wndListBox) {
	CClientDC dc(this);
	CFont* pOldFont = dc.SelectObject(&afxGlobalData.fontRegular);

	int cxExtentMax = 0;

	for (int i = 0; i < wndListBox.GetCount(); i++) {
		CString strItem;
		wndListBox.GetText(i, strItem);

		cxExtentMax = max(cxExtentMax, (int)dc.GetTextExtent(strItem).cx);
	}

	wndListBox.SetHorizontalExtent(cxExtentMax);
	dc.SelectObject(pOldFont);
}

void CWatchdogOutputWnd::FillDebugWindow() {
	m_wndOutputDebug.AddString(_T("Debug output is being displayed here."));
	m_wndOutputDebug.AddString(_T("The output is being displayed in rows of a list view"));
	m_wndOutputDebug.AddString(_T("but you can change the way it is displayed as you wish..."));
}

void CWatchdogOutputWnd::FillFindWindow() {
	m_wndOutputFind.AddString(_T("Find output is being displayed here."));
	m_wndOutputFind.AddString(_T("The output is being displayed in rows of a list view"));
	m_wndOutputFind.AddString(_T("but you can change the way it is displayed as you wish..."));
}

void CWatchdogOutputWnd::UpdateFonts() {
	m_wndOutputBuild.SetFont(&afxGlobalData.fontRegular);
	m_wndOutputDebug.SetFont(&afxGlobalData.fontRegular);
	m_wndOutputFind.SetFont(&afxGlobalData.fontRegular);
}

void CWatchdogOutputWnd::OnTimer(UINT_PTR nIDEvent) {
	CString str, str2;
	bool fUpdate = false;
	CString strTime = GetStringOfLocalDateTime(); // 消息的前缀，使用当地时间

	// 如果显示列表超过10000个，则删除前面的1000个。
	if (m_wndOutputBuild.GetCount() > 10000) m_wndOutputBuild.TruncateList(1000);
	// 将输出信息拷贝到消息队列中。
	//if ((lTotal = gl_systemMessage.InformationSize()) > 0) {
	const long lCurrentPos = m_wndOutputBuild.GetCurSel();
	if (m_wndOutputBuild.GetCount() <= (lCurrentPos + 4)) fUpdate = true;
	//gl_systemMessage.DisplayInformation(&m_wndOutputBuild, strTime);
	if (fUpdate) {
		m_wndOutputBuild.SetCurAtLastLine();
	}
	//}
	CDockablePane::OnTimer(nIDEvent);
}

/////////////////////////////////////////////////////////////////////////////
// COutputList1

COutputList::COutputList() noexcept {}

COutputList::~COutputList() {}

BEGIN_MESSAGE_MAP(COutputList, CListBox)
	ON_WM_CONTEXTMENU()
	ON_COMMAND(ID_EDIT_COPY, OnEditCopy)
	ON_COMMAND(ID_EDIT_CLEAR, OnEditClear)
	ON_COMMAND(ID_VIEW_OUTPUTWND, OnViewOutput)
	ON_WM_WINDOWPOSCHANGING()
END_MESSAGE_MAP()
/////////////////////////////////////////////////////////////////////////////
// COutputList message handlers

void COutputList::OnContextMenu(CWnd* /*pWnd*/, CPoint point) {
	CMenu menu;
	menu.LoadMenu(IDR_OUTPUT_POPUP);

	const CMenu* pSumMenu = menu.GetSubMenu(0);

	if (AfxGetMainWnd()->IsKindOf(RUNTIME_CLASS(CMDIFrameWndEx))) {
		CMFCPopupMenu* pPopupMenu = new CMFCPopupMenu;

		if (!pPopupMenu->Create(this, point.x, point.y, (HMENU)pSumMenu->m_hMenu, FALSE, TRUE))
			return;

		static_cast<CMDIFrameWndEx*>(AfxGetMainWnd())->OnShowPopupMenu(pPopupMenu);
		UpdateDialogControls(this, FALSE);
	}

	SetFocus();
}

void COutputList::OnEditCopy() {
	MessageBox(_T("Copy output"));
}

void COutputList::OnEditClear() {
	MessageBox(_T("Clear output"));
}

void COutputList::OnViewOutput() {
	CDockablePane* pParentBar = DYNAMIC_DOWNCAST(CDockablePane, GetOwner());
	CMDIFrameWndEx* pMainFrame = DYNAMIC_DOWNCAST(CMDIFrameWndEx, GetTopLevelFrame());

	if (pMainFrame != nullptr && pParentBar != nullptr) {
		pMainFrame->SetFocus();
		pMainFrame->ShowPane(pParentBar, FALSE, FALSE, FALSE);
		pMainFrame->RecalcLayout();
	}
}

void COutputList::TruncateList(long lNumberOfTruncation) {
	for (int i = 0; i < lNumberOfTruncation; i++) {
		DeleteString(0);
	}
}

void COutputList::SetCurAtLastLine() {
	SetCurSel(GetCount() - 1);
	SetTopIndex(GetCount() - 1);
}
