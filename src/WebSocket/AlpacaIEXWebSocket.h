////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Alpaca对免费账户的流量限制，为500次/小时， 20000次/天， 5GB/月。
//
// thresholdLevel 0接收所有的IEX数据时，每秒数据量为1M-9M;thresholdLevel5接收所有IEX数据时，每秒数据量为10-50K。
//
// thresholdLevel 5：all Last Trade updates and only Quote updates that are deemed major updates by our system.
// thresholdLevel 0: ALL Top-of-Book AND Last Trade updates.
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#pragma once

#include"VirtualWebSocket.h"

void ProcessAlpacaIEXWebSocket(const ix::WebSocketMessagePtr& msg);

class CAlpacaIEXSocket final {
public:
	CAlpacaIEXSocket() {
		m_sSymbol = "";
		m_dBidSize = m_dBidPrice = m_dMidPrice = m_dAskPrice = m_dAskSize = m_dLastPrice = m_dLastSize = 0;
		m_iAfterHour = m_iHalted = m_iISO = m_iOddlot = m_iNMSRule611 = 0;
	}

public:
	std::chrono::time_point<std::chrono::system_clock, std::chrono::nanoseconds> m_tpTime;
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

using CAlpacaIEXSocketPtr = shared_ptr<CAlpacaIEXSocket>;

class CAlpacaIEXWebSocket : public CVirtualWebSocket {
public:
	CAlpacaIEXWebSocket();
	CAlpacaIEXWebSocket(const CAlpacaIEXWebSocket& other) = delete;
	CAlpacaIEXWebSocket(CAlpacaIEXWebSocket&& other) noexcept = delete;
	CAlpacaIEXWebSocket& operator=(const CAlpacaIEXWebSocket& other) = delete;
	CAlpacaIEXWebSocket& operator=(CAlpacaIEXWebSocket&& other) noexcept = delete;
	~CAlpacaIEXWebSocket() override = default;

	bool ParseWebSocketData(shared_ptr<string> pData) override { return ParseAlpacaIEXWebSocketData(pData); }

	void MonitorWebSocket(const vector<string>& vSymbol);

	static string CreateMessage(const vector<string>& vSymbol);
	bool ParseAlpacaIEXWebSocketData(shared_ptr<string> pData);

protected:
	void Connect() override;
	void Send(const vector<string>& vSymbol) override;
};

using CAlpacaIEXWebSocketPtr = shared_ptr<CAlpacaIEXWebSocket>;

inline CAlpacaIEXWebSocketPtr gl_pAlpacaIEXWebSocket;
