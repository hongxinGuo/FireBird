module;

module FireBirdLib.Country;

CCountry::CCountry() {
	Reset();
}

void CCountry::Reset() {
	m_strCode2 = " ";
	m_strCode3 = " ";
	m_strCodeNo = " ";
	m_strCountry = " ";
	m_strCurrency = " ";
	m_strCurrencyCode = " ";
}
