#pragma once

#include"FinnhubForexSymbol.h"

using namespace std;
#include<vector>
#include<map>

class CDataFinnhubForexSymbol : public CObject {
public:
	CDataFinnhubForexSymbol();
	~CDataFinnhubForexSymbol();
	void Reset(void);

	bool IsForexSymbol(CString strSymbol) { if (m_mapForexSymbol.find(strSymbol) == m_mapForexSymbol.end()) return false; else return true; }
	size_t GetForexSymbolSize(void) noexcept { return m_vForexSymbol.size(); }
	CForexSymbolPtr GetForexSymbol(long lIndex) { return m_vForexSymbol.at(lIndex); }
	CForexSymbolPtr GetForexSymbol(CString strSymbol) { return GetForexSymbol(m_mapForexSymbol.at(strSymbol)); }
	bool Delete(CForexSymbolPtr pSymbol);
	void Add(CForexSymbolPtr pSymbol);

	bool LoadDB(void);
	bool UpdateDB(void);

protected:
	vector<CForexSymbolPtr> m_vForexSymbol;
	map<CString, long> m_mapForexSymbol;
	long m_lLastTotalForexSymbol;
};
