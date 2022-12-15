#include"pch.h"

#include"FinnhubStockExchange.h"

CFinnhubStockExchange::CFinnhubStockExchange() : CObject() {
	m_strCode = _T(" ");
	m_strName = _T(" ");
	m_strMic = _T(" ");
	m_strTimeZone = _T(" ");
	m_strHour = _T(" ");
	m_strCloseDate = _T(" ");
	m_strCountry = _T("");
	m_strSource = _T("");

	m_fUpdated = false;
}

void CFinnhubStockExchange::Append(CSetFinnhubStockExchange& setFinnhubExchange) {
	setFinnhubExchange.AddNew();
	setFinnhubExchange.m_Code = m_strCode;
	setFinnhubExchange.m_Name = m_strName;
	setFinnhubExchange.m_Mic = m_strMic;
	setFinnhubExchange.m_TimeZone = m_strTimeZone;
	setFinnhubExchange.m_Hour = m_strHour;
	setFinnhubExchange.m_CloseDate = m_strCloseDate;
	setFinnhubExchange.m_Country = m_strCountry;
	setFinnhubExchange.m_Source = m_strSource;
	setFinnhubExchange.Update();
}

void CFinnhubStockExchange::Load(CSetFinnhubStockExchange& setFinnhubExchange) {
	m_strCode = setFinnhubExchange.m_Code;
	m_strName = setFinnhubExchange.m_Name;
	m_strMic = setFinnhubExchange.m_Mic;
	m_strTimeZone = setFinnhubExchange.m_TimeZone;
	m_strHour = setFinnhubExchange.m_Hour;
	m_strCloseDate = setFinnhubExchange.m_CloseDate;
	m_strCountry = setFinnhubExchange.m_Country;
	m_strSource = setFinnhubExchange.m_Source;
}
