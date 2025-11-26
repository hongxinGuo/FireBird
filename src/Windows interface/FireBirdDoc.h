// FireBirdDoc.h: CFireBirdDoc 类的接口
//

#pragma once

#include"MovingAverage.h"
#include"IndicatorKDJ.h"

class CFireBirdDoc : public CDocument {
protected: // 仅从序列化创建
	CFireBirdDoc();
	DECLARE_DYNCREATE(CFireBirdDoc)

	// 特性
public:
	bool IsEmpty() const noexcept { return m_pCurrentStock == nullptr; }
	bool IsDayLineLoaded() const { return m_pCurrentStock->IsDayLineLoaded(); }

	// 操作
public:
	bool IsDataReady() const noexcept { return m_bDataReady; }
	void SetDataReady(bool bFlag) noexcept { m_bDataReady = bFlag; }

	CVirtualStockPtr GetCurrentStock() const noexcept { return m_pCurrentStock; }
	void SetCurrentStock(const CVirtualStockPtr& pStock);

	void CalculateDayLineMovingAverage(const CVirtualStockPtr& pStock);
	void CalculateWeekLineMovingAverage(const CVirtualStockPtr& pStock);
	void CalculateMonthLineMovingAverage(const CVirtualStockPtr& pStock);

	std::pair<long, long> GetDayLineHighLow(int iCandleNumber) const;
	std::pair<long, long> GetWeekLineHighLow(int iCandleNumber) const;

	void ShowDayLine(CDC* pDC, CPen* pNewPen, CRect rectClient, int iStepWidth, long lHigh, long lLow) const {
		m_pCurrentStock->DayLine()->ToShow(pDC, pNewPen, rectClient, iStepWidth, lHigh, lLow);
	}

	void ShowDayLine5MovingAverage(CDC* pDC, CPen* pNewPen, CRect rectClient, int iStepWidth, long lHigh, long lLow) {
		m_dayLine5MovingAverage.ToShow(pDC, pNewPen, rectClient, iStepWidth, lHigh, lLow);
	}
	void ShowDayLine10MovingAverage(CDC* pDC, CPen* pNewPen, CRect rectClient, int iStepWidth, long lHigh, long lLow) {
		m_dayLine10MovingAverage.ToShow(pDC, pNewPen, rectClient, iStepWidth, lHigh, lLow);
	}
	void ShowDayLine30MovingAverage(CDC* pDC, CPen* pNewPen, CRect rectClient, int iStepWidth, long lHigh, long lLow) {
		m_dayLine30MovingAverage.ToShow(pDC, pNewPen, rectClient, iStepWidth, lHigh, lLow);
	}
	void ShowDayLine50MovingAverage(CDC* pDC, CPen* pNewPen, CRect rectClient, int iStepWidth, long lHigh, long lLow) {
		m_dayLine50MovingAverage.ToShow(pDC, pNewPen, rectClient, iStepWidth, lHigh, lLow);
	}
	void ShowDayLine120MovingAverage(CDC* pDC, CPen* pNewPen, CRect rectClient, int iStepWidth, long lHigh, long lLow) {
		m_dayLine120MovingAverage.ToShow(pDC, pNewPen, rectClient, iStepWidth, lHigh, lLow);
	}
	void ShowDayLine250MovingAverage(CDC* pDC, CPen* pNewPen, CRect rectClient, int iStepWidth, long lHigh, long lLow) {
		m_dayLine250MovingAverage.ToShow(pDC, pNewPen, rectClient, iStepWidth, lHigh, lLow);
	}

	void ShowWeekLine(CDC* pDC, CPen* pNewPen, CRect rectClient, int iStepWidth, long lHigh, long lLow) const {
		m_pCurrentStock->WeekLine()->ToShow(pDC, pNewPen, rectClient, iStepWidth, lHigh, lLow);
	}

