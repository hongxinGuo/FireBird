#pragma once

#include"FinnhubStockExchange.h"

class CDataFinnhubStockExchange final {
public:
	CDataFinnhubStockExchange();
	~CDataFinnhubStockExchange() = default;
	void Reset();

	CFinnhubStockExchangePtr GetExchange(const long lIndex) const { return m_vFinnhubStockExchange.at(lIndex); }
	CString GetExchangeCode(const long lIndex) const { return m_vFinnhubStockExchange.at(lIndex)->m_strCode; }
	long GetExchangeSize(void) const noexcept { return static_cast<long>(m_mapFinnhubStockExchange.size()); }

	bool LoadDB(void);

protected:
	vector<CFinnhubStockExchangePtr> m_vFinnhubStockExchange;
	map<CString, size_t> m_mapFinnhubStockExchange;
};
