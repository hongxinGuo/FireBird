#pragma once

#include"ChinaMarket.h"

namespace testing {
	class CMockChinaMarket final : public CChinaMarket {
	public:
		MOCK_METHOD(void, TaskCreateTask, (long), (override));
		MOCK_METHOD(void, TaskExitSystem, (), (override));
		MOCK_METHOD(bool, TaskResetMarket, (long lCurrentTime), (override));

		MOCK_METHOD(void, UpdateOptionDB, (), (override));
		MOCK_METHOD(void, AppendChosenStockDB, (), (override));
	};

	using CMockChinaMarketPtr = shared_ptr<CMockChinaMarket>;
}
