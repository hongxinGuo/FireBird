
// StockAnalysisView.cpp: CStockAnalysisView 类的实现
//

#include "stdafx.h"


#include"globedef.h"

#include"Stock.h"
#include"Market.h"


// SHARED_HANDLERS 可以在实现预览、缩略图和搜索筛选器句柄的
// ATL 项目中进行定义，并允许与该项目共享文档代码。
#ifndef SHARED_HANDLERS
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
//  ON_WM_CHAR()
//  ON_WM_KEYUP()
  ON_WM_SIZE()
//  ON_WM_CHAR()
//  ON_WM_KEYUP()
END_MESSAGE_MAP()

// CStockAnalysisView 构造/析构

CStockAnalysisView::CStockAnalysisView() noexcept
{
	// TODO: 在此处添加构造代码
  m_iCurrentShowType = 1; // 显示日线数据
  m_lCurrentPos = 0;

  m_fShowRS = false;
  m_fShow3DayRS = false;;
  m_fShow5DayRS = true;
  m_fShow10DayRS = true;
  m_fShow30DayRS = true;
  m_fShow60DayRS = true;
  m_fShow120DayRS = false;

  m_uIdTimer = 0;

  m_fCreateMemoryDC = false;
}

CStockAnalysisView::~CStockAnalysisView()
{
}


bool CStockAnalysisView::ShowGuaDan(CDC * pDC, CStockPtr pStock, int iXStart, int iYStart, int iYEnd) {

  CString str = _T("abcd");
  CSize sizeText = pDC->GetTextExtent(str);
  int iNumberOfLine = (iYEnd - iYStart) / sizeText.cy;

  long lStartPrice = ((long)(pStock->GetCurrentGuaDanTransactionPrice() * 100) - iNumberOfLine / 2) * 10;
  char buffer[20];
  int j = 0;

  for (int i = iNumberOfLine; i > 0; i--) {
    sprintf_s(buffer, "%7.2f", ((double)lStartPrice + i * 10) / 1000);
    CString strPrice;
    str = buffer;
    long lCurrentPrice = lStartPrice + i * 10;
    
    if (!pStock->HaveGuaDan(lCurrentPrice)) { // 此价位没有挂单
      strPrice = _T("    ----------");
    }
    else {
      sprintf_s(buffer, "    %10d", pStock->GetGuaDan(lCurrentPrice));
      strPrice = buffer;
    }

    str += strPrice;
    pDC->TextOut(iXStart + 10, iYStart + sizeText.cy * j + 10, str);
    j++;
  }

  return true;
}

bool CStockAnalysisView::ShowCurrentTransactionInfo(CDC * pDC, CStockPtr pStock, int iXStart, int iYStart) {
  if (gl_ChinaStockMarket.GetTotalStock() > 0) {
    pStock = gl_ChinaStockMarket.GetStockPtr(0); // 600000
  }
  else return false;


  CString str = _T("abcd");
  CSize sizeText = pDC->GetTextExtent(str);
  char buffer[30];

  //sprintf_s(buffer, "%8.3f", pStock->m_dCurrentGuaDanTransactionPrice);
  str = _T("当前挂单成交价格： ");
  str += buffer;
  pDC->TextOut(iXStart, iYStart + 10, str);

  return true;
}

