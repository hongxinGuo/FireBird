#pragma once

#include"FinnhubStockExchange.h"

#include<vector>
#include<map>

class CDataFinnhubStockExchange final : public CObject {
public:
	CDataFinnhubStockExchange();
	~CDataFinnhubStockExchange() override = default;
	void Reset();

	CFinnhubStockExchangePtr GetExchange(const long lIndex) const { return m_vFinnhubStockExchange.at(lIndex); }
	CString GetExchangeCode(const long lIndex) const { return m_vFinnhubStockExchange.at(lIndex)->m_strCode; }
	size_t GetExchangeSize(void) const noexcept { return m_mapFinnhubStockExchange.size(); }

	bool LoadDB(void);

protected:
	vector<CFinnhubStockExchangePtr> m_vFinnhubStockExchange;
	map<CString, long> m_mapFinnhubStockExchange;
};
