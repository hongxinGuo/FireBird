#include "pch.h"

#include"JsonParse.h"
#include"JsonGetValue.h"
#include"TimeConvert.h"

#include "TiingoForexWebSocket.h"

#include "TiingoDataSource.h"

void ProcessTiingoForexWebSocket(const ix::WebSocketMessagePtr& msg) {
	gl_pTiingoForexWebSocket->SetError(false);
	switch (msg->type) {
	case ix::WebSocketMessageType::Message:
		// 当系统退出时，停止接收WebSocket的过程需要时间，在此期间此回调函数继续执行，而存储器已经析构了，导致出现内存泄漏。
		// 故而需要判断是否系统正在退出（只有在没有退出系统时方可存储接收到的数据）。
		if (!gl_systemConfiguration.IsExitingSystem()) { gl_pTiingoForexWebSocket->PushData(msg->str); }
		break;
	case ix::WebSocketMessageType::Error:
		gl_pTiingoForexWebSocket->SetError(true);
		gl_systemMessage.PushErrorMessage(msg->errorInfo.reason.c_str());
		break;
	case ix::WebSocketMessageType::Open:
		gl_systemMessage.PushWebSocketInfoMessage(_T("Tiingo Forex WebSocket Open"));
		break;
	case ix::WebSocketMessageType::Close:
		gl_systemMessage.PushWebSocketInfoMessage(_T("Tiingo Forex WebSocket Close"));
		break;
	case ix::WebSocketMessageType::Fragment:
		gl_systemMessage.PushWebSocketInfoMessage(_T("Tiingo Forex WebSocket Fragment"));
		break;
	case ix::WebSocketMessageType::Ping:
		gl_systemMessage.PushWebSocketInfoMessage(_T("Tiingo Forex WebSocket Ping"));
		break;
	case ix::WebSocketMessageType::Pong:
		gl_systemMessage.PushWebSocketInfoMessage(_T("Tiingo Forex WebSocket Pong"));
		break;
	default: // error
		break;
	}
}

CTiingoForexWebSocket::CTiingoForexWebSocket() {
	ASSERT(gl_systemConfiguration.IsInitialized());
	m_url = _T("wss://api.tiingo.com/fx");
}

CTiingoForexWebSocket::~CTiingoForexWebSocket() {
}

/// <summary>
/// Tiingo Forex的数据源格式：wss://api.tiingo.com/fx，其密钥是随后发送的。
/// </summary>
/// <returns></returns>
void CTiingoForexWebSocket::Connect() {
	Connecting(m_url, ProcessTiingoForexWebSocket);
}

//////////////////////////////////////////////////////////////////////////////////////////////////
//
// 5状态下每秒接收100K左右。
//
// 有效的thresholdLevel为：2， 5， 7
// thresholdLevel 5: ALL Top-of-Book updates
// thresholdLevel 7: A top-of-book update that is due to a change in either the bid/ask price or size.
// {"messageType":"E","response":{"code":400,"message":"thresholdLevel not valid}}
//
//////////////////////////////////////////////////////////////////////////////////////////////////
void CTiingoForexWebSocket::Send(const vectorString& vSymbol) {
	ASSERT(IsOpen());

	const string messageAuth(CreateMessage(vSymbol));
	ix::WebSocketSendInfo info = m_webSocket.send(messageAuth);
	gl_systemMessage.PushInnerSystemInformationMessage(messageAuth.c_str());
}

void CTiingoForexWebSocket::MonitorWebSocket(const vectorString& vSymbol) {
	if (IsConnecting()) return; // 如果正在连接，则不监控该socket
	CVirtualWebSocket::MonitorWebSocket(gl_pTiingoDataSource->IsWebError(), gl_systemConfiguration.IsUsingTiingoForexWebSocket(), vSymbol);
}

