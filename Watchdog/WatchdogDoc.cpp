// WatchdogDoc.cpp : implementation of the CWatchdogDoc class
//
module;
#include"pch.h"

// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
//#include "Watchdog.h"
#endif
module Watchdog.Document;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CWatchdogDoc

IMPLEMENT_DYNCREATE(CWatchdogDoc, CDocument)

BEGIN_MESSAGE_MAP(CWatchdogDoc, CDocument)
END_MESSAGE_MAP()

// CWatchdogDoc construction/destruction

CWatchdogDoc::CWatchdogDoc() noexcept {
}

CWatchdogDoc::~CWatchdogDoc() {
}

BOOL CWatchdogDoc::OnNewDocument() {
	if (!CDocument::OnNewDocument())
		return FALSE;

	// (SDI documents will reuse this document)

	return TRUE;
}

// CWatchdogDoc serialization

void CWatchdogDoc::Serialize(CArchive& ar) {
	if (ar.IsStoring()) {
	}
	else {
	}
}

#ifdef SHARED_HANDLERS

// Support for thumbnails
void CWatchdogDoc::OnDrawThumbnail(CDC& dc, LPRECT lprcBounds)
{
	// Modify this code to draw the document's data
	dc.FillSolidRect(lprcBounds, RGB(255, 255, 255));

	CString strText = _T("TODO: implement thumbnail drawing here");
	LOGFONT lf;

	CFont* pDefaultGUIFont = CFont::FromHandle((HFONT) GetStockObject(DEFAULT_GUI_FONT));
	pDefaultGUIFont->GetLogFont(&lf);
	lf.lfHeight = 36;

	CFont fontDraw;
	fontDraw.CreateFontIndirect(&lf);

	CFont* pOldFont = dc.SelectObject(&fontDraw);
	dc.DrawText(strText, lprcBounds, DT_CENTER | DT_WORDBREAK);
	dc.SelectObject(pOldFont);
}

// Support for Search Handlers
void CWatchdogDoc::InitializeSearchContent()
{
	CString strSearchContent;
	// Set search contents from document's data.
	// The content parts should be separated by ";"

	// For example:  strSearchContent = _T("point;rectangle;circle;ole object;");
	SetSearchContent(strSearchContent);
}

void CWatchdogDoc::SetSearchContent(const CString& value)
{
	if (value.IsEmpty())
	{
		RemoveChunk(PKEY_Search_Contents.fmtid, PKEY_Search_Contents.pid);
	}
	else
	{
		CMFCFilterChunkValueImpl *pChunk = nullptr;
		ATLTRY(pChunk = new CMFCFilterChunkValueImpl);
		if (pChunk != nullptr)
		{
			pChunk->SetTextValue(PKEY_Search_Contents, value, CHUNK_TEXT);
			SetChunkValue(pChunk);
		}
	}
}

#endif // SHARED_HANDLERS

// CWatchdogDoc diagnostics

#ifdef _DEBUG
void CWatchdogDoc::AssertValid() const {
	CDocument::AssertValid();
}

void CWatchdogDoc::Dump(CDumpContext& dc) const {
	CDocument::Dump(dc);
}
#endif //_DEBUG

// CWatchdogDoc commands