	void ShowWeekLine5MovingAverage(CDC* pDC, CPen* pNewPen, CRect rectClient, int iStepWidth, long lHigh, long lLow) {
		m_weekLine5MovingAverage.ToShow(pDC, pNewPen, rectClient, iStepWidth, lHigh, lLow);
	}
	void ShowWeekLine10MovingAverage(CDC* pDC, CPen* pNewPen, CRect rectClient, int iStepWidth, long lHigh, long lLow) {
		m_weekLine10MovingAverage.ToShow(pDC, pNewPen, rectClient, iStepWidth, lHigh, lLow);
	}
	void ShowWeekLine30MovingAverage(CDC* pDC, CPen* pNewPen, CRect rectClient, int iStepWidth, long lHigh, long lLow) {
		m_weekLine30MovingAverage.ToShow(pDC, pNewPen, rectClient, iStepWidth, lHigh, lLow);
	}
	void ShowWeekLine50MovingAverage(CDC* pDC, CPen* pNewPen, CRect rectClient, int iStepWidth, long lHigh, long lLow) {
		m_weekLine50MovingAverage.ToShow(pDC, pNewPen, rectClient, iStepWidth, lHigh, lLow);
	}
	void ShowWeekLine120MovingAverage(CDC* pDC, CPen* pNewPen, CRect rectClient, int iStepWidth, long lHigh, long lLow) {
		m_weekLine120MovingAverage.ToShow(pDC, pNewPen, rectClient, iStepWidth, lHigh, lLow);
	}
	void ShowWeekLine250MovingAverage(CDC* pDC, CPen* pNewPen, CRect rectClient, int iStepWidth, long lHigh, long lLow) {
		m_weekLine250MovingAverage.ToShow(pDC, pNewPen, rectClient, iStepWidth, lHigh, lLow);
	}

	void ShowMonthLine5MovingAverage(CDC* pDC, CPen* pNewPen, CRect rectClient, int iStepWidth, long lHigh, long lLow) {
		m_monthLine5MovingAverage.ToShow(pDC, pNewPen, rectClient, iStepWidth, lHigh, lLow);
	}
	void ShowMonthLine10MovingAverage(CDC* pDC, CPen* pNewPen, CRect rectClient, int iStepWidth, long lHigh, long lLow) {
		m_monthLine10MovingAverage.ToShow(pDC, pNewPen, rectClient, iStepWidth, lHigh, lLow);
	}
	void ShowMonthLine30MovingAverage(CDC* pDC, CPen* pNewPen, CRect rectClient, int iStepWidth, long lHigh, long lLow) {
		m_monthLine30MovingAverage.ToShow(pDC, pNewPen, rectClient, iStepWidth, lHigh, lLow);
	}
	void ShowMonthLine50MovingAverage(CDC* pDC, CPen* pNewPen, CRect rectClient, int iStepWidth, long lHigh, long lLow) {
		m_monthLine50MovingAverage.ToShow(pDC, pNewPen, rectClient, iStepWidth, lHigh, lLow);
	}
	void ShowMonthLine120MovingAverage(CDC* pDC, CPen* pNewPen, CRect rectClient, int iStepWidth, long lHigh, long lLow) {
		m_monthLine120MovingAverage.ToShow(pDC, pNewPen, rectClient, iStepWidth, lHigh, lLow);
	}
	void ShowMonthLine250MovingAverage(CDC* pDC, CPen* pNewPen, CRect rectClient, int iStepWidth, long lHigh, long lLow) {
		m_monthLine250MovingAverage.ToShow(pDC, pNewPen, rectClient, iStepWidth, lHigh, lLow);
	}

