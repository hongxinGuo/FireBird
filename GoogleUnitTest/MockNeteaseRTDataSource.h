#pragma once

#include"NeteaseRTDataSource.h"

namespace testing {
	class CMockNeteaseRTDataSource final : public CNeteaseRTDataSource {
	public:
		MOCK_METHOD(void, StartReadingThread, (), (override));
		MOCK_METHOD(bool, ReadingWebData, (), (override));
	};

	using CMockNeteaseRTDataSourcePtr = shared_ptr<CMockNeteaseRTDataSource>;
}
