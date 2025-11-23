#pragma once

#include "VirtualDataHistoryCandleExtend.h"
#include"VirtualSetStockSymbol.h"

using std::shared_ptr;
using std::atomic_bool;

class CVirtualStock;
using CVirtualStockPtr = shared_ptr<CVirtualStock>;

class CVirtualStock {
public:
	CVirtualStock();
	// 不允许赋值。
	CVirtualStock(const CVirtualStock&) = delete;
	CVirtualStock& operator=(const CVirtualStock&) = delete;
	CVirtualStock(const CVirtualStock&&) noexcept = delete;
	CVirtualStock& operator=(const CVirtualStock&&) noexcept = delete;
	virtual ~CVirtualStock() = default;

	virtual void ResetAllUpdateDate();
	virtual int GetRatio() const = 0;

	void LoadUpdateDate(const string& strUpdateDate);
	virtual void LoadSymbol(CVirtualSetStockSymbol& setStockSymbol);
	virtual void AppendSymbol(CVirtualSetStockSymbol& setStockSymbol);
	virtual void UpdateSymbol(CVirtualSetStockSymbol& setStockSymbol);
	virtual void SaveSymbol(CVirtualSetStockSymbol& setStockSymbol);

	string GetDescription() const noexcept { return m_strDescription; }
	void SetDescription(const string& strDescription) noexcept { m_strDescription = strDescription; }
	string GetExchangeCode() const noexcept { return m_strExchangeCode; }
	void SetExchangeCode(const string& strExchangeCode) noexcept { m_strExchangeCode = strExchangeCode; }
	string GetSymbol() const noexcept { return m_strSymbol; }
	void SetSymbol(const string& str) noexcept {
		m_strSymbol = str;
		const string s = m_strSymbol.substr(0, 6);
	}
	string GetDisplaySymbol() const noexcept { return m_strDisplaySymbol; }
	void SetDisplaySymbol(const string& str) noexcept { m_strDisplaySymbol = str; }

	// 基本实时数据，需要更新
	std::chrono::sys_seconds GetTimePoint() const noexcept { return m_tpTime; }
	virtual void SetTimePoint(const std::chrono::sys_seconds time) noexcept { m_tpTime = time; }
	time_t GetTransactionTime() const noexcept { return m_tpTime.time_since_epoch().count(); }
	virtual void SetTransactionTime(const time_t time) noexcept { m_tpTime = std::chrono::time_point_cast<std::chrono::seconds>(std::chrono::system_clock::from_time_t(time)); }
	long GetLastClose() const noexcept { return m_lLastClose; }
	virtual void SetLastClose(const long lValue) noexcept { m_lLastClose = lValue; }
	long GetOpen() const noexcept { return m_lOpen; }
	virtual void SetOpen(const long lValue) noexcept { m_lOpen = lValue; }
	long GetHigh() const noexcept { return m_lHigh; }
	virtual void SetHigh(const long lValue) noexcept { m_lHigh = lValue; }
	long GetLow() const noexcept { return m_lLow; }
	virtual void SetLow(const long lValue) noexcept { m_lLow = lValue; }
	long GetNew() const noexcept { return m_lNew; }
	virtual void SetNew(const long lValue) noexcept { m_lNew = lValue; }
	INT64 GetAmount() const noexcept { return m_llAmount; }
	virtual void SetAmount(const INT64 llValue) noexcept { m_llAmount = llValue; }
	INT64 GetVolume() const noexcept { return m_llVolume; }
	virtual void SetVolume(const INT64 llValue) noexcept { m_llVolume = llValue; }

	long GetUpDown() const noexcept { return m_lUpDown; }
	virtual void SetUpDown(const long lValue) noexcept { m_lUpDown = lValue; }
	double GetUpDownRate() const noexcept { return m_dUpDownRate; }
	virtual void SetUpDownRate(const double dValue) noexcept { m_dUpDownRate = dValue; }
	double GetChangeHandRate() const noexcept { return m_dChangeHandRate; }
	virtual void SetChangeHandRate(const double dValue) noexcept { m_dChangeHandRate = dValue; }
	INT64 GetTotalValue() const noexcept { return m_llTotalValue; }
	virtual void SetTotalValue(const INT64 llValue) noexcept { m_llTotalValue = llValue; }
	INT64 GetCurrentValue() const noexcept { return m_llCurrentValue; }
	virtual void SetCurrentValue(const INT64 llValue) noexcept { m_llCurrentValue = llValue; }

