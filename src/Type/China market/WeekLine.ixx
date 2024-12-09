#include"pch.h"
module;
export module FireBird.HistoryCandle.WeekLine;

import FireBird.HistoryCandle.VirtualExtend;

export {
	class CWeekLine final : public CVirtualHistoryCandleExtend {
	public:
		CWeekLine();

		bool UpdateWeekLine(const CVirtualHistoryCandleExtendPtr& pHistoryCandleExtend);
	};

	using CWeekLinePtr = shared_ptr<CWeekLine>;
}
