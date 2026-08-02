module;

export module FireBirdLib.HistoryCandle.MonthLine;

export import FireBirdLib.HistoryCandle;

import std;
using std::shared_ptr;

export {
	class CMonthLine final : public CVirtualHistoryCandle {
	public:
		CMonthLine();
	};

	using CMonthLinePtr = shared_ptr<CMonthLine>;
}
