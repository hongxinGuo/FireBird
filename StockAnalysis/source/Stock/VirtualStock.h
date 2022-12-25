#pragma once

#include"VirtualSetStockSymbol.h"

#include<memory>
#include<atomic>

class CVirtualStock;
using CVirtualStockPtr = shared_ptr<CVirtualStock>;

class CVirtualStock : public CObject {
public:
	CVirtualStock();
	// 不允许赋值。
	CVirtualStock(const CVirtualStock&) = delete;
	CVirtualStock& operator=(const CVirtualStock&) = delete;
	CVirtualStock(const CVirtualStock&&) noexcept = delete;
	CVirtualStock& operator=(const CVirtualStock&&) noexcept = delete;
	~CVirtualStock() override = default;
	virtual void Reset(void);
	virtual int GetRatio(void) const = 0;

	virtual void LoadSymbol(CVirtualSetStockSymbol& setStockSymbol);
	virtual void AppendSymbol(CVirtualSetStockSymbol& setStockSymbol);
	virtual void UpdateSymbol(CVirtualSetStockSymbol& setStockSymbol);
	virtual void SaveSymbol(CVirtualSetStockSymbol& setStockSymbol);

public:
	void UpdateRealTimeData(CVirtualStockPtr pStock);

	CString GetDescription(void) const noexcept { return m_strDescription; }
	void SetDescription(const CString& strDescription) noexcept { m_strDescription = strDescription; }
	CString GetExchangeCode(void) const noexcept { return m_strExchangeCode; }
	void SetExchangeCode(const CString& strExchangeCode) noexcept { m_strExchangeCode = strExchangeCode; }
	CString GetSymbol(void) const noexcept { return m_strSymbol; }
	void SetSymbol(const CString& str) noexcept { m_strSymbol = str; }
	CString GetDisplaySymbol(void) const noexcept { return m_strDisplaySymbol; }
	void SetDisplaySymbol(const CString& str) noexcept { m_strDisplaySymbol = str; }

	// 基本实时数据，需要更新
	time_t GetTransactionTime(void) const noexcept { return m_TransactionTime; }
	virtual void SetTransactionTime(const time_t time) noexcept { m_TransactionTime = time; }
	long GetLastClose(void) const noexcept { return m_lLastClose; }
	virtual void SetLastClose(const long lValue) noexcept { m_lLastClose = lValue; }
	long GetOpen(void) const noexcept { return m_lOpen; }
	virtual void SetOpen(const long lValue) noexcept { m_lOpen = lValue; }
	long GetHigh(void) const noexcept { return m_lHigh; }
	virtual void SetHigh(const long lValue) noexcept { m_lHigh = lValue; }
	long GetLow(void) const noexcept { return m_lLow; }
	virtual void SetLow(const long lValue) noexcept { m_lLow = lValue; }
	long GetNew(void) const noexcept { return m_lNew; }
	virtual void SetNew(const long lValue) noexcept { m_lNew = lValue; }
	INT64 GetAmount(void) const noexcept { return m_llAmount; }
	virtual void SetAmount(const INT64 llValue) noexcept { m_llAmount = llValue; }
	INT64 GetVolume(void) const noexcept { return m_llVolume; }
	virtual void SetVolume(const INT64 llValue) noexcept { m_llVolume = llValue; }

	long GetUpDown(void) const noexcept { return m_lUpDown; }
	virtual void SetUpDown(const long lValue) noexcept { m_lUpDown = lValue; }
	double GetUpDownRate(void) const noexcept { return m_dUpDownRate; }
	virtual void SetUpDownRate(const double dValue) noexcept { m_dUpDownRate = dValue; }
	double GetChangeHandRate(void) const noexcept { return m_dChangeHandRate; }
	virtual void SetChangeHandRate(const double dValue) noexcept { m_dChangeHandRate = dValue; }
	INT64 GetTotalValue(void) const noexcept { return m_llTotalValue; }
	virtual void SetTotalValue(const INT64 llValue) noexcept { m_llTotalValue = llValue; }
	INT64 GetCurrentValue(void) const noexcept { return m_llCurrentValue; }
	virtual void SetCurrentValue(const INT64 llValue) noexcept { m_llCurrentValue = llValue; }

	//
	bool IsTodayNewStock(void) const noexcept { return m_fTodayNewStock; }
	void SetTodayNewStock(const bool fFlag) noexcept { m_fTodayNewStock = fFlag; }
	bool IsUpdateProfileDB(void) const noexcept { return m_fUpdateProfileDB; }

	bool IsUpdateProfileDBAndClearFlag(void) noexcept {
		const bool fNeedSaving = m_fUpdateProfileDB.exchange(false);
		return fNeedSaving;
	}

	void SetUpdateProfileDB(const bool fFlag) noexcept { m_fUpdateProfileDB = fFlag; }
	bool IsUpdateCompanyNewsDB(void) const noexcept { return m_fUpdateCompanyNewsDB; }

