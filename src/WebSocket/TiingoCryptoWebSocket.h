#pragma once

#include"VirtualWebSocket.h"
#include <ixwebsocket/IXWebSocket.h>

void ProcessTiingoCryptoWebSocket(const ix::WebSocketMessagePtr& msg);

class CTiingoCryptoSocket {
public:
	CTiingoCryptoSocket() {
		m_chMessageType = ' ';
		m_sSymbol = _T("");
		m_strExchange = _T("");
		m_dBidSize = m_dBidPrice = m_dMidPrice = m_dAskPrice = m_dAskSize = m_dLastPrice = m_dLastSize = 0;
	}

public:
	char m_chMessageType; // 'Q' or 'T'
	string m_sSymbol;
	string m_strExchange;
	double m_dBidSize;
	double m_dBidPrice;
	double m_dMidPrice;
	double m_dAskPrice;
	double m_dAskSize;
	double m_dLastPrice;
	double m_dLastSize;
};

using CTiingoCryptoSocketPtr = shared_ptr<CTiingoCryptoSocket>;

class CTiingoCryptoWebSocket : public CVirtualWebSocket {
public:
	CTiingoCryptoWebSocket();
	~CTiingoCryptoWebSocket(void) override = default;

	bool Connect(void) override;
	bool Send(vectorString vSymbol) override;

	string CreateMessage2(vectorString vSymbol);

	bool CreateThreadConnectWebSocketAndSendMessage(vectorString vSymbol);

	string CreateMessage(vectorString vSymbol);
	bool ParseTiingoCryptoWebSocketData(shared_ptr<string> pData);
};

using CTiingoCryptoWebSocketPtr = shared_ptr<CTiingoCryptoWebSocket>;

extern CTiingoCryptoWebSocket gl_tiingoCryptoWebSocket;
