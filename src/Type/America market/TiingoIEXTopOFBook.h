#pragma once

#include<memory>
#include<vector>

#include "ProductTiingo.h"

class CTiingoIEXTopOFBook final {
public:
	CTiingoIEXTopOFBook();
	// 不允许复制和赋值。
	CTiingoIEXTopOFBook(const CTiingoIEXTopOFBook&) = delete;
	CTiingoIEXTopOFBook& operator=(const CTiingoIEXTopOFBook&) = delete;
	CTiingoIEXTopOFBook(const CTiingoIEXTopOFBook&&) noexcept = delete;
	CTiingoIEXTopOFBook& operator=(const CTiingoIEXTopOFBook&&) noexcept = delete;
	~CTiingoIEXTopOFBook() = default;

public:
	CString m_strTicker{ _T("") };
	INT64 m_llTimestamp{ 0 };

	// 
	long m_lLastClose{ 0 }; // 前收盘。单位：0.001元
	long m_lOpen{ 0 }; // 开盘价
	long m_lHigh{ 0 }; // 最高价
	long m_lLow{ 0 }; // 最低价
	long m_lNew{ 0 }; // 最新价

	INT64 m_llVolume{ 0 };
};

using CTiingoIEXTopOFBookPtr = shared_ptr<CTiingoIEXTopOFBook>;
using CTiingoIEXTopOFBooksPtr = shared_ptr<vector<CTiingoIEXTopOFBookPtr>>;
