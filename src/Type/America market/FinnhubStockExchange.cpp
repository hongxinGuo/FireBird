#include"pch.h"

#include"FinnhubStockExchange.h"

CFinnhubStockExchange::CFinnhubStockExchange() {
	m_strCode = _T(" ");
	m_strName = _T(" ");
	m_strMic = _T(" ");
	m_strTimeZone = _T(" ");
	m_strPreMarket = _T(" ");
	m_strHour = _T(" ");
	m_strPostMarket = _T(" ");
	m_strCloseDate = _T(" ");
	m_strCountry = _T("");
	m_strCountryName = _T(" ");
	m_strSource = _T("");
	m_strMyUnknownColumn = _T(" ");

	m_fStockSymbolUpdated = false;
	m_fMarketHolidayUpdated = false;
	m_fMarketStatusUpdated = false;
}

void CFinnhubStockExchange::Append(CSetFinnhubStockExchange& setFinnhubExchange) const {
	setFinnhubExchange.AddNew();
	setFinnhubExchange.m_Code = m_strCode;
	setFinnhubExchange.m_Name = m_strName;
	setFinnhubExchange.m_Mic = m_strMic;
	setFinnhubExchange.m_TimeZone = m_strTimeZone;
	setFinnhubExchange.m_PreMarket = m_strPreMarket;
	setFinnhubExchange.m_Hour = m_strHour;
	setFinnhubExchange.m_PostMarket = m_strPostMarket;
	setFinnhubExchange.m_CloseDate = m_strCloseDate;
	setFinnhubExchange.m_Country = m_strCountry;
	setFinnhubExchange.m_CountryName = m_strCountryName;
	setFinnhubExchange.m_Source = m_strSource;
	setFinnhubExchange.m_MyUnknownColumn = m_strMyUnknownColumn;
	setFinnhubExchange.Update();
}

void CFinnhubStockExchange::Load(const CSetFinnhubStockExchange& setFinnhubExchange) {
	m_strCode = setFinnhubExchange.m_Code;
	m_strName = setFinnhubExchange.m_Name;
	m_strMic = setFinnhubExchange.m_Mic;
	m_strTimeZone = setFinnhubExchange.m_TimeZone;
	m_strPreMarket = setFinnhubExchange.m_PreMarket;
	m_strHour = setFinnhubExchange.m_Hour;
	m_strPostMarket = setFinnhubExchange.m_PostMarket;
	m_strCloseDate = setFinnhubExchange.m_CloseDate;
	m_strCountry = setFinnhubExchange.m_Country;
	m_strCountryName = setFinnhubExchange.m_CountryName;
	m_strSource = setFinnhubExchange.m_Source;
	m_strMyUnknownColumn = setFinnhubExchange.m_MyUnknownColumn;
}
