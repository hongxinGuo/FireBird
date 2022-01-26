#pragma once

#include"ChinaStock.h"
#include"SetChinaStockSymbol.h"

using namespace std;
#include<vector>
#include<map>

class CDataChinaStock : public CObject {
public:
	CDataChinaStock(CSetChinaStockSymbol* pset = nullptr); // 使用此类有两个类型，一个无需存储功能，一个需要进行存储。在生成时区别之。
	~CDataChinaStock();
	void Reset(void);

	bool IsStock(CString strSymbol) { if (m_mapStock.find(strSymbol) == m_mapStock.end()) return false; else return true; }
	bool IsStock(CChinaStockPtr pStock) { return IsStock(pStock->GetSymbol()); }
	CChinaStockPtr GetStock(long lIndex) { return m_vStock.at(lIndex); }
	CChinaStockPtr GetStock(CString strSymbol) { return m_vStock.at(m_mapStock.at(strSymbol)); }
	long GetIndex(CString strSymbol) { return m_mapStock.at(strSymbol); }
	long GetStockIndex(CString strSymbol) { return m_mapStock.at(strSymbol); }
	size_t GetStockSize(void) noexcept { return m_vStock.size(); }
	long GetLastStockSize(void) noexcept { return m_lLastTotalStock; }

	bool SortStock(void);

	bool Delete(CChinaStockPtr pStock);
	void Add(CChinaStockPtr pStock);

	bool LoadDB(void);
	bool UpdateDB(void);

protected:
	vector<CChinaStockPtr> m_vStock;
	map<CString, long> m_mapStock;
	long m_lLastTotalStock;
	CSetChinaStockSymbol* m_pSetChinaStockSymbol;
};
