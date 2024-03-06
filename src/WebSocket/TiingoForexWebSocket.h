//////////////////////////////////////////////////////////////////////////////////////////////////
//
// 5状态下每秒接收100K左右。
//
// thresholdLevel 5: ALL Top-of-Book updates
// thresholdLevel 7: A top-of-book update that is due to a change in either the bid/ask price or size.
//
//////////////////////////////////////////////////////////////////////////////////////////////////
#pragma once

#include"VirtualWebSocket.h"
#include <ixwebsocket/IXWebSocket.h>

void ProcessTiingoForexWebSocket(const ix::WebSocketMessagePtr& msg);

class CTiingoForexSocket {
public:
	CTiingoForexSocket() {
		m_sDateTime = _T("");
		m_tTime = 0;
		m_chMessageType = 'Q';
		m_sSymbol = _T("");
		m_dBidSize = m_dBidPrice = m_dMidPrice = m_dAskPrice = m_dAskSize = 0;
	}

public:
	char m_chMessageType; // 必须是'Q'
	string m_sDateTime; // json ISO format
	time_t m_tTime;
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
	CTiingoForexWebSocket(const CTiingoForexWebSocket& other) = delete;
	CTiingoForexWebSocket(CTiingoForexWebSocket&& other) noexcept = delete;
	CTiingoForexWebSocket& operator=(const CTiingoForexWebSocket& other) = delete;
	CTiingoForexWebSocket& operator=(CTiingoForexWebSocket&& other) noexcept = delete;
	~CTiingoForexWebSocket() override;

	void Connect() override;
	void Send(const vectorString& vSymbol) override;

	void MonitorWebSocket(const vectorString& vSymbol);

	static string CreateMessage(const vectorString& vSymbol);
	bool ParseTiingoForexWebSocketData(shared_ptr<string> pData);
};

using CTiingoForexWebSocketPtr = shared_ptr<CTiingoForexWebSocket>;

extern CTiingoForexWebSocketPtr gl_pTiingoForexWebSocket;
