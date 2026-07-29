module;

export module DayLine;

import VirtualHistoryCandle;

import std;
using std::vector;
using std::shared_ptr;

export {
	class CDayLine final : public CVirtualHistoryCandle {
	public:
		CDayLine();
		CDayLine(const CDayLine& other) = default;
		CDayLine& operator=(const CDayLine& other) = default;
		CDayLine(CDayLine&& other) noexcept = default;
		CDayLine& operator=(CDayLine&& other) noexcept = default;
		~CDayLine() override = default;
	};

	using CDayLinePtr = shared_ptr<CDayLine>;
	using CDayLinesPtr = shared_ptr<vector<CDayLine>>;
}
