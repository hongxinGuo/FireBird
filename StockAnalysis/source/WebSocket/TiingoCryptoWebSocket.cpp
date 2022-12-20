#include "pch.h"

#include"JsonParse.h"
#include"JsonGetValue.h"

#include"TiingoWebInquiry.h"
#include "TiingoCryptoWebSocket.h"

void ProcessTiingoCryptoWebSocket(const ix::WebSocketMessagePtr& msg) {
	switch (msg->type) {
	case ix::WebSocketMessageType::Message:
		// 当系统退出时，停止接收WebSocket的过程需要时间，在此期间此回调函数继续执行，而存储器已经析构了，导致出现内存泄漏。
		// 故而需要判断是否系统正在退出（只有在没有退出系统时方可存储接收到的数据）。
		if (!gl_systemStatus.IsExitingSystem()) {
			gl_tiingoCryptoWebSocket.PushData(msg->str);
		}
		break;
	case ix::WebSocketMessageType::Error:
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

UINT ThreadConnectTiingoCryptoWebSocketAndSendMessage(not_null<CTiingoCryptoWebSocket*> pDataTiingoCryptoWebSocket, vector<CString> vSymbol) {
	if (static bool s_fConnecting = false; !s_fConnecting) {
		s_fConnecting = true;
		if (pDataTiingoCryptoWebSocket->ConnectWebSocketAndSendMessage(vSymbol)) {
			gl_systemMessage.PushInnerSystemInformationMessage(_T("开启Tiingo Crypto web socket服务"));
		}
		s_fConnecting = false;
	}

	return 73;
}

CTiingoCryptoWebSocket::CTiingoCryptoWebSocket() : CVirtualWebSocket() {
	m_url = _T("wss://api.tiingo.com/crypto");
}

/// <summary>
/// Tiingo Crypto的数据源格式：wss://api.tiingo.com/crypto，其密钥是随后发送的。
/// </summary>
/// <param name=""></param>
/// <returns></returns>
bool CTiingoCryptoWebSocket::Connect(void) {
	Connecting(m_url, ProcessTiingoCryptoWebSocket);

	return true;
}

//////////////////////////////////////////////////////////////////////////////////////////////
//
// 接收所有的Crypto数据时，每秒数据量大致在50-100K附近。
//
// thresholdlevel 2: Top-of-Book AND Last Trade updates.
// thresholdlevel 5: only Last Trade updates.
//
//////////////////////////////////////////////////////////////////////////////////////////////
bool CTiingoCryptoWebSocket::Send(vector<CString> vSymbol) {
	ASSERT(IsOpen());

	const string messageAuth(CreateMessage(vSymbol));

	ASSERT(IsOpen());

	ix::WebSocketSendInfo info = SendMessage(messageAuth);
	gl_systemMessage.PushInnerSystemInformationMessage(messageAuth.c_str());

	return true;
}

CString CTiingoCryptoWebSocket::CreateMessage(vector<CString> vSymbol) {
	const CString strPrefix = _T("{\"eventName\":\"subscribe\",\"authorization\":\"");
	const CString strMiddle = _T("\",\"eventData\":{\"thresholdLevel\":2,\"tickers\":["); // 5：Trade Updates per-exchange.2：Top-of-Book quote updates as well as Trade updates. Both quote and trade updates are per-exchange
	const CString strSuffix = _T("]}}"); // 5：Trade Updates per-exchange.2：Top-of-Book quote updates as well as Trade updates. Both quote and trade updates are per-exchange
	CString strAuth = gl_pTiingoWebInquiry->GetInquiryToken();

	strAuth = strAuth.Right(strAuth.GetLength() - 7);
	vSymbol.push_back(_T("dkaeth")); // 多加一个Tiingo制式的代码。由于目前自选crypto使用的是finnhub制式的代码格式，皆为无效代码。
	vSymbol.push_back(_T("ksmust")); // 多加一个Tiingo制式的代码。由于目前自选crypto使用的是finnhub制式的代码格式，皆为无效代码。
	const CString strSymbols = CreateTiingoWebSocketSymbolString(vSymbol);

	CString str = strPrefix + strAuth + strMiddle + strSymbols + strSuffix;

	return str;
}

bool CTiingoCryptoWebSocket::CreateThreadConnectWebSocketAndSendMessage(vector<CString> vSymbol) {
	thread thread1(ThreadConnectTiingoCryptoWebSocketAndSendMessage, this, vSymbol);
	thread1.detach();

	return true;
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
//
// </summary>
// <param name="pData"></param>
// <returns></returns>
bool CTiingoCryptoWebSocket::ParseTiingoCryptoWebSocketData(shared_ptr<string> pData) {
	string sSymbol;
	string strSymbol;
	string sTickers, sExchange;

	try {
		if (json pt; NlohmannCreateJson(&pt, *pData)) {
			CTiingoCryptoSocketPtr pCryptoData;
			string sService;
			string sDatetime;
			string sMessageType;
			char chType;
			string sType;
			json pt3;
			json pt2;
			json::iterator it;
			sType = jsonGetString(&pt, _T("messageType"));
			chType = sType.at(0);
			switch (chType) {
			case 'I': // 注册 {\"messageType\":\"I\",\"response\":{\"code\":200,\"message\":\"Success\"},\"data\":{\"subscriptionId\":2563396}}
				jsonGetChild(&pt, _T("data"), &pt2);
				try {
					pt3 = pt2.at(_T("tickers"));
					for (auto it2 = pt3.begin(); it2 != pt3.end(); ++it2) {
						strSymbol = jsonGetString(it2);
						m_vCurrentSymbol.push_back(strSymbol.c_str());
					}
				}
				catch (json::exception&) {
					ASSERT(GetSubscriptionId() == 0);
					SetSubscriptionId(jsonGetInt(&pt2, _T("subscriptionId")));
				}
				break;
			case 'H': // heart beat {\"messageType\":\"H\",\"response\":{\"code\":200,\"message\":\"HeartBeat\"}}
				// do nothing
				break;
			case 'A': // new data
				pCryptoData = make_shared<CTiingoCryptoSocket>();
				sService = jsonGetString(&pt, _T("service"));
				if (sService != _T("crypto_data")) return false; // 格式不符则退出
				if (!jsonGetChild(&pt, _T("data"), &pt2)) return false;
				it = pt2.begin();
				sMessageType = it->get<string>(); // ‘Q’或者‘T’
				if (sMessageType.at(0) == 'T') {
					//last trade message {\"service\":\"crypto_data\",\"data\":[\"T\",\"jstusdt\",\"2021-08-10T23:56:55.237000+00:00\",\"huobi\",3952.5,0.062108],\"messageType\":\"A\"}
					pCryptoData->m_chMessageType = 'T';
					++it;
					sTickers = jsonGetString(it); // 证券名称
					pCryptoData->m_strSymbol = sTickers.c_str();
					++it;
					sDatetime = jsonGetString(it); // 时间串："2019-07-05T15:49:15.157000+00:00"
					++it;
					sExchange = jsonGetString(it); // 交易所
					pCryptoData->m_strExchange = sExchange.c_str();
					++it;
					pCryptoData->m_dLastSize = jsonGetDouble(it); // 最新数量
					++it;
					pCryptoData->m_dLastPrice = jsonGetDouble(it); // 最新价格
				}
				else if (sMessageType.at(0) == 'Q') {
					// 'Q' top-of-book update message.
					pCryptoData->m_chMessageType = 'Q';
					++it;
					sTickers = jsonGetString(it); // 证券名称
					pCryptoData->m_strSymbol = sTickers.c_str();
					++it;
					sDatetime = jsonGetString(it); // 时间串："2019-07-05T15:49:15.157000+00:00"
					++it;
					sExchange = jsonGetString(it);// 交易所
					pCryptoData->m_strExchange = sExchange.c_str();
					++it;
					pCryptoData->m_dBidSize = jsonGetDouble(it); // 买价数量
					++it;
					pCryptoData->m_dBidPrice = jsonGetDouble(it); // 买价
					++it;
					pCryptoData->m_dMidPrice = jsonGetDouble(it); // 中间价 （BidPrice + AskPrice)/2
					++it;
					pCryptoData->m_dAskSize = jsonGetDouble(it); // 卖价数量
					++it;
					pCryptoData->m_dAskPrice = jsonGetDouble(it); // 卖价
				}
				else {
					// 格式不对
					return false;
				}
				gl_SystemData.PushTiingoCryptoSocket(pCryptoData);
				m_fReceivingData = true;
				break;
			default: // 错误
				return false;
				break;
			}
		}
		else {
			return false;
		}
	}
	catch (json::exception& e) {
		ReportJSonErrorToSystemMessage(_T("Tiingo Crypto WebSocket "), e.what());
		return false;
	}

	return true;
}
