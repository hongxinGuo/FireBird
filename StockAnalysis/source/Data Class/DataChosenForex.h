#pragma once

#include"FinnhubForexSymbol.h"

using namespace std;
#include<vector>
#include<map>

class CDataChosenForex : public CObject {
public:
	CDataChosenForex();
	~CDataChosenForex();
	void Reset(void);

	CForexSymbolPtr GetForex(long lIndex) { return m_vWorldChosenForex.at(lIndex); }
	size_t GetSize(void) { return m_vWorldChosenForex.size(); }

	bool LoadDB(void);
	//bool UpdateDB(void);

protected:
	vector<CForexSymbolPtr> m_vWorldChosenForex;
	map<CString, long> m_mapWorldChosenForex;
	long m_lChosenForexPos;
};
