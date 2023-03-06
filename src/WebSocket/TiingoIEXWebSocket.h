#pragma once

#include"VirtualWebSocket.h"
#include <ixwebsocket/IXWebSocket.h>

void ProcessTiingoIEXWebSocket(const ix::WebSocketMessagePtr& msg);

class CTiingoIEXSocket final {
public:
	CTiingoIEXSocket() {
		m_chMessageType = 'Q';
		m_sSymbol = _T("");
		m_iNanoseconds = 0;
		m_dBidSize = m_dBidPrice = m_dMidPrice = m_dAskPrice = m_dAskSize = m_dLastPrice = m_dLastSize = 0;
		m_iAfterHour = m_iHalted = m_iISO = m_iOddlot = m_iNMSRule611 = 0;
	}

public:
	char m_chMessageType; // 'Q', 'T', or 'B'
	INT64 m_iNanoseconds;
	string m_sSymbol;
	double m_dBidSize;
	double m_dBidPrice;
	double m_dMidPrice;
	double m_dAskPrice;
	double m_dAskSize;
	double m_dLastPrice;
	double m_dLastSize;
	int m_iHalted; // 1 for halted, 0 for not halted
	int m_iAfterHour; // 1 for after hour, 0 for market hours
	int m_iISO; // 1 for intermarket sweep order, 0 for non-ISO order
	int m_iOddlot; // 1 for trade is an odd lot, 0 for trade is round or mix lot. Only available for Trade updates, null otherwise
	int m_iNMSRule611; // 1 if the trade is not subject to NMS Rule 611 (trade through), 0 if the trade is subject to Rule NMS 611. Only available for Trade updates, null otherwise.
};

using CTiingoIEXSocketPtr = shared_ptr<CTiingoIEXSocket>;

class CTiingoIEXWebSocket final : public CVirtualWebSocket {
public:
	CTiingoIEXWebSocket();
	~CTiingoIEXWebSocket(void) override = default;

	bool Connect(void) override;
	bool Send(vectorString vSymbol) override;

	bool CreateThreadConnectWebSocketAndSendMessage(vectorString vSymbol);

	string CreateMessage(vectorString vSymbol);
	bool ParseTiingoIEXWebSocketData(shared_ptr<string> pData);
};

using CTiingoIEXWebSocketPtr = shared_ptr<CTiingoIEXWebSocket>;

extern CTiingoIEXWebSocket gl_tiingoIEXWebSocket;
