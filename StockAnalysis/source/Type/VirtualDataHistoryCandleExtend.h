////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// 中国股票实例数据容器（日线容器、周线容器等）的基类。
// 纯虚类，不允许生成实例。
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#pragma once

#include"VirtualHistoryCandleExtend.h"

#include<vector>
#include<memory>
#include<atomic>

class CVirtualDataHistoryCandleExtend : public CObject {
public:
	CVirtualDataHistoryCandleExtend();
	~CVirtualDataHistoryCandleExtend() override = default;
	void Reset(void); // 这些实现类需要采用这种方法重置内部状态，因为系统会一直运行，每天都需要重置状态。

	// 所有的派生类皆需要定义此两个存储和提取函数，不允许调用此基类函数
	virtual bool SaveDB(CString strStockSymbol) {
		TRACE(_T("调用了基类SaveDB\n"));
		return false;
	}

	virtual bool LoadDB(CString strStockSymbol) {
		TRACE(_T("调用了基类LoadDB\n"));
		return false;
	}

	bool SaveExtendDB(CVirtualSetHistoryCandleExtend* pSetHistoryCandleExtend);

	bool LoadBasicDB(CVirtualSetHistoryCandleBasic* pSetHistoryCandleBasic);
	bool LoadExtendDB(CVirtualSetHistoryCandleExtend* pSetHistoryCandleExtend);

	void UpdateData(vector<CVirtualHistoryCandleExtendPtr>& vTempData, bool fRevertSave = false);
	void UpdateData(vector<CDayLinePtr>& vTempDayLine, bool fRevertSave = false);

	void ShowData(CDC* pDC, CRect rectClient);

protected:
	bool UpdateBasicDB(CVirtualSetHistoryCandleBasic* pSetHistoryCandleBasic, CString strStockSymbol = _T(""));

public:
	vector<CVirtualHistoryCandleExtendPtr>* GetContainer(void) noexcept { return &m_vHistoryData; }

	long Size(void) const noexcept { return static_cast<long>(m_vHistoryData.size()); }
	bool GetStartEndDate(long& lStartDate, long& lEndDate);

	void Unload(void) noexcept {
		m_vHistoryData.clear();
		m_fDataLoaded = false;
	}

	CVirtualHistoryCandleExtendPtr GetData(const long lIndex) const { return m_vHistoryData.at(lIndex); }

	bool StoreData(const CVirtualHistoryCandleExtendPtr pData) {
		m_vHistoryData.push_back(pData);
		return true;
	}

	bool IsDatabaseTodayUpdated(void) const noexcept { return (m_fDatabaseTodayUpdated); }
	void SetDatabaseTodayUpdated(const bool fUpdate) noexcept { m_fDatabaseTodayUpdated = fUpdate; }
	bool IsDataLoaded(void) const noexcept { return m_fDataLoaded; }
	void SetDataLoaded(const bool fFlag) noexcept { m_fDataLoaded = fFlag; }

	bool CalculateRS0(void);
	virtual bool CalculateRS1(INT64 lNumber);
	bool CalculateRSIndex0(void);
	virtual bool CalculateRSIndex1(INT64 lNumber);
	bool CalculateRSLogarithm0(void);
	virtual bool CalculateRSLogarithm1(INT64 lNumber);

	void GetRS1(vector<double>& vRS);
	void GetRSIndex1(vector<double>& vRS);
	void GetRSLogarithm1(vector<double>& vRS);
	void GetRS3(vector<double>& vRS);
	void GetRS5(vector<double>& vRS);
	void GetRS10(vector<double>& vRS);
	void GetRS30(vector<double>& vRS);
	void GetRS60(vector<double>& vRS);
	void GetRS120(vector<double>& vRS);

protected:
	vector<CVirtualHistoryCandleExtendPtr> m_vHistoryData;

	atomic_bool m_fDataLoaded; // 数据装载与否标识

	bool m_fDatabaseTodayUpdated; // 数据库今日是否已更新标识

protected:
	bool m_fLoadDataFirst;
};

typedef shared_ptr<CVirtualDataHistoryCandleExtend> CVirtualDataHistoryCandleExtendPtr;
