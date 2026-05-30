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

// CFireBirdView

IMPLEMENT_DYNCREATE(CFireBirdView, CView)

BEGIN_MESSAGE_MAP(CFireBirdView, CView)
	// 标准打印命令
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CFireBirdView::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_TIMER()
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_COMMAND(ID_SHOW_DAYLINE, &CFireBirdView::OnShowDayLine)
	ON_UPDATE_COMMAND_UI(ID_SHOW_DAYLINE, &CFireBirdView::OnUpdateShowDayLine)
	ON_COMMAND(ID_SHOW_WEEKLINE, &CFireBirdView::OnShowWeekLine)
	ON_UPDATE_COMMAND_UI(ID_SHOW_WEEKLINE, &CFireBirdView::OnUpdateShowWeekLine)
	ON_WM_SETFOCUS()
	ON_COMMAND(ID_SHOW_MONTH_LINE, &CFireBirdView::OnShowMonthLine)
	ON_UPDATE_COMMAND_UI(ID_SHOW_MONTH_LINE, &CFireBirdView::OnUpdateShowMonthLine)
	ON_WM_MOUSEWHEEL()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONUP()
	ON_COMMAND(ID_SHOW_INDICATOR_KDJ, &CFireBirdView::OnShowIndicatorKdj)
	ON_UPDATE_COMMAND_UI(ID_SHOW_INDICATOR_KDJ, &CFireBirdView::OnUpdateShowIndicatorKdj)
	ON_COMMAND(ID_SHOW_INDICATOR_MACD, &CFireBirdView::OnShowIndicatorMacd)
	ON_UPDATE_COMMAND_UI(ID_SHOW_INDICATOR_MACD, &CFireBirdView::OnUpdateShowIndicatorMacd)
	ON_COMMAND(ID_SHOW_INDICATOR_RSI, &CFireBirdView::OnShowIndicatorRsi)
	ON_UPDATE_COMMAND_UI(ID_SHOW_INDICATOR_RSI, &CFireBirdView::OnUpdateShowIndicatorRsi)
	ON_COMMAND(ID_SHOW_AV5, &CFireBirdView::OnShowAv5)
	ON_UPDATE_COMMAND_UI(ID_SHOW_AV5, &CFireBirdView::OnUpdateShowAv5)
	ON_COMMAND(ID_SHOW_AV10, &CFireBirdView::OnShowAv10)
	ON_UPDATE_COMMAND_UI(ID_SHOW_AV10, &CFireBirdView::OnUpdateShowAv10)
	ON_COMMAND(ID_SHOW_AV30, &CFireBirdView::OnShowAv30)
	ON_UPDATE_COMMAND_UI(ID_SHOW_AV30, &CFireBirdView::OnUpdateShowAv30)
	ON_COMMAND(ID_SHOW_AV60, &CFireBirdView::OnShowAv60)
	ON_UPDATE_COMMAND_UI(ID_SHOW_AV60, &CFireBirdView::OnUpdateShowAv60)
	ON_COMMAND(ID_SHOW_AV120, &CFireBirdView::OnShowAv120)
	ON_UPDATE_COMMAND_UI(ID_SHOW_AV120, &CFireBirdView::OnUpdateShowAv120)
	ON_COMMAND(ID_SHOW_AV250, &CFireBirdView::OnShowAv250)
	ON_UPDATE_COMMAND_UI(ID_SHOW_AV250, &CFireBirdView::OnUpdateShowAv250)
END_MESSAGE_MAP()

// CFireBirdView 构造/析构

CFireBirdView::CFireBirdView() {
	m_iCurrentShowType = SHOW_DAY_LINE_DATA_; // 显示日线数据
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

	m_fShow5Days = false;
	m_fShow10Days = true;
	m_fShow30Days = true;
	m_fShow60Days = false;
	m_fShow120Days = false;
	m_fShow250Days = false;
	m_fShowTransactionGraph = false;

	m_uIdTimer = 0;

	m_fCreateMemoryDC = false;
}

