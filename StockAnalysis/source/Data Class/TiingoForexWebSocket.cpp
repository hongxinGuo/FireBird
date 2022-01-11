#include "pch.h"

#include"WebInquirer.h"
#include "TiingoForexWebSocket.h"

void FunctionProcessTiingoForexWebSocket(const ix::WebSocketMessagePtr& msg) {
	switch (msg->type) {
	case ix::WebSocketMessageType::Message:
		// 当系统退出时，停止接收WebSocket的过程需要时间，在此期间此回调函数继续执行，而存储器已经析构了，导致出现内存泄漏。
		// 故而需要判断是否系统正在退出（只有在没有退出系统时方可存储接收到的数据）。
		if (!gl_fExitingSystem) {
			gl_WebInquirer.PushTiingoForexWebSocketData(msg->str);
			//gl_systemMessage.PushInnerSystemInformationMessage(msg->str.c_str());
		}
		break;
	case ix::WebSocketMessageType::Error:
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

UINT ThreadConnectingTiingoForexWebSocketAndSendMessage(not_null<CTiingoForexWebSocket*> pDataTiingoForexWebSocket, vector<CString> vSymbol) {
	gl_ThreadStatus.IncreaseSavingThread();
	pDataTiingoForexWebSocket->ConnectingWebSocketAndSendMessage(vSymbol);
	gl_ThreadStatus.DecreaseSavingThread();

	return 73;
}

CTiingoForexWebSocket::CTiingoForexWebSocket() : CVirtualWebSocket() {
}

CTiingoForexWebSocket::~CTiingoForexWebSocket(void) {
}

/// <summary>
/// Tiingo Forex的数据源格式：wss://api.tiingo.com/fx，其密钥是随后发送的。
/// </summary>
/// <param name=""></param>
/// <returns></returns>
bool CTiingoForexWebSocket::Connect(void) {
	std::string url("wss://api.tiingo.com/fx");

	Connecting(url, FunctionProcessTiingoForexWebSocket);
	return true;
}

//////////////////////////////////////////////////////////////////////////////////////////////////
//
// 5状态下每秒接收100K左右。
//
// threshlodlevel 5: ALL Top-of-Book updates
// thresholdlevel 7: A top-of-book update that is due to a change in either the bid/ask price or size.
//
//////////////////////////////////////////////////////////////////////////////////////////////////
bool CTiingoForexWebSocket::Send(vector<CString> vSymbol)
{
	static bool sm_fSendAuth = true;
	CString str;
	CString strPreffix = _T("{\"eventName\":\"subscribe\",\"authorization\":\"");
	CString strMiddle = _T("\",\"eventData\":{\"thresholdLevel\":5,\"tickers\":["); //7：A top - of - book update that is due to a change in either the bid / ask price or size.
	CString strSuffix = _T("]}}"); //7：A top - of - book update that is due to a change in either the bid / ask price or size.
	CString strAuth = gl_pTiingoWebInquiry->GetInquiringStringSuffix();
	strAuth = strAuth.Right(strAuth.GetLength() - 7);

	vSymbol.push_back(_T("gbpaud")); // 多加一个Tiingo制式的代码。由于目前自选crypto使用的是finnhub制式的代码格式，皆为无效代码。
	vSymbol.push_back(_T("eurusd")); // 多加一个Tiingo制式的代码。由于目前自选crypto使用的是finnhub制式的代码格式，皆为无效代码。
	CString strSymbols = CreateTiingoWebSocketSymbolString(vSymbol);

	str = strPreffix + strAuth + strMiddle + strSymbols + strSuffix;

	string messageAuth(str);
	ix::WebSocketSendInfo info;

	ASSERT(IsOpen());

	if (sm_fSendAuth) {
		info = Sending(messageAuth);
	}

	return true;
}

bool CTiingoForexWebSocket::CreatingThreadConnectingWebSocketAndSendMessage(vector<CString> vSymbol) {
	thread thread1(ThreadConnectingTiingoForexWebSocketAndSendMessage, this, vSymbol);
	thread1.detach();

	return true;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
//
// https://api.tiingo.com/documentation/websockets/forex
//
// 共四种格式：
// {"messageType":"I","response":{"code":200,"message":"Success"},"data":{"subscriptionId":2563396}}
// {"messageType":"H","response":{"code":200,"message":"HeartBeat"}}
// {"messageType":"A","service":"fx","data":["Q","eurnok","2019-07-05T15:49:15.157000+00:00",5000000.0,9.6764,9.678135,5000000.0,9.67987]}
// {"messageType":"A","service":"fx","data":["Q","gbpaud","2019-07-05T15:49:15.236000+00:00",1000000.0,1.79457,1.79477,5000000.0,1.79497]}
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool CTiingoForexWebSocket::ParseTiingoForexWebSocketData(shared_ptr<string> pData) {
	ptree pt, pt2, pt3;
	ptree::iterator it;
	string sType, sSymbol, sService;
	char chType;

	string sMessageType, sTickers, sDatetime;
	CTiingoForexSocketPtr pForexData = nullptr;

	try {
		if (ConvertToJSON(pt, *pData)) {
			sType = pt.get<string>(_T("messageType"));
			chType = sType.at(0);
			switch (chType) {
			case 'I': // 注册 {\"messageType\":\"I\",\"response\":{\"code\":200,\"message\":\"Success\"},\"data\":{\"subscriptionId\":2563396}}
				pt2 = pt.get_child(_T("data"));
				ASSERT(GetSubscriptionId() == 0);
				SetSubscriptionId(pt2.get<int>(_T("subscriptionId")));
				break;
			case 'H': // HeartBeat {"messageType":"H","response":{"code":200,"message":"HeartBeat"}}
				// do nothing
				break;
			case 'A': // new data
				sService = pt.get<string>(_T("service"));
				if (sService.compare(_T("fx")) != 0) return false; // 只有此项
				pForexData = make_shared<CTiingoForexSocket>();
				pt2 = pt.get_child(_T("data"));
				it = pt2.begin();
				pt3 = it->second;
				sMessageType = pt3.get_value<string>(); // 必须是‘Q’
				pForexData->m_chMessageType = sMessageType.at(0);
				it++;
				pt3 = it->second;
				sTickers = pt3.get_value<string>(); // 证券名称
				pForexData->m_strSymbol = sTickers.c_str();
				it++;
				pt3 = it->second;
				sDatetime = pt3.get_value<string>(); // 时间串："2019-07-05T15:49:15.157000+00:00"
				it++;
				pt3 = it->second;
				pForexData->m_dBidSize = pt3.get_value<double>(); // 买价数量
				it++;
				pt3 = it->second;
				pForexData->m_dBidPrice = pt3.get_value<double>(); // 买价
				it++;
				pt3 = it->second;
				pForexData->m_dMidPrice = pt3.get_value<double>(); // 中间价 （BidPrice + AskPrice)/2
				it++;
				pt3 = it->second;
				pForexData->m_dAskSize = pt3.get_value<double>(); // 卖价数量
				it++;
				pt3 = it->second;
				pForexData->m_dAskPrice = pt3.get_value<double>(); // 卖价
				gl_SystemData.PushTiingoForexSocket(pForexData);
				break;
			default:
				// error
				gl_systemMessage.PushErrorMessage(_T("Tiingo Forex WebSocket type Error"));
				return false;
				break;
			}
		}
		else {
			// ERROR
			return false;
		}
	}
	catch (ptree_error& e) {
		ReportJSonErrorToSystemMessage(_T("Tiingo Forex WebSocket "), e);
		return false;
	}

	return true;
}