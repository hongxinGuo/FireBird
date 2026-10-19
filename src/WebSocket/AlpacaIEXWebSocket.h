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

class CAlpacaIEXSocketQuote final {
public:
	CAlpacaIEXSocketQuote() {
		m_sSymbol = "";
		m_dBidSize = m_dBidPrice = m_dAskPrice = m_dAskSize = 0;
	}

	std::chrono::time_point<std::chrono::system_clock, std::chrono::nanoseconds> m_tpTime;
	string m_sSymbol;
	int m_dBidSize;
	double m_dBidPrice;
	int m_dAskPrice;
	double m_dAskSize;
};

using CAlpacaIEXSocketQuotePtr = shared_ptr<CAlpacaIEXSocketQuote>;

class CAlpacaIEXSocketTrade final {
public:
	CAlpacaIEXSocketTrade() {
		m_sSymbol = "";
		m_dSize = m_dPrice = 0;
	}

	std::chrono::time_point<std::chrono::system_clock, std::chrono::nanoseconds> m_tpTime;
	string m_sSymbol;
	int m_dSize;
	double m_dPrice;
};

using CAlpacaIEXSocketTradePtr = shared_ptr<CAlpacaIEXSocketTrade>;

class CAlpacaIEXSocketBar final {
public:
	CAlpacaIEXSocketBar() {
		m_sSymbol = "";
		m_dOpen = m_dHigh = m_dLow = m_dClose = 0;
		m_iVolume = 0;
	}

	std::chrono::time_point<std::chrono::system_clock, std::chrono::nanoseconds> m_tpTime;
	string m_sSymbol;
	double m_dOpen;
	double m_dHigh;
	double m_dLow;
	double m_dClose;
	int m_iVolume;
};

using CAlpacaIEXSocketBarPtr = shared_ptr<CAlpacaIEXSocketBar>;

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
