#pragma once

#include"WorldStock.h"

namespace testing {
	class CMockWorldStock final : public CWorldStock {
	public:
		MOCK_METHOD(bool, UpdateEPSSurpriseDB, (), (override));
	};

	using CMockWorldStockPtr = shared_ptr<CMockWorldStock>;
}
