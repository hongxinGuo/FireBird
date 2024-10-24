#pragma once

class CContainerFinnhubCryptoExchange final {
public:
	CContainerFinnhubCryptoExchange();
	CContainerFinnhubCryptoExchange(const CContainerFinnhubCryptoExchange& other) = delete;
	CContainerFinnhubCryptoExchange(CContainerFinnhubCryptoExchange&& other) noexcept = delete;
	CContainerFinnhubCryptoExchange& operator=(const CContainerFinnhubCryptoExchange& other) = delete;
	CContainerFinnhubCryptoExchange& operator=(CContainerFinnhubCryptoExchange&& other) noexcept = delete;
	~CContainerFinnhubCryptoExchange() = default;
	void Reset();

	bool IsExchange(const string& sExchange) const noexcept { return m_mapCryptoExchange.contains(sExchange); }

	size_t Size() const noexcept { return m_vCryptoExchange.size(); }
	string GetExchange(const size_t lIndex) const noexcept { return m_vCryptoExchange.at(lIndex); }
	bool Delete(const string& sCryptoExchange);
	void Add(const string& sCryptoExchange);

	bool LoadDB();
	bool UpdateDB();

	bool IsNeedUpdate() const noexcept {
		if (m_lLastTotalCryptoExchange < m_vCryptoExchange.size()) return true;
		return false;
	}

protected:
	vectorString m_vCryptoExchange;
	map<string, size_t> m_mapCryptoExchange;
	long m_lLastTotalCryptoExchange;
};