void CStockAnalysisView::ShowRealtimeStockData(CDC * pdc) {
  char pch[20];
  CString str;
  COLORREF crGreen(RGB(0, 255, 0)), crRed(RGB(255, 0, 0)), crYellow(RGB(255, 255, 0));;
  COLORREF crBefore;
  COLORREF crBlue(RGB(0, 0, 255)), crWhite(RGB(255, 255, 255));
  CPen *ppen = nullptr, penWhite(PS_SOLID, 1, crWhite), penWhite2(PS_SOLID, 2, crWhite), penRed(PS_SOLID, 1, crRed);
  CPoint ptCurrent;

  CStockPtr pStock;

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

  crBefore = pdc->SetBkColor(crYellow);
  pStock = gl_ChinaStockMarket.GetShowStock();

  ppen = pdc->SelectObject(&penRed);
  ptCurrent.x = iTextStart - 5;
  ptCurrent.y = 0;
  pdc->MoveTo(ptCurrent);
  ptCurrent.y = 768;
  pdc->LineTo(ptCurrent);
  ptCurrent.x = iTextStart - 5;
  ptCurrent.y = y1 - 5;
  pdc->MoveTo(ptCurrent);
  ptCurrent.x = 1024;
  pdc->LineTo(ptCurrent);
  ptCurrent.x = iTextStart - 5;
  ptCurrent.y = y6 - 5;
  pdc->MoveTo(ptCurrent);
  ptCurrent.x = 1024;
  pdc->LineTo(ptCurrent);
  ptCurrent.x = iTextStart - 5;
  ptCurrent.y = y9 - 5;
  pdc->MoveTo(ptCurrent);
  ptCurrent.x = 1024;
  pdc->LineTo(ptCurrent);
  ptCurrent.x = iTextStart - 5;
  ptCurrent.y = y12 - 5;
  pdc->MoveTo(ptCurrent);
  ptCurrent.x = 1024;
  pdc->LineTo(ptCurrent);
  ptCurrent.x = iGuadanXBegin;
  ptCurrent.y = 0;
  pdc->MoveTo(ptCurrent);
  ptCurrent.y = 1060;
  pdc->LineTo(ptCurrent);
  pdc->SelectObject(ppen);

  if (gl_ChinaStockMarket.m_pCurrentStock != nullptr) {
    ShowGuaDan(pdc, gl_ChinaStockMarket.m_pCurrentStock, 10, 10, 500);

    //ShowCurrentTransactionInfo(pdc, gl_sMarketm_pCurrentStock, 200, 10);
  }

  pdc->SelectObject(ppen);
}

