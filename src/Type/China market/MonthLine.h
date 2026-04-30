#pragma once

#include"VirtualHistoryCandleBasic.h"

class CMonthLine final : public CVirtualHistoryCandleBasic {
public:
	CMonthLine() = default;
};

using CMonthLinePtr = shared_ptr<CMonthLine>;
