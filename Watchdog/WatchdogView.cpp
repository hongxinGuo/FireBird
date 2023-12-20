// WatchdogView.cpp : implementation of the CWatchdogView class
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "Watchdog.h"
#endif

#include "WatchdogDoc.h"
#include "WatchdogView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CWatchdogView

IMPLEMENT_DYNCREATE(CWatchdogView, CView)

BEGIN_MESSAGE_MAP(CWatchdogView, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CWatchdogView::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
END_MESSAGE_MAP()

// CWatchdogView construction/destruction

CWatchdogView::CWatchdogView() noexcept {
	// 
}

CWatchdogView::~CWatchdogView() {}

BOOL CWatchdogView::PreCreateWindow(CREATESTRUCT& cs) {
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// CWatchdogView drawing

void CWatchdogView::OnDraw(CDC* /*pDC*/) {
	const CWatchdogDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;
}

// CWatchdogView printing

void CWatchdogView::OnFilePrintPreview() {
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL CWatchdogView::OnPreparePrinting(CPrintInfo* pInfo) {
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CWatchdogView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/) {
	//
}

void CWatchdogView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/) {
	//
}

void CWatchdogView::OnRButtonUp(UINT /* nFlags */, CPoint point) {
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CWatchdogView::OnContextMenu(CWnd* /* pWnd */, CPoint point) {
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}

// CWatchdogView diagnostics

#ifdef _DEBUG
void CWatchdogView::AssertValid() const {
	CView::AssertValid();
}

void CWatchdogView::Dump(CDumpContext& dc) const {
	CView::Dump(dc);
}

CWatchdogDoc* CWatchdogView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CWatchdogDoc)));
	return (CWatchdogDoc*)m_pDocument;
}
#endif //_DEBUG

// CWatchdogView message handlers
