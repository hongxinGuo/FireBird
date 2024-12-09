#include "pch.h"
module;
#include"globedef.h"
#include"JsonParse.h"
#include "TiingoDataSource.h"
#include <ixwebsocket/IXWebSocket.h>
module FireBird.WebSocket.TiingoCrypto;

import FireBird.Accessory.JsonGetValue;
import FireBird.System.Message;

void ProcessTiingoCryptoWebSocket(const ix::WebSocketMessagePtr& msg) {
	gl_pTiingoCryptoWebSocket->SetError(false);
	switch (msg->type) {
	case ix::WebSocketMessageType::Message:
		// 当系统退出时，停止接收WebSocket的过程需要时间，在此期间此回调函数继续执行，而存储器已经析构了，导致出现内存泄漏。
		// 故而需要判断是否系统正在退出（只有在没有退出系统时方可存储接收到的数据）。
		if (!gl_systemConfiguration.IsExitingSystem()) {
			gl_pTiingoCryptoWebSocket->PushData(msg->str);
		}
		break;
	case ix::WebSocketMessageType::Error:
		gl_pTiingoCryptoWebSocket->SetError(true);
		gl_systemMessage.PushErrorMessage(msg->errorInfo.reason.c_str());
		break;
	case ix::WebSocketMessageType::Open:
		gl_systemMessage.PushWebSocketInfoMessage(_T("Tiingo Crypto WebSocket Open"));
		break;
	case ix::WebSocketMessageType::Close:
		gl_systemMessage.PushWebSocketInfoMessage(_T("Tiingo Crypto WebSocket Close"));
		break;
	case ix::WebSocketMessageType::Fragment:
		gl_systemMessage.PushWebSocketInfoMessage(_T("Tiingo Crypto WebSocket Fragment"));
		break;
	case ix::WebSocketMessageType::Ping:
		gl_systemMessage.PushWebSocketInfoMessage(_T("Tiingo Crypto WebSocket Ping"));
		break;
	case ix::WebSocketMessageType::Pong:
		gl_systemMessage.PushWebSocketInfoMessage(_T("Tiingo Crypto WebSocket Pong"));
		break;
	default: // error
		break;
	}
}

CTiingoCryptoWebSocket::CTiingoCryptoWebSocket() {
	ASSERT(gl_systemConfiguration.IsInitialized());
	m_url = _T("wss://api.tiingo.com/crypto");
}

/// <summary>
/// Tiingo Crypto的数据源格式：wss://api.tiingo.com/crypto，其密钥是随后发送的。
/// </summary>
/// 
void CTiingoCryptoWebSocket::Connect() {
	Connecting(m_url, ProcessTiingoCryptoWebSocket);
}

//////////////////////////////////////////////////////////////////////////////////////////////
//
// 接收所有的Crypto数据时，每秒数据量大致在50-100K附近。
//
// thresholdLevel 2: Top-of-Book AND Last Trade updates.
// thresholdLevel 5: only Last Trade updates.
//
//////////////////////////////////////////////////////////////////////////////////////////////
void CTiingoCryptoWebSocket::Send(const vectorString& vSymbol) {
	ASSERT(IsOpen());

	const string messageAuth(CreateMessage(vSymbol));

	ASSERT(IsOpen());

	ix::WebSocketSendInfo info = m_webSocket.send(messageAuth);
	gl_systemMessage.PushInnerSystemInformationMessage(messageAuth.c_str());
}

void CTiingoCryptoWebSocket::MonitorWebSocket(const vectorString& vSymbol) {
	if (IsConnecting()) return; // 如果正在连接，则不监控该socket
	CVirtualWebSocket::MonitorWebSocket(gl_pTiingoDataSource->IsWebError(), gl_systemConfiguration.IsUsingTiingoCryptoWebSocket(), vSymbol);
}

