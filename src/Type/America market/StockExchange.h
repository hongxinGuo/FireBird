#pragma once

#include"SetStockExchange.h"

#include<memory>

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

public:
	CString m_strCode; // 两位的交易所代码。上海市场为SS， 美国市场为US，。。。
	CString m_strName;
	CString m_strMic;
	CString m_strTimeZone;
	CString m_strPreMarket;
	CString m_strHour;
	CString m_strPostMarket;
	CString m_strCloseDate;
	CString m_strCountry;
	CString m_strCountryName;
	CString m_strSource;
	CString m_strMyUnknownColumn;

	long m_lMarketOpenTime{ 0 };
	long m_lMarketCloseTime{ 0 };

	bool m_fStockSymbolUpdated;
	bool m_fMarketStatusUpdated;
	bool m_fMarketHolidayUpdated;
};

using CStockExchangePtr = shared_ptr<CStockExchange>;
