////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// 中国股票实例数据容器（日线容器、周线容器等）的基类。
// 纯虚类，不允许生成实例。
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#pragma once

#include"Accessory.h"
#include"VirtualHistoryCandleExtend.h"

using namespace std;
#include<vector>
#include<memory>
#include<atomic>

class CVirtualHistoryCandleExtendContainer : public CObject {
public:
	CVirtualHistoryCandleExtendContainer();
	virtual ~CVirtualHistoryCandleExtendContainer();
	void Reset(void); // 这些实现类需要采用这种方法重置内部状态，因为系统会一直运行，每天都需要重置状态。

	virtual bool SaveData(CString strStockSymbol) { TRACE(_T("调用了基类SaveData\n")); return false; }
	virtual bool LoadData(CString strStockSymbol) { TRACE(_T("调用了基类LoadData\n")); return false; }

	bool SaveBasicData(CVirtualSetHistoryCandleBasic* psetHistoryCandleBasic, CString strStockSymbol = _T(""));
	bool SaveExtendData(CVirtualSetHistoryCandleExtend* psetHistoryCandleExtend);

	bool LoadBasicData(CVirtualSetHistoryCandleBasic* psetHistoryCandleBasic);
	bool LoadExtendData(CVirtualSetHistoryCandleExtend* psetHistoryCandleExtend);

	void UpdateData(vector<CVirtualHistoryCandleExtendPtr>& vTempLine);

	void ShowData(CDC* pDC, CRect rectClient);

public:
	vector<CVirtualHistoryCandleExtendPtr>* GetContainer(void) noexcept { return &m_vHistoryData; }

	size_t GetDataSize(void) const noexcept { return m_vHistoryData.size(); }
	void Unload(void) noexcept { m_vHistoryData.clear(); m_fDataLoaded = false; }
	CVirtualHistoryCandleExtendPtr GetData(long lIndex) const { return m_vHistoryData.at(lIndex); }
	bool StoreData(CVirtualHistoryCandleExtendPtr pData) { m_vHistoryData.push_back(pData); return true; }

	bool IsDatabaseTodayUpdated(void) const noexcept { return (m_fDatabaseTodayUpdated); }
	void SetDatabaseTodayUpdated(bool fUpdate) noexcept { m_fDatabaseTodayUpdated = fUpdate; }
	bool IsDataLoaded(void) const noexcept { return m_fDataLoaded; }
	void SetDataLoaded(bool fFlag) noexcept { m_fDataLoaded = fFlag; }

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

typedef shared_ptr<CVirtualHistoryCandleExtendContainer> CVirtualHistoryCandleExtendContainerPtr;