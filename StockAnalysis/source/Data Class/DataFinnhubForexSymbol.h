#pragma once

#include"FinnhubForexSymbol.h"

class CDataFinnhubForexSymbol final : public CObject {
public:
	CDataFinnhubForexSymbol();
	~CDataFinnhubForexSymbol() override = default;
	void Reset(void);

	bool IsForexSymbol(const CString& strSymbol) const {
		if (m_mapForexSymbol.contains(strSymbol)) return true;
		else return false;
	}

	size_t GetForexSymbolSize(void) const noexcept { return m_vForexSymbol.size(); }
	CForexSymbolPtr GetForexSymbol(const long lIndex) const { return m_vForexSymbol.at(lIndex); }
	CForexSymbolPtr GetForexSymbol(const CString& strSymbol) const { return GetForexSymbol(m_mapForexSymbol.at(strSymbol)); }
	bool Delete(CForexSymbolPtr pSymbol);
	void Add(CForexSymbolPtr pSymbol);

	bool LoadDB(void);
	bool UpdateDB(void);

	bool IsNeedUpdate(void);

protected:
	vector<CForexSymbolPtr> m_vForexSymbol;
	map<CString, size_t> m_mapForexSymbol;
	long m_lLastTotalForexSymbol;
};
