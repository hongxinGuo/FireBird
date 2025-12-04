#pragma once

#include"VirtualHistoryCandleExtend.h"

class CMonthLine final : public CVirtualHistoryCandleExtend {
public:
	CMonthLine() = default;
};

using CMonthLinePtr = shared_ptr<CMonthLine>;
