// FireBirdDoc.cpp: CFireBirdDoc 类的实现
//

#include"pch.h"
// SHARED_HANDayLineERS 可以在实现预览、缩略图和搜索筛选器句柄的
// ATL 项目中进行定义，并允许与该项目共享文档代码。
#ifndef SHARED_HANDayLineERS
#endif

#include "FireBirdDoc.h"

#include "Thread.h"

IMPLEMENT_DYNCREATE(CFireBirdDoc, CDocument)

BEGIN_MESSAGE_MAP(CFireBirdDoc, CDocument)
END_MESSAGE_MAP()

// CFireBirdDoc 构造/析构
CFireBirdDoc::CFireBirdDoc() {}

void CFireBirdDoc::SetCurrentStock(const CVirtualStockPtr& pStock) {
	m_pCurrentStock = pStock;
	if (pStock != nullptr) {
		if (!pStock->IsDayLineLoaded() || !pStock->IsWeekLineLoaded()) {
			m_bDataReady = false;
			gl_runtime.thread_executor()->post([this, pStock] {
				pStock->LoadHistoryCandleDB();
				ASSERT(pStock->IsDayLineLoaded());
				ASSERT(pStock->IsWeekLineLoaded());
				CalculateDayLineMovingAverage();
				CalculateWeekLineMovingAverage();
				m_bDataReady = true;
			});
		}
	}
}
void CFireBirdDoc::CalculateDayLineMovingAverage() {
	auto pvDayLine = m_pCurrentStock->DayLine();

	m_dayLine5MovingAverage.Calculate(pvDayLine);
	m_dayLine10MovingAverage.Calculate(pvDayLine);
	m_dayLine30MovingAverage.Calculate(pvDayLine);
	m_dayLine50MovingAverage.Calculate(pvDayLine);
	m_dayLine120MovingAverage.Calculate(pvDayLine);
	m_dayLine250MovingAverage.Calculate(pvDayLine);
}

void CFireBirdDoc::CalculateWeekLineMovingAverage() {
	auto pvWeekLine = m_pCurrentStock->WeekLine();

	m_weekLine5MovingAverage.Calculate(pvWeekLine);
	m_weekLine10MovingAverage.Calculate(pvWeekLine);
	m_weekLine30MovingAverage.Calculate(pvWeekLine);
	m_weekLine50MovingAverage.Calculate(pvWeekLine);
	m_weekLine120MovingAverage.Calculate(pvWeekLine);
	m_weekLine250MovingAverage.Calculate(pvWeekLine);
}

void CFireBirdDoc::CalculateMonthLineMovingAverage() {
	auto pvMonthLine = m_pCurrentStock->MonthLine();

	m_monthLine5MovingAverage.Calculate(pvMonthLine);
	m_monthLine10MovingAverage.Calculate(pvMonthLine);
	m_monthLine30MovingAverage.Calculate(pvMonthLine);
	m_monthLine50MovingAverage.Calculate(pvMonthLine);
	m_monthLine120MovingAverage.Calculate(pvMonthLine);
	m_monthLine250MovingAverage.Calculate(pvMonthLine);
}

BOOL CFireBirdDoc::OnNewDocument() {
	if (!CDocument::OnNewDocument()) return FALSE;

	// (SDI 文档将重用该文档)

	return TRUE;
}

// CFireBirdDoc 序列化

void CFireBirdDoc::Serialize(CArchive& ar) {
	if (ar.IsStoring()) {
	}
	else {
	}
}

#ifdef SHARED_HANDayLineERS

// 缩略图的支持
void CFireBirdDoc::OnDrawThumbnail(CDC& dc, LPRECT lprcBounds) {
  // 修改此代码以绘制文档数据
  dc.FillSolidRect(lprcBounds, RGB(255, 255, 255));

  CString strText = _T("TODO: implement thumbnail drawing here");
  LOGFONT lf;

  CFont* pDefaultGUIFont = CFont::FromHandle((HFONT)GetStockObject(DEFAULT_GUI_FONT));
  pDefaultGUIFont->GetLogFont(&lf);
  lf.lfHeight = 36;

  CFont fontDraw;
  fontDraw.CreateFontIndirect(&lf);

  CFont* pOldFont = dc.SelectObject(&fontDraw);
  dc.DrawText(strText, lprcBounds, DT_CENTER | DT_WORDBREAK);
  dc.SelectObject(pOldFont);
}

// 搜索处理程序的支持
void CFireBirdDoc::InitializeSearchContent() {
  CString strSearchContent;
  // 从文档数据设置搜索内容。
  // 内容部分应由“;”分隔

  // 例如:     strSearchContent = _T("point;rectangle;circle;ole object;")；
  SetSearchContent(strSearchContent);
}

void CFireBirdDoc::SetSearchContent(const CString& value) {
  if (value.IsEmpty())
  {
    RemoveChunk(PKEY_Search_Contents.fmtid, PKEY_Search_Contents.pid);
  }
  else
  {
    CMFCFilterChunkValueImpl* pChunk = nullptr;
    ATLTRY(pChunk = new CMFCFilterChunkValueImpl);
    if (pChunk != nullptr)
    {
      pChunk->SetTextValue(PKEY_Search_Contents, value, CHUNK_TEXT);
      SetChunkValue(pChunk);
    }
  }
}

#endif // SHARED_HANDayLineERS

// CFireBirdDoc 诊断

// CFireBirdDoc 命令
