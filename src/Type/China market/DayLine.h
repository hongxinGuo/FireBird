#pragma once

#include"VirtualHistoryCandle.h"

#include<memory>

using std::vector;

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
