#pragma once

 

#include"ContainerChinaDayLine.h"

namespace testing {
	class CMockDataChinaDayLine final : public CContainerChinaDayLine {
	public:
		MOCK_METHOD(bool, CalculateRS1, (INT64 lNumber), (override));
		MOCK_METHOD(bool, CalculateRSLogarithm1, (INT64 lNumber), (override));
		MOCK_METHOD(bool, CalculateRSIndex1, (INT64 lNumber), (override));
	};

	using CMockDataChinaDayLinePtr = shared_ptr<CMockDataChinaDayLine>;
}
