#pragma once

#include"RSReference.h"
#include"ChinaStock.h"

class CDataChinaStock : public CObject {
public:
	CDataChinaStock();
	// 只能有一个实例,不允许赋值。
	CDataChinaStock(const CDataChinaStock&) = delete;
	CDataChinaStock& operator=(const CDataChinaStock&) = delete;
	CDataChinaStock(const CDataChinaStock&&) noexcept = delete;
	CDataChinaStock& operator=(const CDataChinaStock&&) noexcept = delete;
	~CDataChinaStock() override = default;
	void Reset(void);

	[[nodiscard]] long GetIndex(const CString& strSymbol) const { return m_mapStock.at(strSymbol); }
	[[nodiscard]] long GetStockSize(void) const noexcept { return static_cast<long>(m_vStock.size()); }
	[[nodiscard]] long GetActiveStockSize(void) const;
	[[nodiscard]] long GetLoadedStockSize(void) const noexcept { return m_lLoadedStock; }

	[[nodiscard]] bool IsStock(const CString& strSymbol) const {
		if (m_mapStock.contains(strSymbol)) return true;
		else return false;
	}

	[[nodiscard]] bool IsAStock(const CString& strStockCode) const;
	[[nodiscard]] bool IsAStock(const not_null<CChinaStockPtr>& pStock) const { return (IsAStock(pStock->GetSymbol())); }
	CChinaStockPtr GetStock(const CString& strStockCode);
	CChinaStockPtr GetStock(const long lIndex);
	CString GetStockName(const CString& strStockCode);

	bool Delete(const CChinaStockPtr& pStock);
	bool Add(const CChinaStockPtr& pStock);

	long LoadStockCodeDB(void);
	bool UpdateStockCodeDB(void);
	bool UnloadDayLine(void) noexcept;
	bool BuildWeekLine(long lStartDate);
	long BuildDayLine(long lCurrentTradeDay);
	bool UpdateTodayTempDB(void);
	bool BuildDayLineRS(long lDate);
	bool BuildWeekLineRS(long lDate);

	[[nodiscard]] bool IsDayLineDBUpdated(void) const noexcept;
	[[nodiscard]] bool IsUpdateStockCodeDB(void) const;
	[[nodiscard]] bool IsDayLineNeedUpdate(void) const noexcept;
	void SetAllDayLineNeedMaintain(void);
	void SetAllDayLineNeedUpdate(void);
	[[nodiscard]] bool IsDayLineNeedSaving(void) const;

	void ClearDayLineDBUpdatedFlag(void) const noexcept;
	void ClearDayLineNeedUpdateStatus(void) const;

	[[nodiscard]] INT64 GetTotalAttackBuyAmount(void) const;
	[[nodiscard]] INT64 GetTotalAttackSellAmount(void) const;

	CString CreateNeteaseDayLineInquiringStr();
	CString GetNextStockInquiringMiddleStr(long& iStockIndex, CString strPostfix, long lTotalNumber) const;

	CString GetNextSinaStockInquiringMiddleStr(const long lTotalNumber) { return GetNextStockInquiringMiddleStr(m_lSinaRTDataInquiringIndex, _T(","), lTotalNumber); }
	CString GetNextTengxunStockInquiringMiddleStr(const long lTotalNumber) { return GetNextStockInquiringMiddleStr(m_lTengxunRTDataInquiringIndex, _T(","), lTotalNumber); }
	CString GetNextNeteaseStockInquiringMiddleStr(const long lTotalNumber);

	[[nodiscard]] bool TaskProcessRTData(void) const;
	bool TaskSaveDayLineData(void);

	[[nodiscard]] long GetDayLineNeedUpdateNumber(void);
	[[nodiscard]] long GetDayLineNeedSaveNumber(void);

	bool Choice10RSStrong2StockSet(void);
	bool Choice10RSStrong1StockSet(void);
	bool Choice10RSStrongStockSet(CRSReference* pRef, int iIndex);

	bool SortStockVector(void);
	[[nodiscard]] double GetUpDownRate(const CString& strClose, const CString& strLastClose) noexcept;

	void SetNeteaseDayLineDataInquiringIndex(const long lIndex) noexcept { m_lNeteaseDayLineDataInquiringIndex = lIndex; }
	[[nodiscard]] long GetNeteaseDayLineDataInquiringIndex(void) const noexcept { return m_lNeteaseDayLineDataInquiringIndex; }
	void SetNeteaseRTDataInquiringIndex(const long lIndex) noexcept { m_lNeteaseRTDataInquiringIndex = lIndex; }
	[[nodiscard]] long GetNeteaseRTDataInquiringIndex(void) const noexcept { return m_lNeteaseRTDataInquiringIndex; }
	void SetSinaRTDataInquiringIndex(const long lIndex) noexcept { m_lSinaRTDataInquiringIndex = lIndex; }
	[[nodiscard]] long GetSinaRTDataInquiringIndex(void) const noexcept { return m_lSinaRTDataInquiringIndex; }
	void SetTengxunRTDataInquiringIndex(const long lIndex) noexcept { m_lTengxunRTDataInquiringIndex = lIndex; }
	[[nodiscard]] long GetTengxunRTDataInquiringIndex(void) const noexcept { return m_lTengxunRTDataInquiringIndex; }
	[[nodiscard]] long GetNextIndex(long lIndex) const;

private:
	bool DeleteDayLineBasicInfo(long lDate);
	bool DeleteDayLineExtendInfo(long lDate);
	bool DeleteTodayTempDB(void);

protected:
	vector<CChinaStockPtr> m_vStock; // 本系统允许的所有股票池（无论代码是否存在）
	map<CString, size_t> m_mapStock; // 将所有被查询的股票代码映射为偏移量（目前只接受A股信息）
	long m_lLoadedStock; // 本次装载的股票总数

	long m_lNeteaseDayLineDataInquiringIndex;
	long m_lNeteaseRTDataInquiringIndex;
	long m_lSinaRTDataInquiringIndex;
	long m_lTengxunRTDataInquiringIndex;
};
