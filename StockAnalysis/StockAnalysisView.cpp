// StockAnalysisView.cpp: CStockAnalysisView 类的实现
//

#include "stdafx.h"

#include"globedef.h"

#include"ChinaStake.h"
#include"ChinaMarket.h"

// SHARED_HANDayLineERS 可以在实现预览、缩略图和搜索筛选器句柄的
// ATL 项目中进行定义，并允许与该项目共享文档代码。
#ifndef SHARED_HANDayLineERS
#include "StockAnalysis.h"
#endif

#include "StockAnalysisDoc.h"
#include "StockAnalysisView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CStockAnalysisView

IMPLEMENT_DYNCREATE(CStockAnalysisView, CView)

BEGIN_MESSAGE_MAP(CStockAnalysisView, CView)
  // 标准打印命令
  ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
  ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
  ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CStockAnalysisView::OnFilePrintPreview)
  ON_WM_CONTEXTMENU()
  ON_WM_RBUTTONUP()
  ON_WM_TIMER()
  ON_WM_CREATE()
  ON_WM_SIZE()
  ON_COMMAND(ID_SHOW_RS3, &CStockAnalysisView::OnShowRs3)
  ON_COMMAND(ID_SHOW_RS5, &CStockAnalysisView::OnShowRs5)
  ON_COMMAND(ID_SHOW_RS10, &CStockAnalysisView::OnShowRs10)
  ON_COMMAND(ID_SHOW_RS30, &CStockAnalysisView::OnShowRs30)
  ON_COMMAND(ID_SHOW_RS60, &CStockAnalysisView::OnShowRs60)
  ON_COMMAND(ID_SHOW_RS120, &CStockAnalysisView::OnShowRs120)
  ON_UPDATE_COMMAND_UI(ID_SHOW_RS10, &CStockAnalysisView::OnUpdateShowRs10)
  ON_UPDATE_COMMAND_UI(ID_SHOW_RS120, &CStockAnalysisView::OnUpdateShowRs120)
  ON_UPDATE_COMMAND_UI(ID_SHOW_RS3, &CStockAnalysisView::OnUpdateShowRs3)
  ON_UPDATE_COMMAND_UI(ID_SHOW_RS30, &CStockAnalysisView::OnUpdateShowRs30)
  ON_UPDATE_COMMAND_UI(ID_SHOW_RS5, &CStockAnalysisView::OnUpdateShowRs5)
  ON_UPDATE_COMMAND_UI(ID_SHOW_RS60, &CStockAnalysisView::OnUpdateShowRs60)
  ON_COMMAND(ID_SHOW_RS_IN_LOGARITHM, &CStockAnalysisView::OnShowRsInLogarithm)
  ON_UPDATE_COMMAND_UI(ID_SHOW_RS_IN_LOGARITHM, &CStockAnalysisView::OnUpdateShowRsInLogarithm)
  ON_COMMAND(ID_SHOW_RS_IN_LINEAR, &CStockAnalysisView::OnShowRsInLinear)
  ON_UPDATE_COMMAND_UI(ID_SHOW_RS_IN_LINEAR, &CStockAnalysisView::OnUpdateShowRsInLinear)
  ON_COMMAND(ID_SHOW_RS_INDEX, &CStockAnalysisView::OnShowRsIndex)
  ON_UPDATE_COMMAND_UI(ID_SHOW_RS_INDEX, &CStockAnalysisView::OnUpdateShowRsIndex)
  ON_COMMAND(ID_SHOW_DAYLINE, &CStockAnalysisView::OnShowDayLine)
  ON_UPDATE_COMMAND_UI(ID_SHOW_DAYLINE, &CStockAnalysisView::OnUpdateShowDayLine)
  ON_COMMAND(ID_SHOW_REALTIME, &CStockAnalysisView::OnShowRealTime)
  ON_UPDATE_COMMAND_UI(ID_SHOW_REALTIME, &CStockAnalysisView::OnUpdateShowRealTime)
  ON_COMMAND(ID_SHOW_WEEKLINE, &CStockAnalysisView::OnShowWeekLine)
  ON_UPDATE_COMMAND_UI(ID_SHOW_WEEKLINE, &CStockAnalysisView::OnUpdateShowWeekLine)
END_MESSAGE_MAP()

// CStockAnalysisView 构造/析构

CStockAnalysisView::CStockAnalysisView() {
  // TODO: 在此处添加构造代码
  m_iCurrentShowType = __SHOW_DAY_LINE_DATA__; // 显示日线数据
  if (gl_pChinaStakeMarket->GetCurrentStock() != nullptr) {
    m_pCurrentHistoryDataContainer = gl_pChinaStakeMarket->GetCurrentStock()->GetDayLineContainer();
  }
  m_lCurrentPos = 0;
  m_rectClient.SetRect(CPoint(0, 0), CPoint(0.0));

  m_fShowRS = false;
  m_fShow3DaysRS = false;;
  m_fShow5DaysRS = false;
  m_fShow10DaysRS = true;
  m_fShow30DaysRS = true;
  m_fShow60DaysRS = false;
  m_fShow120DaysRS = false;
  m_iShowRSOption = 0; // 默认值为指数相对强度
  m_fShowTransactionGraph = false;
  m_pCurrentHistoryDataContainer = nullptr;

  m_uIdTimer = 0;

  m_fCreateMemoryDC = false;
}

CStockAnalysisView::~CStockAnalysisView() {
}

