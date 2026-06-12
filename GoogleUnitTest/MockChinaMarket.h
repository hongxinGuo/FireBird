#pragma once

#include"ChinaMarket.h"

namespace testing {
	class CMockChinaMarket final : public CChinaMarket {
	public:
		MOCK_METHOD(void, TaskCreateTask, (), (override));
		MOCK_METHOD(void, TaskExitSystem, (), (override));
		MOCK_METHOD(bool, TaskResetMarket, (), (override));

		MOCK_METHOD(void, UpdateOptionDB, (), (override));
		MOCK_METHOD(void, AppendChosenStockDB, (), (override));
	};

	using CMockChinaMarketPtr = shared_ptr<CMockChinaMarket>;
}
