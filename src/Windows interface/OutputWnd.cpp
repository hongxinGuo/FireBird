#include"pch.h"

#include "OutputWnd.h"
#include "resource.h"
#include "MainFrm.h"

#include"ChinaMarket.h"
#include "WorldMarket.h"

/////////////////////////////////////////////////////////////////////////////
// COutputBar

COutputWnd::COutputWnd() {
	m_uIdTimer = 0;
}

COutputWnd::~COutputWnd() {
}

BEGIN_MESSAGE_MAP(COutputWnd, CDockablePane)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_TIMER()
END_MESSAGE_MAP()

int COutputWnd::OnCreate(LPCREATESTRUCT lpCreateStruct) {
	if (CDockablePane::OnCreate(lpCreateStruct) == -1) return -1;

	CRect rectDummy;
	rectDummy.SetRectEmpty();

	// 创建选项卡窗口:
	if (!m_wndTabs.Create(CMFCTabCtrl::STYLE_FLAT, rectDummy, this, 1)) {
		TRACE0("未能创建输出选项卡窗口\n");
		return -1;      // 未能创建
	}

	// 创建输出窗格:
	constexpr DWORD dwStyle = LBS_NOINTEGRALHEIGHT | WS_CHILD | WS_VISIBLE | WS_HSCROLL | WS_VSCROLL;

	if (!m_wndOutputInformation.Create(dwStyle, rectDummy, &m_wndTabs, 2) ||
		!m_wndOutputDayLineInfo.Create(dwStyle, rectDummy, &m_wndTabs, 3) ||
		!m_wndOutputTransaction.Create(dwStyle, rectDummy, &m_wndTabs, 4) ||
		!m_wndOutputCancelSell.Create(dwStyle, rectDummy, &m_wndTabs, 5) ||
		!m_wndOutputCancelBuy.Create(dwStyle, rectDummy, &m_wndTabs, 6) ||
		!m_wndOutputTrace2.Create(dwStyle, rectDummy, &m_wndTabs, 7) ||
		!m_wndOutputWebSocketInfo.Create(dwStyle, rectDummy, &m_wndTabs, 8) ||
		!m_wndOutputInnerSystemInformation.Create(dwStyle, rectDummy, &m_wndTabs, 9) ||
		!m_wndChinaMarketTaskQueue.Create(dwStyle, rectDummy, &m_wndTabs, 10) ||
		!m_wndWorldMarketTaskQueue.Create(dwStyle, rectDummy, &m_wndTabs, 11) ||
		!m_wndErrorMessage.Create(dwStyle, rectDummy, &m_wndTabs, 12)) {
		TRACE0("未能创建输出窗口\n");
		return -1;      // 未能创建
	}

	UpdateFonts();

	CString strTabName;

	// 将列表窗口附加到选项卡:
	BOOL bNameValid = strTabName.LoadString(IDS_INFORMATION_TAB);
	ASSERT(bNameValid);
	m_wndTabs.AddTab(&m_wndOutputInformation, strTabName, (UINT)0);
	bNameValid = strTabName.LoadString(IDS_DAYLINE_INFO_TAB);
	ASSERT(bNameValid);
	m_wndTabs.AddTab(&m_wndOutputDayLineInfo, strTabName, (UINT)1);
	bNameValid = strTabName.LoadString(IDS_CHINA_MARKET_TASK_QUEUE);
	ASSERT(bNameValid);
	m_wndTabs.AddTab(&m_wndChinaMarketTaskQueue, strTabName, (UINT)2);  // 错误消息
	bNameValid = strTabName.LoadString(IDS_WORLD_MARKET_TASK_QUEUE);
	ASSERT(bNameValid);
	m_wndTabs.AddTab(&m_wndWorldMarketTaskQueue, strTabName, (UINT)3);  // 错误消息
	bNameValid = strTabName.LoadString(IDS_TRANSACTION_TAB);
	ASSERT(bNameValid);
	m_wndTabs.AddTab(&m_wndOutputTransaction, strTabName, (UINT)4);
	bNameValid = strTabName.LoadString(IDS_CANCEL_SELL_TAB);
	ASSERT(bNameValid);
	m_wndTabs.AddTab(&m_wndOutputCancelSell, strTabName, (UINT)5);
	bNameValid = strTabName.LoadString(IDS_CANCEL_BUY_TAB);
	ASSERT(bNameValid);
	m_wndTabs.AddTab(&m_wndOutputCancelBuy, strTabName, (UINT)6);
	bNameValid = strTabName.LoadString(IDS_TRACE2_TAB);
	ASSERT(bNameValid);
	m_wndTabs.AddTab(&m_wndOutputTrace2, strTabName, (UINT)7);
	bNameValid = strTabName.LoadString(IDS_WEB_SOCKET_INFO_TAB);
	ASSERT(bNameValid);
	m_wndTabs.AddTab(&m_wndOutputWebSocketInfo, strTabName, (UINT)8);
	bNameValid = strTabName.LoadString(IDS_INNER_SYSTEM_INFORMATION_TAB2); // WebSocket消息
	ASSERT(bNameValid);
	m_wndTabs.AddTab(&m_wndOutputInnerSystemInformation, strTabName, (UINT)9); // 软件系统消息
	bNameValid = strTabName.LoadString(IDS_ERROR_MESSAGE);
	ASSERT(bNameValid);
	m_wndTabs.AddTab(&m_wndErrorMessage, strTabName, (UINT)10);  // 错误消息

	// 设置1000毫秒每次的软调度
	m_uIdTimer = SetTimer(static_cast<UINT_PTR>(3), 1000, nullptr);
	if (m_uIdTimer == 0) {
		CString str;
	}

	return 0;
}