bool CStockAnalysisView::ShowGuadan(CDC* pDC, CChinaStockPtr pStake, int iXStart, int iYStart, int iYEnd) {
  CString str = _T("abcd");
  CSize sizeText = SysCallGetTextExtent(pDC, str);
  int iNumberOfLine = (iYEnd - iYStart) / sizeText.cy;

  long lStartPrice = ((long)(pStake->GetCurrentGuadanTransactionPrice() * 100) - iNumberOfLine / 2) * 10;
  char buffer[20];
  int j = 0;

  for (int i = iNumberOfLine; i > 0; i--) {
    sprintf_s(buffer, _T("%7.2f"), ((double)lStartPrice + i * 10) / 1000);
    CString strPrice;
    str = buffer;
    long lCurrentPrice = lStartPrice + i * 10;

    if (!pStake->HaveGuadan(lCurrentPrice)) { // 此价位没有挂单
      strPrice = _T("    ----------");
    }
    else {
      sprintf_s(buffer, "    %10I64d", pStake->GetGuadan(lCurrentPrice));
      strPrice = buffer;
    }

    str += strPrice;
    pDC->TextOut(iXStart + 10, iYStart + sizeText.cy * j + 10, str);
    j++;
  }

  return true;
}

bool CStockAnalysisView::ShowCurrentTransactionInfo(CDC* pDC, CChinaStockPtr pStake, int iXStart, int iYStart) {
  if (gl_pChinaStakeMarket->GetTotalActiveStock() > 0) {
    pStake = gl_pChinaStakeMarket->GetStock(0); // 600000
  }
  else return false;

  CString str = _T("abcd");
  CSize sizeText = pDC->GetTextExtent(str);
  char buffer[30];

  sprintf_s(buffer, _T("%8.3f"), pStake->GetCurrentGuadanTransactionPrice());
  str = _T("当前挂单成交价格： ");
  str += buffer;
  pDC->TextOut(iXStart, iYStart + 10, str);

  return true;
}

void CStockAnalysisView::ShowRealtimeData(CDC* pDC) {
  const int cFirstPosition = 0;
  const int cSecondPosition = cFirstPosition + 200;
  const int cThirdPosition = cSecondPosition + 300;
  CRect rectBuySell(cFirstPosition, 0, cFirstPosition + 150, 400);
  CRect rectOrdinaryBuySell(cSecondPosition, 0, cSecondPosition + 300, 400);
  CRect rectAttackBuySell(cThirdPosition, 0, cThirdPosition + 100, 400);
  CRect rectCanceledBuySell(0, 450, 300, 850);
  if (gl_pChinaStakeMarket->GetCurrentStock() != nullptr) {
    ShowBuySell(pDC, gl_pChinaStakeMarket->GetCurrentStock(), rectBuySell);
    ShowOrdinaryBuySell(pDC, gl_pChinaStakeMarket->GetCurrentStock(), rectOrdinaryBuySell);
    ShowAttackBuySell(pDC, gl_pChinaStakeMarket->GetCurrentStock(), rectAttackBuySell);
    ShowCanceledBuySell(pDC, gl_pChinaStakeMarket->GetCurrentStock(), rectCanceledBuySell);
  }

  //ShowRealtimeGuadan(pDC);
}

void CStockAnalysisView::ShowRealtimeGuadan(CDC* pDC) {
  CString str;
  COLORREF crGreen(RGB(0, 255, 0)), crRed(RGB(255, 0, 0)), crYellow(RGB(255, 255, 0));;
  COLORREF crBefore;
  COLORREF crBlue(RGB(0, 0, 255)), crWhite(RGB(255, 255, 255));
  CPen* ppen = nullptr, penWhite(PS_SOLID, 1, crWhite), penWhite2(PS_SOLID, 2, crWhite), penRed(PS_SOLID, 1, crRed);
  CPoint ptCurrent;

  CChinaStockPtr pStake;
  CChinaStockPtr pCurrentStock = gl_pChinaStakeMarket->GetCurrentStock();

  int iGraphXStart = 60, iGraphXEnd = iGraphXStart + 480, iGraphYStart = 20, iGraphYEnd = iGraphYStart + 300;
  int iGraphYEnd2 = iGraphYEnd + 100;
  int iGuadanXBegin = iGraphXEnd + 80, iGuadanXVolume = iGuadanXBegin + 200, iGuadanXVolume1 = iGuadanXBegin + 100;
  int iGuadanYMiddle = 600;
  int iTextStart = iGuadanXVolume + 70, iTextStart1 = iTextStart + 50, iTextNext = iTextStart1 + 100;
  int iTextNext1 = iTextNext + 50, iTextNext2 = iTextNext1 + 50;
  int iTextStart3 = iTextNext2 + 50, iTextNext3 = iTextStart3 + 60;
  int iPSell = iGraphXEnd, iVSell = iPSell + 100;
  int y0 = 0, y1 = y0 + 30, y2 = y1 + 20, y3 = y2 + 20, y4 = y3 + 20, y5 = y4 + 20, y6 = y5 + 30;
  int y7 = y6 + 20, y8 = y7 + 20, y9 = y8 + 30, y10 = y9 + 20, y11 = y10 + 20;
  int y12 = y11 + 30, y13 = y12 + 20, y14 = y13 + 20;

  crBefore = pDC->SetBkColor(crYellow);
  pStake = gl_pChinaStakeMarket->GetCurrentStock();

  ppen = SysCallSelectObject(pDC, &penRed);
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

    //ShowCurrentTransactionInfo(pdc, pCurrentStock, 200, 10);
  }

  SysCallSelectObject(pDC, ppen);
}