	//
	bool IsTodayNewStock() const noexcept { return m_fTodayNewStock; }
	void SetTodayNewStock(const bool fFlag) noexcept { m_fTodayNewStock = fFlag; }

	bool IsActive() const noexcept { return m_fActive; }
	void SetActive(const bool fFlag) noexcept { m_fActive = fFlag; }

	virtual CVirtualDataHistoryCandleExtend* DayLine() noexcept {
		ASSERT(0);
		return nullptr;
	}
	virtual CVirtualDataHistoryCandleExtend* WeekLine() noexcept {
		ASSERT(0);
		return nullptr;
	}
	virtual CVirtualDataHistoryCandleExtend* MonthLine() noexcept {
		ASSERT(0);
		return nullptr;
	}

	void LoadHistoryCandleDB() {
		LoadDayLineDB();
		LoadWeekLineDB();
	}
	virtual bool LoadDayLineDB() {
		ASSERT(0);
		return false;
	}
	virtual bool LoadWeekLineDB() {
		ASSERT(0);
		return false;
	}

	long GetDayLineStartDate();
	void SetDayLineStartDate(const long lDate) noexcept { m_jsonUpdateDate[_T("DayLineStartDate")] = lDate; }
	long GetDayLineEndDate();
	void SetDayLineEndDate(const long lDate) noexcept { m_jsonUpdateDate[_T("DayLineEndDate")] = lDate; }

	long GetIPOStatus() const noexcept { return m_lIPOStatus; }
	void SetIPOStatus(const long lValue) noexcept { m_lIPOStatus = lValue; }
	bool IsDelisted() const noexcept { return (m_lIPOStatus == _STOCK_DELISTED_); }
	bool IsNotYetList() const noexcept { return (m_lIPOStatus == _STOCK_NOT_YET_LIST_); }
	bool IsNullStock() const noexcept { return (m_lIPOStatus == _STOCK_NULL_); }
	bool IsIPOed() const noexcept { return (m_lIPOStatus == _STOCK_IPOED_); }
	bool IsNotChecked() const noexcept { return (m_lIPOStatus == _STOCK_NOT_CHECKED_); }

	// 由于处理日线历史数据的函数位于不同的线程中，故而需要同步机制设置标识
	bool IsUpdateDayLine() const noexcept { return m_fUpdateDayLine; }
	void SetUpdateDayLine(const bool fFlag) noexcept { m_fUpdateDayLine = fFlag; }

	bool IsUpdateDayLineDB() const noexcept { return m_fUpdateDayLineDB; }
	void SetUpdateDayLineDB(const bool fFlag) noexcept { m_fUpdateDayLineDB = fFlag; }
	bool IsUpdateProfileDB() const noexcept { return m_fUpdateProfileDB; }
	void SetUpdateProfileDB(const bool fFlag) noexcept { m_fUpdateProfileDB = fFlag; }

	bool IsUpdateCompanyNewsDB() const noexcept { return m_fUpdateCompanyNewsDB; }
	void SetUpdateCompanyNewsDB(const bool fFlag) noexcept { m_fUpdateCompanyNewsDB = fFlag; }

	bool IsSameStock(const CVirtualStockPtr& pStock) const;

	virtual bool IsDayLineLoaded() const noexcept {
		ASSERT(0);
		return false;
	}
	virtual void SetDayLineLoaded(const bool fFlag) noexcept { ASSERT(0); }
	virtual bool IsWeekLineLoaded() const noexcept {
		ASSERT(0);
		return false;
	}
	virtual void SetWeekLineLoaded(const bool fFlag) noexcept { ASSERT(0); }

