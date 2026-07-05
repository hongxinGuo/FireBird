// FireBirdDoc.cpp: CFireBirdDoc 类的实现
//

#include"pch.h"
// SHARED_HANDayLineERS 可以在实现预览、缩略图和搜索筛选器句柄的
// ATL 项目中进行定义，并允许与该项目共享文档代码。
#ifndef SHARED_HANDayLineERS
#endif

#include "FireBirdDoc.h"

#include "ContainerTiingoStockMonthLine.h"
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
			if (IsTiingoStock(pStock)) {
				m_pDataDayLine = make_shared<CContainerTiingoStockDayLine>();
				m_pDataDayLine->LoadDB(pStock->GetSymbol());
				m_pDataDayLine->SplitAdjust();
				m_pDataWeekLine = make_shared<CContainerTiingoStockWeekLine>();
				m_pDataWeekLine->CreateWeekLine(*m_pDataDayLine);
				m_pDataMonthLine = make_shared<CContainerTiingoStockMonthLine>();
				m_pDataMonthLine->CreateMonthLine(*m_pDataDayLine);
			}
			else { // 中国市场股票
				ASSERT(IsChinaStock(pStock));
				m_pDataDayLine = make_shared<CContainerChinaStockDayLine>();
				m_pDataDayLine->LoadDB(pStock->GetSymbol());
				m_pDataDayLine->SplitAdjust();
				m_pDataWeekLine = make_shared<CContainerChinaStockWeekLine>();
				m_pDataWeekLine->CreateWeekLine(*m_pDataDayLine);
				m_pDataMonthLine = make_shared<CContainerChinaStockMonthLine>();
				m_pDataMonthLine->CreateMonthLine(*m_pDataDayLine);
			}


			CalculateDayLineMovingAverage(*m_pDataDayLine);
			CalculateWeekLineMovingAverage(*m_pDataWeekLine);
			CalculateMonthLineMovingAverage(*m_pDataMonthLine);
			m_dayLineKDJ.SetCandle(m_pDataDayLine);
			m_dayLineKDJ.Calculate();
			m_weekLineKDJ.SetCandle(m_pDataWeekLine);
			m_weekLineKDJ.Calculate();
			m_monthLineKDJ.SetCandle(m_pDataMonthLine);
			m_monthLineKDJ.Calculate();
			m_dayLineMACD.SetCandle(m_pDataDayLine);
			m_dayLineMACD.Calculate();
			m_weekLineMACD.SetCandle(m_pDataWeekLine);
			m_weekLineMACD.Calculate();
			m_monthLineMACD.SetCandle(m_pDataMonthLine);
			m_monthLineMACD.Calculate();
			m_dayLineRSI.SetCandle(m_pDataDayLine);
			m_dayLineRSI.Calculate();
			m_weekLineRSI.SetCandle(m_pDataWeekLine);
			m_weekLineRSI.Calculate();
			m_monthLineRSI.SetCandle(m_pDataMonthLine);
			m_monthLineRSI.Calculate();
			m_dayLineBoll.SetCandle(m_pDataDayLine);
			m_dayLineBoll.Calculate();
			m_weekLineBoll.SetCandle(m_pDataWeekLine);
			m_weekLineBoll.Calculate();
			m_monthLineBoll.SetCandle(m_pDataMonthLine);
			m_monthLineBoll.Calculate();
			m_bDataReady = true;
		});
	} 
}

void CFireBirdDoc::CalculateDayLineMovingAverage(CVirtualDataHistoryCandle& historyCandle) {
	m_dayLine5MovingAverage.Calculate(historyCandle);
	m_dayLine10MovingAverage.Calculate(historyCandle);
	m_dayLine30MovingAverage.Calculate(historyCandle);
	m_dayLine50MovingAverage.Calculate(historyCandle);
	m_dayLine120MovingAverage.Calculate(historyCandle);
	m_dayLine250MovingAverage.Calculate(historyCandle);
	ASSERT(m_dayLine50MovingAverage.Size() == m_pDataDayLine->Size() - 50);
}

void CFireBirdDoc::CalculateWeekLineMovingAverage(CVirtualDataHistoryCandle& historyCandle) {
	m_weekLine5MovingAverage.Calculate(historyCandle);
	m_weekLine10MovingAverage.Calculate(historyCandle);
	m_weekLine30MovingAverage.Calculate(historyCandle);
	m_weekLine50MovingAverage.Calculate(historyCandle);
	m_weekLine120MovingAverage.Calculate(historyCandle);
	m_weekLine250MovingAverage.Calculate(historyCandle);
	ASSERT(m_weekLine50MovingAverage.Size() == m_pDataWeekLine->Size() - 50);
}

void CFireBirdDoc::CalculateMonthLineMovingAverage(CVirtualDataHistoryCandle& historyCandle) {
	m_monthLine5MovingAverage.Calculate(historyCandle);
	m_monthLine10MovingAverage.Calculate(historyCandle);
	m_monthLine30MovingAverage.Calculate(historyCandle);
	m_monthLine50MovingAverage.Calculate(historyCandle);
	m_monthLine120MovingAverage.Calculate(historyCandle);
	m_monthLine250MovingAverage.Calculate(historyCandle);
	ASSERT(m_monthLine50MovingAverage.Size() == m_pDataMonthLine->Size() - 50);
}

std::pair<long, long> CFireBirdDoc::GetDayLineHighLow(int iCandleNumber) const {
	auto pairHighLow = m_pDataDayLine->GetHighLow(iCandleNumber);
	return pairHighLow;
}

std::pair<long, long> CFireBirdDoc::GetWeekLineHighLow(int iCandleNumber) const {
	auto pairHighLow = m_pDataWeekLine->GetHighLow(iCandleNumber);
	return pairHighLow;
}

std::pair<long, long> CFireBirdDoc::GetMonthLineHighLow(int iCandleNumber) const {
	auto pairHighLow = m_pDataMonthLine->GetHighLow(iCandleNumber);
	return pairHighLow;
}

chrono::local_days CFireBirdDoc::GetDayLineDate(size_t countDownIndex) const {
	if (m_pCurrentStock == nullptr) return chrono::local_days(chrono::days(0));
	if (m_pDataDayLine->Size() < countDownIndex) return m_pDataDayLine->GetData(0)->GetDate();
	return m_pDataDayLine->GetData(m_pDataDayLine->Size() - countDownIndex)->GetDate();
}

chrono::local_days CFireBirdDoc::GetWeekLineDate(size_t countDownIndex) const {
	if (m_pCurrentStock == nullptr) return chrono::local_days(chrono::days(0));
	if (m_pDataWeekLine->Size() < countDownIndex) return m_pDataWeekLine->GetData(0)->GetDate();
	return m_pDataWeekLine->GetData(m_pDataWeekLine->Size() - countDownIndex)->GetDate();
}

chrono::local_days CFireBirdDoc::GetMonthLineDate(size_t countDownIndex) const {
	if (m_pCurrentStock == nullptr) return chrono::local_days(chrono::days(0));
	if (m_pDataMonthLine->Size() < countDownIndex) return m_pDataMonthLine->GetData(0)->GetDate();
	return m_pDataMonthLine->GetData(m_pDataMonthLine->Size() - countDownIndex)->GetDate();
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
