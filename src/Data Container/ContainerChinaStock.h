#pragma once

#include"RSReference.h"
#include"ChinaStock.h"
#include"ContainerVirtualStock.h"

class CContainerChinaStock : public CContainerVirtualStock {
public:
	CContainerChinaStock();
	// 只能有一个实例,不允许赋值。
	CContainerChinaStock(const CContainerChinaStock&) = delete;
	CContainerChinaStock& operator=(const CContainerChinaStock&) = delete;
	CContainerChinaStock(const CContainerChinaStock&&) noexcept = delete;
	CContainerChinaStock& operator=(const CContainerChinaStock&&) noexcept = delete;
	~CContainerChinaStock() override;
	void Reset() override;

	[[nodiscard]] size_t GetActiveStockSize() const;
	[[nodiscard]] auto GetLoadedStockSize() const noexcept { return m_lLoadedStock; }

	CChinaStockPtr GetStock(const string& strStockCode) { return dynamic_pointer_cast<CChinaStock>(Get(strStockCode)); }
	CChinaStockPtr GetStock(const size_t lIndex) { return dynamic_pointer_cast<CChinaStock>(Get(lIndex)); }
	string GetStockName(const string& strStockCode);

	long LoadStockProfileDB();
	void UpdateStockProfileDB();
	void UnloadDayLine() noexcept;
	bool BuildWeekLine(long lStartDate);
	long BuildDayLine(long lCurrentTradeDay);
	void UpdateTempRTDB();
	static void DeleteTempRTData();
	bool BuildDayLineRS(long lDate);
	bool BuildWeekLineRS(long lDate);

	[[nodiscard]] bool IsDayLineDBUpdated() noexcept;

	void SetDayLineNeedMaintain() const;
	void SetUpdateDayLine() const;

	void ClearDayLineDBUpdatedFlag() noexcept;
	void ClearDayLineNeedUpdateStatus() const;

	[[nodiscard]] INT64 GetTotalAttackBuyAmount();
	[[nodiscard]] INT64 GetTotalAttackSellAmount();

	string CreateNeteaseDayLineInquiringStr();
	string CreateTengxunDayLineInquiringStr();

	void ProcessRTData();
	bool TaskUpdateDayLineDB();

	[[nodiscard]] long GetDayLineNeedSaveNumber() const;

	bool Choice10RSStrong2StockSet();
	bool Choice10RSStrong1StockSet();
	bool Choice10RSStrongStockSet(CRSReference* pRef, int iIndex);

	[[nodiscard]] static double GetUpDownRate(const CString& strClose, const CString& strLastClose) noexcept;

private:
	static void DeleteDayLineBasicInfo(long lDate);
	static void DeleteDayLineExtendInfo(long lDate);

protected:
	size_t m_lLoadedStock; // 本次装载的股票总数
};
