// FireBirdDoc.cpp: CFireBirdDoc 类的实现
//

#include"pch.h"
// SHARED_HANDayLineERS 可以在实现预览、缩略图和搜索筛选器句柄的
// ATL 项目中进行定义，并允许与该项目共享文档代码。
#ifndef SHARED_HANDayLineERS
#endif

#include "FireBirdDoc.h"

IMPLEMENT_DYNCREATE(CFireBirdDoc, CDocument)

BEGIN_MESSAGE_MAP(CFireBirdDoc, CDocument)
END_MESSAGE_MAP()

// CFireBirdDoc 构造/析构
CFireBirdDoc::CFireBirdDoc() {}

void CFireBirdDoc::CalculateDayLineMA() {
	auto pvDayLine = m_pCurrentStock->DayLine();
	auto dayLineSize = m_pCurrentStock->DayLine()->Size();
	long long total;

	m_DayLine5MA.resize(dayLineSize);
	m_DayLine10MA.resize(dayLineSize);
	m_DayLine30MA.resize(dayLineSize);
	m_DayLine50MA.resize(dayLineSize);
	m_DayLine120MA.resize(dayLineSize);
	m_DayLine250MA.resize(dayLineSize);

	for (size_t index = 4; index < dayLineSize; index++) {
		total = 0;
		for (size_t i = 0; i < 5; i++) {
			total += pvDayLine->GetData(index - i)->GetClose();
		}
		m_DayLine5MA[index] = total / 5;
	}
	for (size_t index = 9; index < dayLineSize; index++) {
		total = 0;
		for (size_t i = 0; i < 10; i++) {
			total += pvDayLine->GetData(index - i)->GetClose();
		}
		m_DayLine10MA[index] = total / 10;
	}
	for (size_t index = 29; index < dayLineSize; index++) {
		total = 0;
		for (size_t i = 0; i < 30; i++) {
			total += pvDayLine->GetData(index - i)->GetClose();
		}
		m_DayLine30MA[index] = total / 30;
	}
	for (size_t index = 49; index < dayLineSize; index++) {
		total = 0;
		for (size_t i = 0; i < 50; i++) {
			total += pvDayLine->GetData(index - i)->GetClose();
		}
		m_DayLine50MA[index] = total / 50;
	}
	for (size_t index = 119; index < dayLineSize; index++) {
		total = 0;
		for (size_t i = 0; i < 120; i++) {
			total += pvDayLine->GetData(index - i)->GetClose();
		}
		m_DayLine120MA[index] = total / 5;
	}
	for (size_t index = 249; index < dayLineSize; index++) {
		total = 0;
		for (size_t i = 0; i < 250; i++) {
			total += pvDayLine->GetData(index - i)->GetClose();
		}
		m_DayLine250MA[index] = total / 250;
	}
}

void CFireBirdDoc::CalculateWeekLineMA() {
	m_WeekLine5MA.resize(m_pCurrentStock->WeekLine()->Size());
	m_WeekLine10MA.resize(m_pCurrentStock->WeekLine()->Size());
	m_WeekLine30MA.resize(m_pCurrentStock->WeekLine()->Size());
	m_WeekLine5MA.resize(m_pCurrentStock->WeekLine()->Size());
	m_WeekLine60MA.resize(m_pCurrentStock->WeekLine()->Size());
	m_WeekLine120MA.resize(m_pCurrentStock->WeekLine()->Size());
	m_WeekLine240MA.resize(m_pCurrentStock->WeekLine()->Size());
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
