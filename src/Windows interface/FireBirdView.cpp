// FireBirdView.cpp: CFireBirdView 类的实现
//

#include"pch.h"

#include"ChinaStock.h"
#include"ChinaMarket.h"

// SHARED_HANDayLineERS 可以在实现预览、缩略图和搜索筛选器句柄的
// ATL 项目中进行定义，并允许与该项目共享文档代码。
#ifndef SHARED_HANDayLineERS
#include "FireBird.h"
#endif

#include "FireBirdDoc.h"
#include "FireBirdView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

void ShowHistoryData(CDC* pDC, CVirtualDataHistoryCandleExtend* pHistoryCandle, CRect rectClient) {
	constexpr COLORREF crGreen(RGB(0, 255, 0)), crWhite(RGB(255, 255, 255)),
	                   crRed(RGB(255, 0, 0));
	CPen penGreen1(PS_SOLID, 1, crGreen), penWhite1(PS_SOLID, 1, crWhite), penRed1(PS_SOLID, 1, crRed);
	long lHigh = 0;
	auto vData = pHistoryCandle->GetDataVector();
	auto it = vData.end();
	--it;
	int i = 0;
	long lLow = (*it)->GetLow();
	for (; it != vData.begin(); --it) {
		if (lHigh < (*it)->GetHigh()) lHigh = (*it)->GetHigh();
		if ((*it)->GetLow() > 0) { if (lLow > (*it)->GetLow()) lLow = (*it)->GetLow(); }
		if (3 * i > vData.size()) break;
		if (rectClient.right <= 3 * i) break; // 画到
		i++;
	}

	it = vData.end();
	--it;
	i = 0;
	pDC->SelectObject(&penRed1);
	for (; it != vData.begin(); --it) {
		const long x = rectClient.right - 2 - i * 3;
		int y = (0.5 - static_cast<double>((*it)->GetHigh() - lLow) / (2 * (lHigh - lLow))) * rectClient.Height();
		pDC->MoveTo(x, y);
		if ((*it)->GetHigh() == (*it)->GetLow()) { y = y - 1; }
		else { y = (0.5 - static_cast<double>((*it)->GetLow() - lLow) / (2 * (lHigh - lLow))) * rectClient.Height(); }
		pDC->LineTo(x, y);
		long lDate = (*it)->GetMarketDate();
		i++;
		if (3 * i > vData.size()) break;
		if (rectClient.right <= 3 * i) break; // 画到窗口左边框为止
	}
}

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
	ON_COMMAND(ID_SHOW_REALTIME, &CFireBirdView::OnShowRealTime)
	ON_UPDATE_COMMAND_UI(ID_SHOW_REALTIME, &CFireBirdView::OnUpdateShowRealTime)
	ON_COMMAND(ID_SHOW_WEEKLINE, &CFireBirdView::OnShowWeekLine)
	ON_UPDATE_COMMAND_UI(ID_SHOW_WEEKLINE, &CFireBirdView::OnUpdateShowWeekLine)
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

bool CFireBirdView::ShowGuadan(CDC* pDC, const CChinaStockPtr& pStock, int iXStart, int iYStart, int iYEnd) {
	string s = _T("");
	const CSize sizeText = SysCallGetTextExtent(pDC, s.c_str());
	const int iNumberOfLine = (iYEnd - iYStart) / sizeText.cy;

	const long lStartPrice = (static_cast<long>(pStock->GetCurrentGuadanTransactionPrice() * 100) - iNumberOfLine / 2) * 10;
	char buffer[20];
	int j = 0;

	for (int i = iNumberOfLine; i > 0; i--) {
		s = fmt::format("{:7.2f}", (static_cast<double>(lStartPrice) + i * 10) / 1000);
		string sPrice;
		const long lCurrentPrice = lStartPrice + i * 10;

		if (!pStock->HaveGuadan(lCurrentPrice)) {
			// 此价位没有挂单
			sPrice = _T("    ----------");
		}
		else {
			sPrice = fmt::format("    {:10Ld}", pStock->GetGuadan(lCurrentPrice));
		}

		s += sPrice;
		pDC->TextOut(iXStart + 10, iYStart + sizeText.cy * j + 10, s.c_str());
		j++;
	}

	return true;
}

bool CFireBirdView::ShowCurrentTransactionInfo(CDC* pDC, int iXStart, int iYStart) {
	CChinaStockPtr pStock;
	if (gl_dataContainerChinaStock.Size() > 0) {
		pStock = gl_dataContainerChinaStock.GetStock(0); // 000001.SS
	}
	else return false;

	string s = fmt::format("当前挂单成交价格：{:8.3f}", pStock->GetCurrentGuadanTransactionPrice());
	pDC->TextOut(iXStart, iYStart + 10, s.c_str());

	return true;
}

