#pragma once

 

import FireBird.WebSocket.TiingoForex;

namespace testing {
	class CMockTiingoForexWebSocket final : public CTiingoForexWebSocket {
	public:
		MOCK_METHOD(ix::ReadyState, GetState, (), (override, const));
		MOCK_METHOD(void, TaskConnectAndSendMessage, (vectorString vSymbol), (override));
		MOCK_METHOD(void, TaskDisconnect, (), (override));
	};

	using CMockTiingoForexWebSocketPtr = shared_ptr<CMockTiingoForexWebSocket>;
}
