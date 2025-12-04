// FireBirdView.cpp: CFireBirdView 类的实现
//

#include"pch.h"

#include"ChinaStock.h"

// SHARED_HANDayLineERS 可以在实现预览、缩略图和搜索筛选器句柄的
// ATL 项目中进行定义，并允许与该项目共享文档代码。
#ifndef SHARED_HANDayLineERS
#include "FireBird.h"
#endif

#include "FireBirdDoc.h"
#include "FireBirdView.h"

#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

void ShowRealtimeVolume(CDC* pDC, const vector<INT64>& vVolume, const vector<INT64>& vData, int iRightPos, CRect rectClient, bool fUpsideDown) {
	ASSERT(vData.size() == 240);

	constexpr COLORREF crGreen(RGB(0, 255, 0)), crWhite(RGB(255, 255, 255)),
	                   crRed(RGB(255, 0, 0));
	int iPenWidth = rectClient.Width() / 240;
	CPen penGreen1(PS_SOLID, iPenWidth, crGreen), penWhite1(PS_SOLID, iPenWidth, crWhite), penRed1(PS_SOLID, iPenWidth, crRed);

	if (fUpsideDown) { // sell volume
		pDC->SelectObject(&penGreen1);
	}
	else { // buy volume
		pDC->SelectObject(&penRed1);
	}
	INT64 height;
	if (vVolume.at(0) > 0) {
		height = vData.at(0) * rectClient.Height() / vVolume.at(0);
		if (fUpsideDown) {
			pDC->MoveTo(rectClient.left, rectClient.top);
			pDC->LineTo(rectClient.left, rectClient.top + height);
		}
		else {
			pDC->MoveTo(rectClient.left, rectClient.bottom);
			pDC->LineTo(rectClient.left, rectClient.bottom - height);
		}
	}

	for (int i = 1; i < iRightPos; i++) {
		INT64 differVolume = vVolume.at(i) - vVolume.at(i - 1);
		if (differVolume > 0) {
			height = (vData.at(i) - vData.at(i - 1)) * rectClient.Height() / differVolume;
			if (fUpsideDown) {
				pDC->MoveTo(rectClient.left + iPenWidth * i, rectClient.top);
				pDC->LineTo(rectClient.left + iPenWidth * i, rectClient.top + height);
			}
			else {
				pDC->MoveTo(rectClient.left + iPenWidth * i, rectClient.bottom);
				pDC->LineTo(rectClient.left + iPenWidth * i, rectClient.bottom - height);
			}
		}
	}
}

// CFireBirdView

IMPLEMENT_DYNCREATE(CFireBirdView, CView)

