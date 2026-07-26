#pragma once

#include"ContainerVirtualStock.h"

class CChinaStock;

using std::chrono::local_days;

class CContainerChinaStock : public CContainerVirtualStock {
public:
	CContainerChinaStock();
	// 只能有一个实例,不允许赋值。
	CContainerChinaStock(const CContainerChinaStock&) = delete;
	CContainerChinaStock& operator=(const CContainerChinaStock&) = delete;
	CContainerChinaStock(const CContainerChinaStock&&) noexcept = delete;
	CContainerChinaStock& operator=(const CContainerChinaStock&&) noexcept = delete;
	~CContainerChinaStock() override = default;
	void Reset() override;

	[[nodiscard]] size_t GetActiveStockSize() const;
	[[nodiscard]] auto GetLoadedStockSize() const noexcept { return m_lLoadedStock; }

	CChinaStockPtr GetStock(const string& strStockCode);
	CChinaStockPtr GetStock(size_t lIndex);
	string GetStockName(const string& strStockCode);

	void ProcessRTData();

	long LoadProfileDB();
	void UpdateProfileDB();

	void UnloadDayLine() noexcept;
	long BuildDayLine(local_days currentTradeDay);

	[[nodiscard]] bool IsDayLineDBUpdated() noexcept;

	void SetDayLineNeedMaintain(int startDate);
	void SetUpdateDayLine() const;

	void ClearDayLineDBUpdatedFlag() noexcept;
	void ClearDayLineNeedUpdateStatus() const;

	string CreateTengxunDayLineInquiringStr();

	void TaskUpdateDayLineDB();

	[[nodiscard]] long GetDayLineNeedSaveNumber() const;

	[[nodiscard]] static double GetUpDownRate(const string& strClose, const string& strLastClose) noexcept;

private:
	static void DeleteDayLine(local_days date);

protected:
	size_t m_lLoadedStock; // 本次装载的股票总数
};

extern CContainerChinaStock gl_dataContainerChinaStock;