void CStockAnalysisView::ShowBuySell(CDC* pDC, CChinaStockPtr pStake, CRect rectArea) {
  const COLORREF  crGreen(RGB(0, 255, 0)), crRed(RGB(255, 0, 0));
  CPen* ppen = nullptr;
  CPen penRed20(PS_SOLID, 10, crRed), penRed30(PS_SOLID, 30, crRed), penRed40(PS_SOLID, 40, crRed);
  CPen  penGreen20(PS_SOLID, 20, crGreen), penGreen30(PS_SOLID, 30, crGreen), penGreen40(PS_SOLID, 40, crGreen);
  CRect rectTop, rectBottom;
  double base;
  double HalfHeight = rectArea.Height() / 2.0;
  double yBase = rectArea.top + HalfHeight;
  double dRatio = 1.0;

  if (pStake->GetVolume() > 0) base = pStake->GetVolume();
  else return;

  rectTop.bottom = yBase;
  rectBottom.top = yBase + 1;
  rectTop.left = rectBottom.left = rectArea.left;
  rectTop.right = rectBottom.right = rectArea.right;
  rectTop.left = rectArea.left;
  rectTop.right = rectArea.left + 30;
  rectTop.top = yBase - HalfHeight * ((double)pStake->GetUnknownVolume() * dRatio / base);
  SysCallFillSolidRect(pDC, rectTop, crRed);
  rectTop.left = rectArea.left + 31;
  rectTop.right = rectArea.left + 60;
  rectTop.top = yBase - HalfHeight * ((double)pStake->GetOrdinaryBuyVolume() * dRatio / base);
  SysCallFillSolidRect(pDC, rectTop, crRed);
  rectTop.left = rectArea.left + 61;
  rectTop.right = rectArea.left + 90;
  rectTop.top = yBase - HalfHeight * ((double)pStake->GetAttackBuyVolume() * dRatio / base);
  SysCallFillSolidRect(pDC, rectTop, crRed);
  rectTop.left = rectArea.left + 91;
  rectTop.right = rectArea.left + 120;
  rectTop.top = yBase - HalfHeight * ((double)pStake->GetStrongBuyVolume() * dRatio / base);
  SysCallFillSolidRect(pDC, rectTop, crRed);

  rectBottom.top = yBase;
  rectBottom.left = rectArea.left + 31;
  rectBottom.right = rectArea.left + 60;
  rectBottom.bottom = yBase + HalfHeight * ((double)pStake->GetOrdinarySellVolume() * dRatio / base);
  SysCallFillSolidRect(pDC, rectBottom, crGreen);
  rectBottom.left = rectArea.left + 61;
  rectBottom.right = rectArea.left + 90;
  rectBottom.bottom = yBase + HalfHeight * ((double)pStake->GetAttackSellVolume() * dRatio / base);
  SysCallFillSolidRect(pDC, rectBottom, crGreen);
  rectBottom.left = rectArea.left + 91;
  rectBottom.right = rectArea.left + 120;
  rectBottom.bottom = yBase + HalfHeight * ((double)pStake->GetStrongSellVolume() * dRatio / base);
  SysCallFillSolidRect(pDC, rectBottom, crGreen);
}

void CStockAnalysisView::ShowAttackBuySell(CDC* pDC, CChinaStockPtr pStake, CRect rectArea) {
  const COLORREF  crGreen(RGB(0, 255, 0)), crRed(RGB(255, 0, 0));
  CPen* ppen = nullptr;
  CPen penRed20(PS_SOLID, 10, crRed), penRed30(PS_SOLID, 30, crRed), penRed40(PS_SOLID, 40, crRed);
  CPen  penGreen20(PS_SOLID, 20, crGreen), penGreen30(PS_SOLID, 30, crGreen), penGreen40(PS_SOLID, 40, crGreen);
  CRect rectTop, rectBottom;
  double base;
  double HalfHeight = rectArea.Height() / 2.0;
  double yBase = rectArea.top + HalfHeight;
  double dRatio = 4.0;

  if (pStake->GetVolume() > 0) base = pStake->GetVolume();
  else return;

  rectTop.bottom = yBase;
  rectBottom.top = yBase + 1;
  rectTop.left = rectBottom.left = rectArea.left;
  rectTop.right = rectBottom.right = rectArea.right;
  rectTop.left = rectArea.left;
  rectTop.right = rectArea.left + 30;
  rectTop.top = yBase - HalfHeight * ((double)pStake->GetAttackBuyBelow50000() * dRatio / base);
  SysCallFillSolidRect(pDC, rectTop, crRed);
  rectTop.left = rectArea.left + 31;
  rectTop.right = rectArea.left + 60;
  rectTop.top = yBase - HalfHeight * ((double)pStake->GetAttackBuyBelow200000() * dRatio / base);
  SysCallFillSolidRect(pDC, rectTop, crRed);
  rectTop.left = rectArea.left + 61;
  rectTop.right = rectArea.left + 90;
  rectTop.top = yBase - HalfHeight * ((double)pStake->GetAttackBuyAbove200000() * dRatio / base);
  SysCallFillSolidRect(pDC, rectTop, crRed);

  rectBottom.top = yBase;
  rectBottom.left = rectArea.left;
  rectBottom.right = rectArea.left + 30;
  rectBottom.bottom = yBase + HalfHeight * ((double)pStake->GetAttackSellBelow50000() * dRatio / base);
  SysCallFillSolidRect(pDC, rectBottom, crGreen);
  rectBottom.left = rectArea.left + 31;
  rectBottom.right = rectArea.left + 60;
  rectBottom.bottom = yBase + HalfHeight * ((double)pStake->GetAttackSellBelow200000() * dRatio / base);
  SysCallFillSolidRect(pDC, rectBottom, crGreen);
  rectBottom.left = rectArea.left + 61;
  rectBottom.right = rectArea.left + 90;
  rectBottom.bottom = yBase + HalfHeight * ((double)pStake->GetAttackSellAbove200000() * dRatio / base);
  SysCallFillSolidRect(pDC, rectBottom, crGreen);
}