///////////////////////////////////////////////////////////////////////
///
/// 格式：
/// {
///  "eventName" : "subscribe",
///  "authorization" : d897a00b7cfc2630d235316a4683156, // tiingo token
///  "eventData" :
///   {
///    "thresholdLevel" : 5,
///		 "tickers" : ["dkaeth","ksmust"]
///   }
/// }
///
///////////////////////////////////////////////////////////////////////
string CTiingoCryptoWebSocket::CreateMessage(const vectorString& vSymbol) {
	vectorString vSymbol2;
	nlohmann::ordered_json message;
	message["eventName"] = _T("subscribe");
	message["authorization"] = gl_pTiingoDataSource->GetInquiryToken();
	message["eventData"]["thresholdLevel"] = 2; // thresholdLevel的有效数字为2或者5
	for (auto str : vSymbol) {
		std::ranges::transform(str, str.begin(), ::tolower); // Tiingo webSocket使用小写字符
		vSymbol2.push_back(str);
	}
	message["eventData"]["tickers"] = vSymbol2;
	message["eventData"]["tickers"].emplace_back("dkaeth"); // tiingo使用的crypto符号与finnhub完全不同，
	message["eventData"]["tickers"].emplace_back("eurusd");// tiingo使用的crypto符号与finnhub完全不同，
	message["eventData"]["tickers"].emplace_back("ksmust");// tiingo使用的crypto符号与finnhub完全不同，

	return message.dump();
}