void CFireBirdView::ShowRealtimeData(CDC* pDC) {
	const CRect rectVolume(2, 0, 962, 100);
	int cFirstPosition = rectVolume.right + 500;
	int cSecondPosition = cFirstPosition + 200;
	int cThirdPosition = cSecondPosition + 300;
	const CRect rectBuySell(cFirstPosition, 0, cFirstPosition + 150, 400);
	const CRect rectOrdinaryBuySell(cSecondPosition, 0, cSecondPosition + 300, 400);
	const CRect rectAttackBuySell(cThirdPosition, 0, cThirdPosition + 100, 400);
	const CRect rectCanceledBuySell(cFirstPosition, 450, cFirstPosition + 300, 850);

	auto thisStock = gl_pChinaMarket->GetCurrentStock();

	if (thisStock != nullptr) {
		ShowVolume(pDC, thisStock);
		//ShowBuySell(pDC, thisStock, rectBuySell);
		//ShowOrdinaryBuySell(pDC, thisStock, rectOrdinaryBuySell);
		//ShowAttackBuySell(pDC, thisStock, rectAttackBuySell);
		//ShowCanceledBuySell(pDC, thisStock, rectCanceledBuySell);
	}

	//ShowRealtimeGuadan(pDC);
}

void CFireBirdView::ShowVolume(CDC* pDC, const CChinaStockPtr& pStock) {
	auto vVolume = pStock->GetVolumeVector();

	const int index = gl_pChinaMarket->XferMarketTimeToIndex();
	ShowRealtimeVolume(pDC, vVolume, pStock->GetOrdinaryBuyVolumeVector(), index, m_rectOrdinaryBuyVolume, false);
	ShowRealtimeVolume(pDC, vVolume, pStock->GetOrdinarySellVolumeVector(), index, m_rectOrdinarySellVolume, true);
	ShowRealtimeVolume(pDC, vVolume, pStock->GetAttackBuyVolumeVector(), index, m_rectAttackBuyVolume, false);
	ShowRealtimeVolume(pDC, vVolume, pStock->GetAttackSellVolumeVector(), index, m_rectAttackSellVolume, true);
	ShowRealtimeVolume(pDC, vVolume, pStock->GetStrongBuyVolumeVector(), index, m_rectStrongBuyVolume, false);
	ShowRealtimeVolume(pDC, vVolume, pStock->GetStrongSellVolumeVector(), index, m_rectStrongSellVolume, true);
	// Note cancel volume有时会超过当时的成交数量，导致显示越界。暂停使用
	//ShowRealtimeVolume(pDC, vVolume, pStock->GetCancelBuyVolumeVector(), index, m_rectCancelBuyVolume, false);
	//ShowRealtimeVolume(pDC, vVolume, pStock->GetCancelSellVolumeVector(), index, m_rectCancelSellVolume, true);
}

void CFireBirdView::ShowRealtimeGuadan(CDC* pDC) {
	CString str;
	COLORREF crGreen(RGB(0, 255, 0));
	constexpr COLORREF crYellow(RGB(255, 255, 0));
	constexpr COLORREF crRed(RGB(255, 0, 0));
	COLORREF crBlue(RGB(0, 0, 255)), crWhite(RGB(255, 255, 255));
	CPen penWhite(PS_SOLID, 1, crWhite), penWhite2(PS_SOLID, 2, crWhite), penRed(PS_SOLID, 1, crRed);
	CPoint ptCurrent;

	const CChinaStockPtr pCurrentStock = gl_pChinaMarket->GetCurrentStock();

	constexpr int iGraphXStart = 60;
	constexpr int iGraphXEnd = iGraphXStart + 480;
	constexpr int iGraphYStart = 20;
	constexpr int iGraphYEnd = iGraphYStart + 300;
	int iGraphYEnd2 = iGraphYEnd + 100;
	constexpr int iGuadanXBegin = iGraphXEnd + 80;
	constexpr int iGuadanXVolume = iGuadanXBegin + 200;
	int iGuadanXVolume1 = iGuadanXBegin + 100;
	int iGuadanYMiddle = 600;
	constexpr int iTextStart = iGuadanXVolume + 70;
	constexpr int iTextStart1 = iTextStart + 50;
	constexpr int iTextNext = iTextStart1 + 100;
	constexpr int iTextNext1 = iTextNext + 50;
	constexpr int iTextNext2 = iTextNext1 + 50;
	constexpr int iTextStart3 = iTextNext2 + 50;
	int iTextNext3 = iTextStart3 + 60;
	constexpr int iPSell = iGraphXEnd;
	int iVSell = iPSell + 100;
	constexpr int y0 = 0;
	constexpr int y1 = y0 + 30;
	constexpr int y2 = y1 + 20;
	constexpr int y3 = y2 + 20;
	constexpr int y4 = y3 + 20;
	constexpr int y5 = y4 + 20;
	constexpr int y6 = y5 + 30;
	constexpr int y7 = y6 + 20;
	constexpr int y8 = y7 + 20;
	constexpr int y9 = y8 + 30;
	constexpr int y10 = y9 + 20;
	constexpr int y11 = y10 + 20;
	constexpr int y12 = y11 + 30;
	constexpr int y13 = y12 + 20;
	int y14 = y13 + 20;

	COLORREF crBefore = pDC->SetBkColor(crYellow);
	CChinaStockPtr pStock = gl_pChinaMarket->GetCurrentStock();

	CPen* ppen = SysCallSelectObject(pDC, &penRed);
	ptCurrent.x = iTextStart - 5;
	ptCurrent.y = 0;
	SysCallMoveTo(pDC, ptCurrent);
	ptCurrent.y = 768;
	SysCallLineTo(pDC, ptCurrent);
	ptCurrent.x = iTextStart - 5;
	ptCurrent.y = y1 - 5;
	SysCallMoveTo(pDC, ptCurrent);
	ptCurrent.x = 1024;
	SysCallLineTo(pDC, ptCurrent);
	ptCurrent.x = iTextStart - 5;
	ptCurrent.y = y6 - 5;
	SysCallMoveTo(pDC, ptCurrent);
	ptCurrent.x = 1024;
	SysCallLineTo(pDC, ptCurrent);
	ptCurrent.x = iTextStart - 5;
	ptCurrent.y = y9 - 5;
	SysCallMoveTo(pDC, ptCurrent);
	ptCurrent.x = 1024;
	SysCallLineTo(pDC, ptCurrent);
	ptCurrent.x = iTextStart - 5;
	ptCurrent.y = y12 - 5;
	SysCallMoveTo(pDC, ptCurrent);
	ptCurrent.x = 1024;
	SysCallLineTo(pDC, ptCurrent);
	ptCurrent.x = iGuadanXBegin;
	ptCurrent.y = 0;
	SysCallMoveTo(pDC, ptCurrent);
	ptCurrent.y = 1060;
	SysCallLineTo(pDC, ptCurrent);
	SysCallSelectObject(pDC, ppen);

	if (pCurrentStock != nullptr) {
		ShowGuadan(pDC, pCurrentStock, 10, 10, 500);

		//ShowCurrentTransactionInfo(pdc, 200, 10);
	}

	SysCallSelectObject(pDC, ppen);
}