BEGIN_MESSAGE_MAP(CFireBirdView, CView)
	// 标准打印命令
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CFireBirdView::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_WM_TIMER()
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_COMMAND(ID_SHOW_RS3, &CFireBirdView::OnShowRs3)
	ON_COMMAND(ID_SHOW_RS5, &CFireBirdView::OnShowRs5)
	ON_COMMAND(ID_SHOW_RS10, &CFireBirdView::OnShowRs10)
	ON_COMMAND(ID_SHOW_RS30, &CFireBirdView::OnShowRs30)
	ON_COMMAND(ID_SHOW_RS60, &CFireBirdView::OnShowRs60)
	ON_COMMAND(ID_SHOW_RS120, &CFireBirdView::OnShowRs120)
	ON_UPDATE_COMMAND_UI(ID_SHOW_RS10, &CFireBirdView::OnUpdateShowRs10)
	ON_UPDATE_COMMAND_UI(ID_SHOW_RS120, &CFireBirdView::OnUpdateShowRs120)
	ON_UPDATE_COMMAND_UI(ID_SHOW_RS3, &CFireBirdView::OnUpdateShowRs3)
	ON_UPDATE_COMMAND_UI(ID_SHOW_RS30, &CFireBirdView::OnUpdateShowRs30)
	ON_UPDATE_COMMAND_UI(ID_SHOW_RS5, &CFireBirdView::OnUpdateShowRs5)
	ON_UPDATE_COMMAND_UI(ID_SHOW_RS60, &CFireBirdView::OnUpdateShowRs60)
	ON_COMMAND(ID_SHOW_RS_IN_LOGARITHM, &CFireBirdView::OnShowRsInLogarithm)
	ON_UPDATE_COMMAND_UI(ID_SHOW_RS_IN_LOGARITHM, &CFireBirdView::OnUpdateShowRsInLogarithm)
	ON_COMMAND(ID_SHOW_RS_IN_LINEAR, &CFireBirdView::OnShowRsInLinear)
	ON_UPDATE_COMMAND_UI(ID_SHOW_RS_IN_LINEAR, &CFireBirdView::OnUpdateShowRsInLinear)
	ON_COMMAND(ID_SHOW_RS_INDEX, &CFireBirdView::OnShowRsIndex)
	ON_UPDATE_COMMAND_UI(ID_SHOW_RS_INDEX, &CFireBirdView::OnUpdateShowRsIndex)
	ON_COMMAND(ID_SHOW_DAYLINE, &CFireBirdView::OnShowDayLine)
	ON_UPDATE_COMMAND_UI(ID_SHOW_DAYLINE, &CFireBirdView::OnUpdateShowDayLine)
	//	ON_COMMAND(ID_SHOW_REALTIME, &CFireBirdView::OnShowRealTime)
	//	ON_UPDATE_COMMAND_UI(ID_SHOW_REALTIME, &CFireBirdView::OnUpdateShowRealTime)
	ON_COMMAND(ID_SHOW_WEEKLINE, &CFireBirdView::OnShowWeekLine)
	ON_UPDATE_COMMAND_UI(ID_SHOW_WEEKLINE, &CFireBirdView::OnUpdateShowWeekLine)
	ON_WM_SETFOCUS()
	ON_COMMAND(ID_SHOW_MONTH_LINE, &CFireBirdView::OnShowMonthLine)
	ON_UPDATE_COMMAND_UI(ID_SHOW_MONTH_LINE, &CFireBirdView::OnUpdateShowMonthLine)
END_MESSAGE_MAP()

// CFireBirdView 构造/析构

CFireBirdView::CFireBirdView() {
	m_iCurrentShowType = _SHOW_DAY_LINE_DATA_; // 显示日线数据
	m_lCurrentPos = 0;
	m_rectClient.SetRect(CPoint(0, 0), CPoint(0.0));
	int iHeight = 100;
	CRect rectArea{ 2, 0, 962, 100 };

	m_rectOrdinaryBuyVolume.top = 0;
	m_rectOrdinaryBuyVolume.left = rectArea.left;
	m_rectOrdinaryBuyVolume.right = rectArea.right;
	m_rectOrdinaryBuyVolume.bottom = iHeight;
	m_rectOrdinarySellVolume.top = m_rectOrdinaryBuyVolume.bottom + 1;
	m_rectOrdinarySellVolume.left = rectArea.left;
	m_rectOrdinarySellVolume.right = rectArea.right;
	m_rectOrdinarySellVolume.bottom = m_rectOrdinaryBuyVolume.bottom + iHeight;
	m_rectAttackBuyVolume.top = m_rectOrdinarySellVolume.bottom;
	m_rectAttackBuyVolume.left = rectArea.left;
	m_rectAttackBuyVolume.right = rectArea.right;
	m_rectAttackBuyVolume.bottom = m_rectOrdinarySellVolume.bottom + iHeight;
	m_rectAttackSellVolume.top = m_rectAttackBuyVolume.bottom + 1;
	m_rectAttackSellVolume.left = rectArea.left;
	m_rectAttackSellVolume.right = rectArea.right;
	m_rectAttackSellVolume.bottom = m_rectAttackBuyVolume.bottom + iHeight;
	m_rectStrongBuyVolume.top = m_rectAttackSellVolume.bottom + 1;
	m_rectStrongBuyVolume.left = rectArea.left;
	m_rectStrongBuyVolume.right = rectArea.right;
	m_rectStrongBuyVolume.bottom = m_rectAttackSellVolume.bottom + iHeight;
	m_rectStrongSellVolume.top = m_rectStrongBuyVolume.bottom + 1;
	m_rectStrongSellVolume.left = rectArea.left;
	m_rectStrongSellVolume.right = rectArea.right;
	m_rectStrongSellVolume.bottom = m_rectStrongBuyVolume.bottom + iHeight;
	m_rectCancelBuyVolume.top = m_rectStrongSellVolume.bottom + 1;
	m_rectCancelBuyVolume.left = rectArea.left;
	m_rectCancelBuyVolume.right = rectArea.right;
	m_rectCancelBuyVolume.bottom = m_rectStrongSellVolume.bottom + iHeight;
	m_rectCancelSellVolume.top = m_rectCancelBuyVolume.bottom + 1;
	m_rectCancelSellVolume.left = rectArea.left;
	m_rectCancelSellVolume.right = rectArea.right;
	m_rectCancelSellVolume.bottom = m_rectCancelBuyVolume.bottom + iHeight;

	m_fShowRS = false;
	m_fShow3DaysRS = false;
	m_fShow5DaysRS = false;
	m_fShow10DaysRS = true;
	m_fShow30DaysRS = true;
	m_fShow60DaysRS = false;
	m_fShow120DaysRS = false;
	m_iShowRSOption = 0; // 默认值为指数相对强度
	m_fShowTransactionGraph = false;

	m_uIdTimer = 0;

	m_fCreateMemoryDC = false;
}

