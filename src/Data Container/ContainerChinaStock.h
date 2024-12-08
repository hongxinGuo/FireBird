#pragma once

#include"ChinaStock.h"
#include"ContainerVirtualStock.h"

import FireBird.RelativeStrongReference;

class CContainerChinaStock : public CContainerVirtualStock {
public:
	CContainerChinaStock();
	// ֻ����һ��ʵ��,������ֵ��
	CContainerChinaStock(const CContainerChinaStock&) = delete;
	CContainerChinaStock& operator=(const CContainerChinaStock&) = delete;
	CContainerChinaStock(const CContainerChinaStock&&) noexcept = delete;
	CContainerChinaStock& operator=(const CContainerChinaStock&&) noexcept = delete;
	~CContainerChinaStock() override;
	void Reset() override;

	[[nodiscard]] size_t GetActiveStockSize() const;
	[[nodiscard]] auto GetLoadedStockSize() const noexcept { return m_lLoadedStock; }

	CChinaStockPtr GetStock(const CString& strStockCode) { return dynamic_pointer_cast<CChinaStock>(Get(strStockCode)); }
	CChinaStockPtr GetStock(const size_t lIndex) { return dynamic_pointer_cast<CChinaStock>(Get(lIndex)); }
	CString GetStockName(const CString& strStockCode);

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

	CString CreateNeteaseDayLineInquiringStr();
	CString CreateTengxunDayLineInquiringStr();

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
	size_t m_lLoadedStock; // ����װ�صĹ�Ʊ����
};