void CFireBirdView::ShowBuySell(CDC* pDC, CChinaStockPtr pStock, CRect rectArea) {
	constexpr COLORREF crGreen(RGB(0, 255, 0)), crRed(RGB(255, 0, 0));
	CPen* ppen = nullptr;
	CPen penRed20(PS_SOLID, 10, crRed), penRed30(PS_SOLID, 30, crRed), penRed40(PS_SOLID, 40, crRed);
	CPen penGreen20(PS_SOLID, 20, crGreen), penGreen30(PS_SOLID, 30, crGreen), penGreen40(PS_SOLID, 40, crGreen);
	CRect rectTop, rectBottom;
	double base;
	double HalfHeight = rectArea.Height() / 2.0;
	double yBase = rectArea.top + HalfHeight;
	double dRatio = 1.0;

	if (pStock->GetVolume() > 0) base = pStock->GetVolume();
	else return;

	rectTop.bottom = yBase;
	rectBottom.top = yBase + 1;
	rectTop.left = rectBottom.left = rectArea.left;
	rectTop.right = rectBottom.right = rectArea.right;
	rectTop.left = rectArea.left;
	rectTop.right = rectArea.left + 30;
	rectTop.top = yBase - HalfHeight * (static_cast<double>(pStock->GetUnknownVolume()) * dRatio / base);
	SysCallFillSolidRect(pDC, rectTop, crRed);
	rectTop.left = rectArea.left + 31;
	rectTop.right = rectArea.left + 60;
	rectTop.top = yBase - HalfHeight * (static_cast<double>(pStock->GetOrdinaryBuyVolume()) * dRatio / base);
	SysCallFillSolidRect(pDC, rectTop, crRed);
	rectTop.left = rectArea.left + 61;
	rectTop.right = rectArea.left + 90;
	rectTop.top = yBase - HalfHeight * (static_cast<double>(pStock->GetAttackBuyVolume()) * dRatio / base);
	SysCallFillSolidRect(pDC, rectTop, crRed);
	rectTop.left = rectArea.left + 91;
	rectTop.right = rectArea.left + 120;
	rectTop.top = yBase - HalfHeight * (static_cast<double>(pStock->GetStrongBuyVolume()) * dRatio / base);
	SysCallFillSolidRect(pDC, rectTop, crRed);

	rectBottom.top = yBase;
	rectBottom.left = rectArea.left + 31;
	rectBottom.right = rectArea.left + 60;
	rectBottom.bottom = yBase + HalfHeight * (static_cast<double>(pStock->GetOrdinarySellVolume()) * dRatio / base);
	SysCallFillSolidRect(pDC, rectBottom, crGreen);
	rectBottom.left = rectArea.left + 61;
	rectBottom.right = rectArea.left + 90;
	rectBottom.bottom = yBase + HalfHeight * (static_cast<double>(pStock->GetAttackSellVolume()) * dRatio / base);
	SysCallFillSolidRect(pDC, rectBottom, crGreen);
	rectBottom.left = rectArea.left + 91;
	rectBottom.right = rectArea.left + 120;
	rectBottom.bottom = yBase + HalfHeight * (static_cast<double>(pStock->GetStrongSellVolume()) * dRatio / base);
	SysCallFillSolidRect(pDC, rectBottom, crGreen);
}