// <summary>
//
// https://api.tiingo.com/documentation/websockets/crypto
//
// 共五种格式：
// {"messageType":"I","response":{"code":200,"message":"Success"},"data":{"subscriptionId":2563396}}
// {"data":{"tickers":["*","uso","msft","tnk"],"thresholdLevel":"0"},"messageType":"I","response":{"code":200,"message":"Success"}}
// {"messageType":"H","response":{"code":200,"message":"HeartBeat"}}
// {"messageType":"A","service":"crypto_data","data":["Q","neojpy","2019-01-30T18:03:40.195515+00:00","bitfinex",38.11162867,787.82,787.83,42.4153887,787.84]}
// {"messageType":"A","service":"crypto_data","data":["T","evxbtc","2019-01-30T18:03:40.056000+00:00","binance",405.0,9.631e-05]}
// {"messageType":"E","response":{"code":400,"message":"thresholdLevel not valid}}
//
// </summary>
// <param name="pData"></param>
// <returns></returns>
bool CTiingoCryptoWebSocket::ParseTiingoCryptoWebSocketData(shared_ptr<string> pData) {
	string sSymbol;
	string strSymbol;
	string sTickers;

	try {
		if (nlohmann::ordered_json js; CreateJsonWithNlohmann(js, *pData)) {
			CTiingoCryptoSocketPtr pCryptoData;
			string sService;
			string sMessageType;
			char chType;
			string sType;
			stringstream ss;
			chrono::time_point<chrono::system_clock, chrono::microseconds> tpTime;
			chrono::minutes Minutes;
			string sString;
			nlohmann::ordered_json js2, js3, js4;
			nlohmann::ordered_json::iterator it;
			sType = jsonGetString(js, _T("messageType"));
			chType = sType.at(0);
			switch (chType) {
			case 'I':  // 共两种。一种是报告当前查询证券代码，另一种是报告注册信息
				js2 = jsonGetChild(js, _T("data"));
				try { //{"data":{"tickers":["*","uso","msft","tnk"],"thresholdLevel":"0"},"messageType":"I","response":{"code":200,"message":"Success"}}
					js3 = js2.at(_T("tickers"));
					for (auto it2 = js3.begin(); it2 != js3.end(); ++it2) { // 是代码："data":{"tickers":["*","uso","msft","tnk"]
						strSymbol = jsonGetString(it2);
						m_vCurrentInquireSymbol.emplace_back(strSymbol.c_str());
					}
				} catch (nlohmann::ordered_json::exception&) { //注册信息：{"messageType":"I","response":{"code":200,"message":"Success"},"data":{"subscriptionId":2563396}}
					ASSERT(GetSubscriptionId() == 0);
					SetSubscriptionId(jsonGetInt(&js2, _T("subscriptionId")));
				}
				break;
			case 'H': // heart beat {\"messageType\":\"H\",\"response\":{\"code\":200,\"message\":\"HeartBeat\"}}
				js3 = jsonGetChild(js, _T("response"));
				m_iStatusCode = js3.at(_T("code"));
				m_statusMessage = js3.at(_T("message"));
				break;
			case 'A': // new data
				pCryptoData = make_shared<CTiingoCryptoSocket>();
				sService = jsonGetString(js, _T("service"));
				if (sService != _T("crypto_data")) return false; // 格式不符则退出
				js2 = jsonGetChild(js, _T("data"));
				it = js2.begin();
				sMessageType = jsonGetString(it); // ‘Q’或者‘T’
				if (sMessageType.at(0) == 'T') {	//last trade message {\"service\":\"crypto_data\",\"data\":[\"T\",\"jstusdt\",\"2021-08-10T23:56:55.237000+00:00\",\"huobi\",3952.5,0.062108],\"messageType\":\"A\"}
					pCryptoData->m_chMessageType = 'T';
					pCryptoData->m_sSymbol = jsonGetString(++it); // 证券名称
					pCryptoData->m_sDateTime = jsonGetString(++it); // 时间串："2019-07-05T15:49:15.157000+00:00"
					ss.str(pCryptoData->m_sDateTime);
					chrono::from_stream(ss, "%FT%H:%M:%9S%Ez", tpTime, &sString, &Minutes);
					pCryptoData->m_tpTime = chrono::time_point_cast<chrono::seconds>(tpTime);
					pCryptoData->m_strExchange = jsonGetString(++it); // 交易所
					pCryptoData->m_dLastSize = jsonGetDouble(++it); // 最新数量
					pCryptoData->m_dLastPrice = jsonGetDouble(++it); // 最新价格
				}
				else if (sMessageType.at(0) == 'Q') {		// 'Q' top-of-book update message.
					pCryptoData->m_chMessageType = 'Q';
					pCryptoData->m_sSymbol = jsonGetString(++it); // 证券名称
					pCryptoData->m_sDateTime = jsonGetString(++it); // 时间串："2019-07-05T15:49:15.157000+00:00"
					ss.str(pCryptoData->m_sDateTime);
					chrono::from_stream(ss, "%FT%H:%M:%9S%Ez", tpTime, &sString, &Minutes);
					pCryptoData->m_tpTime = chrono::time_point_cast<chrono::seconds>(tpTime);
					pCryptoData->m_strExchange = jsonGetString(++it);// 交易所
					pCryptoData->m_dBidSize = jsonGetDouble(++it); // 买价数量
					pCryptoData->m_dBidPrice = jsonGetDouble(++it); // 买价
					pCryptoData->m_dMidPrice = jsonGetDouble(++it); // 中间价 （BidPrice + AskPrice)/2
					pCryptoData->m_dAskSize = jsonGetDouble(++it); // 卖价数量
					pCryptoData->m_dAskPrice = jsonGetDouble(++it); // 卖价
				}
				else {// 格式不对
					return false;
				}
				gl_SystemData.PushTiingoCryptoSocket(pCryptoData);
				m_HeartbeatTime = GetUTCTime();
				break;
			case 'E':  //error message {"messageType":"E","response":{"code":400,"message":"thresholdLevel not valid}}
				js4 = jsonGetChild(js, _T("response"));
				m_iStatusCode = js4.at(_T("code"));
				m_statusMessage = js4.at(_T("message"));
				break;
			default: // 错误
				return false;
			}
		}
		else { return false; }
	} catch (nlohmann::ordered_json::exception& e) {
		ReportJSonErrorToSystemMessage(_T("Tiingo Crypto WebSocket "), e.what());
		return false;
	}

	return true;
}
