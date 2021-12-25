#pragma once

#include"FinnhubForexSymbol.h"

using namespace std;
#include<vector>
#include<map>

class CDataChoicedForex : CObject {
public:
	CDataChoicedForex();
	~CDataChoicedForex();
	void Reset(void);

	CForexSymbolPtr GetForex(long lIndex) { return m_vWorldChoicedForex.at(lIndex); }
	size_t GetSize(void) { return m_vWorldChoicedForex.size(); }

	bool LoadDB(void);
	//bool UpdateDB(void);

protected:
	vector<CForexSymbolPtr> m_vWorldChoicedForex;
	map<CString, long> m_mapWorldChoicedForex;
	long m_lChoicedForexPos;
};