void CFireBirdView::ShowAttackBuySell(CDC* pDC, const CChinaStockPtr& pStock, CRect rectArea) {
	constexpr COLORREF crGreen(RGB(0, 255, 0)), crRed(RGB(255, 0, 0));
	CPen* ppen = nullptr;
	CPen penRed20(PS_SOLID, 10, crRed), penRed30(PS_SOLID, 30, crRed), penRed40(PS_SOLID, 40, crRed);
	CPen penGreen20(PS_SOLID, 20, crGreen), penGreen30(PS_SOLID, 30, crGreen), penGreen40(PS_SOLID, 40, crGreen);
	CRect rectTop, rectBottom;
	double base;
	const double HalfHeight = rectArea.Height() / 2.0;
	const double yBase = rectArea.top + HalfHeight;
	constexpr double dRatio = 4.0;

	if (pStock->GetVolume() > 0) base = pStock->GetVolume();
	else return;

	rectTop.bottom = yBase;
	rectBottom.top = yBase + 1;
	rectTop.left = rectBottom.left = rectArea.left;
	rectTop.right = rectBottom.right = rectArea.right;
	rectTop.left = rectArea.left;
	rectTop.right = rectArea.left + 30;
	rectTop.top = yBase - HalfHeight * (static_cast<double>(pStock->GetAttackBuyBelow50000()) * dRatio / base);
	SysCallFillSolidRect(pDC, rectTop, crRed);
	rectTop.left = rectArea.left + 31;
	rectTop.right = rectArea.left + 60;
	rectTop.top = yBase - HalfHeight * (static_cast<double>(pStock->GetAttackBuyBelow200000()) * dRatio / base);
	SysCallFillSolidRect(pDC, rectTop, crRed);
	rectTop.left = rectArea.left + 61;
	rectTop.right = rectArea.left + 90;
	rectTop.top = yBase - HalfHeight * (static_cast<double>(pStock->GetAttackBuyAbove200000()) * dRatio / base);
	SysCallFillSolidRect(pDC, rectTop, crRed);

	rectBottom.top = yBase;
	rectBottom.left = rectArea.left;
	rectBottom.right = rectArea.left + 30;
	rectBottom.bottom = yBase + HalfHeight * (static_cast<double>(pStock->GetAttackSellBelow50000()) * dRatio / base);
	SysCallFillSolidRect(pDC, rectBottom, crGreen);
	rectBottom.left = rectArea.left + 31;
	rectBottom.right = rectArea.left + 60;
	rectBottom.bottom = yBase + HalfHeight * (static_cast<double>(pStock->GetAttackSellBelow200000()) * dRatio / base);
	SysCallFillSolidRect(pDC, rectBottom, crGreen);
	rectBottom.left = rectArea.left + 61;
	rectBottom.right = rectArea.left + 90;
	rectBottom.bottom = yBase + HalfHeight * (static_cast<double>(pStock->GetAttackSellAbove200000()) * dRatio / base);
	SysCallFillSolidRect(pDC, rectBottom, crGreen);
}

void CFireBirdView::ShowOrdinaryBuySell(CDC* pDC, const CChinaStockPtr& pStock, CRect rectArea) {
	constexpr COLORREF crGreen(RGB(0, 255, 0)), crRed(RGB(255, 0, 0));
	CPen* ppen = nullptr;
	CPen penRed20(PS_SOLID, 10, crRed), penRed30(PS_SOLID, 30, crRed), penRed40(PS_SOLID, 40, crRed);
	CPen penGreen20(PS_SOLID, 20, crGreen), penGreen30(PS_SOLID, 30, crGreen), penGreen40(PS_SOLID, 40, crGreen);
	CRect rectTop, rectBottom;
	double base;
	double HalfHeight = rectArea.Height() / 2.0;
	double yBase = rectArea.top + HalfHeight;
	double dRatio = 4.0;

	if (pStock->GetVolume() > 0) base = pStock->GetVolume();
	else return;

	rectTop.bottom = yBase;
	rectBottom.top = yBase + 1;
	rectTop.left = rectBottom.left = rectArea.left;
	rectTop.right = rectBottom.right = rectArea.right;
	rectTop.left = rectArea.left;
	rectTop.right = rectArea.left + 30;
	rectTop.top = yBase - HalfHeight * (static_cast<double>(pStock->GetOrdinaryBuyVolumeBelow5000()) * dRatio / base);
	SysCallFillSolidRect(pDC, rectTop, crRed);
	rectTop.left = rectArea.left + 31;
	rectTop.right = rectArea.left + 60;
	rectTop.top = yBase - HalfHeight * (static_cast<double>(pStock->GetOrdinaryBuyVolumeBelow10000()) * dRatio / base);
	SysCallFillSolidRect(pDC, rectTop, crRed);
	rectTop.left = rectArea.left + 61;
	rectTop.right = rectArea.left + 90;
	rectTop.top = yBase - HalfHeight * (static_cast<double>(pStock->GetOrdinaryBuyVolumeBelow20000()) * dRatio / base);
	SysCallFillSolidRect(pDC, rectTop, crRed);
	rectTop.left = rectArea.left + 91;
	rectTop.right = rectArea.left + 120;
	rectTop.top = yBase - HalfHeight * (static_cast<double>(pStock->GetOrdinaryBuyVolumeBelow50000()) * dRatio / base);
	SysCallFillSolidRect(pDC, rectTop, crRed);
	rectTop.left = rectArea.left + 121;
	rectTop.right = rectArea.left + 150;
	rectTop.top = yBase - HalfHeight * (static_cast<double>(pStock->GetOrdinaryBuyVolumeBelow100000()) * dRatio / base);
	SysCallFillSolidRect(pDC, rectTop, crRed);
	rectTop.left = rectArea.left + 151;
	rectTop.right = rectArea.left + 180;
	rectTop.top = yBase - HalfHeight * (static_cast<double>(pStock->GetOrdinaryBuyVolumeBelow200000()) * dRatio / base);
	SysCallFillSolidRect(pDC, rectTop, crRed);
	rectTop.left = rectArea.left + 181;
	rectTop.right = rectArea.left + 210;
	rectTop.top = yBase - HalfHeight * (static_cast<double>(pStock->GetOrdinaryBuyVolumeAbove200000()) * dRatio / base);
	SysCallFillSolidRect(pDC, rectTop, crRed);

	rectBottom.top = yBase;
	rectBottom.left = rectArea.left;
	rectBottom.right = rectArea.left + 30;
	rectBottom.bottom = yBase + HalfHeight * (static_cast<double>(pStock->GetOrdinarySellVolumeBelow5000()) * dRatio / base);
	SysCallFillSolidRect(pDC, rectBottom, crGreen);
	rectBottom.left = rectArea.left + 31;
	rectBottom.right = rectArea.left + 60;
	rectBottom.bottom = yBase + HalfHeight * (static_cast<double>(pStock->GetOrdinarySellVolumeBelow10000()) * dRatio / base);
	SysCallFillSolidRect(pDC, rectBottom, crGreen);
	rectBottom.left = rectArea.left + 61;
	rectBottom.right = rectArea.left + 90;
	rectBottom.bottom = yBase + HalfHeight * (static_cast<double>(pStock->GetOrdinarySellVolumeBelow20000()) * dRatio / base);
	SysCallFillSolidRect(pDC, rectBottom, crGreen);
	rectBottom.left = rectArea.left + 91;
	rectBottom.right = rectArea.left + 120;
	rectBottom.bottom = yBase + HalfHeight * (static_cast<double>(pStock->GetOrdinarySellVolumeBelow50000()) * dRatio / base);
	SysCallFillSolidRect(pDC, rectBottom, crGreen);
	rectBottom.left = rectArea.left + 121;
	rectBottom.right = rectArea.left + 150;
	rectBottom.bottom = yBase + HalfHeight * (static_cast<double>(pStock->GetOrdinarySellVolumeBelow100000()) * dRatio / base);
	SysCallFillSolidRect(pDC, rectBottom, crGreen);
	rectBottom.left = rectArea.left + 151;
	rectBottom.right = rectArea.left + 180;
	rectBottom.bottom = yBase + HalfHeight * (static_cast<double>(pStock->GetOrdinarySellVolumeBelow200000()) * dRatio / base);
	SysCallFillSolidRect(pDC, rectBottom, crGreen);
	rectBottom.left = rectArea.left + 181;
	rectBottom.right = rectArea.left + 210;
	rectBottom.bottom = yBase + HalfHeight * (static_cast<double>(pStock->GetOrdinarySellVolumeAbove200000()) * dRatio / base);
	SysCallFillSolidRect(pDC, rectBottom, crGreen);
}

