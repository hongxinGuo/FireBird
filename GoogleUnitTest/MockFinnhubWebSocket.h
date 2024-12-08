#pragma once

//#include"gmock/gmock.h"

import FireBird.WebSocket.Finnhub;

namespace testing {
	class CMockFinnhubWebSocket final : public CFinnhubWebSocket {
	public:
		MOCK_METHOD(ix::ReadyState, GetState, (), (override, const));
		MOCK_METHOD(void, TaskConnectAndSendMessage, (vectorString vSymbol), (override));
		MOCK_METHOD(void, TaskDisconnect, (), (override));
	};

	using CMockFinnhubWebSocketPtr = shared_ptr<CMockFinnhubWebSocket>;
}
