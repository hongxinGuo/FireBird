module;

export module FireBirdLib.HistoryCandle.WeekLine;

import FireBirdLib.HistoryCandle.Virtual;

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
