//////////////////////////////////////////////////////////////////////////////////////////////////
//
// 5状态下每秒接收100K左右。
//
// thresholdLevel 5: ALL Top-of-Book updates
// thresholdLevel 7: A top-of-book update that is due to a change in either the bid/ask price or size.
//
//////////////////////////////////////////////////////////////////////////////////////////////////
module;

export module TiingoForexWebSocket;


import VirtualWebSocket;

import std;
using std::string;
using std::vector;
using std::shared_ptr;

export {
	void ProcessTiingoForexWebSocket(const ix::WebSocketMessagePtr& msg);

	class CTiingoForexSocket {
	public:
		CTiingoForexSocket() {
			m_sDateTime = "";
			m_chMessageType = 'Q';
			m_sSymbol = "";
			m_dBidSize = m_dBidPrice = m_dMidPrice = m_dAskPrice = m_dAskSize = 0;
		}

	public:
		char m_chMessageType; // 必须是'Q'
		string m_sDateTime; // json ISO format
		std::chrono::sys_seconds m_tpTime;
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
		~CTiingoForexWebSocket() override = default;

		bool ParseWebSocketData(shared_ptr<string> pData) override { return ParseTiingoForexWebSocketData(pData); }

		void MonitorWebSocket(const vector<string>& vSymbol);

		static string CreateMessage(const vector<string>& vSymbol);
		bool ParseTiingoForexWebSocketData(const shared_ptr<string>& pData);

	protected:
		void Connect() override;
		void Send(const vectorString& vSymbol) override;
	};

	using CTiingoForexWebSocketPtr = shared_ptr<CTiingoForexWebSocket>;

	extern CTiingoForexWebSocketPtr gl_pTiingoForexWebSocket;
}