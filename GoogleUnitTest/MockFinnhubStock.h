#pragma once

#include"FinnhubStock.h"

namespace testing {
	class CMockFinnhubStock final : public CFinnhubStock {
	public:
		MOCK_METHOD(bool, UpdateEPSSurpriseDB, (), (override));
	};

	using CMockWorldStockPtr = shared_ptr<CMockFinnhubStock>;
}
