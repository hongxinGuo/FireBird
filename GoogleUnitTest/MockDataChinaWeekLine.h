#pragma once
#include"DataChinaWeekLine.h"

namespace testing {
	class CMockDataChinaWeekLine : public CDataChinaWeekLine {
	public:
		MOCK_METHOD(bool, CalculateRS1, (INT64 lNumber), (override));
		MOCK_METHOD(bool, CalculateRSLogarithm1, (INT64 lNumber), (override));
		MOCK_METHOD(bool, CalculateRSIndex1, (INT64 lNumber), (override));
	};

	typedef shared_ptr<CMockDataChinaWeekLine> CMockDataChinaWeekLinePtr;
}
