
// StockAnalysisView.cpp: CStockAnalysisView 类的实现
//

#include "stdafx.h"
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
END_MESSAGE_MAP()

// CStockAnalysisView 构造/析构

CStockAnalysisView::CStockAnalysisView() noexcept
{
	// TODO: 在此处添加构造代码

}

CStockAnalysisView::~CStockAnalysisView()
{
}

BOOL CStockAnalysisView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	return CView::PreCreateWindow(cs);
}

// CStockAnalysisView 绘图

void CStockAnalysisView::OnDraw(CDC* /*pDC*/)
{
	CStockAnalysisDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: 在此处为本机数据添加绘制代码
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
