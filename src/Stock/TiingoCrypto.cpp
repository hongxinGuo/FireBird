#include"pch.h"
module;
module FireBird.Stock.TiingoCrypto;
import FireBird.System.Message;

CTiingoCrypto::CTiingoCrypto() {
}

void CTiingoCrypto::Load(CSetTiingoCrypto& setTiingoCrypto) {
	m_strSymbol = setTiingoCrypto.m_Ticker;
	m_strName = setTiingoCrypto.m_Name;
	m_strDescription = setTiingoCrypto.m_Description;
	m_strBaseCurrency = setTiingoCrypto.m_BaseCurrency;
	m_strQuoteCurrency = setTiingoCrypto.m_QuoteCurrency;

	LoadUpdateDate(setTiingoCrypto.m_UpdateDate);
}

void CTiingoCrypto::Append(CSetTiingoCrypto& setTiingoCrypto) {
	setTiingoCrypto.AddNew();
	Save(setTiingoCrypto);
	setTiingoCrypto.Update();
}

void CTiingoCrypto::Save(CSetTiingoCrypto& setTiingoCrypto) {
	if ((m_strSymbol.GetLength() > 45)
		|| (m_strName.GetLength() > 100)
		|| (m_strDescription.GetLength() > 100)
		|| (m_strBaseCurrency.GetLength() > 20)
		|| (m_strQuoteCurrency.GetLength() > 20)) {
		gl_systemMessage.PushErrorMessage(_T("Tiingo Crypto ") + m_strSymbol + _T(" ×Ö·û´®Ì«³¤"));
	}
	m_strName = m_strName.Left(99);
	m_strSymbol = m_strSymbol.Left(44);
	m_strDescription = m_strDescription.Left(99);
	m_strBaseCurrency = m_strBaseCurrency.Left(19);
	m_strQuoteCurrency = m_strQuoteCurrency.Left(19);

	setTiingoCrypto.m_Ticker = m_strSymbol;
	setTiingoCrypto.m_Name = m_strName;
	setTiingoCrypto.m_Description = m_strDescription;
	setTiingoCrypto.m_BaseCurrency = m_strBaseCurrency;
	setTiingoCrypto.m_QuoteCurrency = m_strQuoteCurrency;
	const string sUpdateDate = m_jsonUpdateDate.dump();
	setTiingoCrypto.m_UpdateDate = sUpdateDate.c_str();
	ASSERT(sUpdateDate.size() < 10000);
}

void CTiingoCrypto::Update(CSetTiingoCrypto& setTiingoCrypto) {
	setTiingoCrypto.Edit();
	Save(setTiingoCrypto);
	setTiingoCrypto.Update();
}