	void ShowDayLineRS3(CDC* pDC, CPen* pNewPen, CRect rectDrawArea, int iStepWidth, double dZoomInRatio) const {
		m_pCurrentStock->ShowDayLineRS3(pDC, pNewPen, rectDrawArea, iStepWidth, dZoomInRatio);
	}
	void ShowDayLineRS5(CDC* pDC, CPen* pNewPen, CRect rectDrawArea, int iStepWidth, double dZoomInRatio) const {
		m_pCurrentStock->ShowDayLineRS5(pDC, pNewPen, rectDrawArea, iStepWidth, dZoomInRatio);
	}
	void ShowDayLineRS10(CDC* pDC, CPen* pNewPen, CRect rectDrawArea, int iStepWidth, double dZoomInRatio) const {
		m_pCurrentStock->ShowDayLineRS10(pDC, pNewPen, rectDrawArea, iStepWidth, dZoomInRatio);
	}
	void ShowDayLineRS30(CDC* pDC, CPen* pNewPen, CRect rectDrawArea, int iStepWidth, double dZoomInRatio) const {
		m_pCurrentStock->ShowDayLineRS30(pDC, pNewPen, rectDrawArea, iStepWidth, dZoomInRatio);
	}
	void ShowDayLineRS60(CDC* pDC, CPen* pNewPen, CRect rectDrawArea, int iStepWidth, double dZoomInRatio) const {
		m_pCurrentStock->ShowDayLineRS60(pDC, pNewPen, rectDrawArea, iStepWidth, dZoomInRatio);
	}
	void ShowDayLineRS120(CDC* pDC, CPen* pNewPen, CRect rectDrawArea, int iStepWidth, double dZoomInRatio) const {
		m_pCurrentStock->ShowDayLineRS120(pDC, pNewPen, rectDrawArea, iStepWidth, dZoomInRatio);
	}
	void ShowDayLineRSIndex(CDC* pDC, CPen* pNewPen, CRect rectDrawArea, int iStepWidth, double dZoomInRatio) const {
		m_pCurrentStock->ShowDayLineRSIndex(pDC, pNewPen, rectDrawArea, iStepWidth, dZoomInRatio);
	}
	void ShowDayLineRSLogarithm(CDC* pDC, CPen* pNewPen, CRect rectDrawArea, int iStepWidth, double dZoomInRatio) const {
		m_pCurrentStock->ShowDayLineRSLogarithm(pDC, pNewPen, rectDrawArea, iStepWidth, dZoomInRatio);
	}
	void ShowDayLineRS1(CDC* pDC, CPen* pNewPen, CRect rectDrawArea, int iStepWidth, double dZoomInRatio) const {
		m_pCurrentStock->ShowDayLineRS1(pDC, pNewPen, rectDrawArea, iStepWidth, dZoomInRatio);
	}

	void ShowWeekLineRS3(CDC* pDC, CPen* pNewPen, CRect rectDrawArea, int iStepWidth, double dZoomInRatio) const {
		m_pCurrentStock->ShowWeekLineRS3(pDC, pNewPen, rectDrawArea, iStepWidth, dZoomInRatio);
	}
	void ShowWeekLineRS5(CDC* pDC, CPen* pNewPen, CRect rectDrawArea, int iStepWidth, double dZoomInRatio) const {
		m_pCurrentStock->ShowWeekLineRS5(pDC, pNewPen, rectDrawArea, iStepWidth, dZoomInRatio);
	}
	void ShowWeekLineRS10(CDC* pDC, CPen* pNewPen, CRect rectDrawArea, int iStepWidth, double dZoomInRatio) const {
		m_pCurrentStock->ShowWeekLineRS10(pDC, pNewPen, rectDrawArea, iStepWidth, dZoomInRatio);
	}
	void ShowWeekLineRS30(CDC* pDC, CPen* pNewPen, CRect rectDrawArea, int iStepWidth, double dZoomInRatio) const {
		m_pCurrentStock->ShowWeekLineRS30(pDC, pNewPen, rectDrawArea, iStepWidth, dZoomInRatio);
	}
	void ShowWeekLineRS60(CDC* pDC, CPen* pNewPen, CRect rectDrawArea, int iStepWidth, double dZoomInRatio) const {
		m_pCurrentStock->ShowWeekLineRS60(pDC, pNewPen, rectDrawArea, iStepWidth, dZoomInRatio);
	}
	void ShowWeekLineRS120(CDC* pDC, CPen* pNewPen, CRect rectDrawArea, int iStepWidth, double dZoomInRatio) const {
		m_pCurrentStock->ShowWeekLineRS120(pDC, pNewPen, rectDrawArea, iStepWidth, dZoomInRatio);
	}
	void ShowWeekLineRSIndex(CDC* pDC, CPen* pNewPen, CRect rectDrawArea, int iStepWidth, double dZoomInRatio) const {
		m_pCurrentStock->ShowWeekLineRSIndex(pDC, pNewPen, rectDrawArea, iStepWidth, dZoomInRatio);
	}
	void ShowWeekLineRSLogarithm(CDC* pDC, CPen* pNewPen, CRect rectDrawArea, int iStepWidth, double dZoomInRatio) const {
		m_pCurrentStock->ShowWeekLineRSLogarithm(pDC, pNewPen, rectDrawArea, iStepWidth, dZoomInRatio);
	}
	void ShowWeekLineRS1(CDC* pDC, CPen* pNewPen, CRect rectDrawArea, int iStepWidth, double dZoomInRatio) const {
		m_pCurrentStock->ShowWeekLineRS1(pDC, pNewPen, rectDrawArea, iStepWidth, dZoomInRatio);
	}

