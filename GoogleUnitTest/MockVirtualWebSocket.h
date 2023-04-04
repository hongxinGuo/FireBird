#pragma once

#include"VirtualWebSocket.h"

namespace testing {
	class CMockVirtualWebSocket : public CVirtualWebSocket {
	public:
		MOCK_METHOD(ix::ReadyState, GetState, (), (override, const));
		MOCK_METHOD(void, Connect, (), (override));
		MOCK_METHOD(void, Send, (vector<string> vSymbol), (override));
		MOCK_METHOD(void, StartWebSocket, (), (override));
		MOCK_METHOD(void, StopWebSocket, (), (override));
	};

	using CMockVirtualWebSocketPtr = shared_ptr<CMockVirtualWebSocket>;
}
