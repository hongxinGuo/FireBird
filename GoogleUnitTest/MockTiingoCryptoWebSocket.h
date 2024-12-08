#pragma once

//#include"gmock/gmock.h"

import FireBird.WebSocket.TiingoCrypto;

namespace testing {
	class CMockTiingoCryptoWebSocket final : public CTiingoCryptoWebSocket {
	public:
		MOCK_METHOD(ix::ReadyState, GetState, (), (override, const));
		MOCK_METHOD(void, TaskConnectAndSendMessage, (vectorString vSymbol), (override));
		MOCK_METHOD(void, TaskDisconnect, (), (override));
	};

	using CMockTiingoCryptoWebSocketPtr = shared_ptr<CMockTiingoCryptoWebSocket>;
}
