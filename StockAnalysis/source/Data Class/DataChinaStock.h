#pragma once

#include"ChinaStock.h"
#include"SetChinaStockSymbol.h"

using namespace std;
#include<vector>
#include<map>

class CDataChinaStock : public CObject {
public:
	CDataChinaStock();
	~CDataChinaStock();
	void Reset(void);

	long GetIndex(CString strSymbol) { return m_mapStock.at(strSymbol); }
	long GetStockSize(void) noexcept { return (long)m_vStock.size(); }
	long GetActiveStockSize(void);
	long GetLoadedStockSize(void) noexcept { return m_lLoadedStock; }
	bool IsStock(CString strSymbol) { if (m_mapStock.find(strSymbol) == m_mapStock.end()) return false; else return true; }
	bool IsAStock(CString strStockCode) const;
	bool IsAStock(not_null<CChinaStockPtr> pStock) const { return(IsAStock(pStock->GetSymbol())); }
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
	bool IsDayLineNeedSaving(void) const;

	void ClearDayLineDBUpdatedFlag(void) noexcept;
	void ClearDayLineNeedUpdateStatus(void);

	INT64 GetTotalAttackBuyAmount(void);
	INT64 GetTotalAttackSellAmount(void);

	bool CreateNeteaseDayLineInquiringStr(CString& strReturn);
	CString GetNextStockInquiringMiddleStr(long& iStockIndex, CString strPostfix, long lTotalNumber);
	CString GetNextSinaStockInquiringMiddleStr(long lTotalNumber) { return GetNextStockInquiringMiddleStr(m_lSinaRTDataInquiringIndex, _T(","), lTotalNumber); }
	CString GetNextTengxunStockInquiringMiddleStr(long lTotalNumber) { return GetNextStockInquiringMiddleStr(m_lTengxunRTDataInquiringIndex, _T(","), lTotalNumber); }
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
	long GetNeteaseDayLineDataInquiringIndex(void) noexcept { return m_lNeteaseDayLineDataInquiringIndex; }
	void SetNeteaseRTDataInquiringIndex(long lIndex) noexcept { m_lNeteaseRTDataInquiringIndex = lIndex; }
	long GetNeteaseRTDataInquiringIndex(void) noexcept { return m_lNeteaseRTDataInquiringIndex; }
	void SetSinaRTDataInquiringIndex(long lIndex) noexcept { m_lSinaRTDataInquiringIndex = lIndex; }
	long GetSinaRTDataInquiringIndex(void) noexcept { return m_lSinaRTDataInquiringIndex; }
	void SetTengxunRTDataInquiringIndex(long lIndex) noexcept { m_lTengxunRTDataInquiringIndex = lIndex; }
	long GetTengxunRTDataInquiringIndex(void) noexcept { return m_lTengxunRTDataInquiringIndex; }

private:
	bool DeleteDayLineBasicInfo(long lDate);
	bool DeleteDayLineExtendInfo(long lDate);
	bool DeleteTodayTempDB(void);

	long IncreaseIndex(long& lIndex, long lEndPosition);

protected:
	vector<CChinaStockPtr> m_vStock; // 本系统允许的所有股票池（无论代码是否存在）
	map<CString, long> m_mapStock; // 将所有被查询的股票代码映射为偏移量（目前只接受A股信息）
	long m_lLoadedStock; // 本次装载的股票总数

	long m_lNeteaseDayLineDataInquiringIndex;
	long m_lNeteaseRTDataInquiringIndex;
	long m_lSinaRTDataInquiringIndex;
	long m_lTengxunRTDataInquiringIndex;
};
