#pragma once

#include"ProductTiingo.h"
#include"DayLine.h"

class CProductTiingoCryptoDayLine final : public CProductTiingo {
public:
	CProductTiingoCryptoDayLine();
	// 不允许赋值。
	CProductTiingoCryptoDayLine(const CProductTiingoCryptoDayLine&) = delete;
	CProductTiingoCryptoDayLine& operator=(const CProductTiingoCryptoDayLine&) = delete;
	CProductTiingoCryptoDayLine(const CProductTiingoCryptoDayLine&&) noexcept = delete;
	CProductTiingoCryptoDayLine& operator=(const CProductTiingoCryptoDayLine&&) noexcept = delete;
	~CProductTiingoCryptoDayLine() override = default;

	string CreateMessage() override;
	void ParseAndStoreWebData(CWebDataPtr pWebData) override;
	CDayLinesPtr ParseTiingoCryptoDayLine(const CWebDataPtr& pWebData);
};

using CTiingoCryptoDayLinePtr = shared_ptr<CProductTiingoCryptoDayLine>;
