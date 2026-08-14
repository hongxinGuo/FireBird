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

#include"ChinaStock.h"
#include "ContainerChinaStockMonthLine.h"
#include "ContainerChinaStockWeekLine.h"
#include "ContainerTiingoStockWeekLine.h"
#include "TiingoStock.h"
#include <VirtualDataHistoryCandle.h>

using namespace std;

IMPLEMENT_DYNCREATE(CFireBirdDoc, CDocument)

BEGIN_MESSAGE_MAP(CFireBirdDoc, CDocument)
END_MESSAGE_MAP()

// CFireBirdDoc 构造/析构
CFireBirdDoc::CFireBirdDoc() = default;

void CFireBirdDoc::SetCurrentStock(const CVirtualStockPtr& pStock) {
	if (m_pCurrentStock == nullptr || !m_pCurrentStock->IsSameStock(pStock)) m_bRefreshView = true;
	m_pCurrentStock = pStock;
	if (pStock == nullptr) return;
		m_bDataReady = false;
		if (m_jtLoadCurrentStockDB.joinable()) {
			m_jtLoadCurrentStockDB.request_stop();
			m_jtLoadCurrentStockDB.join();
		}
		m_jtLoadCurrentStockDB = std::jthread([this, pStock](std::stop_token st) {
			if (st.stop_requested()) return;
			if (IsTiingoStock(pStock)) {
				m_pDataDayLine = make_shared<CContainerTiingoStockDayLine>();
				m_pDataDayLine->LoadDB(pStock->GetSymbol());
				if (st.stop_requested()) return;
				m_pDataDayLine->SplitAdjust();
				m_pDataWeekLine = make_shared<CContainerTiingoStockWeekLine>();
				m_pDataWeekLine->CreateWeekLine(*m_pDataDayLine);
				m_pDataWeekLine->SetSplitAdjusted(true);
				m_pDataMonthLine = make_shared<CContainerTiingoStockMonthLine>();
				m_pDataMonthLine->CreateMonthLine(*m_pDataDayLine);
				m_pDataMonthLine->SetSplitAdjusted(true);
			}
			else { // 中国市场股票
				ABSL_DCHECK(IsChinaStock(pStock));
				m_pDataDayLine = make_shared<CContainerChinaStockDayLine>();
				m_pDataDayLine->LoadDB(pStock->GetSymbol());
				if (st.stop_requested()) return;
				m_pDataDayLine->SplitAdjust();
				m_pDataWeekLine = make_shared<CContainerChinaStockWeekLine>();
				m_pDataWeekLine->CreateWeekLine(*m_pDataDayLine);
				m_pDataWeekLine->SetSplitAdjusted(true);
				m_pDataMonthLine = make_shared<CContainerChinaStockMonthLine>();
				m_pDataMonthLine->CreateMonthLine(*m_pDataDayLine);
				m_pDataMonthLine->SetSplitAdjusted(true);
			}
			if (st.stop_requested()) return;
			CalculateDayLineMovingAverage(*m_pDataDayLine);
			CalculateWeekLineMovingAverage(*m_pDataWeekLine);
			CalculateMonthLineMovingAverage(*m_pDataMonthLine);
			if (st.stop_requested()) return;
			m_dayLineKDJ.SetCandle(m_pDataDayLine);
			m_dayLineKDJ.Calculate();
			if (st.stop_requested()) return;
			m_weekLineKDJ.SetCandle(m_pDataWeekLine);
			m_weekLineKDJ.Calculate();
			m_monthLineKDJ.SetCandle(m_pDataMonthLine);
			m_monthLineKDJ.Calculate();
			if (st.stop_requested()) return;
			m_dayLineMACD.SetCandle(m_pDataDayLine);
			m_dayLineMACD.Calculate();
			if (st.stop_requested()) return;
			m_weekLineMACD.SetCandle(m_pDataWeekLine);
			m_weekLineMACD.Calculate();
			if (st.stop_requested()) return;
			m_monthLineMACD.SetCandle(m_pDataMonthLine);
			m_monthLineMACD.Calculate();
			if (st.stop_requested()) return;
			m_dayLineRSI.SetCandle(m_pDataDayLine);
			m_dayLineRSI.Calculate();
			if (st.stop_requested()) return;
			m_weekLineRSI.SetCandle(m_pDataWeekLine);
			m_weekLineRSI.Calculate();
			if (st.stop_requested()) return;
			m_monthLineRSI.SetCandle(m_pDataMonthLine);
			m_monthLineRSI.Calculate();
			if (st.stop_requested()) return;
			m_dayLineBoll.SetCandle(m_pDataDayLine);
			m_dayLineBoll.Calculate();
			if (st.stop_requested()) return;
			m_weekLineBoll.SetCandle(m_pDataWeekLine);
			m_weekLineBoll.Calculate();
			if (st.stop_requested()) return;
			m_monthLineBoll.SetCandle(m_pDataMonthLine);
			m_monthLineBoll.Calculate();
			m_bDataReady = true;
		});
}

void CFireBirdDoc::CalculateDayLineMovingAverage(CVirtualDataHistoryCandle& historyCandle) {
	ABSL_DCHECK(historyCandle.IsSplitAdjusted());
	m_dayLine5MovingAverage.Calculate(historyCandle);
	m_dayLine10MovingAverage.Calculate(historyCandle);
	m_dayLine30MovingAverage.Calculate(historyCandle);
	m_dayLine50MovingAverage.Calculate(historyCandle);
	m_dayLine120MovingAverage.Calculate(historyCandle);
	m_dayLine250MovingAverage.Calculate(historyCandle);
	ABSL_DCHECK(m_dayLine50MovingAverage.Size() == m_pDataDayLine->Size() - 50);
}

void CFireBirdDoc::CalculateWeekLineMovingAverage(CVirtualDataHistoryCandle& historyCandle) {
	ABSL_DCHECK(historyCandle.IsSplitAdjusted());
	m_weekLine5MovingAverage.Calculate(historyCandle);
	m_weekLine10MovingAverage.Calculate(historyCandle);
	m_weekLine30MovingAverage.Calculate(historyCandle);
	m_weekLine50MovingAverage.Calculate(historyCandle);
	m_weekLine120MovingAverage.Calculate(historyCandle);
	m_weekLine250MovingAverage.Calculate(historyCandle);
	ABSL_DCHECK(m_weekLine50MovingAverage.Size() == m_pDataWeekLine->Size() - 50);
}

void CFireBirdDoc::CalculateMonthLineMovingAverage(CVirtualDataHistoryCandle& historyCandle) {
	ABSL_DCHECK(historyCandle.IsSplitAdjusted());
	m_monthLine5MovingAverage.Calculate(historyCandle);
	m_monthLine10MovingAverage.Calculate(historyCandle);
	m_monthLine30MovingAverage.Calculate(historyCandle);
	m_monthLine50MovingAverage.Calculate(historyCandle);
	m_monthLine120MovingAverage.Calculate(historyCandle);
	m_monthLine250MovingAverage.Calculate(historyCandle);
	ABSL_DCHECK(m_monthLine50MovingAverage.Size() == m_pDataMonthLine->Size() - 50);
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