void CFireBirdView::ShowHistoryData(CDC* pDC, CRect rectDrawArea) {
	constexpr COLORREF crGreen(RGB(0, 255, 0)), crWhite(RGB(255, 255, 255)),
	                   crRed(RGB(255, 0, 0)), crYellow(RGB(255, 255, 0));
	CPen penGreen1(PS_SOLID, 1, crGreen), penWhite1(PS_SOLID, 1, crWhite), penRed1(PS_SOLID, 1, crRed);
	CPen penYellow(PS_SOLID, 1, crYellow);
	std::pair<long, long> pairHighLow;
	switch (m_iCurrentShowType) {
	case _SHOW_DAY_LINE_DATA_:
		pairHighLow = GetDocument()->GetDayLineHighLow(rectDrawArea.Width() / m_iCandleWidth);
		m_lDayLineHigh = pairHighLow.first;
		m_lDayLineLow = pairHighLow.second;
		GetDocument()->ShowDayLine(pDC, &penRed1, rectDrawArea, m_iCandleWidth, m_lDayLineHigh, m_lDayLineLow);
		GetDocument()->ShowDayLine5MovingAverage(pDC, &penRed1, rectDrawArea, m_iCandleWidth, m_lDayLineHigh, m_lDayLineLow);
		GetDocument()->ShowDayLine30MovingAverage(pDC, &penYellow, rectDrawArea, m_iCandleWidth, m_lDayLineHigh, m_lDayLineLow);
		GetDocument()->ShowDayLine50MovingAverage(pDC, &penWhite1, rectDrawArea, m_iCandleWidth, m_lDayLineHigh, m_lDayLineLow);
		GetDocument()->ShowDayLine250MovingAverage(pDC, &penGreen1, rectDrawArea, m_iCandleWidth, m_lDayLineHigh, m_lDayLineLow);

		GetDocument()->ShowDayLineKDJ(pDC, m_rectIndicator, m_iCandleWidth);
		break;
	case _SHOW_WEEK_LINE_DATA_:
		pairHighLow = GetDocument()->GetWeekLineHighLow(rectDrawArea.Width() / m_iCandleWidth);
		m_lWeekLineHigh = pairHighLow.first;
		m_lWeekLineLow = pairHighLow.second;
		GetDocument()->ShowWeekLine(pDC, &penRed1, rectDrawArea, m_iCandleWidth, m_lWeekLineHigh, m_lWeekLineLow);
		GetDocument()->ShowWeekLine5MovingAverage(pDC, &penRed1, rectDrawArea, m_iCandleWidth, m_lWeekLineHigh, m_lWeekLineLow);
		GetDocument()->ShowWeekLine30MovingAverage(pDC, &penYellow, rectDrawArea, m_iCandleWidth, m_lWeekLineHigh, m_lWeekLineLow);
		GetDocument()->ShowWeekLine50MovingAverage(pDC, &penWhite1, rectDrawArea, m_iCandleWidth, m_lWeekLineHigh, m_lWeekLineLow);
		GetDocument()->ShowWeekLine250MovingAverage(pDC, &penGreen1, rectDrawArea, m_iCandleWidth, m_lWeekLineHigh, m_lWeekLineLow);
		GetDocument()->ShowWeekLineKDJ(pDC, m_rectIndicator, m_iCandleWidth);
		break;
	case _SHOW_MONTH_LINE_DATA_:
		pairHighLow = GetDocument()->GetMonthLineHighLow(rectDrawArea.Width() / m_iCandleWidth);
		m_lMonthLineHigh = pairHighLow.first;
		m_lMonthLineLow = pairHighLow.second;
		GetDocument()->ShowMonthLine(pDC, &penRed1, rectDrawArea, m_iCandleWidth, m_lMonthLineHigh, m_lMonthLineLow);
		GetDocument()->ShowMonthLine5MovingAverage(pDC, &penRed1, rectDrawArea, m_iCandleWidth, m_lMonthLineHigh, m_lMonthLineLow);
		GetDocument()->ShowMonthLine30MovingAverage(pDC, &penYellow, rectDrawArea, m_iCandleWidth, m_lMonthLineHigh, m_lMonthLineLow);
		GetDocument()->ShowMonthLine50MovingAverage(pDC, &penWhite1, rectDrawArea, m_iCandleWidth, m_lMonthLineHigh, m_lMonthLineLow);
		GetDocument()->ShowMonthLine250MovingAverage(pDC, &penGreen1, rectDrawArea, m_iCandleWidth, m_lMonthLineHigh, m_lMonthLineLow);
		GetDocument()->ShowMonthLineKDJ(pDC, m_rectIndicator, m_iCandleWidth);
		break;
	default:
		break;
	}
}

