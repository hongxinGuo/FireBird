#pragma once

#include"VirtualDataSource.h"

namespace testing {
	class CMockVirtualDataSource final : public CVirtualDataSource {
	public:
		MOCK_METHOD(bool, OpenFile, (const CString& strInquiring), (override));
		MOCK_METHOD(bool, PrepareNextInquiringString, (), (override));
		MOCK_METHOD(void, StartReadingThread, (), (override));
		MOCK_METHOD(UINT, ReadWebFileOneTime, (), (override));
	};
}