void CFireBirdView::ShowCanceledBuySell(CDC* pDC, const CChinaStockPtr& pStock, CRect rectArea) {
	constexpr COLORREF crGreen(RGB(0, 255, 0)), crRed(RGB(255, 0, 0));
	CPen penRed20(PS_SOLID, 10, crRed), penRed30(PS_SOLID, 30, crRed), penRed40(PS_SOLID, 40, crRed);
	CPen penGreen20(PS_SOLID, 20, crGreen), penGreen30(PS_SOLID, 30, crGreen), penGreen40(PS_SOLID, 40, crGreen);

	double base;
	const double HalfHeight = rectArea.Height() / 2.0;
	const double yBase = rectArea.top + HalfHeight;
	constexpr double dRatio = 1.0;
	CRect rectTop, rectBottom;

	if ((pStock->GetCanceledBuyVolume() == 0) && (pStock->GetCanceledSellVolume() == 0)) return;
	if ((pStock->GetCanceledBuyVolume() > pStock->GetCanceledSellVolume())) base = pStock->GetCanceledBuyVolume();
	else base = pStock->GetCanceledSellVolume();

	rectTop.bottom = yBase;
	rectBottom.top = yBase - 1;
	rectTop.left = rectBottom.left = rectArea.left;
	rectTop.right = rectBottom.right = rectArea.right;
	rectTop.left = rectArea.left;
	rectTop.right = rectArea.left + 30;
	rectTop.top = yBase - HalfHeight * (static_cast<double>(pStock->GetCanceledBuyVolumeBelow5000()) * dRatio / base);
	SysCallFillSolidRect(pDC, rectTop, crRed);
	rectTop.left = rectArea.left + 31;
	rectTop.right = rectArea.left + 60;
	rectTop.top = yBase - HalfHeight * (static_cast<double>(pStock->GetCanceledBuyVolumeBelow10000()) * dRatio / base);
	SysCallFillSolidRect(pDC, rectTop, crRed);
	rectTop.left = rectArea.left + 61;
	rectTop.right = rectArea.left + 90;
	rectTop.top = yBase - HalfHeight * (static_cast<double>(pStock->GetCanceledBuyVolumeBelow20000()) * dRatio / base);
	SysCallFillSolidRect(pDC, rectTop, crRed);
	rectTop.left = rectArea.left + 91;
	rectTop.right = rectArea.left + 120;
	rectTop.top = yBase - HalfHeight * (static_cast<double>(pStock->GetCanceledBuyVolumeBelow50000()) * dRatio / base);
	SysCallFillSolidRect(pDC, rectTop, crRed);
	rectTop.left = rectArea.left + 121;
	rectTop.right = rectArea.left + 150;
	rectTop.top = yBase - HalfHeight * (static_cast<double>(pStock->GetCanceledBuyVolumeBelow100000()) * dRatio / base);
	SysCallFillSolidRect(pDC, rectTop, crRed);
	rectTop.left = rectArea.left + 151;
	rectTop.right = rectArea.left + 180;
	rectTop.top = yBase - HalfHeight * (static_cast<double>(pStock->GetCanceledBuyVolumeBelow200000()) * dRatio / base);
	SysCallFillSolidRect(pDC, rectTop, crRed);
	rectTop.left = rectArea.left + 181;
	rectTop.right = rectArea.left + 210;
	rectTop.top = yBase - HalfHeight * (static_cast<double>(pStock->GetCanceledBuyVolumeAbove200000()) * dRatio / base);
	SysCallFillSolidRect(pDC, rectTop, crRed);

	rectBottom.top = yBase;
	rectBottom.left = rectArea.left;
	rectBottom.right = rectArea.left + 30;
	rectBottom.bottom = yBase + HalfHeight * (pStock->GetCanceledSellVolumeBelow5000() * dRatio / base);
	SysCallFillSolidRect(pDC, rectBottom, crGreen);
	rectBottom.left = rectArea.left + 31;
	rectBottom.right = rectArea.left + 60;
	rectBottom.bottom = yBase + HalfHeight * (pStock->GetCanceledSellVolumeBelow10000() * dRatio / base);
	SysCallFillSolidRect(pDC, rectBottom, crGreen);
	rectBottom.left = rectArea.left + 61;
	rectBottom.right = rectArea.left + 90;
	rectBottom.bottom = yBase + HalfHeight * (pStock->GetCanceledSellVolumeBelow20000() * dRatio / base);
	SysCallFillSolidRect(pDC, rectBottom, crGreen);
	rectBottom.left = rectArea.left + 91;
	rectBottom.right = rectArea.left + 120;
	rectBottom.bottom = yBase + HalfHeight * (pStock->GetCanceledSellVolumeBelow50000() * dRatio / base);
	SysCallFillSolidRect(pDC, rectBottom, crGreen);
	rectBottom.left = rectArea.left + 121;
	rectBottom.right = rectArea.left + 150;
	rectBottom.bottom = yBase + HalfHeight * (pStock->GetCanceledSellVolumeBelow100000() * dRatio / base);
	SysCallFillSolidRect(pDC, rectBottom, crGreen);
	rectBottom.left = rectArea.left + 151;
	rectBottom.right = rectArea.left + 180;
	rectBottom.bottom = yBase + HalfHeight * (pStock->GetCanceledSellVolumeBelow200000() * dRatio / base);
	SysCallFillSolidRect(pDC, rectBottom, crGreen);
	rectBottom.left = rectArea.left + 181;
	rectBottom.right = rectArea.left + 210;
	rectBottom.bottom = yBase + HalfHeight * (pStock->GetCanceledSellVolumeAbove200000() * dRatio / base);
	SysCallFillSolidRect(pDC, rectBottom, crGreen);
}

