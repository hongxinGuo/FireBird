#pragma once

#include"SetStockExchange.h"

class CStockExchange final {
public:
	CStockExchange();
	~CStockExchange() = default;

	void Append(CSetStockExchange& setExchange) const;

	void Load(const CSetStockExchange& setExchange);

	void SetStockSymbolUpdated(const bool fFlag) noexcept { m_fStockSymbolUpdated = fFlag; }
	bool IsStockSymbolUpdated() const noexcept { return m_fStockSymbolUpdated; }
	void SetMarketStatusUpdated(const bool fFlag) noexcept { m_fMarketStatusUpdated = fFlag; }
	bool IsMarketStatusUpdated() const noexcept { return m_fMarketStatusUpdated; }
	void SetMarketHolidayUpdated(const bool fFlag) noexcept { m_fMarketHolidayUpdated = fFlag; }
	bool IsMarketHolidayUpdated() const noexcept { return m_fMarketHolidayUpdated; }

	string GetExchangeCode() const noexcept { return m_strCode; }
	void SetExchangeCode(const string& code) noexcept { m_strCode = code; }

protected:
	string m_strCode; // 两位的交易所代码。上海市场为SS， 美国市场为US，。。。
public:
	string m_strName;
	string m_strMic;
	string m_strTimeZone;
	string m_strPreMarket;
	string m_strHour;
	string m_strPostMarket;
	string m_strCloseDate;
	string m_strCountry;
	string m_strCountryName;
	string m_strSource;
	string m_strMyUnknownColumn;

	long m_lMarketOpenTime{ 0 };
	long m_lMarketCloseTime{ 0 };

	bool m_fStockSymbolUpdated;
	bool m_fMarketStatusUpdated;
	bool m_fMarketHolidayUpdated;
};

using CStockExchangePtr = shared_ptr<CStockExchange>;
