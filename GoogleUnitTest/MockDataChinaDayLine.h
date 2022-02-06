#pragma once
#include"DataChinaDayLine.h"

namespace testing {
	class CMockDataChinaDayLine : public CDataChinaDayLine {
	public:
		MOCK_METHOD(bool, CalculateRS1, (INT64 lNumber), (override));
		MOCK_METHOD(bool, CalculateRSLogarithm1, (INT64 lNumber), (override));
		MOCK_METHOD(bool, CalculateRSIndex1, (INT64 lNumber), (override));
	};

	typedef shared_ptr<CMockDataChinaDayLine> CMockDataChinaDayLinePtr;
}
