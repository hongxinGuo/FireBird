#pragma once

#include"ContainerChinaDayLine.h"

namespace testing {
	class CMockDataChinaDayLine final : public CContainerChinaDayLine {
	public:
	};

	using CMockDataChinaDayLinePtr = shared_ptr<CMockDataChinaDayLine>;
}
