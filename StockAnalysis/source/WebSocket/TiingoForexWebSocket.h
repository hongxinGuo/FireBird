#pragma once

#include"VirtualWebSocket.h"
#include <ixwebsocket/IXWebSocket.h>

void ProcessTiingoForexWebSocket(const ix::WebSocketMessagePtr& msg);

class CTiingoForexWebSocket : public CVirtualWebSocket {
public:
	CTiingoForexWebSocket();
	~CTiingoForexWebSocket(void);

	virtual bool Connect(void);
	virtual bool Send(vector<CString> vSymbol) override;

	bool CreatingThreadConnectWebSocketAndSendMessage(vector<CString> vSymbol);

	CString CreateMessage(vector<CString> vSymbol);
	bool ParseTiingoForexWebSocketData(shared_ptr<string> pData);
};

typedef shared_ptr<CTiingoForexWebSocket> CTiingoForexWebSocketPtr;

class CTiingoForexSocket : public CObject {
public:
	CTiingoForexSocket() {
		m_chMessageType = 'Q';
		m_strSymbol = _T("");
		m_dBidSize = m_dBidPrice = m_dMidPrice = m_dAskPrice = m_dAskSize = 0;
	}

public:
	char m_chMessageType; // ±ÿ–Î «'Q'
	CString m_strSymbol;
	CTime m_date;
	double m_dBidSize;
	double m_dBidPrice;
	double m_dMidPrice;
	double m_dAskPrice;
	double m_dAskSize;
};

typedef shared_ptr<CTiingoForexSocket> CTiingoForexSocketPtr;

extern CTiingoForexWebSocket gl_tiingoForexWebSocket;
