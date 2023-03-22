#include"pch.h"

#include"TiingoCryptoSymbol.h"

CTiingoCryptoSymbol::CTiingoCryptoSymbol() {
	Reset();
}

void CTiingoCryptoSymbol::Reset() {
	m_strTicker = _T("");
	m_strName = _T("");
	m_strDescription = _T("");
	m_strBaseCurrency = _T("");
	m_strQuoteCurrency = _T("");
}

void CTiingoCryptoSymbol::Load(CSetTiingoCrypto& setTiingoCrypto) {
	m_strTicker = setTiingoCrypto.m_Ticker;
	m_strName = setTiingoCrypto.m_Name;
	m_strDescription = setTiingoCrypto.m_Description;
	m_strBaseCurrency = setTiingoCrypto.m_BaseCurrency;
	m_strQuoteCurrency = setTiingoCrypto.m_QuoteCurrency;
}

void CTiingoCryptoSymbol::Append(CSetTiingoCrypto& setTiingoCrypto) {
	setTiingoCrypto.AddNew();
	Save(setTiingoCrypto);
	setTiingoCrypto.Update();
}

void CTiingoCryptoSymbol::Save(CSetTiingoCrypto& setTiingoCrypto) {
	if ((m_strTicker.GetLength() > 45)
		|| (m_strName.GetLength() > 100)
		|| (m_strDescription.GetLength() > 100)
		|| (m_strBaseCurrency.GetLength() > 20)
		|| (m_strQuoteCurrency.GetLength() > 20)) {
		gl_systemMessage.PushErrorMessage(_T("Tiingo Crypto ") + m_strTicker + _T(" ×Ö·û´®Ì«³¤"));
	}
	m_strName = m_strName.Left(100);
	m_strTicker = m_strTicker.Left(45);
	m_strDescription = m_strDescription.Left(100);
	m_strBaseCurrency = m_strBaseCurrency.Left(20);
	m_strQuoteCurrency = m_strQuoteCurrency.Left(20);

	setTiingoCrypto.m_Ticker = m_strTicker;
	setTiingoCrypto.m_Name = m_strName;
	setTiingoCrypto.m_Description = m_strDescription;
	setTiingoCrypto.m_BaseCurrency = m_strBaseCurrency;
	setTiingoCrypto.m_QuoteCurrency = m_strQuoteCurrency;
}
