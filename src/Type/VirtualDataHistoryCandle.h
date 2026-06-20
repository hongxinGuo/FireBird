////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// 股票实例数据容器（日线容器、周线容器等）的基类。
// 纯虚类，不允许生成实例。
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#pragma once

#include "DayLine.h"
#include "VirtualHistoryCandle.h"

class CVirtualDataHistoryCandle {
public:
	CVirtualDataHistoryCandle();
	virtual ~CVirtualDataHistoryCandle() = default;

	void Reset(); // 这些实现类需要采用这种方法重置内部状态，因为系统会一直运行，每天都需要重置状态。
	auto Size() const noexcept { return m_vHistoryData.size(); }
	bool Empty() const noexcept { return m_vHistoryData.empty(); }
	void Reserve(size_t size) { m_vHistoryData.reserve(size); }

	// 所有的派生类皆需要定义此两个存储和提取函数，不允许调用此基类函数
	virtual void SaveDB(const string&) { ASSERT(0); }
	virtual void LoadDB(const string&) { ASSERT(0); }

	void UpdateData(const vector<CVirtualHistoryCandle>& vTempData);
	void UpdateData(const vector<CDayLine>& vTempData);
	void UpdateData(const CDayLinesPtr& pvTempDayLine);

	int GetRatio() const {
		if (m_ratio == 0)
			ASSERT(0);
		return m_ratio;
	}

	vector<CVirtualHistoryCandle>* GetContainer() noexcept { return &m_vHistoryData; }

	bool GetStartEndDate(chrono::local_days& lStartDate, chrono::local_days& lEndDate) const;

	void Unload() noexcept {
		m_vHistoryData.clear();
		m_fDataLoaded = false;
	}
	CVirtualHistoryCandle* GetData(const size_t lIndex) { return &m_vHistoryData.at(lIndex); }
	vector<CVirtualHistoryCandle>& GetDataVector() { return m_vHistoryData; }
	CVirtualHistoryCandle* GetCandle(chrono::local_days date);
	CVirtualHistoryCandle* GetCandle2(chrono::local_days date);

	void Add(CVirtualHistoryCandle data) {
		data.SetRatio(m_ratio);
		m_vHistoryData.push_back(data);
	}
	void Add(const CDayLine& data) { Add(static_cast<CVirtualHistoryCandle>(data)); }
	bool HaveDayLine(chrono::local_days date);

	bool IsDatabaseTodayUpdated() const noexcept { return (m_fDatabaseTodayUpdated); }
	void SetDatabaseTodayUpdated(const bool fUpdate) noexcept { m_fDatabaseTodayUpdated = fUpdate; }
	bool IsDataLoaded() const noexcept { return m_fDataLoaded.load(); }
	void SetDataLoaded(const bool fFlag) noexcept { m_fDataLoaded = fFlag; }

	void CalculateMA(size_t length);

	// 显示此容器
	std::pair<long, long> GetHighLow(int iCandleNumber);
	void ToShow(CDC* pDC, CRect rectClient, int iStepWidth, long lHigh, long lLow);

	void ShowLine(CDC* pDC, CPen* pNewPen, CRect rectDrawArea, int iStepWidth, const vector<double>& vData);

protected:
	int m_ratio{ 0 };
	vector<CVirtualHistoryCandle> m_vHistoryData;
	atomic_bool m_fDataLoaded{ false }; // 数据装载与否标识
	bool m_fDatabaseTodayUpdated{ false }; // 数据库今日是否已更新标识
};

using CVirtualDataHistoryCandlePtr = shared_ptr<CVirtualDataHistoryCandle>;
