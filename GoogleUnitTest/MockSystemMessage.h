#pragma once

#include"SystemMessage.h"

namespace testing {
	class CMockSystemDeque final : public CSystemDeque {
	public:
		MOCK_METHOD(void, SysCallOutputListAddString, (COutputList* pOutputList, const CString& str), (override));
	};
}
