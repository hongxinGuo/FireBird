#pragma once

#include"VirtualWebSocket.h"

void FunctionProcessFinnhubWebSocket(const ix::WebSocketMessagePtr& msg);

class CFinnhubWebSocket : public CVirtualWebSocket {
public:
	CFinnhubWebSocket();
	~CFinnhubWebSocket(void);

	virtual bool Connect(void);
	virtual bool Send(vector<CString> vSymbol) override;
	string CreateFinnhubWebSocketString(CString strSymbol);

	bool CreatingThreadConnectingWebSocketAndSendMessage(vector<CString> vSymbol);

	bool ParseFinnhubWebSocketData(shared_ptr<string> pData);
};
typedef shared_ptr<CFinnhubWebSocket> CFinnhubWebSocketPtr;

class CFinnhubSocket : public CObject {
public:
	CFinnhubSocket() {
		m_strSymbol = _T("");
		m_dLastPrice = m_dLastVolume = 0;
		m_iSeconds = 0;
		m_strCode = _T("");
	}
public:
	CString m_strSymbol;
	double m_dLastPrice;
	INT64 m_iSeconds; // UNIX milliseconds timestamp
	double m_dLastVolume;
	CString m_strCode; // trade conditions
};

typedef shared_ptr<CFinnhubSocket> CFinnhubSocketPtr;