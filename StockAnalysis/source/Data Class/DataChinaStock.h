#pragma once

#include"ChinaStock.h"
#include"SetChinaStockSymbol.h"

using namespace std;
#include<vector>
#include<map>

class CDataChinaStock : public CObject {
public:
	CDataChinaStock(CSetChinaStockSymbol* pset = nullptr);
	~CDataChinaStock();
	void Reset(void);

	bool IsStock(CString strSymbol) { if (m_mapChinaStock.find(strSymbol) == m_mapChinaStock.end()) return false; else return true; }
	bool IsStock(CChinaStockPtr pStock) { return IsStock(pStock->GetSymbol()); }
	CChinaStockPtr GetStock(long lIndex) { return m_vChinaStock.at(lIndex); }
	CChinaStockPtr GetStock(CString strSymbol) { return m_vChinaStock.at(m_mapChinaStock.at(strSymbol)); }
	long GetIndex(CString strSymbol) { return m_mapChinaStock.at(strSymbol); }
	long GetStockIndex(CString strSymbol) { return m_mapChinaStock.at(strSymbol); }
	size_t GetStockSize(void) noexcept { return m_vChinaStock.size(); }
	long GetLastStockSize(void) noexcept { return m_lLastTotalStock; }

	bool SortStock(void);

	bool Delete(CChinaStockPtr pStock);
	void Add(CChinaStockPtr pStock);

	bool LoadDB(void);
	bool UpdateDB(void);

protected:
	vector<CChinaStockPtr> m_vChinaStock;
	map<CString, long> m_mapChinaStock;
	long m_lLastTotalStock;
	CSetChinaStockSymbol* m_pSetChinaStockSymbol;
};
