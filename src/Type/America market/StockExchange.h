#pragma once

class CStockExchange final {
public:
	CStockExchange();
	~CStockExchange() = default;

	void SetUpdateStockSymbol(const bool fFlag) noexcept { m_fUpdateStockSymbol = fFlag; }
	bool IsUpdateStockSymbol() const noexcept { return m_fUpdateStockSymbol; }
	void SetUpdateMarketStatus(const bool fFlag) noexcept { m_fUpdateMarketStatus = fFlag; }
	bool IsUpdateMarketStatus() const noexcept { return m_fUpdateMarketStatus; }
	void SetUpdateMarketHoliday(const bool fFlag) noexcept { m_fUpdateMarketHoliday = fFlag; }
	bool IsUpdateMarketHoliday() const noexcept { return m_fUpdateMarketHoliday; }

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

	chrono::hh_mm_ss<chrono::seconds> m_marketOpenTime{ 0h };
	chrono::hh_mm_ss<chrono::seconds> m_marketCloseTime{ 0h };

	bool m_fUpdateStockSymbol{ true };
	bool m_fUpdateMarketStatus{ true };
	bool m_fUpdateMarketHoliday{ true };
};

using CStockExchangePtr = shared_ptr<CStockExchange>;
