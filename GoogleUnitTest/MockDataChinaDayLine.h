#pragma once

#include"ContainerChinaDayLine.h"

namespace testing {
	class CMockDataChinaDayLine final : public CContainerChinaStockDayLine {
	public:
	};

	using CMockDataChinaDayLinePtr = shared_ptr<CMockDataChinaDayLine>;
}
