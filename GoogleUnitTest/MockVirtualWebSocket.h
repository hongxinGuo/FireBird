#pragma once

 

import FireBird.WebSocket.Virtual;

namespace testing {
	class CMockVirtualWebSocket final : public CVirtualWebSocket {
	public:
		MOCK_METHOD(ix::ReadyState, GetState, (), (override, const));
		MOCK_METHOD(void, Connect, (), (override));
		MOCK_METHOD(void, Send, (const vectorString& vSymbol), (override));
		MOCK_METHOD(void, StartWebSocket, (), (override));
		MOCK_METHOD(void, StopWebSocket, (), (override));
		MOCK_METHOD(void, TaskConnectAndSendMessage, (vectorString vSymbol), (override));
		MOCK_METHOD(void, TaskDisconnect, (), (override));
	};

	using CMockVirtualWebSocketPtr = shared_ptr<CMockVirtualWebSocket>;
}