void CFireBirdView::ShowCandleData(CDC* pDC, CRect rectDrawArea) {
	constexpr COLORREF crGreen(RGB(0, 255, 0)), crWhite(RGB(255, 255, 255)),
	                   crRed(RGB(255, 0, 0)), crYellow(RGB(255, 255, 0));
	CPen penGreen1(PS_SOLID, 1, crGreen), penWhite1(PS_SOLID, 1, crWhite), penRed1(PS_SOLID, 1, crRed);
	CPen penYellow(PS_SOLID, 1, crYellow);
	CPen penWhiteDash(PS_DOT, 1, crWhite);

	std::pair<long, long> pairHighLow;
	switch (m_iCurrentShowType) {
	case SHOW_DAY_LINE_DATA_:
		pairHighLow = GetDocument()->GetDayLineHighLow(rectDrawArea.Width() / m_iCandleWidth);
		m_lDayLineHigh = pairHighLow.first;
		m_lDayLineLow = pairHighLow.second;
		GetDocument()->ShowDayLine(pDC, rectDrawArea, m_iCandleWidth, m_lDayLineHigh, m_lDayLineLow);
		if (m_fShow5Days) GetDocument()->ShowDayLine5MovingAverage(pDC, &penRed1, rectDrawArea, m_iCandleWidth, m_lDayLineHigh, m_lDayLineLow);
		if (m_fShow30Days) GetDocument()->ShowDayLine30MovingAverage(pDC, &penYellow, rectDrawArea, m_iCandleWidth, m_lDayLineHigh, m_lDayLineLow);
		if (m_fShow60Days) GetDocument()->ShowDayLine50MovingAverage(pDC, &penWhite1, rectDrawArea, m_iCandleWidth, m_lDayLineHigh, m_lDayLineLow);
		if (m_fShow120Days) GetDocument()->ShowDayLine120MovingAverage(pDC, &penYellow, rectDrawArea, m_iCandleWidth, m_lDayLineHigh, m_lDayLineLow);
		if (m_fShow250Days) GetDocument()->ShowDayLine250MovingAverage(pDC, &penGreen1, rectDrawArea, m_iCandleWidth, m_lDayLineHigh, m_lDayLineLow);

		break;
	case SHOW_WEEK_LINE_DATA_:
		pairHighLow = GetDocument()->GetWeekLineHighLow(rectDrawArea.Width() / m_iCandleWidth);
		m_lWeekLineHigh = pairHighLow.first;
		m_lWeekLineLow = pairHighLow.second;
		GetDocument()->ShowWeekLine(pDC, rectDrawArea, m_iCandleWidth, m_lWeekLineHigh, m_lWeekLineLow);
		if (m_fShow5Days) GetDocument()->ShowWeekLine5MovingAverage(pDC, &penRed1, rectDrawArea, m_iCandleWidth, m_lWeekLineHigh, m_lWeekLineLow);
		if (m_fShow10Days) GetDocument()->ShowWeekLine10MovingAverage(pDC, &penRed1, rectDrawArea, m_iCandleWidth, m_lWeekLineHigh, m_lWeekLineLow);
		if (m_fShow30Days) GetDocument()->ShowWeekLine30MovingAverage(pDC, &penYellow, rectDrawArea, m_iCandleWidth, m_lWeekLineHigh, m_lWeekLineLow);
		if (m_fShow60Days) GetDocument()->ShowWeekLine50MovingAverage(pDC, &penWhite1, rectDrawArea, m_iCandleWidth, m_lWeekLineHigh, m_lWeekLineLow);
		if (m_fShow120Days) GetDocument()->ShowWeekLine120MovingAverage(pDC, &penYellow, rectDrawArea, m_iCandleWidth, m_lWeekLineHigh, m_lWeekLineLow);
		if (m_fShow250Days) GetDocument()->ShowWeekLine250MovingAverage(pDC, &penGreen1, rectDrawArea, m_iCandleWidth, m_lWeekLineHigh, m_lWeekLineLow);
		break;
	case SHOW_MONTH_LINE_DATA_:
		pairHighLow = GetDocument()->GetMonthLineHighLow(rectDrawArea.Width() / m_iCandleWidth);
		m_lMonthLineHigh = pairHighLow.first;
		m_lMonthLineLow = pairHighLow.second;
		GetDocument()->ShowMonthLine(pDC, rectDrawArea, m_iCandleWidth, m_lMonthLineHigh, m_lMonthLineLow);
		if (m_fShow5Days) GetDocument()->ShowMonthLine5MovingAverage(pDC, &penRed1, rectDrawArea, m_iCandleWidth, m_lMonthLineHigh, m_lMonthLineLow);
		if (m_fShow10Days) GetDocument()->ShowMonthLine10MovingAverage(pDC, &penRed1, rectDrawArea, m_iCandleWidth, m_lMonthLineHigh, m_lMonthLineLow);
		if (m_fShow30Days) GetDocument()->ShowMonthLine30MovingAverage(pDC, &penYellow, rectDrawArea, m_iCandleWidth, m_lMonthLineHigh, m_lMonthLineLow);
		if (m_fShow60Days) GetDocument()->ShowMonthLine50MovingAverage(pDC, &penWhite1, rectDrawArea, m_iCandleWidth, m_lMonthLineHigh, m_lMonthLineLow);
		if (m_fShow120Days) GetDocument()->ShowMonthLine120MovingAverage(pDC, &penWhite1, rectDrawArea, m_iCandleWidth, m_lMonthLineHigh, m_lMonthLineLow);
		if (m_fShow250Days) GetDocument()->ShowMonthLine250MovingAverage(pDC, &penGreen1, rectDrawArea, m_iCandleWidth, m_lMonthLineHigh, m_lMonthLineLow);
		break;
	default:
		break;
	}

	// 显示鼠标位置的价格线
	if (m_rectCandle.PtInRect(m_ptMouse)) {
		ShowCross(pDC, m_ptMouse);
	}
}

