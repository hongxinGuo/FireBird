#pragma once

#include "DayLine.h"
#include"ProductTiingo.h"

class CDayLine;

class CProductTiingoForexDayLine final : public CProductTiingo {
public:
	CProductTiingoForexDayLine();
	// 不允许赋值。
	CProductTiingoForexDayLine(const CProductTiingoForexDayLine&) = delete;
	CProductTiingoForexDayLine& operator=(const CProductTiingoForexDayLine&) = delete;
	CProductTiingoForexDayLine(const CProductTiingoForexDayLine&&) noexcept = delete;
	CProductTiingoForexDayLine& operator=(const CProductTiingoForexDayLine&&) noexcept = delete;
	~CProductTiingoForexDayLine() override = default;

	void InquireData(const std::stop_token& st, const string& strHeaders, const string& strParams, const string& strSuffix, const string& strInquiryToken) override; // default do nothing
	void WebStatusCheck(cpr::Response& r) override;
	void UpdateSystemStatus() override;

	shared_ptr<vector<string>> CreateMessage() override;
	CDayLinesPtr Parse(const string& text);
};

using CTiingoForexDayLinePtr = shared_ptr<CProductTiingoForexDayLine>;