void CStockAnalysisView::ShowOrdinaryBuySell(CDC* pDC, CChinaStockPtr pStake, CRect rectArea) {
  const COLORREF  crGreen(RGB(0, 255, 0)), crRed(RGB(255, 0, 0));
  CPen* ppen = nullptr;
  CPen penRed20(PS_SOLID, 10, crRed), penRed30(PS_SOLID, 30, crRed), penRed40(PS_SOLID, 40, crRed);
  CPen  penGreen20(PS_SOLID, 20, crGreen), penGreen30(PS_SOLID, 30, crGreen), penGreen40(PS_SOLID, 40, crGreen);
  CRect rectTop, rectBottom;
  double base;
  double HalfHeight = rectArea.Height() / 2.0;
  double yBase = rectArea.top + HalfHeight;
  double dRatio = 4.0;

  if (pStake->GetVolume() > 0) base = pStake->GetVolume();
  else return;

  rectTop.bottom = yBase;
  rectBottom.top = yBase + 1;
  rectTop.left = rectBottom.left = rectArea.left;
  rectTop.right = rectBottom.right = rectArea.right;
  rectTop.left = rectArea.left;
  rectTop.right = rectArea.left + 30;
  rectTop.top = yBase - HalfHeight * ((double)pStake->GetOrdinaryBuyVolumeBelow5000() * dRatio / base);
  SysCallFillSolidRect(pDC, rectTop, crRed);
  rectTop.left = rectArea.left + 31;
  rectTop.right = rectArea.left + 60;
  rectTop.top = yBase - HalfHeight * ((double)pStake->GetOrdinaryBuyVolumeBelow10000() * dRatio / base);
  SysCallFillSolidRect(pDC, rectTop, crRed);
  rectTop.left = rectArea.left + 61;
  rectTop.right = rectArea.left + 90;
  rectTop.top = yBase - HalfHeight * ((double)pStake->GetOrdinaryBuyVolumeBelow20000() * dRatio / base);
  SysCallFillSolidRect(pDC, rectTop, crRed);
  rectTop.left = rectArea.left + 91;
  rectTop.right = rectArea.left + 120;
  rectTop.top = yBase - HalfHeight * ((double)pStake->GetOrdinaryBuyVolumeBelow50000() * dRatio / base);
  SysCallFillSolidRect(pDC, rectTop, crRed);
  rectTop.left = rectArea.left + 121;
  rectTop.right = rectArea.left + 150;
  rectTop.top = yBase - HalfHeight * ((double)pStake->GetOrdinaryBuyVolumeBelow100000() * dRatio / base);
  SysCallFillSolidRect(pDC, rectTop, crRed);
  rectTop.left = rectArea.left + 151;
  rectTop.right = rectArea.left + 180;
  rectTop.top = yBase - HalfHeight * ((double)pStake->GetOrdinaryBuyVolumeBelow200000() * dRatio / base);
  SysCallFillSolidRect(pDC, rectTop, crRed);
  rectTop.left = rectArea.left + 181;
  rectTop.right = rectArea.left + 210;
  rectTop.top = yBase - HalfHeight * ((double)pStake->GetOrdinaryBuyVolumeAbove200000() * dRatio / base);
  SysCallFillSolidRect(pDC, rectTop, crRed);

  rectBottom.top = yBase;
  rectBottom.left = rectArea.left;
  rectBottom.right = rectArea.left + 30;
  rectBottom.bottom = yBase + HalfHeight * ((double)pStake->GetOrdinarySellVolumeBelow5000() * dRatio / base);
  SysCallFillSolidRect(pDC, rectBottom, crGreen);
  rectBottom.left = rectArea.left + 31;
  rectBottom.right = rectArea.left + 60;
  rectBottom.bottom = yBase + HalfHeight * ((double)pStake->GetOrdinarySellVolumeBelow10000() * dRatio / base);
  SysCallFillSolidRect(pDC, rectBottom, crGreen);
  rectBottom.left = rectArea.left + 61;
  rectBottom.right = rectArea.left + 90;
  rectBottom.bottom = yBase + HalfHeight * ((double)pStake->GetOrdinarySellVolumeBelow20000() * dRatio / base);
  SysCallFillSolidRect(pDC, rectBottom, crGreen);
  rectBottom.left = rectArea.left + 91;
  rectBottom.right = rectArea.left + 120;
  rectBottom.bottom = yBase + HalfHeight * ((double)pStake->GetOrdinarySellVolumeBelow50000() * dRatio / base);
  SysCallFillSolidRect(pDC, rectBottom, crGreen);
  rectBottom.left = rectArea.left + 121;
  rectBottom.right = rectArea.left + 150;
  rectBottom.bottom = yBase + HalfHeight * ((double)pStake->GetOrdinarySellVolumeBelow100000() * dRatio / base);
  SysCallFillSolidRect(pDC, rectBottom, crGreen);
  rectBottom.left = rectArea.left + 151;
  rectBottom.right = rectArea.left + 180;
  rectBottom.bottom = yBase + HalfHeight * ((double)pStake->GetOrdinarySellVolumeBelow200000() * dRatio / base);
  SysCallFillSolidRect(pDC, rectBottom, crGreen);
  rectBottom.left = rectArea.left + 181;
  rectBottom.right = rectArea.left + 210;
  rectBottom.bottom = yBase + HalfHeight * ((double)pStake->GetOrdinarySellVolumeAbove200000() * dRatio / base);
  SysCallFillSolidRect(pDC, rectBottom, crGreen);
}