void CStockAnalysisView::ShowStockDayLine(CDC * pDC)
{
  const COLORREF crBlack(RGB(0, 0, 0)), crGreen(RGB(0, 255, 0)), crRed(RGB(255, 0, 0)), crYellow(RGB(255, 255, 0));
  const COLORREF crBlue(RGB(0, 0, 255)), crWhite(RGB(255, 255, 255));
  CPen *ppen = nullptr, penWhite1(PS_SOLID, 1, crWhite), penWhite2(PS_SOLID, 2, crWhite), penRed1(PS_SOLID, 1, crRed);
  CPen penRed3(PS_SOLID, 3, crRed), penGreen3(PS_SOLID, 3, crGreen), penWhite3(PS_SOLID, 3, crWhite);
  CPen penRed2(PS_SOLID, 2, crRed), penGreen2(PS_SOLID, 2, crGreen);
  CPen penYellow2(PS_SOLID, 2, crYellow), penBlue2(PS_SOLID, 2, crBlue), penBlue3(PS_SOLID, 3, crBlue);
  CPen penYellow1(PS_SOLID, 1, crYellow), penYellow3(PS_SOLID, 3, crYellow);
  CPoint ptCurrent;


  if (gl_ChinaStockMarket.m_pCurrentStock == nullptr) return;
  if (!gl_ChinaStockMarket.m_pCurrentStock->IsDayLineLoaded()) return;
  const long lDayLineNumber = gl_ChinaStockMarket.m_pCurrentStock->m_vDayLine.size();

  const long lXHigh = m_rectClient.bottom / 2;
  const long lXLow = m_rectClient.bottom;
  const long lYEnd = m_rectClient.right;
  long i = 0;
  long y = 0;
  ppen = pDC->SelectObject(&penRed1);
  pDC->MoveTo(m_rectClient.right, m_rectClient.bottom * 3 / 4);
  pDC->LineTo(0, m_rectClient.bottom * 3 / 4);
  // 显示各相对强度
  vector<CDayLinePtr>::iterator it = gl_ChinaStockMarket.m_pCurrentStock->m_vDayLine.end();
  long lDay = 0;

  // 画相对强度
  if (m_fShowRS) {
    pDC->SelectObject(&penWhite1);
    i = 1;
    it--;
    y = m_rectClient.bottom - (*it--)->GetRelativeStrong() * m_rectClient.bottom / 200;
    pDC->MoveTo(m_rectClient.right -1, y);
    for (; it != gl_ChinaStockMarket.m_pCurrentStock->m_vDayLine.begin(); it--) {
      y = m_rectClient.bottom - (*it)->GetRelativeStrong() * m_rectClient.bottom / 200;
      pDC->LineTo(m_rectClient.right - 1 - 3 * i++, y);
      if (3 * i > lDayLineNumber) break;
      if (m_rectClient.right <= 3 * i) break; // 画到窗口左边框为止
    }
  }
  // 画相对强度3日均线
  if (m_fShow3DayRS) {
    pDC->SelectObject(&penYellow2);
    it = gl_ChinaStockMarket.m_pCurrentStock->m_vDayLine.end();
    i = 1;
    it--;
    y = m_rectClient.bottom - (*it--)->m_d3DayRS * m_rectClient.bottom / 200;
    pDC->MoveTo(m_rectClient.right - 1, y);
    for (; it != gl_ChinaStockMarket.m_pCurrentStock->m_vDayLine.begin(); it--) {
      y = m_rectClient.bottom - (*it)->m_d3DayRS * m_rectClient.bottom / 200;
      pDC->LineTo(m_rectClient.right - 1 - 3 * i++, y);
      lDay = (*it)->GetDay();
      if (3 * i > lDayLineNumber) break;
      if (m_rectClient.right <= 3 * i) break; // 画到窗口左边框为止
    }
  }

  // 画相对强度5日均线
  if (m_fShow5DayRS) {
    pDC->SelectObject(&penRed2);
    it = gl_ChinaStockMarket.m_pCurrentStock->m_vDayLine.end();
    i = 1;
    it--;
    y = m_rectClient.bottom - (*it--)->m_d5DayRS * m_rectClient.bottom / 200;
    pDC->MoveTo(m_rectClient.right - 1, y);
    for (; it != gl_ChinaStockMarket.m_pCurrentStock->m_vDayLine.begin(); it--) {
      y = m_rectClient.bottom - (*it)->m_d5DayRS * m_rectClient.bottom / 200;
      pDC->LineTo(m_rectClient.right - 1 - 3 * i, y);
      lDay = (*it)->GetDay();
      i++;
      if (3 * i > lDayLineNumber) break;
      if (m_rectClient.right <= 3 * i) break; // 画到窗口左边框为止
    }
  }

  // 画相对强度10日均线
  if (m_fShow10DayRS) {
    pDC->SelectObject(&penBlue2);
    it = gl_ChinaStockMarket.m_pCurrentStock->m_vDayLine.end();
    i = 1;
    it--;
    y = m_rectClient.bottom - (*it--)->m_d10DayRS * m_rectClient.bottom / 200;
    pDC->MoveTo(m_rectClient.right - 1, y);
    for (; it != gl_ChinaStockMarket.m_pCurrentStock->m_vDayLine.begin(); it--) {
      y = m_rectClient.bottom - (*it)->m_d10DayRS * m_rectClient.bottom / 200;
      pDC->LineTo(m_rectClient.right - 1 - 3 * i, y);
      lDay = (*it)->GetDay();
      i++;
      if (3 * i > lDayLineNumber) break;
      if (m_rectClient.right <= 3 * i) break; // 画到窗口左边框为止
    }
  }

  // 画相对强度30日均线
  if (m_fShow30DayRS) {
    pDC->SelectObject(&penGreen2);
    it = gl_ChinaStockMarket.m_pCurrentStock->m_vDayLine.end();
    i = 1;
    it--;
    y = m_rectClient.bottom - (*it--)->m_d30DayRS * m_rectClient.bottom / 200;
    pDC->MoveTo(m_rectClient.right - 1, y);
    for (; it != gl_ChinaStockMarket.m_pCurrentStock->m_vDayLine.begin(); it--) {
      y = m_rectClient.bottom - (*it)->m_d30DayRS * m_rectClient.bottom / 200;
      pDC->LineTo(m_rectClient.right - 1 - 3 * i, y);
      i++;
      if (3 * i > lDayLineNumber) break;
      if (m_rectClient.right <= 3 * i) break; // 画到窗口左边框为止
    }
  }
  
  // 画相对强度60日均线
  if (m_fShow60DayRS) {
    pDC->SelectObject(&penWhite2);
    it = gl_ChinaStockMarket.m_pCurrentStock->m_vDayLine.end();
    i = 1;
    it--;
    y = m_rectClient.bottom - (*it--)->m_d60DayRS * m_rectClient.bottom / 200;
    pDC->MoveTo(m_rectClient.right - 1, y);
    for (; it != gl_ChinaStockMarket.m_pCurrentStock->m_vDayLine.begin(); it--) {
      y = m_rectClient.bottom - (*it)->m_d60DayRS * m_rectClient.bottom / 200;
      pDC->LineTo(m_rectClient.right - 1 - 3 * i, y);
      i++;
      if (3 * i > lDayLineNumber) break;
      if (m_rectClient.right <= 3 * i) break; // 画到窗口左边框为止
    }
  }
  
  // 画相对强度120日均线
  if (m_fShow120DayRS) {
    pDC->SelectObject(&penYellow2);
    it = gl_ChinaStockMarket.m_pCurrentStock->m_vDayLine.end();
    i = 1;
    it--;
    y = m_rectClient.bottom - (*it--)->m_d120DayRS * m_rectClient.bottom / 200;
    pDC->MoveTo(m_rectClient.right - 1, y);
    for (; it != gl_ChinaStockMarket.m_pCurrentStock->m_vDayLine.begin(); it--) {
      y = m_rectClient.bottom - (*it)->m_d120DayRS * m_rectClient.bottom / 200;
      pDC->LineTo(m_rectClient.right - 1 - 3 * i, y);
      i++;
      if (3 * i > lDayLineNumber) break;
      if (m_rectClient.right <= 3 * i) break; // 画到窗口左边框为止
    }
  }

  ////////////////////////////////////////////////////////////////画日线蜡烛线
  long lHigh = 0;
  it = gl_ChinaStockMarket.m_pCurrentStock->m_vDayLine.end();
  it--;
  i = 0;
  long lLow = (*it)->GetLow();
  for (; it != gl_ChinaStockMarket.m_pCurrentStock->m_vDayLine.begin(); it--) {
    if (lHigh < (*it)->GetHigh()) lHigh = (*it)->GetHigh();
    if ((*it)->GetLow() > 0) {
      if (lLow > (*it)->GetLow()) lLow = (*it)->GetLow();
    }
    if (3 * i > lDayLineNumber) break;
    if (m_rectClient.right <= 3 * i) break; // 画到
    else i++;
  }

  it = gl_ChinaStockMarket.m_pCurrentStock->m_vDayLine.end();
  it--;
  i = 0;
  long x = 0;
  pDC->SelectObject(&penWhite1);
  for (; it != gl_ChinaStockMarket.m_pCurrentStock->m_vDayLine.begin(); it--) {
    x = m_rectClient.right - 2 - i * 3;
    y = (0.5 - (double)((*it)->GetHigh() - lLow) / (2 * (lHigh - lLow))) * m_rectClient.Height();
    pDC->MoveTo(x, y);
    y = (0.5 - (double)((*it)->GetLow() - lLow) / (2 * (lHigh - lLow))) * m_rectClient.Height();
    pDC->LineTo(x, y);
    lDay = (*it)->GetDay();
    i++;
    if (3 * i > lDayLineNumber) break;
    if (m_rectClient.right <= 3 * i) break; // 画到窗口左边框为止
  }
  pDC->SelectObject(ppen);
}