void CFireBirdView::ShowCross(CDC* pDC, CPoint ptCurrent) {
	constexpr COLORREF crWhite(RGB(255, 255, 255));
	CPen penWhiteDash(PS_SOLID, 1, crWhite);

	// 显示鼠标位置的价格线
	double value = 0;
	long date = 19700101;
	switch (m_iCurrentShowType) {
	case SHOW_DAY_LINE_DATA_:
		value = static_cast<double>(m_rectCandle.Height() - (ptCurrent.y - m_rectCandle.top)) * (m_lDayLineHigh - m_lDayLineLow) / m_rectCandle.Height() + m_lDayLineLow;
		date = GetDocument()->GetDayLineDate((m_rectCandle.Width() + m_iCandleWidth - (ptCurrent.x - m_rectCandle.left)) / m_iCandleWidth);
		break;
	case SHOW_WEEK_LINE_DATA_:
		value = static_cast<double>(m_rectCandle.Height() - (ptCurrent.y - m_rectCandle.top)) * (m_lWeekLineHigh - m_lWeekLineLow) / m_rectCandle.Height() + m_lWeekLineLow;
		date = GetDocument()->GetWeekLineDate((m_rectCandle.Width() + m_iCandleWidth - (ptCurrent.x - m_rectCandle.left)) / m_iCandleWidth);
		break;
	case SHOW_MONTH_LINE_DATA_:
		value = static_cast<double>(m_rectCandle.Height() - (ptCurrent.y - m_rectCandle.top)) * (m_lMonthLineHigh - m_lMonthLineLow) / m_rectCandle.Height() + m_lMonthLineLow;
		date = GetDocument()->GetMonthLineDate((m_rectCandle.Width() + m_iCandleWidth - (ptCurrent.x - m_rectCandle.left)) / m_iCandleWidth);
		break;
	default:
		ASSERT(0);
		break;
	}
	auto strDate = fmt::format("{}-{:02}-{:02},        ", date / 10000, (date % 10000) / 100, date % 100);
	value /= GetDocument()->GetCurrentStock()->GetRatio();
	string strValue = fmt::format("{:.2f}", value);
	CPen* pOldPen = pDC->SelectObject(&penWhiteDash);
	auto oldTextColor = pDC->SetTextColor(RGB(255, 255, 255));
	pDC->SetBkColor(RGB(0, 0, 0));
	wstring wstr = Utf8ToW(strDate + strValue); //Note: Windows GDI TextOutW 使用宽字符
	pDC->TextOutW(0, 0, wstr.c_str(), static_cast<int>(wstr.size()));
	pDC->SetTextColor(oldTextColor);

	// 显示十字
	auto iPrevMode = pDC->SetROP2(R2_NOT);
	pDC->SelectObject(&penWhiteDash);
	pDC->MoveTo(0, ptCurrent.y);
	pDC->LineTo(m_rectCandle.right, ptCurrent.y);

	pDC->MoveTo(ptCurrent.x, 0);
	pDC->LineTo(ptCurrent.x, m_rectClient.bottom);
	pDC->SetROP2(iPrevMode);

	pDC->SelectObject(pOldPen);
}