void CStockAnalysisView::ShowCanceledBuySell(CDC* pDC, CChinaStockPtr pStake, CRect rectArea) {
  const COLORREF  crGreen(RGB(0, 255, 0)), crRed(RGB(255, 0, 0));
  CPen* ppen = nullptr;
  CPen penRed20(PS_SOLID, 10, crRed), penRed30(PS_SOLID, 30, crRed), penRed40(PS_SOLID, 40, crRed);
  CPen  penGreen20(PS_SOLID, 20, crGreen), penGreen30(PS_SOLID, 30, crGreen), penGreen40(PS_SOLID, 40, crGreen);

  double base;
  double HalfHeight = rectArea.Height() / 2.0;
  double yBase = rectArea.top + HalfHeight;
  double dRatio = 1.0;
  CRect rectTop, rectBottom;

  if ((pStake->GetCanceledBuyVolume() == 0) && (pStake->GetCanceledSellVolume() == 0)) return;
  if ((pStake->GetCanceledBuyVolume() > pStake->GetCanceledSellVolume())) base = pStake->GetCanceledBuyVolume();
  else base = pStake->GetCanceledSellVolume();

  rectTop.bottom = yBase;
  rectBottom.top = yBase - 1;
  rectTop.left = rectBottom.left = rectArea.left;
  rectTop.right = rectBottom.right = rectArea.right;
  rectTop.left = rectArea.left;
  rectTop.right = rectArea.left + 30;
  rectTop.top = yBase - HalfHeight * ((double)pStake->GetCanceledBuyVolumeBelow5000() * dRatio / base);
  SysCallFillSolidRect(pDC, rectTop, crRed);
  rectTop.left = rectArea.left + 31;
  rectTop.right = rectArea.left + 60;
  rectTop.top = yBase - HalfHeight * ((double)pStake->GetCanceledBuyVolumeBelow10000() * dRatio / base);
  SysCallFillSolidRect(pDC, rectTop, crRed);
  rectTop.left = rectArea.left + 61;
  rectTop.right = rectArea.left + 90;
  rectTop.top = yBase - HalfHeight * ((double)pStake->GetCanceledBuyVolumeBelow20000() * dRatio / base);
  SysCallFillSolidRect(pDC, rectTop, crRed);
  rectTop.left = rectArea.left + 91;
  rectTop.right = rectArea.left + 120;
  rectTop.top = yBase - HalfHeight * ((double)pStake->GetCanceledBuyVolumeBelow50000() * dRatio / base);
  SysCallFillSolidRect(pDC, rectTop, crRed);
  rectTop.left = rectArea.left + 121;
  rectTop.right = rectArea.left + 150;
  rectTop.top = yBase - HalfHeight * ((double)pStake->GetCanceledBuyVolumeBelow100000() * dRatio / base);
  SysCallFillSolidRect(pDC, rectTop, crRed);
  rectTop.left = rectArea.left + 151;
  rectTop.right = rectArea.left + 180;
  rectTop.top = yBase - HalfHeight * ((double)pStake->GetCanceledBuyVolumeBelow200000() * dRatio / base);
  SysCallFillSolidRect(pDC, rectTop, crRed);
  rectTop.left = rectArea.left + 181;
  rectTop.right = rectArea.left + 210;
  rectTop.top = yBase - HalfHeight * ((double)pStake->GetCanceledBuyVolumeAbove200000() * dRatio / base);
  SysCallFillSolidRect(pDC, rectTop, crRed);

  rectBottom.top = yBase;
  rectBottom.left = rectArea.left;
  rectBottom.right = rectArea.left + 30;
  rectBottom.bottom = yBase + HalfHeight * (pStake->GetCanceledSellVolumeBelow5000() * dRatio / base);
  SysCallFillSolidRect(pDC, rectBottom, crGreen);
  rectBottom.left = rectArea.left + 31;
  rectBottom.right = rectArea.left + 60;
  rectBottom.bottom = yBase + HalfHeight * (pStake->GetCanceledSellVolumeBelow10000() * dRatio / base);
  SysCallFillSolidRect(pDC, rectBottom, crGreen);
  rectBottom.left = rectArea.left + 61;
  rectBottom.right = rectArea.left + 90;
  rectBottom.bottom = yBase + HalfHeight * (pStake->GetCanceledSellVolumeBelow20000() * dRatio / base);
  SysCallFillSolidRect(pDC, rectBottom, crGreen);
  rectBottom.left = rectArea.left + 91;
  rectBottom.right = rectArea.left + 120;
  rectBottom.bottom = yBase + HalfHeight * (pStake->GetCanceledSellVolumeBelow50000() * dRatio / base);
  SysCallFillSolidRect(pDC, rectBottom, crGreen);
  rectBottom.left = rectArea.left + 121;
  rectBottom.right = rectArea.left + 150;
  rectBottom.bottom = yBase + HalfHeight * (pStake->GetCanceledSellVolumeBelow100000() * dRatio / base);
  SysCallFillSolidRect(pDC, rectBottom, crGreen);
  rectBottom.left = rectArea.left + 151;
  rectBottom.right = rectArea.left + 180;
  rectBottom.bottom = yBase + HalfHeight * (pStake->GetCanceledSellVolumeBelow200000() * dRatio / base);
  SysCallFillSolidRect(pDC, rectBottom, crGreen);
  rectBottom.left = rectArea.left + 181;
  rectBottom.right = rectArea.left + 210;
  rectBottom.bottom = yBase + HalfHeight * (pStake->GetCanceledSellVolumeAbove200000() * dRatio / base);
  SysCallFillSolidRect(pDC, rectBottom, crGreen);
}