void CFireBirdView::ShowStockHistoryDataLine(CDC* pDC) {
	constexpr COLORREF crGreen(RGB(0, 255, 0)), crRed(RGB(255, 0, 0)), crYellow(RGB(255, 255, 0));
	constexpr COLORREF crBlue(RGB(0, 0, 255)), crWhite(RGB(255, 255, 255));
	CPen penWhite1(PS_SOLID, 1, crWhite), penWhite2(PS_SOLID, 2, crWhite), penWhite3(PS_SOLID, 3, crWhite);
	CPen penRed1(PS_SOLID, 1, crRed), penRed2(PS_SOLID, 2, crRed), penRed3(PS_SOLID, 3, crRed);
	CPen penGreen1(PS_SOLID, 1, crGreen), penGreen2(PS_SOLID, 2, crGreen), penGreen3(PS_SOLID, 3, crGreen);
	CPen penYellow1(PS_SOLID, 1, crYellow), penYellow2(PS_SOLID, 2, crYellow), penYellow3(PS_SOLID, 3, crYellow);
	CPen penBlue1(PS_SOLID, 1, crBlue), penBlue2(PS_SOLID, 2, crBlue), penBlue3(PS_SOLID, 3, crBlue);
	auto pCurrentStock = gl_pCurrentStock;

	if (pCurrentStock == nullptr) return;
	CPen* ppen = pDC->SelectObject(&penRed1);
	SysCallMoveTo(pDC, m_rectClient.right, m_rectClient.bottom * 3 / 4);
	SysCallLineTo(pDC, 0, m_rectClient.bottom * 3 / 4);

	//当前被操作的历史数据容器
	//CVirtualDataHistoryCandleExtend* pHistoryData;
	switch (m_iCurrentShowType) {
	case _SHOW_DAY_LINE_DATA_:
		if (IsChinaStock(GetCurrentStock())) {
			// 画相对强度
			if (m_fShowRS) {
				pDC->SelectObject(&penWhite1);
				switch (m_iShowRSOption) {
				case 0: // 显示相对指数的强度
					GetCurrentStock()->ShowDayLineRSIndex(pDC, &penWhite1, m_rectRS, m_iCandleWidth, 1);
					break;
				case 1:
					GetCurrentStock()->ShowDayLineRS1(pDC, &penWhite1, m_rectRS, m_iCandleWidth, 1);
					break;
				case 2:
					GetCurrentStock()->ShowDayLineRSLogarithm(pDC, &penWhite1, m_rectRS, m_iCandleWidth, 1);
					break;
				default:
					// 错误
					break;
				}
			}

			// 画相对强度3日均线
			if (m_fShow3DaysRS) {
				GetCurrentStock()->ShowDayLineRS3(pDC, &penYellow1, m_rectRS, m_iCandleWidth, 1.5);
			}
			// 画相对强度5日均线
			if (m_fShow5DaysRS) {
				GetCurrentStock()->ShowDayLineRS5(pDC, &penGreen1, m_rectRS, m_iCandleWidth, 1.5);
			}
			// 画相对强度10日均线
			if (m_fShow10DaysRS) {
				GetCurrentStock()->ShowDayLineRS10(pDC, &penRed1, m_rectRS, m_iCandleWidth, 3);
			}
			// 画相对强度30日均线
			if (m_fShow30DaysRS) {
				GetCurrentStock()->ShowDayLineRS30(pDC, &penYellow1, m_rectRS, m_iCandleWidth, 3);
			}
			// 画相对强度60日均线
			if (m_fShow60DaysRS) {
				GetCurrentStock()->ShowDayLineRS60(pDC, &penBlue1, m_rectRS, m_iCandleWidth, 6);
			}
			// 画相对强度120日均线
			if (m_fShow120DaysRS) {
				GetCurrentStock()->ShowDayLineRS120(pDC, &penWhite1, m_rectRS, m_iCandleWidth, 6);
			}
		}
		break;
	case _SHOW_WEEK_LINE_DATA_:
		break; // 周线不显示相对强度
	case _SHOW_MONTH_LINE_DATA_:
		break; // 月线不显示相对强度
	default:
		break;
	}

	////////////////////////////////////////////////////////////////画日线蜡烛线
	ShowHistoryData(pDC, m_rectCandle);

	pDC->SelectObject(ppen);
}

