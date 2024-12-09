#pragma once

 

import FireBird.WebSocket.TiingoIEX;

namespace testing {
	class CMockTiingoIEXWebSocket final : public CTiingoIEXWebSocket {
	public:
		MOCK_METHOD(ix::ReadyState, GetState, (), (override, const));
		MOCK_METHOD(void, TaskConnectAndSendMessage, (vectorString vSymbol), (override));
		MOCK_METHOD(void, TaskDisconnect, (), (override));
	};

	using CMockTiingoIEXWebSocketPtr = shared_ptr<CMockTiingoIEXWebSocket>;
}