void CFireBirdView::ShowStockHistoryDataLine(CDC* pDC) {
	constexpr COLORREF crBlack(RGB(0, 0, 0)), crGreen(RGB(0, 255, 0)), crRed(RGB(255, 0, 0)), crYellow(RGB(255, 255, 0));
	constexpr COLORREF crBlue(RGB(0, 0, 255)), crWhite(RGB(255, 255, 255));
	CPen* ppen = nullptr;
	CPen penWhite1(PS_SOLID, 1, crWhite), penWhite2(PS_SOLID, 2, crWhite), penWhite3(PS_SOLID, 3, crWhite);
	CPen penRed1(PS_SOLID, 1, crRed), penRed2(PS_SOLID, 2, crRed), penRed3(PS_SOLID, 3, crRed);
	CPen penGreen1(PS_SOLID, 1, crGreen), penGreen2(PS_SOLID, 2, crGreen), penGreen3(PS_SOLID, 3, crGreen);
	CPen penYellow1(PS_SOLID, 1, crYellow), penYellow2(PS_SOLID, 2, crYellow), penYellow3(PS_SOLID, 3, crYellow);
	CPen penBlue1(PS_SOLID, 1, crBlue), penBlue2(PS_SOLID, 2, crBlue), penBlue3(PS_SOLID, 3, crBlue);
	CPoint ptCurrent;
	const CChinaStockPtr pCurrentStock = gl_pChinaMarket->GetCurrentStock();

	if (pCurrentStock == nullptr) return;
	//当前被操作的历史数据容器
	CVirtualDataHistoryCandleExtend* pHistoryData;
	switch (m_iCurrentShowType) {
	case _SHOW_DAY_LINE_DATA_:
		pHistoryData = pCurrentStock->GetDataChinaDayLine();
		break;
	case _SHOW_WEEK_LINE_DATA_:
		pHistoryData = pCurrentStock->GetDataChinaWeekLine();
		break;
	default:
		pHistoryData = nullptr;
		break;
	}

	if (m_vRSShow.size() != pHistoryData->Size()) m_vRSShow.resize(pHistoryData->Size());

	if (!pHistoryData->IsDataLoaded()) return;

	const long lXHigh = m_rectClient.bottom / 2;
	const long lXLow = m_rectClient.bottom;
	const long lYEnd = m_rectClient.right;
	ppen = pDC->SelectObject(&penRed1);
	SysCallMoveTo(pDC, m_rectClient.right, m_rectClient.bottom * 3 / 4);
	SysCallLineTo(pDC, 0, m_rectClient.bottom * 3 / 4);

	// 画相对强度
	if (m_fShowRS) {
		pDC->SelectObject(&penWhite1);
		switch (m_iShowRSOption) {
		case 0: // 显示相对指数的强度
			pHistoryData->GetRSIndex1(m_vRSShow);
			break;
		case 1:
			pHistoryData->GetRS1(m_vRSShow);
			break;
		case 2:
			pHistoryData->GetRSLogarithm1(m_vRSShow);
			break;
		default:
			// 错误
			break;
		}
		if (m_vRSShow.size() > 0) ShowCurrentRS(pDC, m_vRSShow);
	}
	// 画相对强度3日均线
	if (m_fShow3DaysRS) {
		pDC->SelectObject(&penYellow1);
		pHistoryData->GetRS3(m_vRSShow);
		if (m_vRSShow.size() > 0) ShowCurrentRS(pDC, m_vRSShow);
	}
	// 画相对强度5日均线
	if (m_fShow5DaysRS) {
		pDC->SelectObject(&penGreen1);
		pHistoryData->GetRS5(m_vRSShow);
		ZoomIn(m_vRSShow, 50, 1.5);
		if (m_vRSShow.size() > 0) ShowCurrentRS(pDC, m_vRSShow);
	}
	// 画相对强度10日均线
	if (m_fShow10DaysRS) {
		pDC->SelectObject(&penRed1);
		pHistoryData->GetRS10(m_vRSShow);
		ZoomIn(m_vRSShow, 50, 3);
		if (m_vRSShow.size() > 0) ShowCurrentRS(pDC, m_vRSShow);
	}
	// 画相对强度30日均线
	if (m_fShow30DaysRS) {
		pDC->SelectObject(&penYellow1);
		pHistoryData->GetRS30(m_vRSShow);
		ZoomIn(m_vRSShow, 50, 3);
		if (m_vRSShow.size() > 0) ShowCurrentRS(pDC, m_vRSShow);
	}
	// 画相对强度60日均线
	if (m_fShow60DaysRS) {
		pDC->SelectObject(&penBlue1);
		pHistoryData->GetRS60(m_vRSShow);
		ZoomIn(m_vRSShow, 50, 6);
		if (m_vRSShow.size() > 0) ShowCurrentRS(pDC, m_vRSShow);
	}
	// 画相对强度120日均线
	if (m_fShow120DaysRS) {
		pDC->SelectObject(&penWhite1);
		pHistoryData->GetRS120(m_vRSShow);
		ZoomIn(m_vRSShow, 50, 6);
		if (m_vRSShow.size() > 0) ShowCurrentRS(pDC, m_vRSShow);
	}

	////////////////////////////////////////////////////////////////画日线蜡烛线
	if (pHistoryData->GetDataVector().size() > 0) ShowHistoryData(pDC, pHistoryData, m_rectClient);
	//pHistoryData->ShowData(pDC, m_rectClient);

	pDC->SelectObject(ppen);
}