void CFireBirdView::ShowIndicator(CDC* pDC, CRect rectDrawArea) {
	switch (m_iShowIndicator) {
	case SHOW_INDICATOR_KDJ_:
		ShowIndicatorKDJ(pDC, rectDrawArea);
		break;
	case SHOW_INDICATOR_RSI_:
		ShowIndicatorRSI(pDC, rectDrawArea);
		break;
	case SHOW_INDICATOR_MACD_:
		ShowIndicatorMACD(pDC, rectDrawArea);
		break;
	case SHOW_INDICATOR_BOLL_:
		ShowIndicatorBoll(pDC, rectDrawArea);
		break;
	default:
		break;
	}
}

void CFireBirdView::ShowIndicatorKDJ(CDC* pDC, CRect rectDrawArea) {
	if (GetDocument()->GetCurrentStock() == nullptr) return;

	Show8020Line(pDC, rectDrawArea);
	switch (m_iCurrentShowType) {
	case SHOW_DAY_LINE_DATA_:
		GetDocument()->ShowDayLineKDJ(pDC, m_rectIndicator, m_iCandleWidth);
		break;
	case SHOW_WEEK_LINE_DATA_:
		GetDocument()->ShowWeekLineKDJ(pDC, m_rectIndicator, m_iCandleWidth);
		break;
	case SHOW_MONTH_LINE_DATA_:
		GetDocument()->ShowMonthLineKDJ(pDC, m_rectIndicator, m_iCandleWidth);
		break;
	default:
		break;
	}
}
void CFireBirdView::ShowIndicatorMACD(CDC* pDC, CRect rectDrawArea) {
	if (GetDocument()->GetCurrentStock() == nullptr) return;

	switch (m_iCurrentShowType) {
	case SHOW_DAY_LINE_DATA_:
		GetDocument()->ShowDayLineMACD(pDC, m_rectIndicator, m_iCandleWidth);
		break;
	case SHOW_WEEK_LINE_DATA_:
		GetDocument()->ShowWeekLineMACD(pDC, m_rectIndicator, m_iCandleWidth);
		break;
	case SHOW_MONTH_LINE_DATA_:
		GetDocument()->ShowMonthLineMACD(pDC, m_rectIndicator, m_iCandleWidth);
		break;
	default:
		break;
	}
}

void CFireBirdView::ShowIndicatorRSI(CDC* pDC, CRect rectDrawArea) {
	if (GetDocument()->GetCurrentStock() == nullptr) return;
	Show8020Line(pDC, rectDrawArea);
	switch (m_iCurrentShowType) {
	case SHOW_DAY_LINE_DATA_:
		GetDocument()->ShowDayLineRSI(pDC, m_rectIndicator, m_iCandleWidth);
		break;
	case SHOW_WEEK_LINE_DATA_:
		GetDocument()->ShowWeekLineRSI(pDC, m_rectIndicator, m_iCandleWidth);
		break;
	case SHOW_MONTH_LINE_DATA_:
		GetDocument()->ShowMonthLineRSI(pDC, m_rectIndicator, m_iCandleWidth);
		break;
	default:
		break;
	}
}
void CFireBirdView::ShowIndicatorBoll(CDC* pDC, CRect rectDrawArea) {
	if (GetDocument()->GetCurrentStock() == nullptr) return;
	Show8020Line(pDC, rectDrawArea);
	switch (m_iCurrentShowType) {
	case SHOW_DAY_LINE_DATA_:
		GetDocument()->ShowDayLineBoll(pDC, m_rectIndicator, m_iCandleWidth);
		break;
	case SHOW_WEEK_LINE_DATA_:
		GetDocument()->ShowWeekLineBoll(pDC, m_rectIndicator, m_iCandleWidth);
		break;
	case SHOW_MONTH_LINE_DATA_:
		GetDocument()->ShowMonthLineBoll(pDC, m_rectIndicator, m_iCandleWidth);
		break;
	default:
		break;
	}
}

