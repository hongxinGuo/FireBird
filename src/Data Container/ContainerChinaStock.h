#pragma once

#include"RSReference.h"
#include"ChinaStock.h"
#include"ContainerVirtualStock.h"

#include <memory>
using std::dynamic_pointer_cast;

class CContainerChinaStock : public CContainerVirtualStock {
public:
	CContainerChinaStock();
	// ֻ����һ��ʵ��,������ֵ��
	CContainerChinaStock(const CContainerChinaStock&) = delete;
	CContainerChinaStock& operator=(const CContainerChinaStock&) = delete;
	CContainerChinaStock(const CContainerChinaStock&&) noexcept = delete;
	CContainerChinaStock& operator=(const CContainerChinaStock&&) noexcept = delete;
	~CContainerChinaStock() override = default;
	void Reset() override;

	[[nodiscard]] long GetActiveStockSize() const;
	[[nodiscard]] long GetLoadedStockSize() const noexcept { return m_lLoadedStock; }

	static bool IsAStock(const CString& strStockCode);
	bool IsAStock(const CChinaStockPtr pStock) const { return IsAStock(pStock->GetSymbol()); }

	CChinaStockPtr GetStock(const CString& strStockCode) { return dynamic_pointer_cast<CChinaStock>(Get(strStockCode)); }
	CChinaStockPtr GetStock(const size_t lIndex) { return dynamic_pointer_cast<CChinaStock>(Get(lIndex)); }
	CString GetStockName(const CString& strStockCode);

	long LoadStockProfileDB();
	bool UpdateStockProfileDB();
	void UnloadDayLine() noexcept;
	bool BuildWeekLine(long lStartDate);
	long BuildDayLine(long lCurrentTradeDay);
	bool SaveTempRTData();
	void DeleteTempRTData();
	bool BuildDayLineRS(long lDate);
	bool BuildWeekLineRS(long lDate);

	[[nodiscard]] bool IsDayLineDBUpdated() noexcept;

	void SetDayLineNeedMaintain() const;
	void SetDayLineNeedUpdate() const;

	void ClearDayLineDBUpdatedFlag() noexcept;
	void ClearDayLineNeedUpdateStatus() const;

	[[nodiscard]] INT64 GetTotalAttackBuyAmount();
	[[nodiscard]] INT64 GetTotalAttackSellAmount();

	CString CreateNeteaseDayLineInquiringStr();
	CString CreateTengxunDayLineInquiringStr();
	CString GetNextStockInquiringMiddleStr(long& iStockIndex, CString strPostfix, long lTotalNumber);

	CString GetNextSinaStockInquiringMiddleStr(const long lTotalNumber) { return GetNextStockInquiringMiddleStr(m_lSinaRTDataInquiringIndex, _T(","), lTotalNumber); }
	CString GetNextTengxunStockInquiringMiddleStr(const long lTotalNumber) { return GetNextStockInquiringMiddleStr(m_lTengxunRTDataInquiringIndex, _T(","), lTotalNumber); }
	CString GetNextNeteaseStockInquiringMiddleStr(long lTotalNumber);

	[[nodiscard]] bool ProcessRTData();
	bool SaveDayLineData();
	virtual void CreateThreadSaveDayLineBasicInfo(CChinaStockPtr pStock);

	[[nodiscard]] long GetDayLineNeedUpdateNumber() const;
	[[nodiscard]] long GetDayLineNeedSaveNumber() const;

	bool Choice10RSStrong2StockSet();
	bool Choice10RSStrong1StockSet();
	bool Choice10RSStrongStockSet(CRSReference* pRef, int iIndex);

	[[nodiscard]] static double GetUpDownRate(const CString& strClose, const CString& strLastClose) noexcept;

	void SetNeteaseRTDataInquiringIndex(const long lIndex) noexcept { m_lNeteaseRTDataInquiringIndex = lIndex; }
	[[nodiscard]] long GetNeteaseRTDataInquiringIndex() const noexcept { return m_lNeteaseRTDataInquiringIndex; }
	void SetSinaRTDataInquiringIndex(const long lIndex) noexcept { m_lSinaRTDataInquiringIndex = lIndex; }
	[[nodiscard]] long GetSinaRTDataInquiringIndex() const noexcept { return m_lSinaRTDataInquiringIndex; }
	void SetTengxunRTDataInquiringIndex(const long lIndex) noexcept { m_lTengxunRTDataInquiringIndex = lIndex; }
	[[nodiscard]] long GetTengxunRTDataInquiringIndex() const noexcept { return m_lTengxunRTDataInquiringIndex; }
	[[nodiscard]] long GetNextIndex(long lIndex) const;

private:
	bool DeleteDayLineBasicInfo(long lDate);
	bool DeleteDayLineExtendInfo(long lDate);

protected:
	long m_lLoadedStock; // ����װ�صĹ�Ʊ����

	long m_lNeteaseRTDataInquiringIndex;
	long m_lSinaRTDataInquiringIndex;
	long m_lTengxunRTDataInquiringIndex;
};