	void SetSelected(bool flag) noexcept { m_bSelected = flag; }
	bool IsSelected() const noexcept { return m_bSelected; }

	void ShowDayLineRS3(CDC* pDC, CPen* pNewPen, CRect rectDrawArea, int iStepWidth, double dZoomInRatio) {
		DayLine()->ShowRS3(pDC, pNewPen, rectDrawArea, iStepWidth, dZoomInRatio);
	}
	void ShowDayLineRS5(CDC* pDC, CPen* pNewPen, CRect rectDrawArea, int iStepWidth, double dZoomInRatio) {
		DayLine()->ShowRS5(pDC, pNewPen, rectDrawArea, iStepWidth, dZoomInRatio);
	}
	void ShowDayLineRS10(CDC* pDC, CPen* pNewPen, CRect rectDrawArea, int iStepWidth, double dZoomInRatio) {
		DayLine()->ShowRS10(pDC, pNewPen, rectDrawArea, iStepWidth, dZoomInRatio);
	}
	void ShowDayLineRS30(CDC* pDC, CPen* pNewPen, CRect rectDrawArea, int iStepWidth, double dZoomInRatio) {
		DayLine()->ShowRS30(pDC, pNewPen, rectDrawArea, iStepWidth, dZoomInRatio);
	}
	void ShowDayLineRS60(CDC* pDC, CPen* pNewPen, CRect rectDrawArea, int iStepWidth, double dZoomInRatio) {
		DayLine()->ShowRS60(pDC, pNewPen, rectDrawArea, iStepWidth, dZoomInRatio);
	}
	void ShowDayLineRS120(CDC* pDC, CPen* pNewPen, CRect rectDrawArea, int iStepWidth, double dZoomInRatio) {
		DayLine()->ShowRS120(pDC, pNewPen, rectDrawArea, iStepWidth, dZoomInRatio);
	}
	void ShowDayLineRSIndex(CDC* pDC, CPen* pNewPen, CRect rectDrawArea, int iStepWidth, double dZoomInRatio) {
		DayLine()->ShowRSIndex(pDC, pNewPen, rectDrawArea, iStepWidth, dZoomInRatio);
	}
	void ShowDayLineRSLogarithm(CDC* pDC, CPen* pNewPen, CRect rectDrawArea, int iStepWidth, double dZoomInRatio) {
		DayLine()->ShowRSLogarithm(pDC, pNewPen, rectDrawArea, iStepWidth, dZoomInRatio);
	}
	void ShowDayLineRS1(CDC* pDC, CPen* pNewPen, CRect rectDrawArea, int iStepWidth, double dZoomInRatio) {
		DayLine()->ShowRS1(pDC, pNewPen, rectDrawArea, iStepWidth, dZoomInRatio);
	}

