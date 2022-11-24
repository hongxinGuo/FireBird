#pragma once

#include"VirtualWebSocket.h"
#include <ixwebsocket/IXWebSocket.h>

void ProcessTiingoCryptoWebSocket(const ix::WebSocketMessagePtr& msg);

class CTiingoCryptoWebSocket : public CVirtualWebSocket {
public:
	CTiingoCryptoWebSocket();
	~CTiingoCryptoWebSocket(void);

	virtual bool Connect(void) override;
	virtual bool Send(vector<CString> vSymbol) override;

	bool CreatingThreadConnectWebSocketAndSendMessage(vector<CString> vSymbol);

	CString CreateMessage(vector<CString> vSymbol);
	bool ParseTiingoCryptoWebSocketData(shared_ptr<string> pData);
};

typedef shared_ptr<CTiingoCryptoWebSocket> CTiingoCryptoWebSocketPtr;

class CTiingoCryptoSocket : public CObject {
public:
	CTiingoCryptoSocket() {
		m_chMessageType = ' ';
		m_strSymbol = _T("");
		m_strExchange = _T("");
		m_dBidSize = m_dBidPrice = m_dMidPrice = m_dAskPrice = m_dAskSize = m_dLastPrice = m_dLastSize = 0;
	}

public:
	char m_chMessageType; // 'Q' or 'T'
	CString m_strSymbol;
	CTime m_date;
	CString m_strExchange;
	double m_dBidSize;
	double m_dBidPrice;
	double m_dMidPrice;
	double m_dAskPrice;
	double m_dAskSize;
	double m_dLastPrice;
	double m_dLastSize;
};

typedef shared_ptr<CTiingoCryptoSocket> CTiingoCryptoSocketPtr;

extern CTiingoCryptoWebSocket gl_tiingoCryptoWebSocket;
