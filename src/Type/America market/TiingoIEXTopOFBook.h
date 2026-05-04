#pragma once

#include<memory>
#include<vector>

class CTiingoIEXTopOfBook final {
public:
	CTiingoIEXTopOfBook();
	// 不允许复制和赋值。
	CTiingoIEXTopOfBook(const CTiingoIEXTopOfBook&) = default;
	CTiingoIEXTopOfBook& operator=(const CTiingoIEXTopOfBook&) = default;
	CTiingoIEXTopOfBook(CTiingoIEXTopOfBook&&) noexcept = default;
	CTiingoIEXTopOfBook& operator=(CTiingoIEXTopOfBook&&) noexcept = default;
	~CTiingoIEXTopOfBook() = default;

	void Reset() noexcept {
		m_strTicker.clear();
		m_timeStamp = std::chrono::sys_seconds(std::chrono::time_point_cast<std::chrono::seconds>(std::chrono::system_clock::from_time_t(0)));
		m_lastSale = std::chrono::sys_seconds(std::chrono::time_point_cast<std::chrono::seconds>(std::chrono::system_clock::from_time_t(0)));
		m_quote = std::chrono::sys_seconds(std::chrono::time_point_cast<std::chrono::seconds>(std::chrono::system_clock::from_time_t(0)));
		m_lLastClose = 0;
		m_lOpen = 0;
		m_lHigh = 0;
		m_lLow = 0;
		m_lNew = 0;
		m_llVolume = 0;
	}

public:
	string m_strTicker{ "" };
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
using CTiingoIEXTopOfBooksPtr = shared_ptr<vector<CTiingoIEXTopOfBook>>;
