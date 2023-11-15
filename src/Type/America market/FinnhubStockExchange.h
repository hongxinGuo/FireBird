#pragma once

#include"SetFinnhubStockExchange.h"

#include<memory>

class CFinnhubStockExchange final {
public:
	CFinnhubStockExchange();
	~CFinnhubStockExchange() = default;

	void Append(CSetFinnhubStockExchange& setFinnhubExchange) const;

	void Load(const CSetFinnhubStockExchange& setFinnhubExchange);

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
	CString m_strHour;
	CString m_strCloseDate;
	CString m_strCountry;
	CString m_strSource;

	bool m_fStockSymbolUpdated;
	bool m_fMarketStatusUpdated;
	bool m_fMarketHolidayUpdated;
};

using CFinnhubStockExchangePtr = shared_ptr<CFinnhubStockExchange>;
