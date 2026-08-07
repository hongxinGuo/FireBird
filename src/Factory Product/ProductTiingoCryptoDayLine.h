#pragma once

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

	shared_ptr<vector<string>> CreateMessage() override;
	void ParseAndStoreWebData(CWebDataPtr pWebData) override;
	shared_ptr<vector<CDayLine>> ParseTiingoCryptoDayLine(const CWebDataPtr& pWebData);
};

using CTiingoCryptoDayLinePtr = shared_ptr<CProductTiingoCryptoDayLine>;
