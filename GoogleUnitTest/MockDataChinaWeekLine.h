#pragma once

#include"DataChinaWeekLine.h"

namespace testing {
	class CMockDataChinaWeekLine final : public CDataChinaWeekLine {
	public:
		MOCK_METHOD(bool, CalculateRS1, (INT64 lNumber), (override));
		MOCK_METHOD(bool, CalculateRSLogarithm1, (INT64 lNumber), (override));
		MOCK_METHOD(bool, CalculateRSIndex1, (INT64 lNumber), (override));
	};

	using CMockDataChinaWeekLinePtr = shared_ptr<CMockDataChinaWeekLine>;
}
