#pragma once

#include"WorldStock.h"
#include"TiingoStock.h"

class CDataTiingoStock final {
public:
	CDataTiingoStock();
	~CDataTiingoStock() = default;
	void Reset(void);

	void Add(CTiingoStockPtr pTiingoStock);
	bool Delete(CTiingoStockPtr pStock);
	size_t GetTotalStock(void) const noexcept { return m_vTiingoStock.size(); }

	bool IsStock(const CString& strSymbol) const noexcept {
		if (m_mapTiingoStock.contains(strSymbol)) return true;
		return false;
	}

	bool IsStock(const CWorldStockPtr& pStock) const { return IsStock(pStock->GetSymbol()); }
	bool IsStock(const CTiingoStockPtr& pTiingoStock) const { return IsStock(pTiingoStock->m_strTicker); }
	CTiingoStockPtr GetStock(const size_t lIndex) const { return m_vTiingoStock.at(lIndex); }
	CTiingoStockPtr GetStock(const CString& strTicker) const { return m_vTiingoStock.at(m_mapTiingoStock.at(strTicker)); }

	bool UpdateDB(void);
	bool LoadDB(void);

	bool IsNeedUpdate(void) const noexcept {
		if (m_lLastTotalTiingoStock < m_vTiingoStock.size()) return true;
		return false;
	}

protected:
	vector<CTiingoStockPtr> m_vTiingoStock;
	map<CString, size_t> m_mapTiingoStock;
	long m_lLastTotalTiingoStock;
};