BOOL CFireBirdView::PreCreateWindow(CREATESTRUCT& cs) {
	//  CREATESTRUCT cs 来修改窗口类或样式

	return CView::PreCreateWindow(cs);
}

// CFireBirdView 绘图

void CFireBirdView::OnDraw(CDC* pDC) {
	const CFireBirdDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc) return;

	//pDC = GetDC();
	Show(pDC);
	//ReleaseDC(pDC);
}

void CFireBirdView::Show(CDC* pdc) {
	CBitmap* pOldBitmap;
	COLORREF crGray(RGB(24, 24, 24));

	// create memory DC
	if (!m_fCreateMemoryDC) {
		m_MemoryDC.CreateCompatibleDC(pdc);
		m_Bitmap.CreateCompatibleBitmap(pdc, gl_systemConfiguration.GetSystemDisplayWidth(), gl_systemConfiguration.GetSystemDisplayHeight());
		m_MemoryDC.FillSolidRect(0, 0, gl_systemConfiguration.GetSystemDisplayWidth(), gl_systemConfiguration.GetSystemDisplayHeight(), crGray);
		m_fCreateMemoryDC = true;
	}

	CRect rect;
	SysCallGetClientRect(&rect);
	auto pStock = GetCurrentStock();
	if (pStock == nullptr || !GetDocument()->IsDataReady()) {
		pOldBitmap = m_MemoryDC.SelectObject(&m_Bitmap);
		m_MemoryDC.FillSolidRect(0, 0, rect.right, rect.bottom, crGray);
		SysCallBitBlt(pdc, 0, 0, rect.right, rect.bottom, &m_MemoryDC, 0, 0, SRCCOPY);
		if (pOldBitmap != nullptr) m_MemoryDC.SelectObject(pOldBitmap);
		return;
	}

	ASSERT(pStock != nullptr);
	ASSERT(GetDocument()->IsDataReady());
	switch (m_iCurrentShowType) {
	case _SHOW_DAY_LINE_DATA_: // show day line(or week line) stock data
	case _SHOW_WEEK_LINE_DATA_:
	case _SHOW_MONTH_LINE_DATA_:
		pOldBitmap = m_MemoryDC.SelectObject(&m_Bitmap);
		m_MemoryDC.FillSolidRect(0, 0, rect.right, rect.bottom, crGray);
		ShowStockHistoryDataLine(&m_MemoryDC);
		SysCallBitBlt(pdc, 0, 0, rect.right, rect.bottom, &m_MemoryDC, 0, 0, SRCCOPY);
		if (pOldBitmap != nullptr) m_MemoryDC.SelectObject(pOldBitmap);
		break;
	default:
		break;
	} // switch
}

