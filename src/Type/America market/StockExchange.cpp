#include"pch.h"

#include"StockExchange.h"

#include "CharSetTransfer.h"

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
