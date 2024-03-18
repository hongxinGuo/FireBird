#pragma once

#include"QuandlDataSource.h"

namespace testing {
	class CMockQuandlDataSource final : public CQuandlDataSource {
	public:
		MOCK_METHOD(ULONGLONG, GetTickCount, (), (override));
		MOCK_METHOD(bool, Inquire, (), (override));
	};

	using CMockQuandlDataSourcePtr = shared_ptr<CMockQuandlDataSource>;
}