// CFireBirdView 打印
void CFireBirdView::OnFilePrintPreview() {
#ifndef SHARED_HANDayLineERS
	AFXPrintPreview(this);
#endif
}

BOOL CFireBirdView::OnPreparePrinting(CPrintInfo* pInfo) {
	// 默认准备
	return DoPreparePrinting(pInfo);
}

void CFireBirdView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/) {
}

void CFireBirdView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/) {
}

void CFireBirdView::OnRButtonUp(UINT /* nFlags */, CPoint point) {
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CFireBirdView::OnContextMenu(CWnd* /* pWnd */, CPoint point) {
#ifndef SHARED_HANDayLineERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}

// CFireBirdView 诊断
#ifdef _DEBUG
CFireBirdDoc* CFireBirdView::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CFireBirdDoc)));
	return dynamic_cast<CFireBirdDoc*>(m_pDocument);
}
#endif //_DEBUG

// CFireBirdView 消息处理程序
void CFireBirdView::OnTimer(UINT_PTR nIDEvent) {
	CDC* pdc = GetDC();

	Show(pdc);

	ReleaseDC(pdc);

	CView::OnTimer(nIDEvent);
}

int CFireBirdView::OnCreate(LPCREATESTRUCT lpCreateStruct) {
	if (CView::OnCreate(lpCreateStruct) == -1) return -1;

	GetClientRect(&m_rectClient);

	m_rectCandle.left = m_rectClient.left;
	m_rectCandle.top = m_rectClient.top;
	m_rectCandle.right = m_rectClient.right;
	m_rectCandle.bottom = m_rectClient.bottom / 2;

	m_rectRS.left = m_rectClient.left;
	m_rectRS.top = m_rectClient.bottom / 2 + 1;
	m_rectRS.right = m_rectClient.right;
	m_rectRS.bottom = m_rectClient.bottom * 3 / 4;

	m_rectIndicator.left = m_rectClient.left;
	m_rectIndicator.top = m_rectClient.bottom * 3 / 4 + 1;
	m_rectIndicator.right = m_rectClient.right;
	m_rectIndicator.bottom = m_rectClient.bottom;

	m_uIdTimer = SetTimer(3, 500, nullptr); // 500毫秒每次调度，用于显示实时股票数据。
	if (m_uIdTimer == 0) {
		string str;
	}

	return 0;
}