void CFireBirdView::Show8020Line(CDC* pDC, CRect rectDrawArea) {
	constexpr COLORREF crWhite(RGB(255, 255, 255));
	CPen penWhiteDash(PS_DOT, 1, crWhite);
	CPen* pOldPen = pDC->SelectObject(&penWhiteDash);
	int y20 = rectDrawArea.top + rectDrawArea.Height() * 8 / 10;
	int y80 = rectDrawArea.top + rectDrawArea.Height() * 2 / 10;
	pDC->MoveTo(rectDrawArea.left, y20);
	pDC->LineTo(rectDrawArea.right, y20);
	pDC->MoveTo(rectDrawArea.left, y80);
	pDC->LineTo(rectDrawArea.right, y80);
	pDC->SelectObject(pOldPen);
}

void CFireBirdView::ShowStockHistoryDataLine(CDC* pDC) {
	if (GetDocument()->GetCurrentStock() == nullptr) return;

	ShowCandleData(pDC, m_rectCandle);
	ShowIndicator(pDC, m_rectIndicator);
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

	pDC = GetDC();
	Show(pDC);
	ReleaseDC(pDC);
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
	case SHOW_DAY_LINE_DATA_: // show day line(or week line) stock data
	case SHOW_WEEK_LINE_DATA_:
	case SHOW_MONTH_LINE_DATA_:
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

//void CFireBirdView::OnRButtonUp(UINT /* nFlags */, CPoint point) {
//	ClientToScreen(&point);
//	OnContextMenu(this, point);
//}

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
	bool bDrawAll = false;
	if (GetDocument()->IsRefreshView() && GetDocument()->IsDataReady()) {
		GetDocument()->SetRefreshView(false);
		bDrawAll = true;
	}

	if (m_bDrawAll) {
		m_bDrawAll = false;
		bDrawAll = true;
	}

	if (bDrawAll) {
		CDC* pDC = GetDC();
		Show(pDC);
		ReleaseDC(pDC);
	}

	CView::OnTimer(nIDEvent);
}

int CFireBirdView::OnCreate(LPCREATESTRUCT lpCreateStruct) {
	if (CView::OnCreate(lpCreateStruct) == -1) return -1;

	GetClientRect(&m_rectClient);

	m_rectInformation.left = m_rectClient.left;
	m_rectInformation.top = m_rectClient.top;
	m_rectInformation.right = m_rectClient.right;
	m_rectInformation.bottom = 20;

	m_rectCandle.left = m_rectClient.left;
	m_rectCandle.top = 21;
	m_rectCandle.right = m_rectClient.right;
	m_rectCandle.bottom = m_rectClient.bottom * 3 / 4 - 50;

	m_rectIndicator.left = m_rectClient.left;
	m_rectIndicator.top = m_rectClient.bottom * 3 / 4 + 1;
	m_rectIndicator.right = m_rectClient.right;
	m_rectIndicator.bottom = m_rectClient.bottom;

	m_uIdTimer = SetTimer(3, 200, nullptr); // 200毫秒每次调度，用于显示实时股票数据。
	if (m_uIdTimer == 0) {
		string str;
	}

	return 0;
}

void CFireBirdView::OnSize(UINT nType, int cx, int cy) {
	SysCallOnSize(nType, cx, cy);

	m_rectClient.right = cx;
	m_rectClient.bottom = cy;

	m_rectInformation.left = m_rectClient.left;
	m_rectInformation.top = m_rectClient.top;
	m_rectInformation.right = m_rectClient.right;
	m_rectInformation.bottom = 20;

	m_rectCandle.left = m_rectClient.left;
	m_rectCandle.top = 21;
	m_rectCandle.right = m_rectClient.right;
	m_rectCandle.bottom = m_rectClient.bottom * 3 / 4 - 50;

	m_rectIndicator.left = m_rectClient.left;
	m_rectIndicator.top = m_rectClient.bottom * 3 / 4 + 1;
	m_rectIndicator.right = m_rectClient.right;
	m_rectIndicator.bottom = m_rectClient.bottom;
}

void CFireBirdView::OnShowAV5() {
	if (m_fShow5Days) m_fShow5Days = false;
	else m_fShow5Days = true;
	m_bDrawAll = true;
}

void CFireBirdView::OnShowAV10() {
	if (m_fShow10Days) m_fShow10Days = false;
	else m_fShow10Days = true;
	m_bDrawAll = true;
}

void CFireBirdView::OnShowAV30() {
	if (m_fShow30Days) m_fShow30Days = false;
	else m_fShow30Days = true;
	m_bDrawAll = true;
}

void CFireBirdView::OnShowAV60() {
	if (m_fShow60Days) m_fShow60Days = false;
	else m_fShow60Days = true;
	m_bDrawAll = true;
}

