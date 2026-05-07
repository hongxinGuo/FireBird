////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// 股票实例数据容器（日线容器、周线容器等）的基类。
// 纯虚类，不允许生成实例。
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#pragma once

#include "DayLine.h"
#include "MonthLine.h"
#include "TiingoCandleLine.h"
#include "VirtualHistoryCandle.h"
#include "VirtualSetHistoryCandle.h"

using std::atomic_bool;

class CVirtualDataHistoryCandle {
public:
	CVirtualDataHistoryCandle();
	virtual ~CVirtualDataHistoryCandle() = default;

	void Reset(); // 这些实现类需要采用这种方法重置内部状态，因为系统会一直运行，每天都需要重置状态。
	auto Size() const noexcept { return m_vHistoryData.size(); }
	bool Empty() const noexcept { return m_vHistoryData.empty(); }
	void Reserve(size_t size) { m_vHistoryData.reserve(size); }

	// 所有的派生类皆需要定义此两个存储和提取函数，不允许调用此基类函数
	virtual bool SaveDB(const string&) {
		TRACE(_T("调用了基类SaveDB\n"));
		return false;
	}
	virtual bool LoadDB(const string&) {
		TRACE(_T("调用了基类LoadDB\n"));
		return false;
	}

	bool SaveBasicDB(CVirtualSetHistoryCandle* pSetHistoryCandle) const;

	bool LoadBasicDB(CVirtualSetHistoryCandle* pSetHistoryCandle);

	void UpdateData(const vector<CVirtualHistoryCandle>& vTempData);
	void UpdateData(const vector<CDayLine>& vTempData);
	void UpdateData(const CDayLinesPtr& pvTempDayLine);

	int GetRatio() const {
		if (m_ratio == 0)
			ASSERT(0);
		return m_ratio;
	}

	vector<CVirtualHistoryCandle>* GetContainer() noexcept { return &m_vHistoryData; }

	bool GetStartEndDate(long& lStartDate, long& lEndDate) const;

	void Unload() noexcept {
		m_vHistoryData.clear();
		m_fDataLoaded = false;
	}
	CVirtualHistoryCandle* GetData(const size_t lIndex) { return &m_vHistoryData.at(lIndex); }
	vector<CVirtualHistoryCandle>& GetDataVector() { return m_vHistoryData; }
	CVirtualHistoryCandle* GetCandle(long lDate);

	void Add(CVirtualHistoryCandle data) {
		data.SetRatio(m_ratio);
		m_vHistoryData.push_back(data);
	}
	bool HaveDayLine(long lDate);

	bool IsDatabaseTodayUpdated() const noexcept { return (m_fDatabaseTodayUpdated); }
	void SetDatabaseTodayUpdated(const bool fUpdate) noexcept { m_fDatabaseTodayUpdated = fUpdate; }
	bool IsDataLoaded() const noexcept { return m_fDataLoaded.load(); }
	void SetDataLoaded(const bool fFlag) noexcept { m_fDataLoaded = fFlag; }

	void CalculateMA(size_t length);
	bool CalculateRS0();
	virtual bool CalculateRS1(INT64 lNumber);
	bool CalculateRSIndex0();
	virtual bool CalculateRSIndex1(INT64 lNumber);
	bool CalculateRSLogarithm0();
	virtual bool CalculateRSLogarithm1(INT64 lNumber);

	void GetRS1(vector<double>& vRS) const;
	void GetRSIndex1(vector<double>& vRS) const;
	void GetRSLogarithm1(vector<double>& vRS) const;
	void GetRS3(vector<double>& vRS) const;
	void GetRS5(vector<double>& vRS) const;
	void GetRS10(vector<double>& vRS) const;
	void GetRS30(vector<double>& vRS) const;
	void GetRS60(vector<double>& vRS) const;
	void GetRS120(vector<double>& vRS) const;

	// 显示此容器
	std::pair<long, long> GetHighLow(int iCandleNumber);
	void ToShow(CDC* pDC, CRect rectClient, int iStepWidth, long lHigh, long lLow);

	void ShowRS3(CDC* pDC, CPen* pNewPen, CRect rectDrawArea, int iStepWidth, double dZoomInRatio);
	void ShowRS5(CDC* pDC, CPen* pNewPen, CRect rectDrawArea, int iStepWidth, double dZoomInRatio);
	void ShowRS10(CDC* pDC, CPen* pNewPen, CRect rectDrawArea, int iStepWidth, double dZoomInRatio);
	void ShowRS30(CDC* pDC, CPen* pNewPen, CRect rectDrawArea, int iStepWidth, double dZoomInRatio);
	void ShowRS60(CDC* pDC, CPen* pNewPen, CRect rectDrawArea, int iStepWidth, double dZoomInRatio);
	void ShowRS120(CDC* pDC, CPen* pNewPen, CRect rectDrawArea, int iStepWidth, double dZoomInRatio);
	void ShowRSIndex(CDC* pDC, CPen* pNewPen, CRect rectDrawArea, int iStepWidth, double dZoomInRatio);
	void ShowRSLogarithm(CDC* pDC, CPen* pNewPen, CRect rectDrawArea, int iStepWidth, double dZoomInRatio);
	void ShowRS1(CDC* pDC, CPen* pNewPen, CRect rectDrawArea, int iStepWidth, double dZoomInRatio);
	void ShowLine(CDC* pDC, CPen* pNewPen, CRect rectDrawArea, int iStepWidth, const vector<double>& vData);

protected:
	bool UpdateBasicDB(CVirtualSetHistoryCandle* pSetHistoryCandle, const string& strStockSymbol = "");

protected:
	int m_ratio{ 0 };
	vector<CVirtualHistoryCandle> m_vHistoryData;
	atomic_bool m_fDataLoaded{ false }; // 数据装载与否标识
	bool m_fDatabaseTodayUpdated{ false }; // 数据库今日是否已更新标识
};

using CVirtualDataHistoryCandlePtr = shared_ptr<CVirtualDataHistoryCandle>;
