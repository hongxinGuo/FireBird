#pragma once

#include"VirtualWebSocket.h"

#include<ixwebsocket/IXWebSocket.h>

void ProcessFinnhubWebSocket(const ix::WebSocketMessagePtr& msg);

class CFinnhubSocket final {
public:
	CFinnhubSocket() {
		m_sSymbol = ("");
		m_dLastPrice = m_dLastVolume = 0;
		m_iSeconds = 0;
	}

public:
	string m_sSymbol;
	double m_dLastPrice;
	INT64 m_iSeconds; // UNIX milliseconds timestamp
	double m_dLastVolume;
	vectorString m_vCode; // trade conditions
};

using CFinnhubSocketPtr = shared_ptr<CFinnhubSocket>;

class CFinnhubWebSocket : public CVirtualWebSocket {
public:
	CFinnhubWebSocket();
	CFinnhubWebSocket(const CFinnhubWebSocket&) = delete;
	CFinnhubWebSocket& operator=(const CFinnhubWebSocket&) = delete;
	CFinnhubWebSocket(const CFinnhubWebSocket&&) noexcept = delete;
	CFinnhubWebSocket& operator=(const CFinnhubWebSocket&&) noexcept = delete;
	~CFinnhubWebSocket() override = default;

	void Connect() override;
	void Send(const vectorString& vSymbol) override;
	static string CreateFinnhubWebSocketString(string sSymbol);

	void MonitorWebSocket(const vectorString& vSymbol);

	bool ParseFinnhubWebSocketData(shared_ptr<string> pData);
	bool ParseFinnhubWebSocketDataWithSidmjson(const shared_ptr<string>& pData);
};

using CFinnhubWebSocketPtr = shared_ptr<CFinnhubWebSocket>;

extern CFinnhubWebSocketPtr gl_pFinnhubWebSocket;
