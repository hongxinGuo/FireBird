#pragma once

#include"FinnhubForexSymbol.h"

class CDataChosenForex final : public CObject {
public:
	CDataChosenForex();
	~CDataChosenForex() override = default;
	void Reset(void);

	CForexSymbolPtr GetForex(const long lIndex) { return m_vWorldChosenForex.at(lIndex); }
	[[nodiscard]] size_t GetSize(void) const noexcept { return m_vWorldChosenForex.size(); }

	bool LoadDB(void);
	//bool UpdateDB(void);

protected:
	vector<CForexSymbolPtr> m_vWorldChosenForex;
	map<CString, long> m_mapWorldChosenForex;
	long m_lChosenForexPos;
};
