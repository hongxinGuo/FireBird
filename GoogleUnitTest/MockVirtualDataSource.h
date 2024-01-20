#pragma once

#include"VirtualDataSource.h"

namespace FireBirdTest {
	class CMockVirtualDataSourceTest;
}

namespace testing {
	class CMockVirtualDataSource final : public CVirtualDataSource {
	public:
		MOCK_METHOD(bool, GenerateInquiryMessage, (const long), (override));
		MOCK_METHOD(void, CreateThreadGetWebDataAndProcessIt, (), (override));
		MOCK_METHOD(void, GetWebData, (), (override));
		MOCK_METHOD(void, ProcessWebDataReceived, (), (override));
		MOCK_METHOD(void, OpenFile, (const CString& strInquiring), (override));
		MOCK_METHOD(void, GetFileHeaderInformation, (), (override));
		MOCK_METHOD(void, GenerateCurrentInquiryMessage, (), (override));
		MOCK_METHOD(UINT, ReadWebFileOneTime, (), (override));
	};

	using CMockVirtualDataSourcePtr = shared_ptr<CMockVirtualDataSource>;
}
