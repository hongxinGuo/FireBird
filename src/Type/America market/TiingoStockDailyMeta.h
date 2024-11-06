#pragma once

using std::vector;
using std::shared_ptr;

class CTiingoStockDailyMeta final {
public:
	CTiingoStockDailyMeta();
	~CTiingoStockDailyMeta() = default;

	void Append(CSetTiingoStockDailyMeta& setTiingoStockDailyMeta) const;
	void Load(const CSetTiingoStockDailyMeta& setTiingoStockDailyMeta);

public:
	CString m_strCode2;
	CString m_strCode3;
	CString m_strCodeNo;
	CString m_strCountry;
	CString m_strCurrency;
	CString m_strCurrencyCode;
};

using CTiingoStockDailyMetaPtr = shared_ptr<CCountry>;
using CCountrysPtr = shared_ptr<vector<CCountryPtr>>;
