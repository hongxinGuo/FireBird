module;

export module WeekLine;

import VirtualHistoryCandle;

import std;
using std::shared_ptr;

export {
	class CWeekLine final : public CVirtualHistoryCandle {
	public:
		CWeekLine();

		bool UpdateWeekLine(const CVirtualHistoryCandle* pDayLine);
	};

	using CWeekLinePtr = shared_ptr<CWeekLine>;
}
