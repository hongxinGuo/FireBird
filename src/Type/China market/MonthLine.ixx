module;

export module MonthLine;

import VirtualHistoryCandle;

import std;
using std::shared_ptr;

export {
	class CMonthLine final : public CVirtualHistoryCandle {
	public:
		CMonthLine();
	};

	using CMonthLinePtr = shared_ptr<CMonthLine>;
}