void CStockAnalysisView::ShowStockHistoryDataLine(CDC* pDC) {
  const COLORREF crBlack(RGB(0, 0, 0)), crGreen(RGB(0, 255, 0)), crRed(RGB(255, 0, 0)), crYellow(RGB(255, 255, 0));
  const COLORREF crBlue(RGB(0, 0, 255)), crWhite(RGB(255, 255, 255));
  CPen* ppen = nullptr;
  CPen penWhite1(PS_SOLID, 1, crWhite), penWhite2(PS_SOLID, 2, crWhite), penWhite3(PS_SOLID, 3, crWhite);
  CPen penRed1(PS_SOLID, 1, crRed), penRed2(PS_SOLID, 2, crRed), penRed3(PS_SOLID, 3, crRed);
  CPen  penGreen1(PS_SOLID, 1, crGreen), penGreen2(PS_SOLID, 2, crGreen), penGreen3(PS_SOLID, 3, crGreen);
  CPen penYellow1(PS_SOLID, 1, crYellow), penYellow2(PS_SOLID, 2, crYellow), penYellow3(PS_SOLID, 3, crYellow);
  CPen penBlue1(PS_SOLID, 1, crBlue), penBlue2(PS_SOLID, 2, crBlue), penBlue3(PS_SOLID, 3, crBlue);
  CPoint ptCurrent;
  CChinaStockPtr pCurrentStock = gl_pChinaStakeMarket->GetCurrentStock();

  if (m_vRSShow.size() != m_pCurrentHistoryDataContainer->GetDataSize()) m_vRSShow.resize(m_pCurrentHistoryDataContainer->GetDataSize());
  if (pCurrentStock == nullptr) return;
  if (!m_pCurrentHistoryDataContainer->IsDataLoaded()) return;

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
    m_pCurrentHistoryDataContainer->GetRSIndex1(m_vRSShow);
    break;
    case 1:
    m_pCurrentHistoryDataContainer->GetRS1(m_vRSShow);
    break;
    case 2:
    m_pCurrentHistoryDataContainer->GetRSLogarithm1(m_vRSShow);
    break;
    default:
    // 错误
    break;
    }
    ShowCurrentRS(pDC, m_vRSShow);
  }
  // 画相对强度3日均线
  if (m_fShow3DaysRS) {
    pDC->SelectObject(&penYellow1);
    m_pCurrentHistoryDataContainer->GetRS3(m_vRSShow);
    ShowCurrentRS(pDC, m_vRSShow);
  }
  // 画相对强度5日均线
  if (m_fShow5DaysRS) {
    pDC->SelectObject(&penGreen1);
    m_pCurrentHistoryDataContainer->GetRS5(m_vRSShow);
    ZoomIn(m_vRSShow, 50, 1.5);
    ShowCurrentRS(pDC, m_vRSShow);
  }
  // 画相对强度10日均线
  if (m_fShow10DaysRS) {
    pDC->SelectObject(&penRed1);
    m_pCurrentHistoryDataContainer->GetRS10(m_vRSShow);
    ZoomIn(m_vRSShow, 50, 3);
    ShowCurrentRS(pDC, m_vRSShow);
  }
  // 画相对强度30日均线
  if (m_fShow30DaysRS) {
    pDC->SelectObject(&penYellow1);
    m_pCurrentHistoryDataContainer->GetRS30(m_vRSShow);
    ZoomIn(m_vRSShow, 50, 3);
    ShowCurrentRS(pDC, m_vRSShow);
  }
  // 画相对强度60日均线
  if (m_fShow60DaysRS) {
    pDC->SelectObject(&penBlue1);
    m_pCurrentHistoryDataContainer->GetRS60(m_vRSShow);
    ZoomIn(m_vRSShow, 50, 6);
    ShowCurrentRS(pDC, m_vRSShow);
  }
  // 画相对强度120日均线
  if (m_fShow120DaysRS) {
    pDC->SelectObject(&penWhite1);
    m_pCurrentHistoryDataContainer->GetRS120(m_vRSShow);
    ZoomIn(m_vRSShow, 50, 6);
    ShowCurrentRS(pDC, m_vRSShow);
  }

  ////////////////////////////////////////////////////////////////画日线蜡烛线
  m_pCurrentHistoryDataContainer->ShowData(pDC, m_rectClient);

  pDC->SelectObject(ppen);
}

void CStockAnalysisView::ShowCurrentRS(CDC* pDC, vector<double>& vRS) {
  vector<double>::iterator it = vRS.end();
  int i = 1;
  it--;
  int y = m_rectClient.bottom - (*it--) * m_rectClient.bottom / 200;
  SysCallMoveTo(pDC, m_rectClient.right - 1, y);
  for (; it != vRS.begin(); it--, i++) {
    if (!RSLineTo(pDC, i, (*it), vRS.size())) break;
  }
}

bool CStockAnalysisView::RSLineTo(CDC* pDC, int i, double dValue, int iSize) {
  int y = m_rectClient.bottom - dValue * m_rectClient.bottom / 200;
  SysCallLineTo(pDC, m_rectClient.right - 1 - 3 * i, y);
  if (3 * i > iSize) return false;
  if (m_rectClient.right <= 3 * i) return false; // 画到窗口左边框为止
  return true;
}

bool CStockAnalysisView::UpdateHistoryDataContainer(CChinaStockPtr pStake) {
  if (pStake != nullptr) {
    switch (m_iCurrentShowType) {
    case __SHOW_DAY_LINE_DATA__:
    m_pCurrentHistoryDataContainer = pStake->GetDayLineContainer();
    break;
    case __SHOW_WEEK_LINE_DATA__:
    m_pCurrentHistoryDataContainer = pStake->GetWeekLineContainer();
    break;
    default:
    m_pCurrentHistoryDataContainer = nullptr;
    break;
    }
  }
  return true;
}

BOOL CStockAnalysisView::PreCreateWindow(CREATESTRUCT& cs) {
  // TODO: 在此处通过修改
  //  CREATESTRUCT cs 来修改窗口类或样式

  return CView::PreCreateWindow(cs);
}

// CStockAnalysisView 绘图

void CStockAnalysisView::OnDraw(CDC* pdc) {
  CStockAnalysisDoc* pDoc = GetDocument();
  ASSERT_VALID(pDoc);
  if (!pDoc)
    return;

  pdc = GetDC();

  Show(pdc);

  ReleaseDC(pdc);
}

void CStockAnalysisView::Show(CDC* pdc) {
  CBitmap* pOldBitmap = nullptr;
  COLORREF crGray(RGB(24, 24, 24));

  // create memory DC
  if (!m_fCreateMemoryDC) {
    m_MemoryDC.CreateCompatibleDC(pdc);
    m_Bitmap.CreateCompatibleBitmap(pdc, 1920, 1200);
    m_MemoryDC.FillSolidRect(0, 0, 1920, 1200, crGray);
    m_fCreateMemoryDC = true;
  }

  CRect rect;
  SysCallGetClientRect(&rect);
  if ((gl_pChinaStakeMarket->GetCurrentStock() != nullptr) && (gl_pChinaStakeMarket->GetCurrentStock()->IsDayLineLoaded())) {
  }
  else {
    pOldBitmap = m_MemoryDC.SelectObject(&m_Bitmap);
    m_MemoryDC.FillSolidRect(0, 0, rect.right, rect.bottom, crGray);
    SysCallBitBlt(pdc, 0, 0, rect.right, rect.bottom, &m_MemoryDC, 0, 0, SRCCOPY);
    if (pOldBitmap != nullptr) m_MemoryDC.SelectObject(pOldBitmap);
    return;
  }

  ASSERT(gl_pChinaStakeMarket->GetCurrentStock() != nullptr);
  ASSERT(gl_pChinaStakeMarket->GetCurrentStock()->IsDayLineLoaded());
  switch (m_iCurrentShowType) {
  case __SHOW_DAY_LINE_DATA__: // show day line(or week line) stake data
  case __SHOW_WEEK_LINE_DATA__:
  pOldBitmap = m_MemoryDC.SelectObject(&m_Bitmap);
  m_MemoryDC.FillSolidRect(0, 0, rect.right, rect.bottom, crGray);
  ShowStockHistoryDataLine(&m_MemoryDC);
  SysCallBitBlt(pdc, 0, 0, rect.right, rect.bottom, &m_MemoryDC, 0, 0, SRCCOPY);
  if (pOldBitmap != nullptr) m_MemoryDC.SelectObject(pOldBitmap);
  break;
  case __SHOW_REAL_TIME_DATA__:	// show realtime stake data
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

// CStockAnalysisView 打印

void CStockAnalysisView::OnFilePrintPreview() {
#ifndef SHARED_HANDayLineERS
  AFXPrintPreview(this);
#endif
}

BOOL CStockAnalysisView::OnPreparePrinting(CPrintInfo* pInfo) {
  // 默认准备
  return DoPreparePrinting(pInfo);
}

void CStockAnalysisView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/) {
  // TODO: 添加额外的打印前进行的初始化过程
}

void CStockAnalysisView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/) {
  // TODO: 添加打印后进行的清理过程
}

