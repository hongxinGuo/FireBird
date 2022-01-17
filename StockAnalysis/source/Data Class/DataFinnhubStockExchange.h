#pragma once

#include"FinnhubStockExchange.h"

using namespace std;
#include<vector>
#include<map>

class CDataFinnhubStockExchange : public CObject {
public:
	CDataFinnhubStockExchange();
	~CDataFinnhubStockExchange();
	void Reset();

	CFinnhubStockExchangePtr GetExchange(long lIndex) { return m_vFinnhubStockExchange.at(lIndex); }
	CString GetExchangeCode(long lIndex) { return m_vFinnhubStockExchange.at(lIndex)->m_strCode; }
	size_t GetExchangeSize(void) noexcept { return m_mapFinnhubStockExchange.size(); }

	bool LoadDB(void);

protected:
	vector<CFinnhubStockExchangePtr> m_vFinnhubStockExchange;
	map<CString, long> m_mapFinnhubStockExchange;
};
