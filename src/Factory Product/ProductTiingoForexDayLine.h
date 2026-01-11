#pragma once

#include"ProductTiingo.h"
#include"DayLine.h"

class CProductTiingoForexDayLine final : public CProductTiingo {
public:
	CProductTiingoForexDayLine();
	// 不允许赋值。
	CProductTiingoForexDayLine(const CProductTiingoForexDayLine&) = delete;
	CProductTiingoForexDayLine& operator=(const CProductTiingoForexDayLine&) = delete;
	CProductTiingoForexDayLine(const CProductTiingoForexDayLine&&) noexcept = delete;
	CProductTiingoForexDayLine& operator=(const CProductTiingoForexDayLine&&) noexcept = delete;
	~CProductTiingoForexDayLine() override = default;

	string CreateMessage() override;
	void ParseAndStoreWebData(CWebDataPtr pWebData) override;
	CDayLinesPtr ParseTiingoForexDayLine(const CWebDataPtr& pWebData);
};

using CTiingoForexDayLinePtr = shared_ptr<CProductTiingoForexDayLine>;