void CStockAnalysisView::OnRButtonUp(UINT /* nFlags */, CPoint point) {
  ClientToScreen(&point);
  OnContextMenu(this, point);
}

void CStockAnalysisView::OnContextMenu(CWnd* /* pWnd */, CPoint point) {
#ifndef SHARED_HANDayLineERS
  theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}

// CStockAnalysisView 诊断

#ifdef _DEBUG
void CStockAnalysisView::AssertValid() const {
  CView::AssertValid();
}

void CStockAnalysisView::Dump(CDumpContext& dc) const {
  CView::Dump(dc);
}

CStockAnalysisDoc* CStockAnalysisView::GetDocument() const // 非调试版本是内联的
{
  ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CStockAnalysisDoc)));
  return (CStockAnalysisDoc*)m_pDocument;
}
#endif //_DEBUG

// CStockAnalysisView 消息处理程序

void CStockAnalysisView::OnTimer(UINT_PTR nIDEvent) {
  // TODO: 在此添加消息处理程序代码和/或调用默认值

  CDC* pdc;
  pdc = GetDC();

  Show(pdc);

  ReleaseDC(pdc);

  CView::OnTimer(nIDEvent);
}

int CStockAnalysisView::OnCreate(LPCREATESTRUCT lpCreateStruct) {
  if (CView::OnCreate(lpCreateStruct) == -1)
    return -1;

  // TODO:  在此添加您专用的创建代码
  GetClientRect(&m_rectClient);

  m_uIdTimer = SetTimer(3, 500, nullptr);     // 500毫秒每次调度，用于显示实时股票数据。
  if (m_uIdTimer == 0) {
    CString str;
  }

  return 0;
}

/////////////////////////////////////////////////////////////////////////////////
//
// 处理以下键盘输入:
//   Page Down : 显示下一个股票．
//   Page Up	 : 显示上一个股票．
//   '1'--'9'  : 股票代码．
//   Return		 : 处理当前缓冲区内的股票代码．
//   BackSpace : 删除缓冲区内的一个数字．
//
//////////////////////////////////////////////////////////////////////////////////

void CStockAnalysisView::OnSize(UINT nType, int cx, int cy) {
  SysCallOnSize(nType, cx, cy);

  // TODO: 在此处添加消息处理程序代码
  m_rectClient.right = cx;
  m_rectClient.bottom = cy;
}

void CStockAnalysisView::OnShowRs3() {
  // TODO: Add your command handler code here
  if (m_fShow3DaysRS) m_fShow3DaysRS = false;
  else m_fShow3DaysRS = true;
}

void CStockAnalysisView::OnShowRs5() {
  // TODO: Add your command handler code here
  if (m_fShow5DaysRS) m_fShow5DaysRS = false;
  else m_fShow5DaysRS = true;
}

void CStockAnalysisView::OnShowRs10() {
  // TODO: Add your command handler code here
  if (m_fShow10DaysRS) m_fShow10DaysRS = false;
  else m_fShow10DaysRS = true;
}

void CStockAnalysisView::OnShowRs30() {
  // TODO: Add your command handler code here
  if (m_fShow30DaysRS) m_fShow30DaysRS = false;
  else m_fShow30DaysRS = true;
}

void CStockAnalysisView::OnShowRs60() {
  // TODO: Add your command handler code here
  if (m_fShow60DaysRS) m_fShow60DaysRS = false;
  else m_fShow60DaysRS = true;
}

void CStockAnalysisView::OnShowRs120() {
  // TODO: Add your command handler code here
  if (m_fShow120DaysRS) m_fShow120DaysRS = false;
  else m_fShow120DaysRS = true;
}

void CStockAnalysisView::OnUpdateShowRs10(CCmdUI* pCmdUI) {
  // TODO: Add your command update UI handler code here
  if (m_fShow10DaysRS) SysCallCmdUISetCheck(pCmdUI, 1);
  else SysCallCmdUISetCheck(pCmdUI, 0);
}

void CStockAnalysisView::OnUpdateShowRs120(CCmdUI* pCmdUI) {
  // TODO: Add your command update UI handler code here
  if (m_fShow120DaysRS) SysCallCmdUISetCheck(pCmdUI, 1);
  else SysCallCmdUISetCheck(pCmdUI, 0);
}

void CStockAnalysisView::OnUpdateShowRs3(CCmdUI* pCmdUI) {
  // TODO: Add your command update UI handler code here
  if (m_fShow3DaysRS) SysCallCmdUISetCheck(pCmdUI, 1);
  else SysCallCmdUISetCheck(pCmdUI, 0);
}

