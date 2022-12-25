#pragma once

#include<vector>
#include<map>

class CDataFinnhubForexExchange final : public CObject {
public:
	CDataFinnhubForexExchange();
	~CDataFinnhubForexExchange() override = default;
	void Reset(void);

	bool IsForexExchange(const CString &strExchange) const noexcept {
		if (m_mapForexExchange.contains(strExchange)) return true;
		else return false;
	}

	size_t GetForexExchangeSize(void) const noexcept { return m_vForexExchange.size(); }
	CString GetForexExchange(const long lIndex) const { return m_vForexExchange.at(lIndex); }
	bool Delete(CString strForexExchange);
	void Add(CString strForexExchange);

	bool LoadDB(void);
	bool UpdateDB(void);

	bool IsNeedUpdate(void) const noexcept {
		if (m_lLastTotalForexExchange < m_vForexExchange.size()) return true;
		else return false;
	}

protected:
	vector<CString> m_vForexExchange;
	map<CString, long> m_mapForexExchange;
	long m_lLastTotalForexExchange;
};
