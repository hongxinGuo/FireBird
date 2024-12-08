////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Tiingo对免费账户的流量限制，为500次/小时， 20000次/天， 5GB/月。
//
// thresholdLevel 0接收所有的IEX数据时，每秒数据量为1M-9M;thresholdLevel5接收所有IEX数据时，每秒数据量为10-50K。
//
// thresholdLevel 5：all Last Trade updates and only Quote updates that are deemed major updates by our system.
// thresholdLevel 0: ALL Top-of-Book AND Last Trade updates.
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include"pch.h"
module;
#include <ixwebsocket/IXWebSocket.h>
export module FireBird.WebSocket.TiingoIEX;

export import FireBird.WebSocket.Virtual;

export {
void ProcessTiingoIEXWebSocket(const ix::WebSocketMessagePtr& msg);

	class CTiingoIEXSocket final {
	public:
		CTiingoIEXSocket() {
			m_chMessageType = 'Q';
			m_sDateTime = _T("");
			m_sSymbol = _T("");
			m_iNanoseconds = 0;
			m_dBidSize = m_dBidPrice = m_dMidPrice = m_dAskPrice = m_dAskSize = m_dLastPrice = m_dLastSize = 0;
			m_iAfterHour = m_iHalted = m_iISO = m_iOddlot = m_iNMSRule611 = 0;
		}

	public:
		char m_chMessageType; // 'Q', 'T', or 'B'
		string m_sDateTime; // json ISO format
		std::chrono::sys_seconds m_tpTime;
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

	class CTiingoIEXWebSocket : public CVirtualWebSocket {
	public:
		CTiingoIEXWebSocket();
		CTiingoIEXWebSocket(const CTiingoIEXWebSocket& other) = delete;
		CTiingoIEXWebSocket(CTiingoIEXWebSocket&& other) noexcept = delete;
		CTiingoIEXWebSocket& operator=(const CTiingoIEXWebSocket& other) = delete;
		CTiingoIEXWebSocket& operator=(CTiingoIEXWebSocket&& other) noexcept = delete;
		~CTiingoIEXWebSocket() override = default;

		void Connect() override;
		void Send(const vectorString& vSymbol) override;

		void MonitorWebSocket(const vectorString& vSymbol);

		static string CreateMessage(const vectorString& vSymbol);
		bool ParseTiingoIEXWebSocketData(shared_ptr<string> pData);
	};

	using CTiingoIEXWebSocketPtr = shared_ptr<CTiingoIEXWebSocket>;
	extern CTiingoIEXWebSocketPtr gl_pTiingoIEXWebSocket;
}
