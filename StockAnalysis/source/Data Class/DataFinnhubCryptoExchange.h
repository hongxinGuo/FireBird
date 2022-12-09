#pragma once

#include<vector>
#include<map>

class CDataFinnhubCryptoExchange final : public CObject {
public:
	CDataFinnhubCryptoExchange();
	~CDataFinnhubCryptoExchange() override = default;
	void Reset(void);

	bool IsCryptoExchange(CString strExchange) const noexcept {
		if (m_mapCryptoExchange.contains(strExchange)) return true;
		else return false;
	}

	size_t GetCryptoExchangeSize(void) const noexcept { return m_vCryptoExchange.size(); }
	CString GetCryptoExchange(long lIndex) const noexcept { return m_vCryptoExchange.at(lIndex); }
	bool Delete(CString strCryptoExchange);
	void Add(CString strCryptoExchange);

	bool LoadDB(void);
	bool UpdateDB(void);

	bool IsNeedUpdate(void) const noexcept {
		if (m_lLastTotalCryptoExchange < m_vCryptoExchange.size()) return true;
		else return false;
	}

protected:
	vector<CString> m_vCryptoExchange;
	map<CString, long> m_mapCryptoExchange;
	long m_lLastTotalCryptoExchange;
};
