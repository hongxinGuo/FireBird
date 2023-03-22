#pragma once

class CDataFinnhubForexExchange final {
public:
	CDataFinnhubForexExchange();
	~CDataFinnhubForexExchange() = default;
	void Reset();

	bool IsForexExchange(const CString& strExchange) const noexcept {
		if (m_mapForexExchange.contains(strExchange)) return true;
		return false;
	}

	size_t GetForexExchangeSize() const noexcept { return m_vForexExchange.size(); }
	CString GetForexExchange(const size_t lIndex) const { return m_vForexExchange.at(lIndex); }
	bool Delete(CString strForexExchange);
	void Add(CString strForexExchange);

	bool LoadDB();
	bool UpdateDB();

	bool IsNeedUpdate() const noexcept {
		if (m_lLastTotalForexExchange < m_vForexExchange.size()) return true;
		return false;
	}

protected:
	vector<CString> m_vForexExchange;
	map<CString, size_t> m_mapForexExchange;
	long m_lLastTotalForexExchange;
};
