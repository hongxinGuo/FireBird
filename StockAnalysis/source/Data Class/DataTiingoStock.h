#pragma once

#include"WorldStock.h"
#include <TiingoStock.h>

using namespace std;
#include<vector>
#include<map>

class CDataTiingoStock : public CObject {
public:
	CDataTiingoStock();
	~CDataTiingoStock();
	void Reset(void);

	void Add(CTiingoStockPtr pTiingoStock);
	bool Delete(CTiingoStockPtr pStock);
	size_t GetTotalStock(void) noexcept { return m_vTiingoStock.size(); }
	bool IsStock(CString strSymbol) { if (m_mapTiingoStock.find(strSymbol) == m_mapTiingoStock.end()) return false; else return true; }
	bool IsStock(CWorldStockPtr pStock) { return IsStock(pStock->GetSymbol()); }
	bool IsStock(CTiingoStockPtr pTiingoStock) { return IsStock(pTiingoStock->m_strTicker); }
	CTiingoStockPtr GetStock(long lIndex) { return m_vTiingoStock.at(lIndex); }
	CTiingoStockPtr GetStock(CString strTicker) { return m_vTiingoStock.at(m_mapTiingoStock.at(strTicker)); }

	bool UpdateDB(void);
	bool LoadDB(void);

protected:
	vector<CTiingoStockPtr> m_vTiingoStock;
	map<CString, long> m_mapTiingoStock;
	long m_lLastTotalTiingoStock;
};