void CStockAnalysisView::OnUpdateShowRs30(CCmdUI* pCmdUI) {
  // TODO: Add your command update UI handler code here
  if (m_fShow30DaysRS) SysCallCmdUISetCheck(pCmdUI, 1);
  else SysCallCmdUISetCheck(pCmdUI, 0);
}

void CStockAnalysisView::OnUpdateShowRs5(CCmdUI* pCmdUI) {
  // TODO: Add your command update UI handler code here
  if (m_fShow5DaysRS) SysCallCmdUISetCheck(pCmdUI, 1);
  else SysCallCmdUISetCheck(pCmdUI, 0);
}

void CStockAnalysisView::OnUpdateShowRs60(CCmdUI* pCmdUI) {
  // TODO: Add your command update UI handler code here
  if (m_fShow60DaysRS) SysCallCmdUISetCheck(pCmdUI, 1);
  else SysCallCmdUISetCheck(pCmdUI, 0);
}

void CStockAnalysisView::OnShowRsInLogarithm() {
  // TODO: Add your command handler code here
  if (m_iShowRSOption != 2) {
    m_iShowRSOption = 2;
    if (gl_pChinaStakeMarket->GetCurrentStock() != nullptr) {
      gl_pChinaStakeMarket->GetCurrentStock()->CalculateDayLineRSLogarithm();
      gl_pChinaStakeMarket->GetCurrentStock()->CalculateDayLineRSLogarithm();
    }
  }
}

void CStockAnalysisView::OnUpdateShowRsInLogarithm(CCmdUI* pCmdUI) {
  // TODO: Add your command update UI handler code here
  if (gl_pChinaStakeMarket->GetCurrentStock() == nullptr) {
    SysCallCmdUIEnable(pCmdUI, false);
  }
  else {
    SysCallCmdUIEnable(pCmdUI, true);
    if (m_iShowRSOption == 2) SysCallCmdUISetCheck(pCmdUI, 1);
    else SysCallCmdUISetCheck(pCmdUI, 0);
  }
}

void CStockAnalysisView::OnShowRsInLinear() {
  // TODO: Add your command handler code here
  if (m_iShowRSOption != 1) {
    m_iShowRSOption = 1;
    if (gl_pChinaStakeMarket->GetCurrentStock() != nullptr) {
      gl_pChinaStakeMarket->GetCurrentStock()->CalculateDayLineRS();
      gl_pChinaStakeMarket->GetCurrentStock()->CalculateWeekLineRS();
    }
  }
}

void CStockAnalysisView::OnUpdateShowRsInLinear(CCmdUI* pCmdUI) {
  // TODO: Add your command update UI handler code here
  if (gl_pChinaStakeMarket->GetCurrentStock() == nullptr) {
    SysCallCmdUIEnable(pCmdUI, false);
  }
  else {
    SysCallCmdUIEnable(pCmdUI, true);
    if (m_iShowRSOption == 1) SysCallCmdUISetCheck(pCmdUI, 1);
    else SysCallCmdUISetCheck(pCmdUI, 0);
  }
}

void CStockAnalysisView::OnShowRsIndex() {
  // TODO: Add your command handler code here
  if (m_iShowRSOption != 0) {
    m_iShowRSOption = 0;
    if (gl_pChinaStakeMarket->GetCurrentStock() != nullptr) {
      gl_pChinaStakeMarket->GetCurrentStock()->CalculateDayLineRSIndex();
      gl_pChinaStakeMarket->GetCurrentStock()->CalculateWeekLineRSIndex();
    }
  }
}

void CStockAnalysisView::OnUpdateShowRsIndex(CCmdUI* pCmdUI) {
  // TODO: Add your command update UI handler code here
  if (gl_pChinaStakeMarket->GetCurrentStock() == nullptr) {
    SysCallCmdUIEnable(pCmdUI, false);
  }
  else {
    SysCallCmdUIEnable(pCmdUI, true);
    if (m_iShowRSOption == 0) SysCallCmdUISetCheck(pCmdUI, 1);
    else SysCallCmdUISetCheck(pCmdUI, 0);
  }
}

void CStockAnalysisView::OnShowDayLine() {
  // TODO: Add your command handler code here
  m_iCurrentShowType = __SHOW_DAY_LINE_DATA__;
  if (gl_pChinaStakeMarket->GetCurrentStock() != nullptr) {
    m_pCurrentHistoryDataContainer = gl_pChinaStakeMarket->GetCurrentStock()->GetDayLineContainer();
  }
}

void CStockAnalysisView::OnUpdateShowDayLine(CCmdUI* pCmdUI) {
  // TODO: Add your command update UI handler code here
  if (m_iCurrentShowType == __SHOW_DAY_LINE_DATA__) SysCallCmdUISetCheck(pCmdUI, 1);
  else SysCallCmdUISetCheck(pCmdUI, 0);
}

void CStockAnalysisView::OnShowRealTime() {
  // TODO: Add your command handler code here
  m_iCurrentShowType = __SHOW_REAL_TIME_DATA__;
}

void CStockAnalysisView::OnUpdateShowRealTime(CCmdUI* pCmdUI) {
  // TODO: Add your command update UI handler code here
  if (m_iCurrentShowType == __SHOW_REAL_TIME_DATA__) SysCallCmdUISetCheck(pCmdUI, 1);
  else SysCallCmdUISetCheck(pCmdUI, 0);
}

void CStockAnalysisView::OnShowWeekLine() {
  // TODO: Add your command handler code here
  m_iCurrentShowType = __SHOW_WEEK_LINE_DATA__;
  if (gl_pChinaStakeMarket->GetCurrentStock() != nullptr) {
    m_pCurrentHistoryDataContainer = gl_pChinaStakeMarket->GetCurrentStock()->GetWeekLineContainer();
  }
}

void CStockAnalysisView::OnUpdateShowWeekLine(CCmdUI* pCmdUI) {
  // TODO: Add your command update UI handler code here
  if (m_iCurrentShowType == __SHOW_WEEK_LINE_DATA__) SysCallCmdUISetCheck(pCmdUI, 1);
  else SysCallCmdUISetCheck(pCmdUI, 0);
}