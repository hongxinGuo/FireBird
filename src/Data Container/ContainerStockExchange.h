#pragma once

#include"StockExchange.h"

class CContainerStockExchange final {
public:
	CContainerStockExchange();
	CContainerStockExchange(const CContainerStockExchange& other) = delete;
	CContainerStockExchange(CContainerStockExchange&& other) noexcept = delete;
	CContainerStockExchange& operator=(const CContainerStockExchange& other) = delete;
	CContainerStockExchange& operator=(CContainerStockExchange&& other) noexcept = delete;
	~CContainerStockExchange() = default;
	void Reset();

	CStockExchangePtr GetExchange(const size_t lIndex) const { return m_vStockExchange.at(lIndex); }
	CStockExchangePtr GetExchange(const string& strExchangeSymbol) const;
	string GetExchangeCode(const size_t lIndex) const { return m_vStockExchange.at(lIndex)->m_strCode; }
	auto Size() const noexcept { return m_mapStockExchange.size(); }

	bool LoadDB();

protected:
	vector<CStockExchangePtr> m_vStockExchange;
	map<string, size_t> m_mapStockExchange;
};
