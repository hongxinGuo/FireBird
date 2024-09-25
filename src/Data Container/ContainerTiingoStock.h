#pragma once

#include"WorldStock.h"
#include"TiingoStock.h"

class CContainerTiingoStock final {
public:
	CContainerTiingoStock();
	CContainerTiingoStock(const CContainerTiingoStock& other) = delete;
	CContainerTiingoStock(CContainerTiingoStock&& other) noexcept = delete;
	CContainerTiingoStock& operator=(const CContainerTiingoStock& other) = delete;
	CContainerTiingoStock& operator=(CContainerTiingoStock&& other) noexcept = delete;
	~CContainerTiingoStock() = default;
	void Reset();

	void Add(const CTiingoStockPtr& pTiingoStock);
	bool Delete(const CTiingoStockPtr& pStock);
	size_t Size() const noexcept { return m_vTiingoStock.size(); }

	bool IsStock(const CString& strSymbol) const noexcept {
		if (m_mapTiingoStock.contains(strSymbol)) return true;
		return false;
	}

	bool IsStock(const CWorldStockPtr& pStock) const { return IsStock(pStock->GetSymbol()); }
	bool IsStock(const CTiingoStockPtr& pTiingoStock) const { return IsStock(pTiingoStock->m_strTicker); }
	CTiingoStockPtr GetStock(const size_t lIndex) const { return m_vTiingoStock.at(lIndex); }
	CTiingoStockPtr GetStock(const CString& strTicker) const { return m_vTiingoStock.at(m_mapTiingoStock.at(strTicker)); }

	bool UpdateDB();
	bool LoadDB();

	bool IsNeedUpdate() const noexcept { return m_lLastTotalTiingoStock < m_vTiingoStock.size(); }

protected:
	vector<CTiingoStockPtr> m_vTiingoStock;
	map<CString, size_t> m_mapTiingoStock;
	long m_lLastTotalTiingoStock{ 0 };
};
