#pragma once

#include"QuandlDataSource.h"

namespace testing {
	class CMockQuandlDataSource final : public CQuandlDataSource {
	public:
		MOCK_METHOD((chrono::time_point<chrono::steady_clock>), GetTickCount, (), (override));
		MOCK_METHOD(bool, Inquire, (), (override));
	};

	using CMockQuandlDataSourcePtr = shared_ptr<CMockQuandlDataSource>;
}
