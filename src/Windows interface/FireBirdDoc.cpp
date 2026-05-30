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
CFireBirdDoc::CFireBirdDoc() = default;

void CFireBirdDoc::SetCurrentStock(const CVirtualStockPtr& pStock) {
	if (m_pCurrentStock == nullptr || !m_pCurrentStock->IsSameStock(pStock)) m_bRefreshView = true;

	m_pCurrentStock = pStock;
	if (pStock != nullptr) {
		m_bDataReady = false;
		gl_runtime.background_executor()->post([this, pStock] {
			if (!pStock->IsDayLineLoaded() || !pStock->IsWeekLineLoaded()) {
				pStock->LoadHistoryCandleDB();
			}
			CalculateDayLineMovingAverage(pStock);
			CalculateWeekLineMovingAverage(pStock);
			CalculateMonthLineMovingAverage(pStock);
			m_dayLineKDJ.SetCandle(pStock->DayLine());
			m_dayLineKDJ.Calculate();
			m_weekLineKDJ.SetCandle(pStock->WeekLine());
			m_weekLineKDJ.Calculate();
			m_monthLineKDJ.SetCandle(pStock->MonthLine());
			m_monthLineKDJ.Calculate();
			m_dayLineMACD.SetCandle(pStock->DayLine());
			m_dayLineMACD.Calculate();
			m_weekLineMACD.SetCandle(pStock->WeekLine());
			m_weekLineMACD.Calculate();
			m_monthLineMACD.SetCandle(pStock->MonthLine());
			m_monthLineMACD.Calculate();
			m_dayLineRSI.SetCandle(pStock->DayLine());
			m_dayLineRSI.Calculate();
			m_weekLineRSI.SetCandle(pStock->WeekLine());
			m_weekLineRSI.Calculate();
			m_monthLineRSI.SetCandle(pStock->MonthLine());
			m_monthLineRSI.Calculate();
			m_bDataReady = true;
		});
	} 
}
void CFireBirdDoc::CalculateDayLineMovingAverage(const CVirtualStockPtr& pStock) {
	auto pvDayLine = pStock->DayLine();

	ASSERT(m_pCurrentStock->GetSymbol() == pStock->GetSymbol());
	m_dayLine5MovingAverage.Calculate(pvDayLine);
	m_dayLine10MovingAverage.Calculate(pvDayLine);
	m_dayLine30MovingAverage.Calculate(pvDayLine);
	m_dayLine50MovingAverage.Calculate(pvDayLine);
	m_dayLine120MovingAverage.Calculate(pvDayLine);
	m_dayLine250MovingAverage.Calculate(pvDayLine);
	ASSERT(m_dayLine50MovingAverage.Size() == pvDayLine->Size() - 50);
}

void CFireBirdDoc::CalculateWeekLineMovingAverage(const CVirtualStockPtr& pStock) {
	auto pvWeekLine = pStock->WeekLine();

	ASSERT(m_pCurrentStock->GetSymbol() == pStock->GetSymbol());
	m_weekLine5MovingAverage.Calculate(pvWeekLine);
	m_weekLine10MovingAverage.Calculate(pvWeekLine);
	m_weekLine30MovingAverage.Calculate(pvWeekLine);
	m_weekLine50MovingAverage.Calculate(pvWeekLine);
	m_weekLine120MovingAverage.Calculate(pvWeekLine);
	m_weekLine250MovingAverage.Calculate(pvWeekLine);
	ASSERT(m_weekLine50MovingAverage.Size() == pvWeekLine->Size() - 50);
}

void CFireBirdDoc::CalculateMonthLineMovingAverage(const CVirtualStockPtr& pStock) {
	auto pvMonthLine = pStock->MonthLine();

	ASSERT(m_pCurrentStock->GetSymbol() == pStock->GetSymbol());
	m_monthLine5MovingAverage.Calculate(pvMonthLine);
	m_monthLine10MovingAverage.Calculate(pvMonthLine);
	m_monthLine30MovingAverage.Calculate(pvMonthLine);
	m_monthLine50MovingAverage.Calculate(pvMonthLine);
	m_monthLine120MovingAverage.Calculate(pvMonthLine);
	m_monthLine250MovingAverage.Calculate(pvMonthLine);
	ASSERT(m_monthLine50MovingAverage.Size() == pvMonthLine->Size() - 50);
}

std::pair<long, long> CFireBirdDoc::GetDayLineHighLow(int iCandleNumber) const {
	auto pairHighLow = m_pCurrentStock->DayLine()->GetHighLow(iCandleNumber);
	return pairHighLow;
}

std::pair<long, long> CFireBirdDoc::GetWeekLineHighLow(int iCandleNumber) const {
	auto pairHighLow = m_pCurrentStock->WeekLine()->GetHighLow(iCandleNumber);
	return pairHighLow;
}

std::pair<long, long> CFireBirdDoc::GetMonthLineHighLow(int iCandleNumber) const {
	auto pairHighLow = m_pCurrentStock->MonthLine()->GetHighLow(iCandleNumber);
	return pairHighLow;
}
long CFireBirdDoc::GetDayLineDate(size_t countDownIndex) const {
	if (m_pCurrentStock == nullptr) return 0;
	auto dayLine = m_pCurrentStock->DayLine();
	if (dayLine->Size() < countDownIndex) return dayLine->GetData(0)->GetDate();
	return dayLine->GetData(dayLine->Size() - countDownIndex)->GetDate();
}

long CFireBirdDoc::GetWeekLineDate(size_t countDownIndex){
	if (m_pCurrentStock == nullptr) return 0;
	auto weekLine = m_pCurrentStock->WeekLine();
	if (weekLine->Size() < countDownIndex) return weekLine->GetData(0)->GetDate();
	return weekLine->GetData(weekLine->Size() - countDownIndex)->GetDate();
}

long CFireBirdDoc::GetMonthLineDate(size_t countDownIndex){
	if (m_pCurrentStock == nullptr) return 0;
	auto mouthLine = m_pCurrentStock->MonthLine();
	if (mouthLine->Size() < countDownIndex) return mouthLine->GetData(0)->GetDate();
	return mouthLine->GetData(mouthLine->Size() - countDownIndex)->GetDate();
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

  CString strText = "TODO: implement thumbnail drawing here");
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

  // 例如:     strSearchContent = "point;rectangle;circle;ole object;")；
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