///////////////////////////////////////////////////////////////////////
///
/// 格式：
/// {
///  "eventName" : "subscribe",
///  "authorization" : d897a00b7c630d2d5316a4683156, // tiingo token
///  "eventData" :
///   {
///    "thresholdLevel" : 5,
///		 "tickers" : ["gbpaud","eurusd"]
///   }
/// }
///
///////////////////////////////////////////////////////////////////////
string CTiingoForexWebSocket::CreateMessage(const vectorString& vSymbol) {
	vectorString vSymbol2;
	json jsonMessage;
	jsonMessage["eventName"] = _T("subscribe");
	jsonMessage["authorization"] = gl_pTiingoDataSource->GetInquiryToken();
	jsonMessage["eventData"]["thresholdLevel"] = 5; // //7：A top - of - book update that is due to a change in either the bid / ask price or size.
	for (auto str : vSymbol) {
		std::ranges::transform(str, str.begin(), ::tolower); // Tiingo webSocket使用小写字符
		vSymbol2.push_back(str);
	}
	jsonMessage["eventData"]["tickers"] = vSymbol2;
	jsonMessage["eventData"]["tickers"].emplace_back(_T("gbpaud"));// 多加一个Tiingo制式的代码。由于目前自选crypto使用的是finnhub制式的代码格式，皆为无效代码。
	jsonMessage["eventData"]["tickers"].emplace_back(_T("eurusd"));// 多加一个Tiingo制式的代码。由于目前自选crypto使用的是finnhub制式的代码格式，皆为无效代码。

	return jsonMessage.dump();
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
//
// https://api.tiingo.com/documentation/websockets/forex
//
// 共五种格式：
// {"messageType":"I","response":{"code":200,"message":"Success"},"data":{"subscriptionId":2563396}}
// {"data":{"tickers":["*","uso","msft","tnk"],"thresholdLevel":"0"},"messageType":"I","response":{"code":200,"message":"Success"}}
// {"messageType":"H","response":{"code":200,"message":"HeartBeat"}}
// {"messageType":"A","service":"fx","data":["Q","eurnok","2019-07-05T15:49:15.157000+00:00",5000000.0,9.6764,9.678135,5000000.0,9.67987]}
// {"messageType":"A","service":"fx","data":["Q","gbpaud","2019-07-05T15:49:15.236000+00:00",1000000.0,1.79457,1.79477,5000000.0,1.79497]}
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool CTiingoForexWebSocket::ParseTiingoForexWebSocketData(shared_ptr<string> pData) {
	string sSymbol;
	string strSymbol;

	string sTickers;
	CTiingoForexSocketPtr pForexData = nullptr;

	try {
		if (json js; CreateJsonWithNlohmann(js, *pData)) {
			string sDatetime;
			string sMessageType;
			char chType;
			string sService;
			string sType;
			json::iterator it;
			json js2, js3, js4;
			sType = jsonGetString(js, _T("messageType"));
			chType = sType.at(0);
			switch (chType) {
			case 'I': // 共两种。一种是报告当前查询证券代码，另一种是报告注册信息
				js2 = jsonGetChild(js, _T("data"));
				try { // {"data":{"tickers": ["*", "uso", "msft", "tnk"] , "thresholdLevel" : "0"}, "messageType" : "I", "response" : {"code":200, "message" : "Success"}}
					js3 = js2.at(_T("tickers"));
					for (auto it3 = js3.begin(); it3 != js3.end(); ++it3) {
						strSymbol = jsonGetString(it3);
						m_vCurrentInquireSymbol.push_back(strSymbol.c_str());
					}
				} catch (json::exception&) { // {\"messageType\":\"I\",\"response\":{\"code\":200,\"message\":\"Success\"},\"data\":{\"subscriptionId\":2563396}}
					ASSERT(GetSubscriptionId() == 0);
					SetSubscriptionId(jsonGetInt(&js2, _T("subscriptionId")));
				}
				break;
			case 'H': // HeartBeat {"messageType":"H","response":{"code":200,"message":"HeartBeat"}}
				js3 = jsonGetChild(js, _T("response"));
				m_iStatusCode = js3.at(_T("code"));
				m_statusMessage = js3.at(_T("message"));
				break;
			case 'E':  //error message {"messageType":"E","response":{"code":400,"message":"thresholdLevel not valid}}
				js4 = jsonGetChild(js, _T("response"));
				m_iStatusCode = js4.at(_T("code"));
				m_statusMessage = js4.at(_T("message"));
				break;
			case 'A': // new data
				sService = jsonGetString(js, _T("service"));
				if (sService != _T("fx")) return false; // 只有此项
				pForexData = make_shared<CTiingoForexSocket>();
				js2 = jsonGetChild(js, _T("data"));
				it = js2.begin();
				sMessageType = it->get<string>(); // 必须是‘Q’
				pForexData->m_chMessageType = sMessageType.at(0);
				pForexData->m_sSymbol = jsonGetString(++it); // 证券名称
				sDatetime = jsonGetString(++it); // 时间串："2019-07-05T15:49:15.157000+00:00"
				pForexData->m_tTime = XferToTTime(sDatetime.c_str(), _T("%4d-%02d-%02dT%02d:%02d:%02d.%06d+%02d:%02d"));
				pForexData->m_dBidSize = jsonGetDouble(++it); // 买价数量
				pForexData->m_dBidPrice = jsonGetDouble(++it); // 买价
				pForexData->m_dMidPrice = jsonGetDouble(++it); // 中间价 （BidPrice + AskPrice)/2
				pForexData->m_dAskSize = jsonGetDouble(++it); // 卖价数量
				pForexData->m_dAskPrice = jsonGetDouble(++it); // 卖价
				gl_SystemData.PushTiingoForexSocket(pForexData);
				m_HeartbeatTime = GetUTCTime();
				break;
			default:
				// error
				gl_systemMessage.PushErrorMessage(_T("Tiingo Forex WebSocket type Error"));
				return false;
			}
		}
		else {
			// ERROR
			return false;
		}
	} catch (json::exception& e) {
		ReportJSonErrorToSystemMessage(_T("Tiingo Forex WebSocket "), e.what());
		return false;
	}

	return true;
}
