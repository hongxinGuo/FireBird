module;
#include <ixwebsocket/IXWebSocket.h>

export module FireBirdLib.WebSocket.Finnhub;
import FireBirdLib.WebSocket.Virtual;

import std;
using std::string;
using std::shared_ptr;
using std::vector;
using std::int64_t;

export {
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
		int64_t m_iSeconds; // UNIX milliseconds timestamp
		double m_dLastVolume;
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

		bool ParseWebSocketData(shared_ptr<string> pData) override { return ParseFinnhubWebSocketData(pData); }

		static string CreateFinnhubWebSocketString(string sSymbol);

		void MonitorWebSocket(const vector<string>& vSymbol);

		bool ParseFinnhubWebSocketData(shared_ptr<string> pData);
		bool ParseFinnhubWebSocketDataWithSidmjson(const shared_ptr<string>& pData);

	protected:
		void Connect() override;
		void Send(const vector<string>& vSymbol) override;
	};

	using CFinnhubWebSocketPtr = shared_ptr<CFinnhubWebSocket>;

	extern CFinnhubWebSocketPtr gl_pFinnhubWebSocket;
}