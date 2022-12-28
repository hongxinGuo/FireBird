#include "pch.h"

#include"JsonParse.h"
#include"JsonGetValue.h"

#include"TiingoWebInquiry.h"
#include "TiingoForexWebSocket.h"

#include<thread>
#include<memory>
using std::thread;
using std::make_shared;

void ProcessTiingoForexWebSocket(const ix::WebSocketMessagePtr& msg) {
	switch (msg->type) {
	case ix::WebSocketMessageType::Message:
		// 当系统退出时，停止接收WebSocket的过程需要时间，在此期间此回调函数继续执行，而存储器已经析构了，导致出现内存泄漏。
		// 故而需要判断是否系统正在退出（只有在没有退出系统时方可存储接收到的数据）。
		if (!gl_systemStatus.IsExitingSystem()) { gl_tiingoForexWebSocket.PushData(msg->str); }
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

UINT ThreadConnectTiingoForexWebSocketAndSendMessage(not_null<CTiingoForexWebSocket*> pDataTiingoForexWebSocket, vector<CString> vSymbol) {
	static bool s_fConnecting = false;
	if (!s_fConnecting) {
		s_fConnecting = true;
		if (pDataTiingoForexWebSocket->ConnectWebSocketAndSendMessage(vSymbol)) { gl_systemMessage.PushInnerSystemInformationMessage(_T("开启Tiingo Forex web socket服务")); }
		s_fConnecting = false;
	}

	return 73;
}

CTiingoForexWebSocket::CTiingoForexWebSocket() : CVirtualWebSocket() { m_url = _T("wss://api.tiingo.com/fx"); }

CTiingoForexWebSocket::~CTiingoForexWebSocket(void) {}

/// <summary>
/// Tiingo Forex的数据源格式：wss://api.tiingo.com/fx，其密钥是随后发送的。
/// </summary>
/// <param name=""></param>
/// <returns></returns>
bool CTiingoForexWebSocket::Connect(void) {
	Connecting(m_url, ProcessTiingoForexWebSocket);
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
bool CTiingoForexWebSocket::Send(vector<CString> vSymbol) {
	ASSERT(IsOpen());

	const string messageAuth(CreateMessage(vSymbol));
	ix::WebSocketSendInfo info = SendMessage(messageAuth);
	gl_systemMessage.PushInnerSystemInformationMessage(messageAuth.c_str());

	return true;
}

CString CTiingoForexWebSocket::CreateMessage(vector<CString> vSymbol) {
	const CString strPrefix = _T("{\"eventName\":\"subscribe\",\"authorization\":\"");
	const CString strMiddle = _T("\",\"eventData\":{\"thresholdLevel\":5,\"tickers\":["); //7：A top - of - book update that is due to a change in either the bid / ask price or size.
	const CString strSuffix = _T("]}}"); //7：A top - of - book update that is due to a change in either the bid / ask price or size.
	CString strAuth = gl_pTiingoWebInquiry->GetInquiryToken();
	strAuth = strAuth.Right(strAuth.GetLength() - 7);

	vSymbol.push_back(_T("gbpaud")); // 多加一个Tiingo制式的代码。由于目前自选crypto使用的是finnhub制式的代码格式，皆为无效代码。
	vSymbol.push_back(_T("eurusd")); // 多加一个Tiingo制式的代码。由于目前自选crypto使用的是finnhub制式的代码格式，皆为无效代码。
	const CString strSymbols = CreateTiingoWebSocketSymbolString(vSymbol);

	CString str = strPrefix + strAuth + strMiddle + strSymbols + strSuffix;

	return str;
}

bool CTiingoForexWebSocket::CreatingThreadConnectWebSocketAndSendMessage(vector<CString> vSymbol) {
	thread thread1(ThreadConnectTiingoForexWebSocketAndSendMessage, this, vSymbol);
	thread1.detach();

	return true;
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
		if (json js; NlohmannCreateJson(&js, *pData)) {
			string sDatetime;
			string sMessageType;
			char chType;
			string sService;
			string sType;
			json::iterator it;
			json js2;
			sType = jsonGetString(&js, _T("messageType"));
			chType = sType.at(0);
			switch (chType) {
			case 'I': // 注册 {\"messageType\":\"I\",\"response\":{\"code\":200,\"message\":\"Success\"},\"data\":{\"subscriptionId\":2563396}}
				js2 = jsonGetChild(&js, _T("data"));
				try {
					json js3 = js2.at(_T("tickers"));
					for (auto it3 = js3.begin(); it3 != js3.end(); ++it3) {
						strSymbol = jsonGetString(it3);
						m_vCurrentSymbol.push_back(strSymbol.c_str());
					}
				} catch (json::exception&) {
					ASSERT(GetSubscriptionId() == 0);
					SetSubscriptionId(jsonGetInt(&js2, _T("subscriptionId")));
				}
				break;
			case 'H': // HeartBeat {"messageType":"H","response":{"code":200,"message":"HeartBeat"}}
				// do nothing
				break;
			case 'A': // new data
				sService = jsonGetString(&js, _T("service"));
				if (sService != _T("fx")) return false; // 只有此项
				pForexData = make_shared<CTiingoForexSocket>();
				js2 = jsonGetChild(&js, _T("data"));
				it = js2.begin();
				sMessageType = it->get<string>(); // 必须是‘Q’
				pForexData->m_chMessageType = sMessageType.at(0);
				++it;
				sTickers = jsonGetString(it); // 证券名称
				pForexData->m_strSymbol = sTickers.c_str();
				++it;
				sDatetime = jsonGetString(it); // 时间串："2019-07-05T15:49:15.157000+00:00"
				++it;
				pForexData->m_dBidSize = jsonGetDouble(it); // 买价数量
				++it;
				pForexData->m_dBidPrice = jsonGetDouble(it); // 买价
				++it;
				pForexData->m_dMidPrice = jsonGetDouble(it); // 中间价 （BidPrice + AskPrice)/2
				++it;
				pForexData->m_dAskSize = jsonGetDouble(it); // 卖价数量
				++it;
				pForexData->m_dAskPrice = jsonGetDouble(it); // 卖价
				gl_SystemData.PushTiingoForexSocket(pForexData);
				m_fReceivingData = true;
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
	} catch (json::exception& e) {
		ReportJSonErrorToSystemMessage(_T("Tiingo Forex WebSocket "), e.what());
		return false;
	}

	return true;
}
