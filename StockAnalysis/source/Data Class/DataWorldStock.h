#pragma once

#include"WorldStock.h"

using namespace std;
#include<vector>

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

	void ResetEPSSurprise(void);
	void ResetPeer(void);
	void ResetDayLine(void);

	bool Delete(CWorldStockPtr pStock);
	void Add(CWorldStockPtr pStock);

	bool LoadDB(void);
	bool UpdateProfileDB(void);

protected:
	vector<CWorldStockPtr> m_vWorldStock;
	map<CString, long> m_mapWorldStock;
	long m_lLastTotalWorldStock;
};