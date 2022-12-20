#pragma once

#include"VirtualWebSocket.h"

#include<ixwebsocket/IXWebSocket.h>

void ProcessFinnhubWebSocket(const ix::WebSocketMessagePtr& msg);

class CFinnhubSocket final : public CObject {
public:
	CFinnhubSocket() {
		m_strSymbol = _T("");
		m_dLastPrice = m_dLastVolume = 0;
		m_iSeconds = 0;
	}

public:
	CString m_strSymbol;
	double m_dLastPrice;
	INT64 m_iSeconds; // UNIX milliseconds timestamp
	double m_dLastVolume;
	vector<string> m_vCode; // trade conditions
};

using CFinnhubSocketPtr = shared_ptr<CFinnhubSocket>;

class CFinnhubWebSocket final : public CVirtualWebSocket {
public:
	CFinnhubWebSocket();
	CFinnhubWebSocket(const CFinnhubWebSocket&) = delete;
	CFinnhubWebSocket& operator=(const CFinnhubWebSocket&) = delete;
	CFinnhubWebSocket(const CFinnhubWebSocket&&) noexcept = delete;
	CFinnhubWebSocket& operator=(const CFinnhubWebSocket&&) noexcept = delete;
	~CFinnhubWebSocket(void) override = default;

	bool Connect(void) override;
	bool Send(vector<CString> vSymbol) override;
	string CreateFinnhubWebSocketString(CString strSymbol);

	bool CreateThreadConnectWebSocketAndSendMessage(vector<CString> vSymbol);

	bool ParseFinnhubWebSocketData(shared_ptr<string> pData);
};

using CFinnhubWebSocketPtr = shared_ptr<CFinnhubWebSocket>;

extern CFinnhubWebSocket gl_finnhubWebSocket;
