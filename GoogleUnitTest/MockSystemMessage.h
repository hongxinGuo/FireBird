#pragma once

//#include"gmock/gmock.h"

import FireBird.System.Message;

namespace testing {
	class CMockSystemDeque final : public CSystemDeque {
	public:
		MOCK_METHOD(void, SysCallOutputListAddString, (COutputList* pOutputList, const CString& str), (override));
	};
}
