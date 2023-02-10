#pragma once

#include"QuandlDataSource.h"

namespace testing {
	class CMockQuandlDataSource final : public CQuandlDataSource {
	public:
		MOCK_METHOD(void, StartReadingThread, (), (override));
		MOCK_METHOD(void, ReadWebData, (), (override));
		MOCK_METHOD(CWebDataPtr, CreateWebDataAfterSucceedReading, (), (override));
	};

	using CMockQuandlDataSourcePtr = shared_ptr<CMockQuandlDataSource>;
}
