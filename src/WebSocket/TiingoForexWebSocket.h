#pragma once

#include"VirtualWebSocket.h"
#include <ixwebsocket/IXWebSocket.h>

void ProcessTiingoForexWebSocket(const ix::WebSocketMessagePtr& msg);

class CTiingoForexSocket {
public:
	CTiingoForexSocket() {
		m_chMessageType = 'Q';
		m_sSymbol = _T("");
		m_dBidSize = m_dBidPrice = m_dMidPrice = m_dAskPrice = m_dAskSize = 0;
	}

public:
	char m_chMessageType; // ±ÿ–Î «'Q'
	time_t tTime;
	string m_sSymbol;
	double m_dBidSize;
	double m_dBidPrice;
	double m_dMidPrice;
	double m_dAskPrice;
	double m_dAskSize;
};

using CTiingoForexSocketPtr = shared_ptr<CTiingoForexSocket>;

class CTiingoForexWebSocket : public CVirtualWebSocket {
public:
	CTiingoForexWebSocket();
	~CTiingoForexWebSocket(void) override;

	bool Connect(void) override;
	bool Send(vectorString vSymbol) override;

	bool CreatingThreadConnectWebSocketAndSendMessage(vectorString vSymbol);

	string CreateMessage(vectorString vSymbol);
	bool ParseTiingoForexWebSocketData(shared_ptr<string> pData);
};

using CTiingoForexWebSocketPtr = shared_ptr<CTiingoForexWebSocket>;

extern CTiingoForexWebSocket gl_tiingoForexWebSocket;