void CFireBirdView::ShowCurrentRS(CDC* pDC, vector<double>& vRS) {
	auto it = vRS.end();
	int i = 1;
	--it;
	const int y = m_rectClient.bottom - (*it--) * m_rectClient.bottom / 200;
	SysCallMoveTo(pDC, m_rectClient.right - 1, y);
	for (; it != vRS.begin(); --it, i++) {
		if (!RSLineTo(pDC, i, (*it), vRS.size())) break;
	}
}

bool CFireBirdView::RSLineTo(CDC* pDC, int i, double dValue, int iSize) {
	int y = m_rectClient.bottom - dValue * m_rectClient.bottom / 200;
	SysCallLineTo(pDC, m_rectClient.right - 1 - 3 * i, y);
	if (3 * i > iSize) return false;
	if (m_rectClient.right <= 3 * i) return false; // 画到窗口左边框为止
	return true;
}

void CFireBirdView::ZoomIn(vector<double>& vData, double dLevel, double dRate) {
	for (int i = 0; i < vData.size(); i++) {
		double d = dLevel + (vData.at(i) - dLevel) * dRate;
		if (d < 0) vData.at(i) = 0;
		else if (d > 100) vData.at(i) = 100;
		else vData.at(i) = d;
	}
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
	CChinaStockPtr pStock = gl_pChinaMarket->GetCurrentStock();
	if (pStock == nullptr || !pStock->IsDayLineLoaded()) {
		pOldBitmap = m_MemoryDC.SelectObject(&m_Bitmap);
		m_MemoryDC.FillSolidRect(0, 0, rect.right, rect.bottom, crGray);
		SysCallBitBlt(pdc, 0, 0, rect.right, rect.bottom, &m_MemoryDC, 0, 0, SRCCOPY);
		if (pOldBitmap != nullptr) m_MemoryDC.SelectObject(pOldBitmap);
		return;
	}

	ASSERT(pStock != nullptr);
	ASSERT(pStock->IsDayLineLoaded());
	switch (m_iCurrentShowType) {
	case _SHOW_DAY_LINE_DATA_: // show day line(or week line) stock data
	case _SHOW_WEEK_LINE_DATA_:
		pOldBitmap = m_MemoryDC.SelectObject(&m_Bitmap);
		m_MemoryDC.FillSolidRect(0, 0, rect.right, rect.bottom, crGray);
		ShowStockHistoryDataLine(&m_MemoryDC);
		SysCallBitBlt(pdc, 0, 0, rect.right, rect.bottom, &m_MemoryDC, 0, 0, SRCCOPY);
		if (pOldBitmap != nullptr) m_MemoryDC.SelectObject(pOldBitmap);
		break;
	case _SHOW_REAL_TIME_DATA_: // show realtime stock data
		pOldBitmap = m_MemoryDC.SelectObject(&m_Bitmap);
		m_MemoryDC.FillSolidRect(0, 0, rect.right, rect.bottom, crGray);
		ShowRealtimeData(&m_MemoryDC);
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

	m_uIdTimer = SetTimer(3, 500, nullptr); // 500毫秒每次调度，用于显示实时股票数据。
	if (m_uIdTimer == 0) {
		CString str;
	}

	return 0;
}

void CFireBirdView::OnSize(UINT nType, int cx, int cy) {
	SysCallOnSize(nType, cx, cy);

	m_rectClient.right = cx;
	m_rectClient.bottom = cy;
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
		if (gl_pChinaMarket->GetCurrentStock() != nullptr) {
			gl_pChinaMarket->GetCurrentStock()->CalculateDayLineRSLogarithm();
			gl_pChinaMarket->GetCurrentStock()->CalculateDayLineRSLogarithm();
		}
	}
}

