#pragma once

#include"NeteaseRTDataSource.h"

namespace testing {
	class CMockNeteaseRTDataSource final : public CNeteaseRTDataSource {
	public:
		MOCK_METHOD(bool, GetWebData, (), (override));
		MOCK_METHOD(void, ReadWebData, (), (override));
		MOCK_METHOD(ULONGLONG, GetTickCount, (), (override));
	};

	using CMockNeteaseRTDataSourcePtr = shared_ptr<CMockNeteaseRTDataSource>;
}
