#pragma once

#include"RSReference.h"
#include"ChinaStock.h"
#include"ContainerVirtualStock.h"

#include <memory>
using std::dynamic_pointer_cast;

class CContainerChinaStock : public CContainerVirtualStock {
public:
	CContainerChinaStock();
	// 只能有一个实例,不允许赋值。
	CContainerChinaStock(const CContainerChinaStock&) = delete;
	CContainerChinaStock& operator=(const CContainerChinaStock&) = delete;
	CContainerChinaStock(const CContainerChinaStock&&) noexcept = delete;
	CContainerChinaStock& operator=(const CContainerChinaStock&&) noexcept = delete;
	~CContainerChinaStock() override = default;
	void Reset(void) override;

	[[nodiscard]] long GetActiveStockSize(void) const;
	[[nodiscard]] long GetLoadedStockSize(void) const noexcept { return m_lLoadedStock; }

	static bool IsAStock(const CString& strStockCode);
	bool IsAStock(const CChinaStockPtr pStock) const { return IsAStock(pStock->GetSymbol()); }

	CChinaStockPtr GetStock(const CString& strStockCode) { return dynamic_pointer_cast<CChinaStock>(Get(strStockCode)); }
	CChinaStockPtr GetStock(const size_t lIndex) { return dynamic_pointer_cast<CChinaStock>(Get(lIndex)); }
	CString GetStockName(const CString& strStockCode);

	long LoadStockProfileDB(void);
	bool UpdateStockProfileDB(void);
	bool UnloadDayLine(void) noexcept;
	bool BuildWeekLine(long lStartDate);
	long BuildDayLine(long lCurrentTradeDay);
	bool UpdateTodayTempDB(void);
	bool DeleteTodayTempDB(void);
	bool BuildDayLineRS(long lDate);
	bool BuildWeekLineRS(long lDate);

	[[nodiscard]] bool IsDayLineDBUpdated(void) noexcept;

	void SetDayLineNeedMaintain(void) const;
	void SetDayLineNeedUpdate(void) const;

	void ClearDayLineDBUpdatedFlag(void) noexcept;
	void ClearDayLineNeedUpdateStatus(void) const;

	[[nodiscard]] INT64 GetTotalAttackBuyAmount(void);
	[[nodiscard]] INT64 GetTotalAttackSellAmount(void);

	CString CreateNeteaseDayLineInquiringStr();
	CString CreateTengxunDayLineInquiringStr();
	CString GetNextStockInquiringMiddleStr(long& iStockIndex, CString strPostfix, long lTotalNumber);

	CString GetNextSinaStockInquiringMiddleStr(const long lTotalNumber) { return GetNextStockInquiringMiddleStr(m_lSinaRTDataInquiringIndex, _T(","), lTotalNumber); }
	CString GetNextTengxunStockInquiringMiddleStr(const long lTotalNumber) { return GetNextStockInquiringMiddleStr(m_lTengxunRTDataInquiringIndex, _T(","), lTotalNumber); }
	CString GetNextNeteaseStockInquiringMiddleStr(long lTotalNumber);

	[[nodiscard]] bool ProcessRTData(void);
	bool SaveDayLineData(void);
	virtual void CreateThreadSaveDayLineBasicInfo(CChinaStockPtr pStock);

	[[nodiscard]] long GetDayLineNeedUpdateNumber(void) const;
	[[nodiscard]] long GetDayLineNeedSaveNumber(void) const;

	bool Choice10RSStrong2StockSet(void);
	bool Choice10RSStrong1StockSet(void);
	bool Choice10RSStrongStockSet(CRSReference* pRef, int iIndex);

	[[nodiscard]] static double GetUpDownRate(const CString& strClose, const CString& strLastClose) noexcept;

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

protected:
	long m_lLoadedStock; // 本次装载的股票总数

	long m_lNeteaseRTDataInquiringIndex;
	long m_lSinaRTDataInquiringIndex;
	long m_lTengxunRTDataInquiringIndex;
};
