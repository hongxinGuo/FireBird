#pragma once

#include"ProductTiingo.h"
#include"DayLine.h"

class CProductTiingoCryptoDayLine final : public CProductTiingo {
public:
	CProductTiingoCryptoDayLine();
	~CProductTiingoCryptoDayLine() override = default;

	string CreateMessage() override;
	void ParseAndStoreWebData(CWebDataPtr pWebData) override;
	CDayLinesPtr ParseTiingoCryptoDayLine(const CWebDataPtr& pWebData);
};

using CTiingoCryptoDayLinePtr = shared_ptr<CProductTiingoCryptoDayLine>;
