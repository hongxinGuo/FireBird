#include "pch.h"

#include"SystemMessage.h"

#include"JsonParse.h"
#include"nlohmannJsonGetValue.h"

#include "AlpacaIEXWebSocket.h"

#include "SystemConfiguration.h"
#include "AlpacaDataSource.h"

using std::istringstream;
using std::chrono::minutes;
using std::chrono::nanoseconds;
using std::chrono::seconds;
using std::make_shared;

void ProcessAlpacaIEXWebSocket(const ix::WebSocketMessagePtr& msg) {
	gl_pAlpacaIEXWebSocket->SetError(false);
	switch (msg->type) {
	case ix::WebSocketMessageType::Message:
		// 当系统退出时，停止接收WebSocket的过程需要时间，在此期间此回调函数继续执行，而存储器已经析构了，导致出现内存泄漏。
		// 故而需要判断是否系统正在退出（只有在没有退出系统时方可存储接收到的数据）。
		if (gl_systemConfiguration.IsExitingSystem()) break;
		gl_pAlpacaIEXWebSocket->PushData(msg->str);
		break;
	case ix::WebSocketMessageType::Error:
		gl_pAlpacaIEXWebSocket->SetError(true);
		gl_systemMessage.PushErrorMessage(msg->errorInfo.reason);
		break;
	case ix::WebSocketMessageType::Open:
		gl_systemMessage.PushWebSocketInfoMessage("Alpaca IEX WebSocket Open");
		break;
	case ix::WebSocketMessageType::Close:
		gl_systemMessage.PushWebSocketInfoMessage("Alpaca IEX WebSocket Close");
		break;
	case ix::WebSocketMessageType::Fragment:
		gl_systemMessage.PushWebSocketInfoMessage("Alpaca IEX WebSocket Fragment");
		break;
	case ix::WebSocketMessageType::Ping:
		gl_systemMessage.PushWebSocketInfoMessage("Alpaca IEX WebSocket Ping");
		break;
	case ix::WebSocketMessageType::Pong:
		gl_pAlpacaIEXWebSocket->SetHeartbeatTime(GetUTCTime());
		gl_systemMessage.PushWebSocketInfoMessage("Alpaca IEX WebSocket Pong");
		break;
	default: // error
		break;
	}
}

CAlpacaIEXWebSocket::CAlpacaIEXWebSocket() {
	ABSL_DCHECK(gl_systemConfiguration.IsInitialized());
	m_url = "wss://stream.data.alpaca.markets/v2/iex";
}

/// <summary>
/// Alpaca IEX的数据源格式：wss://stream.data.alpaca.markets/v2/iex
/// </summary>
/// 
/// <returns></returns>
void CAlpacaIEXWebSocket::Connect() {
	string sAuth = gl_systemConfiguration.GetAlpacaApiKey();
	string sSecretKey = gl_systemConfiguration.GetAlpacaSecretKey();

	ix::WebSocketHttpHeaders headers;
	headers["APCA-API-KEY-ID"] = sAuth;
	headers["APCA-API-SECRET-KEY"] = sSecretKey;

	m_webSocket.setExtraHeaders(headers); // 密匙在这里设置
	Connecting(m_url, ProcessAlpacaIEXWebSocket);
}

void CAlpacaIEXWebSocket::Send(const vector<string>& vSymbol) {
	ABSL_DCHECK(IsOpen());

	const string messageAuth(CreateMessage(vSymbol));
	m_webSocket.send(messageAuth);

	gl_systemMessage.PushInnerSystemInformationMessage(messageAuth);
}

void CAlpacaIEXWebSocket::MonitorWebSocket(const vector<string>& vSymbol) {
	if (IsConnecting()) return; // 如果正在连接，则不监控该socket
	CVirtualWebSocket::MonitorWebSocket(gl_pAlpacaDataSource->IsWebError(), gl_systemConfiguration.IsUsingAlpacaIEXWebSocket(), vSymbol);
}

///////////////////////////////////////////////////////////////////////
///
/// 格式：
/// {
///  "eventName" : "subscribe",
///  "authorization" : d897a00b7cc630d235316a4683156, // tiingo token
///  "eventData" :
///   {
///    "thresholdLevel" : 5,
///		 "tickers" : ["AAPL","MSFT", "RIG"]
///   }
/// }
///
///////////////////////////////////////////////////////////////////////
string CAlpacaIEXWebSocket::CreateMessage(const vector<string>& vSymbol) {
	string sMSG = R"({"action": "subscribe","quotes" : ["FSK","RIG"]})";

	string sTestMsg = R"({"action": "subscribe","quotes" : ["FAKEPACA"]})";
	return sMSG;
}

