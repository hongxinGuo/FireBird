#pragma once

#include <gmock/gmock-function-mocker.h>

#include"WorldMarket.h"

namespace testing {
	class CMockWorldMarket final : public CWorldMarket {
	public:
		MOCK_METHOD(bool, UpdateFinnhubStockDayLineDB, (std::stop_token), (override));
		MOCK_METHOD(bool, UpdateInsiderSentimentDB, (std::stop_token), (override));
		MOCK_METHOD(bool, UpdateCompanyNewsDB, (std::stop_token), (override));
		MOCK_METHOD(bool, UpdateTiingoIndustry, (std::stop_token), (override));
		MOCK_METHOD(bool, UpdateSicIndustry, (), (override));
		MOCK_METHOD(bool, UpdateNaicsIndustry, (), (override));
		MOCK_METHOD(local_seconds, GetResetTime, (), (override));
	};

	using CMockWorldMarketPtr = shared_ptr<CMockWorldMarket>;
}
