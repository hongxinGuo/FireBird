#pragma once

#include"SinaRTDataSource.h"

namespace testing {
	class CMockSinaRTDataSource final : public CSinaRTDataSource {
	public:
		MOCK_METHOD(void, StartReadingThread, (), (override));
		MOCK_METHOD(void, ReadWebData, (), (override));
		MOCK_METHOD(ULONGLONG, GetTickCount, (), (override));
	};

	using CMockSinaRTDataSourcePtr = shared_ptr<CMockSinaRTDataSource>;
}