void CFireBirdView::OnShowAV120() {
	if (m_fShow120Days) m_fShow120Days = false;
	else m_fShow120Days = true;
	m_bDrawAll = true;
}

void CFireBirdView::OnShowAV250() {
	if (m_fShow250Days) m_fShow250Days = false;
	else m_fShow250Days = true;
	m_bDrawAll = true;
}

void CFireBirdView::OnUpdateShowAV10(CCmdUI* pCmdUI) {
	if (m_fShow10Days) SysCallCmdUISetCheck(pCmdUI, 1);
	else SysCallCmdUISetCheck(pCmdUI, 0);
}

void CFireBirdView::OnUpdateShowAV250(CCmdUI* pCmdUI) {
	if (m_fShow250Days) SysCallCmdUISetCheck(pCmdUI, 1);
	else SysCallCmdUISetCheck(pCmdUI, 0);
}

void CFireBirdView::OnUpdateShowAV30(CCmdUI* pCmdUI) {
	if (m_fShow30Days) SysCallCmdUISetCheck(pCmdUI, 1);
	else SysCallCmdUISetCheck(pCmdUI, 0);
}

void CFireBirdView::OnUpdateShowAV5(CCmdUI* pCmdUI) {
	if (m_fShow5Days) SysCallCmdUISetCheck(pCmdUI, 1);
	else SysCallCmdUISetCheck(pCmdUI, 0);
}

void CFireBirdView::OnUpdateShowAV60(CCmdUI* pCmdUI) {
	if (m_fShow60Days) SysCallCmdUISetCheck(pCmdUI, 1);
	else SysCallCmdUISetCheck(pCmdUI, 0);
}

void CFireBirdView::OnShowDayLine() {
	if (m_iCurrentShowType != SHOW_DAY_LINE_DATA_) m_bDrawAll = true;
	m_iCurrentShowType = SHOW_DAY_LINE_DATA_;
}

void CFireBirdView::OnUpdateShowDayLine(CCmdUI* pCmdUI) {
	if (m_iCurrentShowType == SHOW_DAY_LINE_DATA_) SysCallCmdUISetCheck(pCmdUI, 1);
	else SysCallCmdUISetCheck(pCmdUI, 0);
}

void CFireBirdView::OnShowWeekLine() {
	if (m_iCurrentShowType != SHOW_WEEK_LINE_DATA_) m_bDrawAll = true;
	m_iCurrentShowType = SHOW_WEEK_LINE_DATA_;
}

void CFireBirdView::OnUpdateShowWeekLine(CCmdUI* pCmdUI) {
	if (m_iCurrentShowType == SHOW_WEEK_LINE_DATA_) SysCallCmdUISetCheck(pCmdUI, 1);
	else SysCallCmdUISetCheck(pCmdUI, 0);
}

void CFireBirdView::OnSetFocus(CWnd* pOldWnd) {
	gl_pCurrentStock = GetDocument()->GetCurrentStock(); // 设置当前被选中的股票

	CView::OnSetFocus(pOldWnd);
}

void CFireBirdView::OnShowMonthLine() {
	if (m_iCurrentShowType != SHOW_MONTH_LINE_DATA_) m_bDrawAll = true;
	m_iCurrentShowType = SHOW_MONTH_LINE_DATA_;
}

void CFireBirdView::OnUpdateShowMonthLine(CCmdUI* pCmdUI) {
	if (m_iCurrentShowType == SHOW_MONTH_LINE_DATA_) SysCallCmdUISetCheck(pCmdUI, 1);
	else SysCallCmdUISetCheck(pCmdUI, 0);
}

BOOL CFireBirdView::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt) {
	if (zDelta < 0) { // 向下滚动，缩小K线图
		if (m_iCandleWidth > 1) {
			m_iCandleWidth -= 1;
			m_bDrawAll = true;
		}
	}
	else { // 向上滚动，放大K线图
		if (m_iCandleWidth < 40) {
			m_iCandleWidth += 1;
			m_bDrawAll = true;
		}
	}

	return CView::OnMouseWheel(nFlags, zDelta, pt);
}