BOOL CStockAnalysisView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	return CView::PreCreateWindow(cs);
}

// CStockAnalysisView 绘图

void CStockAnalysisView::OnDraw(CDC* pdc)
{
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

  // create memory DC
  if (!m_fCreateMemoryDC) {
    m_MemoryDC.CreateCompatibleDC(pdc);
    m_Bitmap.CreateCompatibleBitmap(pdc, 1920, 1200);
    m_MemoryDC.SetBkColor(RGB(0, 0, 0));
    m_MemoryDC.BitBlt(0, 0, 1920, 1200, NULL, 0, 0, BLACKNESS);
    m_fCreateMemoryDC = TRUE;
  }

  CRect rect;
  GetClientRect(&rect);

  switch (m_iCurrentShowType) {
  case 1: // show day line stock data
    if (gl_ChinaStockMarket.GetShowStock() == nullptr) return;
    pOldBitmap = m_MemoryDC.SelectObject(&m_Bitmap);
    m_MemoryDC.BitBlt(0, 0, rect.right, rect.bottom, NULL, 0, 0, BLACKNESS);
    ShowStockDayLine(&m_MemoryDC);
    pdc->BitBlt(0, 0, rect.right, rect.bottom, &m_MemoryDC, 0, 0, SRCCOPY);
    m_MemoryDC.SelectObject(pOldBitmap);
    break;
  case 2:	// show realtime stock data
    pOldBitmap = m_MemoryDC.SelectObject(&m_Bitmap);
    m_MemoryDC.BitBlt(0, 0, rect.right, rect.bottom, NULL, 0, 0, BLACKNESS);
    ShowRealtimeStockData(&m_MemoryDC);
    pdc->BitBlt(0, 0, rect.right, rect.bottom,
      &m_MemoryDC, 0, 0, SRCCOPY);
    m_MemoryDC.SelectObject(pOldBitmap);
    break;
  default:
    break;
  } // switch
}

