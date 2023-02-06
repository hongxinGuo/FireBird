#pragma once

#include"VirtualDataSource.h"

namespace FireBirdTest {
	class CMockVirtualDataSourceTest;
}

namespace testing {
	class CMockVirtualDataSource final : public CVirtualDataSource {
	public:
		MOCK_METHOD(bool, Inquire, (const long), (override));
		MOCK_METHOD(bool, ProcessInquiringMessage, (), (override));
		MOCK_METHOD(bool, ProcessWebDataReceived, (), (override));
		MOCK_METHOD(void, ParseAndStoreData, (CVirtualProductWebDataPtr pProductWebData, CWebDataPtr pWebData), (override));
		MOCK_METHOD(bool, UpdateStatus, (), (override));
		MOCK_METHOD(void, OpenFile, (const CString& strInquiring), (override));
		MOCK_METHOD(void, GetFileHeaderInformation, (), (override));
		MOCK_METHOD(bool, PrepareNextInquiringString, (), (override));
		MOCK_METHOD(void, StartReadingThread, (), (override));
		MOCK_METHOD(UINT, ReadWebFileOneTime, (), (override));
	};

	using CMockVirtualDataSourcePtr = shared_ptr<CMockVirtualDataSource>;
}
