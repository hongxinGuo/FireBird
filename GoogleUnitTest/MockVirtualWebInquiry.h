#pragma once

#include"VirtualWebInquiry.h"

namespace testing {
	class CMockVirtualWebInquiry final : public CVirtualWebInquiry {
	public:
		MOCK_METHOD(bool, OpenFile, (CString strInquiring), (override));
		MOCK_METHOD(bool, PrepareNextInquiringString, (), (override));
		MOCK_METHOD(CString, GetNextInquiringMiddleString, (long, bool), (override));
		MOCK_METHOD(void, StartReadingThread, (), (override));
		MOCK_METHOD(UINT, ReadWebFileOneTime, (), (override));
	};
}
