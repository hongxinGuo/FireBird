#pragma once

#include"VirtualWebSocket.h"
#include <ixwebsocket/IXWebSocket.h>

void ProcessTiingoCryptoWebSocket(const ix::WebSocketMessagePtr& msg);

class CTiingoCryptoSocket {
public:
	CTiingoCryptoSocket() {
		m_chMessageType = ' ';
		m_sDateTime = _T("");
		m_sSymbol = _T("");
		m_strExchange = _T("");
		m_dBidSize = m_dBidPrice = m_dMidPrice = m_dAskPrice = m_dAskSize = m_dLastPrice = m_dLastSize = 0;
	}

public:
	char m_chMessageType; // 'Q' or 'T'
	string m_sDateTime; // json ISO format
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
	~CTiingoCryptoWebSocket() override = default;

	void Connect() override;
	void Send(const vectorString& vSymbol) override;

	void CreateThreadConnectWebSocketAndSendMessage(vectorString vSymbol) override;

	static string CreateMessage(const vectorString& vSymbol);
	bool ParseTiingoCryptoWebSocketData(shared_ptr<string> pData);
};

using CTiingoCryptoWebSocketPtr = shared_ptr<CTiingoCryptoWebSocket>;

extern CTiingoCryptoWebSocket gl_tiingoCryptoWebSocket;