// CStockAnalysisView 打印

void CStockAnalysisView::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL CStockAnalysisView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 默认准备
	return DoPreparePrinting(pInfo);
}

void CStockAnalysisView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加额外的打印前进行的初始化过程
}

void CStockAnalysisView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加打印后进行的清理过程
}

void CStockAnalysisView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CStockAnalysisView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CStockAnalysisView 诊断

#ifdef _DEBUG
void CStockAnalysisView::AssertValid() const
{
	CView::AssertValid();
}

void CStockAnalysisView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CStockAnalysisDoc* CStockAnalysisView::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CStockAnalysisDoc)));
	return (CStockAnalysisDoc*)m_pDocument;
}
#endif //_DEBUG


// CStockAnalysisView 消息处理程序


void CStockAnalysisView::OnTimer(UINT_PTR nIDEvent)
{
  // TODO: 在此添加消息处理程序代码和/或调用默认值

  CDC* pdc;
  pdc = GetDC();
  
  Show(pdc);

  ReleaseDC(pdc);

  CView::OnTimer(nIDEvent);
}


int CStockAnalysisView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
  if (CView::OnCreate(lpCreateStruct) == -1)
    return -1;

  // TODO:  在此添加您专用的创建代码
  GetClientRect(&m_rectClient);

  m_uIdTimer = SetTimer(3, 1000, nullptr);     // 1000毫秒每次调度，用于显示实时股票数据。
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

void CStockAnalysisView::OnSize(UINT nType, int cx, int cy)
{
  CView::OnSize(nType, cx, cy);

  // TODO: 在此处添加消息处理程序代码
  m_rectClient.right = cx;
  m_rectClient.bottom = cy;
}