	bool IsUpdateCompanyNewsDBAndClearFlag(void) noexcept {
		const bool fNeedSaving = m_fUpdateCompanyNewsDB.exchange(false);
		return fNeedSaving;
	}

	void SetUpdateCompanyNewsDB(const bool fFlag) noexcept { m_fUpdateCompanyNewsDB = fFlag; }
	bool IsClearDayLineVector(void) const noexcept { return m_fClearDayLineVector; }

	bool IsClearDayLineVectorAndClearFlag(void) noexcept {
		const bool fNeedSaving = m_fClearDayLineVector.exchange(false);
		return fNeedSaving;
	}

	void SetClearDayLineVector(const bool fFlag) noexcept { m_fClearDayLineVector = fFlag; }
	bool IsActive(void) const noexcept { return m_fActive; }
	void SetActive(const bool fFlag) noexcept { m_fActive = fFlag; }

	long GetDayLineStartDate(void) const noexcept { return m_lDayLineStartDate; }
	void SetDayLineStartDate(const long lDate) noexcept { m_lDayLineStartDate = lDate; }
	long GetDayLineEndDate(void) const noexcept { return m_lDayLineEndDate; }
	void SetDayLineEndDate(const long lDate) noexcept { m_lDayLineEndDate = lDate; }

	long GetIPOStatus(void) const noexcept { return m_lIPOStatus; }
	void SetIPOStatus(const long lValue) noexcept { m_lIPOStatus = lValue; }
	bool IsDelisted(void) const noexcept { return (m_lIPOStatus == _STOCK_DELISTED_); }
	bool IsNotYetList(void) const noexcept { return (m_lIPOStatus == _STOCK_NOT_YET_LIST_); }
	bool IsNullStock(void) const noexcept { return (m_lIPOStatus == _STOCK_NULL_); }
	bool IsIPOed(void) const noexcept { return (m_lIPOStatus == _STOCK_IPOED_); }
	bool IsNotChecked(void) const noexcept { return (m_lIPOStatus == _STOCK_NOT_CHECKED_); }

	// 由于处理日线历史数据的函数位于不同的线程中，故而需要同步机制设置标识
	bool IsDayLineNeedUpdate(void) const noexcept { return m_fDayLineNeedUpdate; }
	void SetDayLineNeedUpdate(const bool fFlag) noexcept { m_fDayLineNeedUpdate = fFlag; }
	bool IsDayLineNeedSaving(void) const noexcept { return m_fDayLineNeedSaving; }
	void SetDayLineNeedSaving(const bool fFlag) noexcept { m_fDayLineNeedSaving = fFlag; }

	bool IsDayLineNeedSavingAndClearFlag(void) noexcept {
		const bool fNeedSaving = m_fDayLineNeedSaving.exchange(false);
		return fNeedSaving;
	}

protected:
	CString m_strDescription; // 该证券的描述
	CString m_strExchangeCode; // 证券所属交易所。美国为US，上海为SS，深圳为SZ；外汇为forex等。
	CString m_strSymbol; // 股票代码。二十位以内，后两位为市场前缀。如600601.SS，000001.SZ, AAPL.US, RIG.US
	CString m_strDisplaySymbol;

	// 实时数据区
	time_t m_TransactionTime; // 实时数据交易时间
	long m_lLastClose; // 以0.001元计的收盘价
	long m_lOpen; // 以0.001元计的开盘价
	long m_lHigh; // 以0.001元计的最高价
	long m_lLow; // 以0.001元计的最低价
	long m_lNew; // 以0.001元计的最新价
	long m_lUpDown; // 涨跌值
	double m_dUpDownRate; // 涨跌率
	INT64 m_llVolume; // 以1股计的成交量
	INT64 m_llAmount; // 以元计的成交金额
	double m_dChangeHandRate; // 换手率
	INT64 m_llTotalValue; // 总市值。单位：万元
	INT64 m_llCurrentValue; // 流通市值。单位：万元

	bool m_fTodayNewStock; // 本日新发现的股票
	atomic_bool m_fUpdateProfileDB;
	atomic_bool m_fUpdateCompanyNewsDB;
	atomic_bool m_fClearDayLineVector;
	bool m_fActive; // 是否本日内有数据读入。由新浪实时行情处理函数和网易日线历史数据处理函数来设置。
	long m_lDayLineStartDate; // 日线数据起始日。这个是处理日线历史数据时得到的起始交易日，
	long m_lDayLineEndDate; // 日线数据更新日。这个是处理日线历史数据时得到的最新日，
	long m_lIPOStatus; // 通过网易历史日线查询，如果只有前缀信息而没有实际内容，可以确认没有实际交易。在这种情况下，新浪实时行情有数据，只是为零而已。默认情况下为已上市
	// 未上市（无效股票代码）为_STOCK_NULL_；正常为_STOCK_IPOED_；已通过IPO但尚未上市或退市为_STOCK_DELISTED；其他情况尚未出现，留待以后处理。

	atomic_bool m_fDayLineNeedUpdate; // 日线需要更新。默认为真
	atomic_bool m_fDayLineNeedSaving; // 日线历史数据已处理，等待存储。
};