void CFireBirdView::OnUpdateShowRsInLogarithm(CCmdUI* pCmdUI) {
	if (gl_pChinaMarket->GetCurrentStock() == nullptr) {
		SysCallCmdUIEnable(pCmdUI, false);
	}
	else {
		SysCallCmdUIEnable(pCmdUI, true);
		if (m_iShowRSOption == 2) SysCallCmdUISetCheck(pCmdUI, 1);
		else SysCallCmdUISetCheck(pCmdUI, 0);
	}
}

void CFireBirdView::OnShowRsInLinear() {
	if (m_iShowRSOption != 1) {
		m_iShowRSOption = 1;
		if (gl_pChinaMarket->GetCurrentStock() != nullptr) {
			gl_pChinaMarket->GetCurrentStock()->CalculateDayLineRS();
			gl_pChinaMarket->GetCurrentStock()->CalculateWeekLineRS();
		}
	}
}

void CFireBirdView::OnUpdateShowRsInLinear(CCmdUI* pCmdUI) {
	if (gl_pChinaMarket->GetCurrentStock() == nullptr) {
		SysCallCmdUIEnable(pCmdUI, false);
	}
	else {
		SysCallCmdUIEnable(pCmdUI, true);
		if (m_iShowRSOption == 1) SysCallCmdUISetCheck(pCmdUI, 1);
		else SysCallCmdUISetCheck(pCmdUI, 0);
	}
}

void CFireBirdView::OnShowRsIndex() {
	if (m_iShowRSOption != 0) {
		m_iShowRSOption = 0;
		if (gl_pChinaMarket->GetCurrentStock() != nullptr) {
			gl_pChinaMarket->GetCurrentStock()->CalculateDayLineRSIndex();
			gl_pChinaMarket->GetCurrentStock()->CalculateWeekLineRSIndex();
		}
	}
}

void CFireBirdView::OnUpdateShowRsIndex(CCmdUI* pCmdUI) {
	if (gl_pChinaMarket->GetCurrentStock() == nullptr) {
		SysCallCmdUIEnable(pCmdUI, false);
	}
	else {
		SysCallCmdUIEnable(pCmdUI, true);
		if (m_iShowRSOption == 0) SysCallCmdUISetCheck(pCmdUI, 1);
		else SysCallCmdUISetCheck(pCmdUI, 0);
	}
}

void CFireBirdView::OnShowDayLine() {
	m_iCurrentShowType = _SHOW_DAY_LINE_DATA_;
}

void CFireBirdView::OnUpdateShowDayLine(CCmdUI* pCmdUI) {
	if (m_iCurrentShowType == _SHOW_DAY_LINE_DATA_) SysCallCmdUISetCheck(pCmdUI, 1);
	else SysCallCmdUISetCheck(pCmdUI, 0);
}

void CFireBirdView::OnShowRealTime() {
	m_iCurrentShowType = _SHOW_REAL_TIME_DATA_;
}

void CFireBirdView::OnUpdateShowRealTime(CCmdUI* pCmdUI) {
	if (m_iCurrentShowType == _SHOW_REAL_TIME_DATA_) SysCallCmdUISetCheck(pCmdUI, 1);
	else SysCallCmdUISetCheck(pCmdUI, 0);
}

void CFireBirdView::OnShowWeekLine() {
	m_iCurrentShowType = _SHOW_WEEK_LINE_DATA_;
}

void CFireBirdView::OnUpdateShowWeekLine(CCmdUI* pCmdUI) {
	if (m_iCurrentShowType == _SHOW_WEEK_LINE_DATA_) SysCallCmdUISetCheck(pCmdUI, 1);
	else SysCallCmdUISetCheck(pCmdUI, 0);
}
