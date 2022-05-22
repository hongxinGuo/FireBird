#include "pch.h"

#include"globedef.h"
#include"accessory.h"
#include"ThreadStatus.h"
#include"SystemMessage.h"
#include"SystemData.h"

#include"WebInquirer.h"
#include "TiingoCryptoWebSocket.h"

void FunctionProcessTiingoCryptoWebSocket(const ix::WebSocketMessagePtr& msg) {
	switch (msg->type) {
	case ix::WebSocketMessageType::Message:
		// 当系统退出时，停止接收WebSocket的过程需要时间，在此期间此回调函数继续执行，而存储器已经析构了，导致出现内存泄漏。
		// 故而需要判断是否系统正在退出（只有在没有退出系统时方可存储接收到的数据）。
		if (!gl_fExitingSystem) {
			gl_WebInquirer.PushTiingoCryptoWebSocketData(msg->str);
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
	static bool s_fConnecting = false;
	if (!s_fConnecting) {
		s_fConnecting = true;
		gl_ThreadStatus.IncreaseSavingThread();
		pDataTiingoCryptoWebSocket->ConnectWebSocketAndSendMessage(vSymbol);
		gl_ThreadStatus.DecreaseSavingThread();
		s_fConnecting = false;
	}

	return 73;
}

CTiingoCryptoWebSocket::CTiingoCryptoWebSocket() : CVirtualWebSocket() {
	m_url = _T("wss://api.tiingo.com/crypto");
}

CTiingoCryptoWebSocket::~CTiingoCryptoWebSocket(void) {
}

/// <summary>
/// Tiingo Crypto的数据源格式：wss://api.tiingo.com/crypto，其密钥是随后发送的。
/// </summary>
/// <param name=""></param>
/// <returns></returns>
bool CTiingoCryptoWebSocket::Connect(void) {
	Connecting(m_url, FunctionProcessTiingoCryptoWebSocket);

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

	string messageAuth(CreateMessage(vSymbol));
	ix::WebSocketSendInfo info;

	ASSERT(IsOpen());

	info = Sending(messageAuth);
	gl_systemMessage.PushInnerSystemInformationMessage(messageAuth.c_str());

	return true;
}

CString CTiingoCryptoWebSocket::CreateMessage(vector<CString> vSymbol) {
	CString str;
	CString strPreffix = _T("{\"eventName\":\"subscribe\",\"authorization\":\"");
	CString strMiddle = _T("\",\"eventData\":{\"thresholdLevel\":2,\"tickers\":["); // 5：Trade Updates per-exchange.2：Top-of-Book quote updates as well as Trade updates. Both quote and trade updates are per-exchange
	CString strSuffix = _T("]}}"); // 5：Trade Updates per-exchange.2：Top-of-Book quote updates as well as Trade updates. Both quote and trade updates are per-exchange
	CString strAuth = gl_pTiingoWebInquiry->GetInquiringStringSuffix();

	strAuth = strAuth.Right(strAuth.GetLength() - 7);
	vSymbol.push_back(_T("dkaeth")); // 多加一个Tiingo制式的代码。由于目前自选crypto使用的是finnhub制式的代码格式，皆为无效代码。
	vSymbol.push_back(_T("ksmust")); // 多加一个Tiingo制式的代码。由于目前自选crypto使用的是finnhub制式的代码格式，皆为无效代码。
	CString strSymbols = CreateTiingoWebSocketSymbolString(vSymbol);

	str = strPreffix + strAuth + strMiddle + strSymbols + strSuffix;

	return str;
}

bool CTiingoCryptoWebSocket::CreatingThreadConnectWebSocketAndSendMessage(vector<CString> vSymbol) {
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
	ptree::iterator it;
	ptree pt, pt2, pt3, pt4;
	string sType, sSymbol;
	char chType;
	string strSymbol;

	string sMessageType, sTickers, sExchange, sDatetime, sService;
	CTiingoCryptoSocketPtr pCryptoData = nullptr;

	try {
		if (ParseWithPTree(pt, *pData)) {
			sType = ptreeGetString(pt, _T("messageType"));
			chType = sType.at(0);
			switch (chType) {
			case 'I': // 注册 {\"messageType\":\"I\",\"response\":{\"code\":200,\"message\":\"Success\"},\"data\":{\"subscriptionId\":2563396}}
				pt2 = pt.get_child(_T("data"));
				try {
					pt3 = pt2.get_child(_T("tickers"));
					for (ptree::iterator it2 = pt3.begin(); it2 != pt3.end(); it2++) {
						pt4 = it2->second;
						strSymbol = pt4.get_value<string>();
						m_vCurrentSymbol.push_back(strSymbol.c_str());
					}
				}
				catch (exception e) {
					ASSERT(GetSubscriptionId() == 0);
					SetSubscriptionId(ptreeGetInt(pt2, _T("subscriptionId")));
				}
				break;
			case 'H': // heart beat {\"messageType\":\"H\",\"response\":{\"code\":200,\"message\":\"HeartBeat\"}}
				// do nothing
				break;
			case 'A': // new data
				pCryptoData = make_shared<CTiingoCryptoSocket>();
				sService = ptreeGetString(pt, _T("service"));
				if (sService.compare(_T("crypto_data")) != 0) return false; // 格式不符则退出
				pt2 = pt.get_child(_T("data"));
				it = pt2.begin();
				pt3 = it->second;
				sMessageType = pt3.get_value<string>(); // ‘Q’或者‘T’
				if (sMessageType.at(0) == 'T') { //last trade message {\"service\":\"crypto_data\",\"data\":[\"T\",\"jstusdt\",\"2021-08-10T23:56:55.237000+00:00\",\"huobi\",3952.5,0.062108],\"messageType\":\"A\"}
					pCryptoData->m_chMessageType = 'T';
					it++;
					pt3 = it->second;
					sTickers = pt3.get_value<string>(); // 证券名称
					pCryptoData->m_strSymbol = sTickers.c_str();
					it++;
					pt3 = it->second;
					sDatetime = pt3.get_value<string>(); // 时间串："2019-07-05T15:49:15.157000+00:00"
					it++;
					pt3 = it->second;
					sExchange = pt3.get_value<string>(); // 交易所
					pCryptoData->m_strExchange = sExchange.c_str();
					it++;
					pt3 = it->second;
					pCryptoData->m_dLastSize = pt3.get_value<double>(); // 最新数量
					it++;
					pt3 = it->second;
					pCryptoData->m_dLastPrice = pt3.get_value<double>(); // 最新价格
				}
				else if (sMessageType.at(0) == 'Q') { // 'Q' top-of-book update message.
					pCryptoData->m_chMessageType = 'Q';
					it++;
					pt3 = it->second;
					sTickers = pt3.get_value<string>(); // 证券名称
					pCryptoData->m_strSymbol = sTickers.c_str();
					it++;
					pt3 = it->second;
					sDatetime = pt3.get_value<string>(); // 时间串："2019-07-05T15:49:15.157000+00:00"
					it++;
					pt3 = it->second;
					sExchange = pt3.get_value<string>(); // 交易所
					pCryptoData->m_strExchange = sExchange.c_str();
					it++;
					pt3 = it->second;
					pCryptoData->m_dBidSize = pt3.get_value<double>(); // 买价数量
					it++;
					pt3 = it->second;
					pCryptoData->m_dBidPrice = pt3.get_value<double>(); // 买价
					it++;
					pt3 = it->second;
					pCryptoData->m_dMidPrice = pt3.get_value<double>(); // 中间价 （BidPrice + AskPrice)/2
					it++;
					pt3 = it->second;
					pCryptoData->m_dAskSize = pt3.get_value<double>(); // 卖价数量
					it++;
					pt3 = it->second;
					pCryptoData->m_dAskPrice = pt3.get_value<double>(); // 卖价
				}
				else { // 格式不对
					return false;
				}
				gl_SystemData.PushTiingoCryptoSocket(pCryptoData);
				m_fReveivingData = true;
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
	catch (ptree_error& e) {
		ReportJSonErrorToSystemMessage(_T("Tiingo Crypto WebSocket "), e);
		return false;
	}

	return true;
}