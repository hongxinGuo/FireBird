#pragma once

#include"VirtualDataSource.h"

namespace FireBirdTest {
	class CMockVirtualDataSourceTest;
}

namespace testing {
	class CMockVirtualDataSource final : public CVirtualDataSource {
	public:
		MOCK_METHOD(bool, GenerateInquiryMessage, (const long), (override));
		MOCK_METHOD(bool, GetWebData, (), (override));
		MOCK_METHOD(bool, ProcessWebDataReceived, (), (override));
		MOCK_METHOD(void, ParseAndStoreData, (CVirtualProductWebDataPtr pProductWebData, CWebDataPtr pWebData), (override));
		MOCK_METHOD(void, UpdateStatus, (), (override));
		MOCK_METHOD(void, OpenFile, (const CString& strInquiring), (override));
		MOCK_METHOD(void, GetFileHeaderInformation, (), (override));
		MOCK_METHOD(void, CreateInquiryMessageFromCurrentProduct, (), (override));
		MOCK_METHOD(void, StartReadingThread, (), (override));
		MOCK_METHOD(UINT, ReadWebFileOneTime, (), (override));
	};

	using CMockVirtualDataSourcePtr = shared_ptr<CMockVirtualDataSource>;
}