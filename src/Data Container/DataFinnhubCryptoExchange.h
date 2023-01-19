#pragma once

class CDataFinnhubCryptoExchange final {
public:
	CDataFinnhubCryptoExchange();
	~CDataFinnhubCryptoExchange() = default;
	void Reset(void);

	bool IsCryptoExchange(const string& sExchange) const noexcept {
		if (m_mapCryptoExchange.contains(sExchange)) return true;
		return false;
	}

	size_t GetCryptoExchangeSize(void) const noexcept { return m_vCryptoExchange.size(); }
	string GetCryptoExchange(const size_t lIndex) const noexcept { return m_vCryptoExchange.at(lIndex); }
	bool Delete(const string& sCryptoExchange);
	void Add(const string& sCryptoExchange);

	bool LoadDB(void);
	bool UpdateDB(void);

	bool IsNeedUpdate(void) const noexcept {
		if (m_lLastTotalCryptoExchange < m_vCryptoExchange.size()) return true;
		return false;
	}

protected:
	vectorString m_vCryptoExchange;
	map<string, size_t> m_mapCryptoExchange;
	long m_lLastTotalCryptoExchange;
};