	void ShowDayLineKDJ(CDC* pDC, CRect rectDraw, int iStepWidth) {
		m_dayLineKDJ.ToShow(pDC, rectDraw, iStepWidth);
	}

	void ShowWeekLineKDJ(CDC* pDC, CRect rectDraw, int iStepWidth) {
		m_weekLineKDJ.ToShow(pDC, rectDraw, iStepWidth);
	}

	// 重写
public:
	BOOL OnNewDocument() override;
	void Serialize(CArchive& ar) override;
#ifdef SHARED_HANDayLineERS
  virtual void InitializeSearchContent();
  virtual void OnDrawThumbnail(CDC& dc, LPRECT lprcBounds);
#endif // SHARED_HANDayLineERS

	// 实现
	~CFireBirdDoc() override = default;

protected:
	atomic_bool m_bDataReady{ false }; // 各种指标最好使用工作线程来异步计算，计算完成时设置此标识为真；当重新设置股票时设置此标识为假。
	CVirtualStockPtr m_pCurrentStock{ nullptr }; // 当前股票

	//日线移动平均线
	CMovingAverage m_dayLine5MovingAverage{ 5 };
	CMovingAverage m_dayLine10MovingAverage{ 10 };
	CMovingAverage m_dayLine30MovingAverage{ 30 };
	CMovingAverage m_dayLine50MovingAverage{ 50 };
	CMovingAverage m_dayLine120MovingAverage{ 120 };
	CMovingAverage m_dayLine250MovingAverage{ 250 };

	// 周线移动平均线
	CMovingAverage m_weekLine5MovingAverage{ 5 };
	CMovingAverage m_weekLine10MovingAverage{ 10 };
	CMovingAverage m_weekLine30MovingAverage{ 30 };
	CMovingAverage m_weekLine50MovingAverage{ 50 };
	CMovingAverage m_weekLine120MovingAverage{ 120 };
	CMovingAverage m_weekLine250MovingAverage{ 250 };

	// 月线移动平均线
	CMovingAverage m_monthLine5MovingAverage{ 5 };
	CMovingAverage m_monthLine10MovingAverage{ 10 };
	CMovingAverage m_monthLine30MovingAverage{ 30 };
	CMovingAverage m_monthLine50MovingAverage{ 50 };
	CMovingAverage m_monthLine120MovingAverage{ 120 };
	CMovingAverage m_monthLine250MovingAverage{ 250 };

	CIndicatorKDJ m_dayLineKDJ;
	CIndicatorKDJ m_weekLineKDJ;

	// 生成的消息映射函数
protected:
	DECLARE_MESSAGE_MAP()

#ifdef SHARED_HANDayLineERS
  // 用于为搜索处理程序设置搜索内容的 Helper 函数
  void SetSearchContent(const CString& value);
#endif // SHARED_HANDayLineERS
};
