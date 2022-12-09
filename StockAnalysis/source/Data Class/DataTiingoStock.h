#pragma once

#include"WorldStock.h"
#include <TiingoStock.h>

#include<vector>
#include<map>

class CDataTiingoStock final : public CObject {
public:
	CDataTiingoStock();
	~CDataTiingoStock() override = default;
	void Reset(void);

	void Add(CTiingoStockPtr pTiingoStock);
	bool Delete(CTiingoStockPtr pStock);
	size_t GetTotalStock(void) const noexcept { return m_vTiingoStock.size(); }

	bool IsStock(CString strSymbol) const noexcept {
		if (m_mapTiingoStock.contains(strSymbol)) return true;
		else return false;
	}

	bool IsStock(CWorldStockPtr pStock) const { return IsStock(pStock->GetSymbol()); }
	bool IsStock(CTiingoStockPtr pTiingoStock) const { return IsStock(pTiingoStock->m_strTicker); }
	CTiingoStockPtr GetStock(long lIndex) const { return m_vTiingoStock.at(lIndex); }
	CTiingoStockPtr GetStock(CString strTicker) const { return m_vTiingoStock.at(m_mapTiingoStock.at(strTicker)); }

	bool UpdateDB(void);
	bool LoadDB(void);

	bool IsNeedUpdate(void) const noexcept {
		if (m_lLastTotalTiingoStock < m_vTiingoStock.size()) return true;
		else return false;
	}

protected:
	vector<CTiingoStockPtr> m_vTiingoStock;
	map<CString, long> m_mapTiingoStock;
	long m_lLastTotalTiingoStock;
};