map<string, int> s_alpacaWebType{
	{ "success", 0 },
	{ "subscription", 1 },
	{ "q", 2 },
	{ "t", 3 },
	{ "b", 4 }
};
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// wss://stream.data.alpaca.markets/v2/iex"
//
// [{"T":"success","msg":"connected"}]
// [{"T":"success","msg":"authenticated"}]
// [{"T":"subscription","trades":["FSK","RIG"],"quotes":["FSK"],"bars":["FSK"]}]
// [{"T":"q","S":"FSK","bx":"O","bp":133.85,"bs":4,"ax":"R","ap":135.77,"as":5,"c":["R"],"z":"A","t":"2024-07-24T07:56:53.639713735Z"}]
// [{"T": "t","i" : 96921,"S" : "AAPL","x" : "D","p" : 126.55,"s" : 1,"t" : "2021-02-22T15:51:44.208Z","c" : ["@", "I"] ,"z" : "C"}]
// [{"T":"b","S":"FSK","o":132.65,"h":136,"l":132.12,"c":134.65,"v":205,"t":"2024-07-24T07:56:00Z","n":16,"vw":133.7}]
//
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool CAlpacaIEXWebSocket::ParseAlpacaIEXWebSocketData(shared_ptr<string> pData) {
	CAlpacaIEXSocketQuotePtr pIEXQuote = nullptr;
	CAlpacaIEXSocketTradePtr pIEXTrade = nullptr;
	CAlpacaIEXSocketBarPtr pIEXBar = nullptr;
	minutes Minutes;
	string sString;
	try {
		if (nlohmannJson js; CreateJsonWithNlohmann(js, *pData)) {
			time_point<system_clock, nanoseconds> tpTime;
			istringstream ss;
			string sMessageType;
			string sService;
			string sType;
			string sMsg;
			nlohmannJson js2, js3, js4;
			nlohmannJson::iterator it;

			for (const auto& item : js) {
				if (item.contains("T")) {
					sType = jsonGetString(item, "T", "");
					switch (s_alpacaWebType.at(sType)) {
					case 0: // "success"
						sMsg = jsonGetString(item, "msg", "");
						if (sMsg == "connected") {
							// do nothing
						}
						else if (sMsg == "authenticated") {
							// do nothing
						}
						break;
					case 1: // "subscription"
						if (item.contains("quotes")) {
							js2 = item["quotes"];
						}
						if (item.contains("trades")) {
							js3 = item["trades"];
						}
						if (item.contains("bars")) {
							js4 = item["bars"];
						}
						break;
					case 2: // quotes
						pIEXQuote = make_shared<CAlpacaIEXSocketQuote>();
						pIEXQuote->m_sSymbol = jsonGetString(item, "S", "");
						pIEXQuote->m_dBidPrice = jsonGetDouble(item, "bp", 0.0);
						pIEXQuote->m_dBidSize = jsonGetInt(item, "bs", 0);
						pIEXQuote->m_dAskPrice = jsonGetInt(item, "ap", 0);
						pIEXQuote->m_dAskSize = jsonGetDouble(item, "as", 0.0);
						sString = jsonGetString(item, "t", "");
						ss.str(sString);
						ss >> std::chrono::parse("%FT%T%Z", pIEXQuote->m_tpTime);
						break;
					case 3: // trades
						pIEXTrade = make_shared<CAlpacaIEXSocketTrade>();
						pIEXTrade->m_sSymbol = jsonGetString(item, "S", "");
						pIEXTrade->m_dPrice = jsonGetDouble(item, "bp", 0.0);
						pIEXTrade->m_dSize = jsonGetInt(item, "bs", 0);
						sString = jsonGetString(item, "t", "");
						ss.str(sString);
						ss >> std::chrono::parse("%FT%T%Z", pIEXTrade->m_tpTime);
						break;
					case 4: // bars
						pIEXBar = make_shared<CAlpacaIEXSocketBar>();
						pIEXBar->m_sSymbol = jsonGetString(item, "S", "");
						pIEXBar->m_dOpen = jsonGetDouble(item, "o", 0.0);
						pIEXBar->m_dHigh = jsonGetDouble(item, "h", 0.0);
						pIEXBar->m_dLow = jsonGetDouble(item, "l", 0.0);
						pIEXBar->m_dClose = jsonGetDouble(item, "c", 0.0);
						pIEXBar->m_iVolume = jsonGetInt(item, "v", 0);
						sString = jsonGetString(item, "t", "");
						ss.str(sString);
						ss >> std::chrono::parse("%FT%T%Z", pIEXBar->m_tpTime);
						break;
					default:
						break;
					}
				}
			}
		}
		else {
			// ERROR
			return false;
		}
	} catch (nlohmannJson::exception& e) {
		ReportJSonErrorToSystemMessage("Alpaca IEX WebSocket ", e.what());
		return false;
	}

	return true;
}
