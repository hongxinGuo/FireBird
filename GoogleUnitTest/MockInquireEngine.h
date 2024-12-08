#pragma once

//#include"gmock/gmock.h"

import DataSource.InquireEngine;

namespace FireBirdTest {
	class CMockInquireEngineTest;
}

namespace testing {
	class CMockInquireEngine final : public CInquireEngine {
	public:
		MOCK_METHOD(void, OpenFile, (), (override));
		MOCK_METHOD(void, GetFileHeaderInformation, (), (override));
		MOCK_METHOD(UINT, ReadWebFileOneTime, (), (override));
	};

	using CMockInquireEnginePtr = shared_ptr<CMockInquireEngine>;
}