void CFireBirdView::OnMouseMove(UINT nFlags, CPoint point) {
	m_ptMouse = point;
	if (GetDocument()->GetCurrentStock() != nullptr) {
		CDC* pDC = GetDC();
		// 擦除旧的十字线
		if (m_bNeedErase) {
			ShowCross(pDC, m_ptMouseOld);
			m_bNeedErase = false;
		}
		// 画新的十字线
		if (m_rectCandle.PtInRect(point)) {
			ASSERT(!m_bNeedErase);
			ShowCross(pDC, point);
			m_bNeedErase = true;
			m_ptMouseOld = point;
		}
		else {
			m_ptMouseOld.x = -1;
			m_ptMouseOld.y = -1;
		}
		ReleaseDC(pDC);
	}

	CView::OnMouseMove(nFlags, point);
}

void CFireBirdView::OnShowIndicatorKdj() {
	if (m_iShowIndicator != SHOW_INDICATOR_KDJ_) {
		m_iShowIndicator = SHOW_INDICATOR_KDJ_;
		InvalidateRect(m_rectIndicator);
	}
}

void CFireBirdView::OnUpdateShowIndicatorKdj(CCmdUI* pCmdUI) {
	if (m_iShowIndicator == SHOW_INDICATOR_KDJ_) SysCallCmdUISetCheck(pCmdUI, 1);
	else SysCallCmdUISetCheck(pCmdUI, 0);
}

void CFireBirdView::OnShowIndicatorMacd() {
	if (m_iShowIndicator != SHOW_INDICATOR_MACD_) {
		m_iShowIndicator = SHOW_INDICATOR_MACD_;
		InvalidateRect(m_rectIndicator);
	}
}

void CFireBirdView::OnUpdateShowIndicatorMacd(CCmdUI* pCmdUI) {
	if (m_iShowIndicator == SHOW_INDICATOR_MACD_) SysCallCmdUISetCheck(pCmdUI, 1);
	else SysCallCmdUISetCheck(pCmdUI, 0);
}

void CFireBirdView::OnShowIndicatorRsi() {
	if (m_iShowIndicator != SHOW_INDICATOR_RSI_) {
		m_iShowIndicator = SHOW_INDICATOR_RSI_;
		InvalidateRect(m_rectIndicator);
	}
}

void CFireBirdView::OnUpdateShowIndicatorRsi(CCmdUI* pCmdUI) {
	if (m_iShowIndicator == SHOW_INDICATOR_RSI_) SysCallCmdUISetCheck(pCmdUI, 1);
	else SysCallCmdUISetCheck(pCmdUI, 0);
}

void CFireBirdView::OnShowAv5() {
	OnShowAV5();
}

void CFireBirdView::OnUpdateShowAv5(CCmdUI* pCmdUI) {
	if (m_fShow5Days) SysCallCmdUISetCheck(pCmdUI, 1);
	else SysCallCmdUISetCheck(pCmdUI, 0);
}

void CFireBirdView::OnShowAv10() {
	OnShowAV10();
}

void CFireBirdView::OnUpdateShowAv10(CCmdUI* pCmdUI) {
	if (m_fShow10Days) SysCallCmdUISetCheck(pCmdUI, 1);
	else SysCallCmdUISetCheck(pCmdUI, 0);
}

void CFireBirdView::OnShowAv30() {
	OnShowAV30();
}

void CFireBirdView::OnUpdateShowAv30(CCmdUI* pCmdUI) {
	if (m_fShow30Days) SysCallCmdUISetCheck(pCmdUI, 1);
	else SysCallCmdUISetCheck(pCmdUI, 0);
}

void CFireBirdView::OnShowAv60() {
	OnShowAV60();
}

void CFireBirdView::OnUpdateShowAv60(CCmdUI* pCmdUI) {
	if (m_fShow60Days) SysCallCmdUISetCheck(pCmdUI, 1);
	else SysCallCmdUISetCheck(pCmdUI, 0);
}

void CFireBirdView::OnShowAv120() {
	OnShowAV120();
}

void CFireBirdView::OnUpdateShowAv120(CCmdUI* pCmdUI) {
	if (m_fShow120Days) SysCallCmdUISetCheck(pCmdUI, 1);
	else SysCallCmdUISetCheck(pCmdUI, 0);
}

void CFireBirdView::OnShowAv250() {
	OnShowAV250();
}

void CFireBirdView::OnUpdateShowAv250(CCmdUI* pCmdUI) {
	if (m_fShow250Days) SysCallCmdUISetCheck(pCmdUI, 1);
	else SysCallCmdUISetCheck(pCmdUI, 0);
}
