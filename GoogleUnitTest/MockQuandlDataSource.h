#pragma once

#include"QuandlDataSource.h"

namespace testing {
	class CMockQuandlDataSource final : public CQuandlDataSource {
	public:
		MOCK_METHOD(void, GetWebData, (), (override));
		MOCK_METHOD(void, ReadWebData, (), (override));
		MOCK_METHOD(CWebDataPtr, CreateWebDataAfterSucceedReading, (), (override));
		MOCK_METHOD(ULONGLONG, GetTickCount, (), (override));
		MOCK_METHOD(bool, InquireQuandl, (), (override));
	};

	using CMockQuandlDataSourcePtr = shared_ptr<CMockQuandlDataSource>;
}
