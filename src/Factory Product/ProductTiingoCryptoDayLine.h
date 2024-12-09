#pragma once

#include"ProductTiingo.h"
import FireBird.HistoryCandle.DayLine;

class CProductTiingoCryptoDayLine final : public CProductTiingo {
public:
	CProductTiingoCryptoDayLine();
	~CProductTiingoCryptoDayLine() override = default;

	CString CreateMessage() override;
	void ParseAndStoreWebData(CWebDataPtr pWebData) override;
	CDayLinesPtr ParseTiingoCryptoDayLine(const CWebDataPtr& pWebData);
};

using CTiingoCryptoDayLinePtr = shared_ptr<CProductTiingoCryptoDayLine>;
