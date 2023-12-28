#pragma once

#include"FinnhubStockExchange.h"

class CContainerFinnhubStockExchange final {
public:
	CContainerFinnhubStockExchange();
	CContainerFinnhubStockExchange(const CContainerFinnhubStockExchange& other) = delete;
	CContainerFinnhubStockExchange(CContainerFinnhubStockExchange&& other) noexcept = delete;
	CContainerFinnhubStockExchange& operator=(const CContainerFinnhubStockExchange& other) = delete;
	CContainerFinnhubStockExchange& operator=(CContainerFinnhubStockExchange&& other) noexcept = delete;
	~CContainerFinnhubStockExchange() = default;
	void Reset();

	CFinnhubStockExchangePtr GetExchange(const long lIndex) const { return m_vFinnhubStockExchange.at(lIndex); }
	CString GetExchangeCode(const long lIndex) const { return m_vFinnhubStockExchange.at(lIndex)->m_strCode; }
	long Size() const noexcept { return static_cast<long>(m_mapFinnhubStockExchange.size()); }

	bool LoadDB();

protected:
	vector<CFinnhubStockExchangePtr> m_vFinnhubStockExchange;
	map<CString, size_t> m_mapFinnhubStockExchange;
};
