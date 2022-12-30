#pragma once

#include"FinnhubForexSymbol.h"

class CDataChosenForex final {
public:
	CDataChosenForex();
	virtual ~CDataChosenForex() = default;
	void Reset(void);

	CForexSymbolPtr GetForex(const long lIndex) { return m_vWorldChosenForex.at(lIndex); }
	[[nodiscard]] long GetSize(void) const noexcept { return static_cast<long>(m_vWorldChosenForex.size()); }

	bool LoadDB(void);
	//bool UpdateDB(void);

protected:
	vector<CForexSymbolPtr> m_vWorldChosenForex;
	map<CString, long> m_mapWorldChosenForex;
	long m_lChosenForexPos;
};
