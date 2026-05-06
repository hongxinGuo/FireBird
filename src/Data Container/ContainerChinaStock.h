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

	void ProcessRTData();

	long LoadStockProfileDB();
	void UpdateStockProfileDB();
	void UnloadDayLine() noexcept;
	bool BuildWeekLine(long lStartDate);
	long BuildDayLine(long lCurrentTradeDay);
	bool BuildDayLineRS(long lDate);
	bool BuildWeekLineRS(long lDate);

	[[nodiscard]] bool IsDayLineDBUpdated() noexcept;

	void SetDayLineNeedMaintain() const;
	void SetUpdateDayLine() const;

	void ClearDayLineDBUpdatedFlag() noexcept;
	void ClearDayLineNeedUpdateStatus() const;

	string CreateNeteaseDayLineInquiringStr();
	string CreateTengxunDayLineInquiringStr();

	bool TaskUpdateDayLineDB();

	[[nodiscard]] long GetDayLineNeedSaveNumber() const;

	bool Choice10RSStrong1StockSet();
	bool Choice10RSStrongStockSet(CRSReference* pRef, int iIndex);

	[[nodiscard]] static double GetUpDownRate(const string& strClose, const string& strLastClose) noexcept;

private:
	static void DeleteDayLineBasicInfo(long lDate);

protected:
	size_t m_lLoadedStock; // 本次装载的股票总数
};