void CFireBirdView::OnSize(UINT nType, int cx, int cy) {
	SysCallOnSize(nType, cx, cy);

	m_rectClient.right = cx;
	m_rectClient.bottom = cy;

	m_rectCandle.left = m_rectClient.left;
	m_rectCandle.top = m_rectClient.top;
	m_rectCandle.right = m_rectClient.right;
	m_rectCandle.bottom = m_rectClient.bottom / 2;

	m_rectRS.left = m_rectClient.left;
	m_rectRS.top = m_rectClient.bottom / 2 + 1;
	m_rectRS.right = m_rectClient.right;
	m_rectRS.bottom = m_rectClient.bottom * 3 / 4;

	m_rectIndicator.left = m_rectClient.left;
	m_rectIndicator.top = m_rectClient.bottom * 3 / 4 + 1;
	m_rectIndicator.right = m_rectClient.right;
	m_rectIndicator.bottom = m_rectClient.bottom;
}

void CFireBirdView::OnShowRs3() {
	if (m_fShow3DaysRS) m_fShow3DaysRS = false;
	else m_fShow3DaysRS = true;
}

void CFireBirdView::OnShowRs5() {
	if (m_fShow5DaysRS) m_fShow5DaysRS = false;
	else m_fShow5DaysRS = true;
}

void CFireBirdView::OnShowRs10() {
	if (m_fShow10DaysRS) m_fShow10DaysRS = false;
	else m_fShow10DaysRS = true;
}

void CFireBirdView::OnShowRs30() {
	if (m_fShow30DaysRS) m_fShow30DaysRS = false;
	else m_fShow30DaysRS = true;
}

void CFireBirdView::OnShowRs60() {
	if (m_fShow60DaysRS) m_fShow60DaysRS = false;
	else m_fShow60DaysRS = true;
}

void CFireBirdView::OnShowRs120() {
	if (m_fShow120DaysRS) m_fShow120DaysRS = false;
	else m_fShow120DaysRS = true;
}

void CFireBirdView::OnUpdateShowRs10(CCmdUI* pCmdUI) {
	if (m_fShow10DaysRS) SysCallCmdUISetCheck(pCmdUI, 1);
	else SysCallCmdUISetCheck(pCmdUI, 0);
}

void CFireBirdView::OnUpdateShowRs120(CCmdUI* pCmdUI) {
	if (m_fShow120DaysRS) SysCallCmdUISetCheck(pCmdUI, 1);
	else SysCallCmdUISetCheck(pCmdUI, 0);
}

void CFireBirdView::OnUpdateShowRs3(CCmdUI* pCmdUI) {
	if (m_fShow3DaysRS) SysCallCmdUISetCheck(pCmdUI, 1);
	else SysCallCmdUISetCheck(pCmdUI, 0);
}

void CFireBirdView::OnUpdateShowRs30(CCmdUI* pCmdUI) {
	if (m_fShow30DaysRS) SysCallCmdUISetCheck(pCmdUI, 1);
	else SysCallCmdUISetCheck(pCmdUI, 0);
}

void CFireBirdView::OnUpdateShowRs5(CCmdUI* pCmdUI) {
	if (m_fShow5DaysRS) SysCallCmdUISetCheck(pCmdUI, 1);
	else SysCallCmdUISetCheck(pCmdUI, 0);
}

void CFireBirdView::OnUpdateShowRs60(CCmdUI* pCmdUI) {
	if (m_fShow60DaysRS) SysCallCmdUISetCheck(pCmdUI, 1);
	else SysCallCmdUISetCheck(pCmdUI, 0);
}

void CFireBirdView::OnShowRsInLogarithm() {
	if (m_iShowRSOption != 2) {
		m_iShowRSOption = 2;
		if (IsChinaStock(GetCurrentStock())) {
			auto pStock = dynamic_pointer_cast<CChinaStock>(GetCurrentStock());
			pStock->CalculateDayLineRSLogarithm();
			pStock->CalculateDayLineRSLogarithm();
		}
	}
}

