#pragma once

#include"ChinaStock.h"

namespace testing {
	class CMockChinaStock final : public CChinaStock {
	public:
		MOCK_METHOD(bool, SaveDayLineBasicInfo, (), (override));
		MOCK_METHOD(bool, LoadDayLine, (const string& strStockCode), (override));
		MOCK_METHOD(bool, SaveWeekLine, (), (override));
		MOCK_METHOD(bool, LoadWeekLine, (), (override));
		MOCK_METHOD(bool, BuildWeekLine, (long lStartDate), (override));
		MOCK_METHOD(bool, Calculate10RSStrong1StockSet, (), (override));
		MOCK_METHOD(bool, Calculate10RSStrong2StockSet, (), (override));
		MOCK_METHOD(bool, Calculate10RSStrongStockSet, (const CRSReference* pRef), (override));
	};

	using CMockChinaStockPtr = shared_ptr<CMockChinaStock>;
}