	void ShowWeekLineRS3(CDC* pDC, CPen* pNewPen, CRect rectDrawArea, int iStepWidth, double dZoomInRatio) {
		WeekLine()->ShowRS3(pDC, pNewPen, rectDrawArea, iStepWidth, dZoomInRatio);
	}
	void ShowWeekLineRS5(CDC* pDC, CPen* pNewPen, CRect rectDrawArea, int iStepWidth, double dZoomInRatio) {
		WeekLine()->ShowRS5(pDC, pNewPen, rectDrawArea, iStepWidth, dZoomInRatio);
	}
	void ShowWeekLineRS10(CDC* pDC, CPen* pNewPen, CRect rectDrawArea, int iStepWidth, double dZoomInRatio) {
		WeekLine()->ShowRS10(pDC, pNewPen, rectDrawArea, iStepWidth, dZoomInRatio);
	}
	void ShowWeekLineRS30(CDC* pDC, CPen* pNewPen, CRect rectDrawArea, int iStepWidth, double dZoomInRatio) {
		WeekLine()->ShowRS30(pDC, pNewPen, rectDrawArea, iStepWidth, dZoomInRatio);
	}
	void ShowWeekLineRS60(CDC* pDC, CPen* pNewPen, CRect rectDrawArea, int iStepWidth, double dZoomInRatio) {
		WeekLine()->ShowRS60(pDC, pNewPen, rectDrawArea, iStepWidth, dZoomInRatio);
	}
	void ShowWeekLineRS120(CDC* pDC, CPen* pNewPen, CRect rectDrawArea, int iStepWidth, double dZoomInRatio) {
		WeekLine()->ShowRS120(pDC, pNewPen, rectDrawArea, iStepWidth, dZoomInRatio);
	}
	void ShowWeekLineRSIndex(CDC* pDC, CPen* pNewPen, CRect rectDrawArea, int iStepWidth, double dZoomInRatio) {
		WeekLine()->ShowRSIndex(pDC, pNewPen, rectDrawArea, iStepWidth, dZoomInRatio);
	}
	void ShowWeekLineRSLogarithm(CDC* pDC, CPen* pNewPen, CRect rectDrawArea, int iStepWidth, double dZoomInRatio) {
		WeekLine()->ShowRSLogarithm(pDC, pNewPen, rectDrawArea, iStepWidth, dZoomInRatio);
	}
	void ShowWeekLineRS1(CDC* pDC, CPen* pNewPen, CRect rectDrawArea, int iStepWidth, double dZoomInRatio) {
		WeekLine()->ShowRS1(pDC, pNewPen, rectDrawArea, iStepWidth, dZoomInRatio);
	}

protected:
	string m_strDescription{ _T("") }; // 该证券的描述
	string m_strExchangeCode{ _T("") }; // 证券所属交易所。美国为US，上海为SS，深圳为SZ；外汇为forex等。
	string m_strSymbol{ _T("") }; // 股票代码。二十位以内，后两位为市场前缀。如600601.SS，000001.SZ, AAPL（美国股票没有后缀）
	string m_strDisplaySymbol{ _T("") };

	json m_jsonUpdateDate{ json({}) }; // 存储所有的更新日期（json格式）。使用这种方式存储后，当增加或减少更新日期时，无需修改相应数据表的结构。

	// 实时数据区
	std::chrono::sys_seconds m_tpTime{};// 实时数据交易时间
	long m_lLastClose{ 0 }; // 以0.001元计的收盘价
	long m_lOpen{ 0 }; // 以0.001元计的开盘价
	long m_lHigh{ 0 }; // 以0.001元计的最高价
	long m_lLow{ 0 }; // 以0.001元计的最低价
	long m_lNew{ 0 }; // 以0.001元计的最新价
	long m_lUpDown{ 0 }; // 涨跌值
	double m_dUpDownRate{ 0 }; // 涨跌率
	INT64 m_llVolume{ 0 }; // 以1股计的成交量
	INT64 m_llAmount{ 0 }; // 以元计的成交金额
	double m_dChangeHandRate{ 0 }; // 换手率
	INT64 m_llTotalValue{ 0 }; // 总市值。单位：万元
	INT64 m_llCurrentValue{ 0 }; // 流通市值。单位：万元

	bool m_fTodayNewStock{ false }; // 本日新发现的股票
	bool m_fActive{ false }; // 是否是活跃股票
	long m_lIPOStatus{ _STOCK_NOT_CHECKED_ }; // 通过网易历史日线查询，如果只有前缀信息而没有实际内容，可以确认没有实际交易。在这种情况下，新浪实时行情有数据，只是为零而已。默认情况下为已上市
	// 未上市（无效股票代码）为_STOCK_NULL_；正常为_STOCK_IPOED_；已通过IPO但尚未上市或退市为_STOCK_DELISTED；其他情况尚未出现，留待以后处理。

	bool m_bSelected{ false };

	atomic_bool m_fUpdateDayLine{ true }; // 日线需要更新。默认为真

	atomic_bool m_fUpdateProfileDB{ false }; // 更新股票简介
	atomic_bool m_fUpdateCompanyNewsDB{ false }; // 更新公司新闻
	atomic_bool m_fUpdateDayLineDB{ false }; // 日线历史数据已处理，等待存储。
};

extern CVirtualStockPtr gl_pCurrentStock;
