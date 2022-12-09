#pragma once

#include"RSReference.h"

#include"ChinaStock.h"

#include<vector>
#include<map>

class CDataChinaStock : public CObject {
public:
	CDataChinaStock();
	// ֻ����һ��ʵ��,������ֵ��
	CDataChinaStock(const CDataChinaStock&) = delete;
	CDataChinaStock& operator=(const CDataChinaStock&) = delete;
	CDataChinaStock(const CDataChinaStock&&) noexcept = delete;
	CDataChinaStock& operator=(const CDataChinaStock&&) noexcept = delete;
	~CDataChinaStock() override = default;
	void Reset(void);

	long GetIndex(CString strSymbol) const { return m_mapStock.at(strSymbol); }
	long GetStockSize(void) const noexcept { return static_cast<long>(m_vStock.size()); }
	long GetActiveStockSize(void) const;
	long GetLoadedStockSize(void) const noexcept { return m_lLoadedStock; }

	bool IsStock(CString strSymbol) const {
		if (m_mapStock.contains(strSymbol)) return true;
		else return false;
	}

	bool IsAStock(CString strStockCode) const;
	bool IsAStock(not_null<CChinaStockPtr> pStock) const { return (IsAStock(pStock->GetSymbol())); }
	CChinaStockPtr GetStock(CString strStockCode);
	CChinaStockPtr GetStock(long lIndex);
	CString GetStockName(CString strStockCode);

	bool Delete(CChinaStockPtr pStock);
	bool Add(CChinaStockPtr pStock);

	long LoadStockCodeDB(void);
	bool UpdateStockCodeDB(void);
	bool UnloadDayLine(void) noexcept;
	bool BuildWeekLine(long lStartDate);
	long BuildDayLine(long lCurrentTradeDay);
	bool UpdateTodayTempDB(void);
	bool BuildDayLineRS(long lDate);
	bool BuildWeekLineRS(long lDate);

	bool IsDayLineDBUpdated(void) noexcept;
	bool IsUpdateStockCodeDB(void);
	bool IsDayLineNeedUpdate(void) const noexcept;
	void SetAllDayLineNeedMaintain(void);
	void SetAllDayLineNeedUpdate(void);
	bool IsDayLineNeedSaving(void) const;

	void ClearDayLineDBUpdatedFlag(void) noexcept;
	void ClearDayLineNeedUpdateStatus(void);

	INT64 GetTotalAttackBuyAmount(void);
	INT64 GetTotalAttackSellAmount(void);

	bool CreateNeteaseDayLineInquiringStr(CString& strReturn);
	CString GetNextStockInquiringMiddleStr(long& iStockIndex, CString strPostfix, long lTotalNumber);

	CString GetNextSinaStockInquiringMiddleStr(long lTotalNumber) {
		return GetNextStockInquiringMiddleStr(m_lSinaRTDataInquiringIndex, _T(","), lTotalNumber);
	}

	CString GetNextTengxunStockInquiringMiddleStr(long lTotalNumber) {
		return GetNextStockInquiringMiddleStr(m_lTengxunRTDataInquiringIndex, _T(","), lTotalNumber);
	}

	CString GetNextNeteaseStockInquiringMiddleStr(long lTotalNumber);

	bool TaskProcessRTData(void);
	bool TaskSaveDayLineData(void);

	long GetDayLineNeedUpdateNumber(void);
	long GetDayLineNeedSaveNumber(void);

	bool Choice10RSStrong2StockSet(void);
	bool Choice10RSStrong1StockSet(void);
	bool Choice10RSStrongStockSet(CRSReference* pRef, int iIndex);

	bool SortStockVector(void);
	double GetUpDownRate(CString strClose, CString strLastClose) noexcept;

	void SetNeteaseDayLineDataInquiringIndex(long lIndex) noexcept { m_lNeteaseDayLineDataInquiringIndex = lIndex; }
	long GetNeteaseDayLineDataInquiringIndex(void) const noexcept { return m_lNeteaseDayLineDataInquiringIndex; }
	void SetNeteaseRTDataInquiringIndex(long lIndex) noexcept { m_lNeteaseRTDataInquiringIndex = lIndex; }
	long GetNeteaseRTDataInquiringIndex(void) const noexcept { return m_lNeteaseRTDataInquiringIndex; }
	void SetSinaRTDataInquiringIndex(long lIndex) noexcept { m_lSinaRTDataInquiringIndex = lIndex; }
	long GetSinaRTDataInquiringIndex(void) const noexcept { return m_lSinaRTDataInquiringIndex; }
	void SetTengxunRTDataInquiringIndex(long lIndex) noexcept { m_lTengxunRTDataInquiringIndex = lIndex; }
	long GetTengxunRTDataInquiringIndex(void) const noexcept { return m_lTengxunRTDataInquiringIndex; }
	long GetNextIndex(long& lIndex);

private:
	bool DeleteDayLineBasicInfo(long lDate);
	bool DeleteDayLineExtendInfo(long lDate);
	bool DeleteTodayTempDB(void);

protected:
	vector<CChinaStockPtr> m_vStock; // ��ϵͳ��������й�Ʊ�أ����۴����Ƿ���ڣ�
	map<CString, long> m_mapStock; // �����б���ѯ�Ĺ�Ʊ����ӳ��Ϊƫ������Ŀǰֻ����A����Ϣ��
	long m_lLoadedStock; // ����װ�صĹ�Ʊ����

	long m_lNeteaseDayLineDataInquiringIndex;
	long m_lNeteaseRTDataInquiringIndex;
	long m_lSinaRTDataInquiringIndex;
	long m_lTengxunRTDataInquiringIndex;
};
