#pragma once

#include "DayLine.h"
#include"ProductTiingo.h"

class CDayLine;

class CProductTiingoCryptoDayLine final : public CProductTiingo {
public:
	CProductTiingoCryptoDayLine();
	// 不允许赋值。
	CProductTiingoCryptoDayLine(const CProductTiingoCryptoDayLine&) = delete;
	CProductTiingoCryptoDayLine& operator=(const CProductTiingoCryptoDayLine&) = delete;
	CProductTiingoCryptoDayLine(const CProductTiingoCryptoDayLine&&) noexcept = delete;
	CProductTiingoCryptoDayLine& operator=(const CProductTiingoCryptoDayLine&&) noexcept = delete;
	~CProductTiingoCryptoDayLine() override = default;

	void InquireData(const std::stop_token& st, const string& strHeaders, const string& strParams, const string& strSuffix, const string& strInquiryToken) override; // default do nothing
	void WebStatusCheck(cpr::Response& r) override;
	void UpdateSystemStatus() override;

	shared_ptr<vector<string>> CreateMessage() override;
	CDayLinesPtr Parse(const string& text);
};

using CTiingoCryptoDayLinePtr = shared_ptr<CProductTiingoCryptoDayLine>;
