#include"pch.h"

#include"StockExchange.h"

CStockExchange::CStockExchange() {
	m_strCode = " ";
	m_strName = " ";
	m_strMic = " ";
	m_strTimeZone = " ";
	m_strPreMarket = " ";
	m_strHour = " ";
	m_strPostMarket = " ";
	m_strCloseDate = " ";
	m_strCountry = "";
	m_strCountryName = " ";
	m_strSource = "";
	m_strMyUnknownColumn = " ";

	m_fUpdateStockSymbol = true;
	m_fUpdateMarketHoliday = true;
	m_fUpdateMarketStatus = true;
}

void CStockExchange::Append(CSetStockExchange& setExchange) const {
	setExchange.AddNew();
	setExchange.m_Code = m_strCode.c_str();
	setExchange.m_Name = m_strName.c_str();
	setExchange.m_Mic = m_strMic.c_str();
	setExchange.m_TimeZone = m_strTimeZone.c_str();
	setExchange.m_PreMarket = m_strPreMarket.c_str();
	setExchange.m_Hour = m_strHour.c_str();
	setExchange.m_PostMarket = m_strPostMarket.c_str();
	setExchange.m_CloseDate = m_strCloseDate.c_str();
	setExchange.m_Country = m_strCountry.c_str();
	setExchange.m_CountryName = m_strCountryName.c_str();
	setExchange.m_Source = m_strSource.c_str();
	setExchange.m_MyUnknownColumn = m_strMyUnknownColumn.c_str();
	setExchange.Update();
}

void CStockExchange::Load(const CSetStockExchange& setExchange) {
	m_strCode = ToUTF8(setExchange.m_Code);
	m_strName = ToUTF8(setExchange.m_Name);
	m_strMic = ToUTF8(setExchange.m_Mic);
	m_strTimeZone = ToUTF8(setExchange.m_TimeZone);
	m_strPreMarket = ToUTF8(setExchange.m_PreMarket);
	m_strHour = ToUTF8(setExchange.m_Hour);
	m_strPostMarket = ToUTF8(setExchange.m_PostMarket);
	m_strCloseDate = ToUTF8(setExchange.m_CloseDate);
	m_strCountry = ToUTF8(setExchange.m_Country);
	m_strCountryName = ToUTF8(setExchange.m_CountryName);
	m_strSource = ToUTF8(setExchange.m_Source);
	m_strMyUnknownColumn = ToUTF8(setExchange.m_MyUnknownColumn);

	int openHour, openMinute, endHour, endMinute;
	sscanf_s(m_strHour.c_str(), "%2d:%2d-%2d:%2d", &openHour, &openMinute, &endHour, &endMinute);
	m_lMarketOpenTime = (openHour * 60 + openMinute) * 60;
	m_lMarketCloseTime = (endHour * 60 + endMinute) * 60;
}
