//////////////////////////////////////////////////////////////////////////////////////////////
//
// 接收所有的Crypto数据时，每秒数据量大致在50-100K附近。
//
// thresholdLevel 2: Top-of-Book AND Last Trade updates.
// thresholdLevel 5: only Last Trade updates.
//
//////////////////////////////////////////////////////////////////////////////////////////////
#pragma once

#include"VirtualWebSocket.h"

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
	char m_chMessageType; // "Q" or "T"
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
	CTiingoCryptoWebSocket(const CTiingoCryptoWebSocket& other) = delete;
	CTiingoCryptoWebSocket(CTiingoCryptoWebSocket&& other) noexcept = delete;
	CTiingoCryptoWebSocket& operator=(const CTiingoCryptoWebSocket& other) = delete;
	CTiingoCryptoWebSocket& operator=(CTiingoCryptoWebSocket&& other) noexcept = delete;
	~CTiingoCryptoWebSocket() override = default;

	void Connect() override;
	void Send(const vectorString& vSymbol) override;

	void MonitorWebSocket(const vectorString& vSymbol);

	static string CreateMessage(const vectorString& vSymbol);
	bool ParseTiingoCryptoWebSocketData(shared_ptr<string> pData);
};

using CTiingoCryptoWebSocketPtr = shared_ptr<CTiingoCryptoWebSocket>;

extern CTiingoCryptoWebSocketPtr gl_pTiingoCryptoWebSocket;
