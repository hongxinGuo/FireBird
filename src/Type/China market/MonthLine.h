#pragma once

#include"VirtualHistoryCandle.h"

class CMonthLine final : public CVirtualHistoryCandle {
public:
	CMonthLine();
};

using CMonthLinePtr = shared_ptr<CMonthLine>;
