#include"pch.h"

#include"StockExchange.h"

CStockExchange::CStockExchange() {
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

void CStockExchange::Append(CSetStockExchange& setExchange) const {
	setExchange.AddNew();
	setExchange.m_Code = m_strCode;
	setExchange.m_Name = m_strName;
	setExchange.m_Mic = m_strMic;
	setExchange.m_TimeZone = m_strTimeZone;
	setExchange.m_PreMarket = m_strPreMarket;
	setExchange.m_Hour = m_strHour;
	setExchange.m_PostMarket = m_strPostMarket;
	setExchange.m_CloseDate = m_strCloseDate;
	setExchange.m_Country = m_strCountry;
	setExchange.m_CountryName = m_strCountryName;
	setExchange.m_Source = m_strSource;
	setExchange.m_MyUnknownColumn = m_strMyUnknownColumn;
	setExchange.Update();
}

void CStockExchange::Load(const CSetStockExchange& setExchange) {
	m_strCode = setExchange.m_Code;
	m_strName = setExchange.m_Name;
	m_strMic = setExchange.m_Mic;
	m_strTimeZone = setExchange.m_TimeZone;
	m_strPreMarket = setExchange.m_PreMarket;
	m_strHour = setExchange.m_Hour;
	m_strPostMarket = setExchange.m_PostMarket;
	m_strCloseDate = setExchange.m_CloseDate;
	m_strCountry = setExchange.m_Country;
	m_strCountryName = setExchange.m_CountryName;
	m_strSource = setExchange.m_Source;
	m_strMyUnknownColumn = setExchange.m_MyUnknownColumn;

	int openHour, openMinute, endHour, endMinute;
	sscanf_s(m_strHour, "%2d:%2d-%2d:%2d", &openHour, &openMinute, &endHour, &endMinute);
	m_lMarketOpenTime = (openHour * 60 + openMinute) * 60;
	m_lMarketCloseTime = (endHour * 60 + endMinute) * 60;
}
