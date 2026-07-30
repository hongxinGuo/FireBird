module;

export module HistoryCandle.MonthLine;

import HistoryCandle;

import std;
using std::shared_ptr;

export {
	class CMonthLine final : public CVirtualHistoryCandle {
	public:
		CMonthLine();
	};

	using CMonthLinePtr = shared_ptr<CMonthLine>;
}
