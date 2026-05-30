// FireBirdDoc.h: CFireBirdDoc 类的接口
//

#pragma once

#include"MovingAverage.h"
#include"IndicatorKDJ.h"
#include "MovingAverageConvergenceDivergence.h"
#include "IndicatorRSI.h"
#include"IndicatorBoll.h"

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
	bool IsRefreshView() const noexcept { return m_bRefreshView; }
	void SetRefreshView(bool bFlag) noexcept { m_bRefreshView = bFlag; }

	CVirtualStockPtr GetCurrentStock() const noexcept { return m_pCurrentStock; }
	void SetCurrentStock(const CVirtualStockPtr& pStock);

	void CalculateDayLineMovingAverage(const CVirtualStockPtr& pStock);
	void CalculateWeekLineMovingAverage(const CVirtualStockPtr& pStock);
	void CalculateMonthLineMovingAverage(const CVirtualStockPtr& pStock);

	std::pair<long, long> GetDayLineHighLow(int iCandleNumber) const;
	std::pair<long, long> GetWeekLineHighLow(int iCandleNumber) const;
	std::pair<long, long> GetMonthLineHighLow(int iCandleNumber) const;

	long GetDayLineDate(size_t countDownIndex) const;
	long GetWeekLineDate(size_t countDownIndex);
	long GetMonthLineDate(size_t countDownIndex);

	void ShowDayLine(CDC* pDC, CRect rectClient, int iStepWidth, long lHigh, long lLow) const {
		m_pCurrentStock->DayLine()->ToShow(pDC, rectClient, iStepWidth, lHigh, lLow);
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

	void ShowWeekLine(CDC* pDC, CRect rectClient, int iStepWidth, long lHigh, long lLow) const {
		m_pCurrentStock->WeekLine()->ToShow(pDC, rectClient, iStepWidth, lHigh, lLow);
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

	void ShowMonthLine(CDC* pDC, CRect rectClient, int iStepWidth, long lHigh, long lLow) const {
		m_pCurrentStock->MonthLine()->ToShow(pDC, rectClient, iStepWidth, lHigh, lLow);
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

	void ShowDayLineKDJ(CDC* pDC, CRect rectDraw, int iStepWidth) {
		m_dayLineKDJ.ToShow(pDC, rectDraw, iStepWidth);
	}

	void ShowWeekLineKDJ(CDC* pDC, CRect rectDraw, int iStepWidth) {
		m_weekLineKDJ.ToShow(pDC, rectDraw, iStepWidth);
	}

	void ShowMonthLineKDJ(CDC* pDC, CRect rectDraw, int iStepWidth) {
		m_monthLineKDJ.ToShow(pDC, rectDraw, iStepWidth);
	}

	void ShowDayLineMACD(CDC* pDC, CRect rectDraw, int iStepWidth) {
		m_dayLineMACD.ToShow(pDC, rectDraw, iStepWidth);
	}

	void ShowWeekLineMACD(CDC* pDC, CRect rectDraw, int iStepWidth) {
		m_weekLineMACD.ToShow(pDC, rectDraw, iStepWidth);
	}

	void ShowMonthLineMACD(CDC* pDC, CRect rectDraw, int iStepWidth) {
		m_monthLineMACD.ToShow(pDC, rectDraw, iStepWidth);
	}

	void ShowDayLineRSI(CDC* pDC, CRect rectDrawArea, int iStepWidth) {
		m_dayLineRSI.ToShow(pDC, rectDrawArea, iStepWidth);
	}

	void ShowWeekLineRSI(CDC* pDC, CRect rectDrawArea, int iStepWidth) {
		m_weekLineRSI.ToShow(pDC, rectDrawArea, iStepWidth);
	}

	void ShowMonthLineRSI(CDC* pDC, CRect rectDrawArea, int iStepWidth) {
		m_monthLineBoll.ToShow(pDC, rectDrawArea, iStepWidth);
	}
	void ShowDayLineBoll(CDC* pDC, CRect rectDrawArea, int iStepWidth) {
		m_dayLineBoll.ToShow(pDC, rectDrawArea, iStepWidth);
	}

	void ShowWeekLineBoll(CDC* pDC, CRect rectDrawArea, int iStepWidth) {
		m_weekLineBoll.ToShow(pDC, rectDrawArea, iStepWidth);
	}

	void ShowMonthLineBoll(CDC* pDC, CRect rectDrawArea, int iStepWidth) {
		m_monthLineBoll.ToShow(pDC, rectDrawArea, iStepWidth);
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
	bool m_bRefreshView{ false }; // 是否需要刷新数据
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
	CIndicatorKDJ m_monthLineKDJ;

	CIndicatorMACD m_dayLineMACD;
	CIndicatorMACD m_weekLineMACD;
	CIndicatorMACD m_monthLineMACD;

	CIndicatorRSI m_dayLineRSI;
	CIndicatorRSI m_weekLineRSI;
	CIndicatorRSI m_monthLineRSI;

	CIndicatorBoll m_dayLineBoll;
	CIndicatorRSI m_weekLineBoll;
	CIndicatorRSI m_monthLineBoll;

	// 生成的消息映射函数
protected:
	DECLARE_MESSAGE_MAP()

#ifdef SHARED_HANDayLineERS
	// 用于为搜索处理程序设置搜索内容的 Helper 函数
	void SetSearchContent(const CString& value);
#endif // SHARED_HANDayLineERS
};
