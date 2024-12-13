#pragma once

#include<memory>
#include<vector>

class CTiingoIEXTopOfBook final {
public:
	CTiingoIEXTopOfBook();
	// 不允许复制和赋值。
	CTiingoIEXTopOfBook(const CTiingoIEXTopOfBook&) = delete;
	CTiingoIEXTopOfBook& operator=(const CTiingoIEXTopOfBook&) = delete;
	CTiingoIEXTopOfBook(const CTiingoIEXTopOfBook&&) noexcept = delete;
	CTiingoIEXTopOfBook& operator=(const CTiingoIEXTopOfBook&&) noexcept = delete;
	~CTiingoIEXTopOfBook() = default;

public:
	CString m_strTicker{ _T("") };
	chrono::sys_seconds m_timeStamp;
	chrono::sys_seconds m_lastSale;
	chrono::sys_seconds m_quote;
	long m_lLastClose{ 0 }; // 前收盘。单位：0.001元
	long m_lOpen{ 0 }; // 开盘价
	long m_lHigh{ 0 }; // 最高价
	long m_lLow{ 0 }; // 最低价
	long m_lNew{ 0 }; // 最新价
	INT64 m_llVolume{ 0 };
};

using CTiingoIEXTopOfBookPtr = shared_ptr<CTiingoIEXTopOfBook>;
using CTiingoIEXTopOfBooksPtr = shared_ptr<vector<CTiingoIEXTopOfBookPtr>>;
