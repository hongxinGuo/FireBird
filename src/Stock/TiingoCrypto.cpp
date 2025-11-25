#include"pch.h"

#include"TiingoCrypto.h"

CTiingoCrypto::CTiingoCrypto() {
}

void CTiingoCrypto::Load(const CSetTiingoCrypto& setTiingoCrypto) {
	m_strSymbol = ToUTF8(setTiingoCrypto.m_Ticker);
	m_strName = ToUTF8(setTiingoCrypto.m_Name);
	m_strDescription = ToUTF8(setTiingoCrypto.m_Description);
	m_strBaseCurrency = ToUTF8(setTiingoCrypto.m_BaseCurrency);
	m_strQuoteCurrency = ToUTF8(setTiingoCrypto.m_QuoteCurrency);

	LoadUpdateDate(ToUTF8(setTiingoCrypto.m_UpdateDate));
}

void CTiingoCrypto::Append(CSetTiingoCrypto& setTiingoCrypto) {
	setTiingoCrypto.AddNew();
	Save(setTiingoCrypto);
	setTiingoCrypto.Update();
}

void CTiingoCrypto::Save(CSetTiingoCrypto& setTiingoCrypto) {
	if ((m_strSymbol.length() > 45)
		|| (m_strName.length() > 100)
		|| (m_strDescription.length() > 100)
		|| (m_strBaseCurrency.length() > 20)
		|| (m_strQuoteCurrency.length() > 20)) {
		string s = "Tiingo Crypto ";
		s += m_strSymbol;
		s += " ×Ö·û´®Ì«³¤";
		gl_systemMessage.PushErrorMessage(s);
	}
	m_strName = m_strName.substr(0, 99);
	m_strSymbol = m_strSymbol.substr(0, 44);
	m_strDescription = m_strDescription.substr(0, 99);
	m_strBaseCurrency = m_strBaseCurrency.substr(0, 19);
	m_strQuoteCurrency = m_strQuoteCurrency.substr(0, 19);

	setTiingoCrypto.m_Ticker = m_strSymbol.c_str();
	setTiingoCrypto.m_Name = m_strName.c_str();
	setTiingoCrypto.m_Description = m_strDescription.c_str();
	setTiingoCrypto.m_BaseCurrency = m_strBaseCurrency.c_str();
	setTiingoCrypto.m_QuoteCurrency = m_strQuoteCurrency.c_str();
	const string sUpdateDate = m_jsonUpdateDate.dump();
	setTiingoCrypto.m_UpdateDate = sUpdateDate.c_str();
	ASSERT(sUpdateDate.size() < 10000);
}

void CTiingoCrypto::Update(CSetTiingoCrypto& setTiingoCrypto) {
	setTiingoCrypto.Edit();
	Save(setTiingoCrypto);
	setTiingoCrypto.Update();
}
