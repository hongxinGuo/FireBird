#pragma once

#include"WorldStock.h"

using namespace std;
#include<vector>
#include<map>

class CDataWorldStock : public CObject {
public:
	CDataWorldStock();
	~CDataWorldStock();
	void Reset(void);

	bool IsStock(CString strSymbol) { if (m_mapWorldStock.find(strSymbol) == m_mapWorldStock.end()) return false; else return true; }
	bool IsStock(CWorldStockPtr pStock) { return IsStock(pStock->GetSymbol()); }
	CWorldStockPtr GetStock(long lIndex) { return m_vWorldStock.at(lIndex); }
	CWorldStockPtr GetStock(CString strSymbol) { return m_vWorldStock.at(m_mapWorldStock.at(strSymbol)); }
	long GetIndex(CString strSymbol) { return m_mapWorldStock.at(strSymbol); }
	long GetStockIndex(CString strSymbol) { return m_mapWorldStock.at(strSymbol); }
	size_t GetStockSize(void) noexcept { return m_vWorldStock.size(); }
	long GetLastStockSize(void) noexcept { return m_lLastTotalWorldStock; }

	bool SortStock(void);

	bool IsStockProfileNeedUpdate(void);
	bool IsBasicFinancialNeedUpdate(void);

	void ResetEPSSurprise(void);
	void ResetPeer(void);
	void ResetBasicFinancial(void);
	void ResetDayLine(void);

	bool Delete(CWorldStockPtr pStock);
	void Add(CWorldStockPtr pStock);

	bool LoadDB(void);
	bool UpdateProfileDB(void);

	bool UpdateBasicFinancialDB(void);
	bool UpdateBasicFinancialMetricDB(vector<CWorldStockPtr> vStock);
	bool UpdateBasicFinancialAnnualDB(vector<CWorldStockPtr> vStock);
	bool UpdateBasicFinancialQuarterDB(vector<CWorldStockPtr> vStock);

	bool CheckStockSymbol(CWorldStockPtr pstock);

	bool IsNeedSaveDayLine(void);
	bool IsNeedSaveInsiderTransaction(void);

protected:
	vector<CWorldStockPtr> m_vWorldStock;
	map<CString, long> m_mapWorldStock;
	long m_lLastTotalWorldStock;
};