void CFireBirdView::OnUpdateShowRsInLogarithm(CCmdUI* pCmdUI) {
	if (IsChinaStock(GetCurrentStock())) {
		SysCallCmdUIEnable(pCmdUI, true);
		if (m_iShowRSOption == 2) SysCallCmdUISetCheck(pCmdUI, 1);
		else SysCallCmdUISetCheck(pCmdUI, 0);
	}
	else {
		SysCallCmdUIEnable(pCmdUI, false);
	}
}

void CFireBirdView::OnShowRsInLinear() {
	if (m_iShowRSOption != 1) {
		m_iShowRSOption = 1;
		if (IsChinaStock(GetCurrentStock())) {
			CChinaStockPtr pStock = dynamic_pointer_cast<CChinaStock>(GetCurrentStock());
			pStock->CalculateDayLineRS();
			pStock->CalculateWeekLineRS();
		}
	}
}

void CFireBirdView::OnUpdateShowRsInLinear(CCmdUI* pCmdUI) {
	if (IsChinaStock(GetCurrentStock())) {
		SysCallCmdUIEnable(pCmdUI, true);
		if (m_iShowRSOption == 1) SysCallCmdUISetCheck(pCmdUI, 1);
		else SysCallCmdUISetCheck(pCmdUI, 0);
	}
	else {
		SysCallCmdUIEnable(pCmdUI, false);
	}
}

void CFireBirdView::OnShowRsIndex() {
	if (m_iShowRSOption != 0) {
		m_iShowRSOption = 0;
		if (IsChinaStock(GetCurrentStock())) {
			auto pStock = dynamic_pointer_cast<CChinaStock>(GetCurrentStock());
			pStock->CalculateDayLineRSIndex();
			pStock->CalculateWeekLineRSIndex();
		}
	}
}

void CFireBirdView::OnUpdateShowRsIndex(CCmdUI* pCmdUI) {
	if (IsChinaStock(GetCurrentStock())) {
		SysCallCmdUIEnable(pCmdUI, true);
		if (m_iShowRSOption == 0) SysCallCmdUISetCheck(pCmdUI, 1);
		else SysCallCmdUISetCheck(pCmdUI, 0);
	}
	else {
		SysCallCmdUIEnable(pCmdUI, false);
	}
}

void CFireBirdView::OnShowDayLine() {
	m_iCurrentShowType = _SHOW_DAY_LINE_DATA_;
}

void CFireBirdView::OnUpdateShowDayLine(CCmdUI* pCmdUI) {
	if (m_iCurrentShowType == _SHOW_DAY_LINE_DATA_) SysCallCmdUISetCheck(pCmdUI, 1);
	else SysCallCmdUISetCheck(pCmdUI, 0);
}

void CFireBirdView::OnShowWeekLine() {
	m_iCurrentShowType = _SHOW_WEEK_LINE_DATA_;
}

void CFireBirdView::OnUpdateShowWeekLine(CCmdUI* pCmdUI) {
	if (m_iCurrentShowType == _SHOW_WEEK_LINE_DATA_) SysCallCmdUISetCheck(pCmdUI, 1);
	else SysCallCmdUISetCheck(pCmdUI, 0);
}

void CFireBirdView::OnSetFocus(CWnd* pOldWnd) {
	// TODO: Add your message handler code here
	gl_pCurrentStock = GetDocument()->GetCurrentStock(); // 设置当前被选中的股票

	CView::OnSetFocus(pOldWnd);
}

void CFireBirdView::OnShowMonthLine() {
	m_iCurrentShowType = _SHOW_MONTH_LINE_DATA_;
}

void CFireBirdView::OnUpdateShowMonthLine(CCmdUI* pCmdUI) {
	if (m_iCurrentShowType == _SHOW_MONTH_LINE_DATA_) SysCallCmdUISetCheck(pCmdUI, 1);
	else SysCallCmdUISetCheck(pCmdUI, 0);
}
