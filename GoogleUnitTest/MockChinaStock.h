#pragma once

#include"ChinaStock.h"

namespace testing {
	class CMockChinaStock final : public CChinaStock {
	public:
		MOCK_METHOD(bool, SaveDayLineDB, (), (override));
		MOCK_METHOD(bool, LoadDayLineDB, (), (override));
		MOCK_METHOD(bool, SaveWeekLine, (), (override));
		MOCK_METHOD(bool, LoadWeekLineDB, (), (override));
		MOCK_METHOD(bool, BuildWeekLine, (long lStartDate), (override));
	};

	using CMockChinaStockPtr = shared_ptr<CMockChinaStock>;
}