void COutputWnd::OnSize(UINT nType, int cx, int cy) {
	CDockablePane::OnSize(nType, cx, cy);

	// 选项卡控件应覆盖整个工作区:
	m_wndTabs.SetWindowPos(nullptr, -1, -1, cx, cy, SWP_NOMOVE | SWP_NOACTIVATE | SWP_NOZORDER);
}

void COutputWnd::AdjustHorzScroll(CListBox& wndListBox) {
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

void COutputWnd::UpdateFonts() {
	m_wndOutputInformation.SetFont(&afxGlobalData.fontRegular);
	m_wndOutputDayLineInfo.SetFont(&afxGlobalData.fontRegular);
	m_wndOutputTransaction.SetFont(&afxGlobalData.fontRegular);
	m_wndOutputCancelSell.SetFont(&afxGlobalData.fontRegular);
	m_wndOutputCancelBuy.SetFont(&afxGlobalData.fontRegular);
	m_wndOutputTrace2.SetFont(&afxGlobalData.fontRegular);
	m_wndOutputWebSocketInfo.SetFont(&afxGlobalData.fontRegular);
	m_wndOutputInnerSystemInformation.SetFont(&afxGlobalData.fontRegular);
	m_wndChinaMarketTaskQueue.SetFont(&afxGlobalData.fontRegular);
	m_wndWorldMarketTaskQueue.SetFont(&afxGlobalData.fontRegular);
	m_wndErrorMessage.SetFont(&afxGlobalData.fontRegular);
}

/////////////////////////////////////////////////////////////////////////////////////////
//
// 显示各项系统消息和警告等。
//
//
//
/////////////////////////////////////////////////////////////////////////////////////////
void COutputWnd::OnTimer(UINT_PTR nIDEvent) {
	CString str2;
	long lCurrentPos;
	bool fUpdate = false;
	const CString strTime = gl_pChinaMarket->GetStringOfLocalDateTime(); // 消息的前缀，使用当地时间

	// 如果显示列表超过10000个，则删除前面的1000个。
	if (m_wndOutputInformation.GetCount() > 10000) m_wndOutputInformation.TruncateList(1000);
	// 将输出信息拷贝到消息队列中。
	if ((gl_systemMessage.InformationSize()) > 0) {
		lCurrentPos = m_wndOutputInformation.GetCurSel();
		if (m_wndOutputInformation.GetCount() <= (lCurrentPos + 4)) fUpdate = true;
		gl_systemMessage.DisplayInformation(&m_wndOutputInformation, strTime);
		if (fUpdate) {
			m_wndOutputInformation.SetCurAtLastLine();
		}
	}

	if (m_wndOutputDayLineInfo.GetCount() > 10000) m_wndOutputDayLineInfo.TruncateList(1000);
	fUpdate = false;
	if ((gl_systemMessage.DayLineInfoSize()) > 0) {
		lCurrentPos = m_wndOutputDayLineInfo.GetCurSel();
		if (m_wndOutputDayLineInfo.GetCount() <= (lCurrentPos + 4)) fUpdate = true;
		gl_systemMessage.DisplayDayLineInfo(&m_wndOutputDayLineInfo, strTime);
		if (fUpdate) {
			m_wndOutputDayLineInfo.SetCurAtLastLine();
		}
	}

	if (m_wndOutputTransaction.GetCount() > 10000) m_wndOutputTransaction.TruncateList(1000);
	fUpdate = false;
	if ((gl_systemMessage.TransactionInfoSize()) > 0) {
		lCurrentPos = m_wndOutputTransaction.GetCurSel();
		if (m_wndOutputTransaction.GetCount() <= (lCurrentPos + 4)) fUpdate = true;
		gl_systemMessage.DisplayTransaction(&m_wndOutputTransaction, strTime);
		if (fUpdate) {
			m_wndOutputTransaction.SetCurAtLastLine();
		}
	}

	if (m_wndOutputCancelSell.GetCount() > 10000) m_wndOutputCancelSell.TruncateList(1000);
	fUpdate = false;
	if ((gl_systemMessage.CancelSellInfoSize()) > 0) {
		lCurrentPos = m_wndOutputCancelSell.GetCurSel();
		if (m_wndOutputCancelSell.GetCount() <= (lCurrentPos + 4)) fUpdate = true;
		gl_systemMessage.DisplayCancelSell(&m_wndOutputCancelSell, strTime);
		if (fUpdate) {
			m_wndOutputCancelSell.SetCurAtLastLine();
		}
	}

	if (m_wndOutputCancelBuy.GetCount() > 10000) m_wndOutputCancelBuy.TruncateList(1000);
	fUpdate = false;
	if ((gl_systemMessage.CancelBuyInfoSize()) > 0) {
		lCurrentPos = m_wndOutputCancelBuy.GetCurSel();
		if (m_wndOutputCancelBuy.GetCount() <= (lCurrentPos + 4)) fUpdate = true;
		gl_systemMessage.DisplayCancelBuy(&m_wndOutputCancelBuy, strTime);
		if (fUpdate) {
			m_wndOutputCancelBuy.SetCurAtLastLine();
		}
	}

	if (m_wndOutputTrace2.GetCount() > 10000) m_wndOutputTrace2.TruncateList(1000);
	fUpdate = false;
	if ((gl_systemMessage.Trace2Size()) > 0) {
		lCurrentPos = m_wndOutputTrace2.GetCurSel();
		if (m_wndOutputTrace2.GetCount() <= (lCurrentPos + 4)) fUpdate = true;
		gl_systemMessage.DisplayTrace2(&m_wndOutputTrace2, strTime);
		if (fUpdate) {
			m_wndOutputTrace2.SetCurAtLastLine();
		}
	}

	if (m_wndOutputWebSocketInfo.GetCount() > 10000) m_wndOutputWebSocketInfo.TruncateList(1000);
	fUpdate = false;
	if ((gl_systemMessage.WebSocketInfoSize()) > 0) {
		lCurrentPos = m_wndOutputWebSocketInfo.GetCurSel();
		if (m_wndOutputWebSocketInfo.GetCount() <= (lCurrentPos + 4)) fUpdate = true;
		gl_systemMessage.DisplayWebSocketInfo(&m_wndOutputWebSocketInfo, strTime);
		if (fUpdate) {
			m_wndOutputWebSocketInfo.SetCurAtLastLine();
		}
	}

	if (m_wndOutputInnerSystemInformation.GetCount() > 10000) m_wndOutputInnerSystemInformation.TruncateList(1000);
	fUpdate = false;
	if ((gl_systemMessage.InnerSystemInfoSize()) > 0) {
		lCurrentPos = m_wndOutputInnerSystemInformation.GetCurSel();
		if (m_wndOutputInnerSystemInformation.GetCount() <= (lCurrentPos + 4)) fUpdate = true;
		gl_systemMessage.DisplayInnerSystemInformation(&m_wndOutputInnerSystemInformation, strTime);
		if (fUpdate) {
			m_wndOutputInnerSystemInformation.SetCurAtLastLine();
		}
	}

	if (m_wndErrorMessage.GetCount() > 10000) m_wndErrorMessage.TruncateList(1000);
	fUpdate = false;
	if ((gl_systemMessage.ErrorMessageSize()) > 0) {
		lCurrentPos = m_wndErrorMessage.GetCurSel();
		if (m_wndErrorMessage.GetCount() <= (lCurrentPos + 4)) fUpdate = true;
		gl_systemMessage.DisplayErrorMessage(&m_wndErrorMessage, strTime);
		if (fUpdate) {
			m_wndErrorMessage.SetCurAtLastLine();
		}
	}

	char buffer[50];
	const auto pvChinaTask = gl_pChinaMarket->DiscardOutDatedTask(gl_pChinaMarket->GetMarketTime());
	if (m_wndChinaMarketTaskQueue.GetCount() > 0) m_wndChinaMarketTaskQueue.TruncateList(m_wndChinaMarketTaskQueue.GetCount());
	int i = 0;
	for (const auto& pTask : *pvChinaTask) {
		CString str = strTime + _T(": ");
		sprintf_s(buffer, _T("%06d"), pTask->GetTime());
		str += buffer;
		str += _T(": ");
		str += gl_mapMarketMapIndex.at(pTask->GetType());
		m_wndChinaMarketTaskQueue.AddString(str);
		if (++i >= m_wndChinaMarketTaskQueue.GetLineNumber()) break;
	}

	const auto pvWorldTask = gl_pWorldMarket->DiscardOutDatedTask(gl_pWorldMarket->GetMarketTime());
	if (m_wndWorldMarketTaskQueue.GetCount() > 0) m_wndWorldMarketTaskQueue.TruncateList(m_wndWorldMarketTaskQueue.GetCount());
	i = 0;
	for (const auto& pTask : *pvWorldTask) {
		CString str = strTime + _T(": ");
		sprintf_s(buffer, _T("%06d"), pTask->GetTime());
		str += buffer;
		str += _T(": ");
		str += gl_mapMarketMapIndex.at(pTask->GetType());
		m_wndWorldMarketTaskQueue.AddString(str);
		if (++i >= m_wndWorldMarketTaskQueue.GetLineNumber()) break;
	}

	// 调用基类的OnTimer函数
	CDockablePane::OnTimer(nIDEvent);
}

/////////////////////////////////////////////////////////////////////////////
// COutputList1

COutputList::COutputList() {
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

COutputList::~COutputList() {
}

BEGIN_MESSAGE_MAP(COutputList, CListBox)
	ON_WM_CONTEXTMENU()
	ON_COMMAND(ID_EDIT_COPY, OnEditCopy)
	ON_COMMAND(ID_EDIT_CLEAR, OnEditClear)
	ON_COMMAND(ID_VIEW_OUTPUTWND, OnViewOutput)
	ON_WM_WINDOWPOSCHANGING()
	ON_WM_SIZE()
END_MESSAGE_MAP()
/////////////////////////////////////////////////////////////////////////////
// COutputList 消息处理程序

void COutputList::OnContextMenu(CWnd* /*pWnd*/, CPoint point) {
	CMenu menu;
	menu.LoadMenu(IDR_OUTPUT_POPUP);

	const CMenu* pSumMenu = menu.GetSubMenu(0);

	if (AfxGetMainWnd()->IsKindOf(RUNTIME_CLASS(CMDIFrameWndEx))) {
		const auto pPopupMenu = new CMFCPopupMenu;

		if (!pPopupMenu->Create(this, point.x, point.y, (HMENU)pSumMenu->m_hMenu, FALSE, TRUE)) return;

		static_cast<CMDIFrameWndEx*>(AfxGetMainWnd())->OnShowPopupMenu(pPopupMenu);
		UpdateDialogControls(this, FALSE);
	}

	SetFocus();
}

void COutputList::OnEditCopy() {
	MessageBox(_T("复制输出"));
}

void COutputList::OnEditClear() {
	MessageBox(_T("清除输出"));
}

void COutputList::OnViewOutput() {
	const auto pParentBar = DYNAMIC_DOWNCAST(CDockablePane, GetOwner());
	const auto pMainFrame = DYNAMIC_DOWNCAST(CMDIFrameWndEx, GetTopLevelFrame());

	if (pMainFrame != nullptr && pParentBar != nullptr) {
		pMainFrame->SetFocus();
		pMainFrame->ShowPane(pParentBar, FALSE, FALSE, FALSE);
		pMainFrame->RecalcLayout();
	}
}

void COutputList::OnSize(UINT nType, int cx, int cy) {
	CListBox::OnSize(nType, cx, cy);

	// TODO: Add your message handler code here
	int h = GetItemHeight(0);
	m_iLineNumber = cy / h;
}
