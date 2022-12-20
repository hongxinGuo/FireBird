#include "pch.h"

#include"JsonParse.h"
#include"JsonGetValue.h"

#include"TiingoWebInquiry.h"
#include "TiingoIEXWebSocket.h"

void ProcessTiingoIEXWebSocket(const ix::WebSocketMessagePtr& msg) {
	switch (msg->type) {
	case ix::WebSocketMessageType::Message:
		// 当系统退出时，停止接收WebSocket的过程需要时间，在此期间此回调函数继续执行，而存储器已经析构了，导致出现内存泄漏。
		// 故而需要判断是否系统正在退出（只有在没有退出系统时方可存储接收到的数据）。
		if (!gl_systemStatus.IsExitingSystem()) {
			gl_tiingoIEXWebSocket.PushData(msg->str);
		}
		break;
	case ix::WebSocketMessageType::Error:
		gl_systemMessage.PushErrorMessage(msg->errorInfo.reason.c_str());
		break;
	case ix::WebSocketMessageType::Open:
		gl_systemMessage.PushWebSocketInfoMessage(_T("Tiingo IEX WebSocket Open"));
		break;
	case ix::WebSocketMessageType::Close:
		gl_systemMessage.PushWebSocketInfoMessage(_T("Tiingo IEX WebSocket Close"));
		break;
	case ix::WebSocketMessageType::Fragment:
		gl_systemMessage.PushWebSocketInfoMessage(_T("Tiingo IEX WebSocket Fragment"));
		break;
	case ix::WebSocketMessageType::Ping:
		gl_systemMessage.PushWebSocketInfoMessage(_T("Tiingo IEX WebSocket Ping"));
		break;
	case ix::WebSocketMessageType::Pong:
		gl_systemMessage.PushWebSocketInfoMessage(_T("Tiingo IEX WebSocket Pong"));
		break;
	default: // error
		break;
	}
}

UINT ThreadConnectTiingoIEXWebSocketAndSendMessage(not_null<CTiingoIEXWebSocket*> pDataTiingoIEXWebSocket, vector<CString> vSymbol) {
	if (static bool s_fConnecting = false; !s_fConnecting) {
		s_fConnecting = true;
		if (pDataTiingoIEXWebSocket->ConnectWebSocketAndSendMessage(vSymbol)) {
			gl_systemMessage.PushInnerSystemInformationMessage(_T("开启Tiingo IEX web socket服务"));
		}
		s_fConnecting = false;
	}

	return 71;
}

CTiingoIEXWebSocket::CTiingoIEXWebSocket() : CVirtualWebSocket() {
	m_url = _T("wss://api.tiingo.com/iex");
}

/// <summary>
/// Tiingo IEX的数据源格式：wss://api.tiingo.com/iex，其密钥是随后发送的。
/// </summary>
/// 
/// <returns></returns>
bool CTiingoIEXWebSocket::Connect(void) {
	Connecting(m_url, ProcessTiingoIEXWebSocket);

	return true;
}

bool CTiingoIEXWebSocket::Send(vector<CString> vSymbol) {
	ASSERT(IsOpen());

	const string messageAuth(CreateMessage(vSymbol));
	SendMessage(messageAuth);

	gl_systemMessage.PushInnerSystemInformationMessage(messageAuth.c_str());

	return true;
}

CString CTiingoIEXWebSocket::CreateMessage(vector<CString> vSymbol) {
	const CString strPrefix = _T("{\"eventName\":\"subscribe\",\"authorization\":\"");
	const CString strMiddle = _T("\",\"eventData\":{\"thresholdLevel\":5,\"tickers\":[");
	const CString strSuffix = _T("]}}");
	CString strAuth = gl_pTiingoWebInquiry->GetInquiryToken();
	strAuth = strAuth.Right(strAuth.GetLength() - 7);

	vSymbol.emplace_back(_T("rig")); // 多加一个Tiingo制式的代码。
	vSymbol.emplace_back(_T("aapl")); // 多加一个Tiingo制式的代码。
	const CString strSymbols = CreateTiingoWebSocketSymbolString(vSymbol); // 去除最后多余的字符','

	CString str = strPrefix + strAuth + strMiddle + strSymbols + strSuffix;

	return str;
}

bool CTiingoIEXWebSocket::CreatingThreadConnectWebSocketAndSendMessage(vector<CString> vSymbol) {
	thread thread1(ThreadConnectTiingoIEXWebSocketAndSendMessage, this, vSymbol);
	thread1.detach();

	return true;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// https://api.tiingo.com/documentation/websockets/iex
//
// 共五种格式：
// {"messageType":"I","data":{"subscriptionId":2563367},"response":{"code":200,"message":"Success"}}
// {"data":{"tickers":["*","uso","msft","tnk"],"thresholdLevel":"0"},"messageType":"I","response":{"code":200,"message":"Success"}}
// {"messageType":"H","response":{"code":200,"message":"HeartBeat"}}
// {"messageType":"A","service":"iex","data":["Q","2019-01-30T13:33:45.383129126-05:00",1548873225383129126,"vym",100,81.58,81.585,81.59,100,null,null,0,0,null,null,null]}
// {"messageType":"A","service":"iex","data":["T","2019-01-30T13:33:45.594808294-05:00",1548873225594808294,"wes",null,null,null,null,null,50.285,200,null,0,0,0,0]}
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool CTiingoIEXWebSocket::ParseTiingoIEXWebSocketData(shared_ptr<string> pData) {
	string sSymbol;
	string strSymbol;
	string sTicker, sExchange, sValue;
	CTiingoIEXSocketPtr pIEXData = nullptr;

	try {
		if (json js; NlohmannCreateJson(&js, *pData)) {
			int i = 0;
			string sMessageType;
			string sDatetime;
			char chType;
			string sService;
			string sType;
			json js2;
			json::iterator it;
			sType = jsonGetString(&js, _T("messageType"));
			if (sType.empty()) return false;
			switch (sType.at(0)) {
			case 'A': // 交易数据
				pIEXData = make_shared<CTiingoIEXSocket>();
				sService = jsonGetString(&js, _T("service"));
				if (sService != _T("iex")) return false; // 此项必须为"iex"
				if (!jsonGetChild(&js, _T("data"), &js2)) return false;
				it = js2.begin();
				sMessageType = jsonGetString(it); // message type, 'Q'、'T'或者'B'
				chType = sMessageType.at(0);
				pIEXData->m_chMessageType = chType;
				switch (chType) {
				case 'Q': // top-of-book update message
					++it;
					sDatetime = jsonGetString(it);
					++it;
					pIEXData->m_iNanoseconds = jsonGetLongLong(it);
					++it;
					sTicker = jsonGetString(it);
					pIEXData->m_strSymbol = sTicker.c_str();
					++it;
					pIEXData->m_dBidSize = jsonGetDouble(it);
					++it;
					pIEXData->m_dBidPrice = jsonGetDouble(it);
					++it;
					pIEXData->m_dMidPrice = jsonGetDouble(it);
					++it;
					pIEXData->m_dAskPrice = jsonGetDouble(it);
					++it;
					pIEXData->m_dAskSize = jsonGetDouble(it);
					++it;
					pIEXData->m_dLastPrice = jsonGetDouble(it);
					++it;
					pIEXData->m_dLastSize = jsonGetDouble(it);
					++it;
					pIEXData->m_iAfterHour = jsonGetInt(it);
					++it;
					pIEXData->m_iISO = jsonGetInt(it);
					++it;
					pIEXData->m_iOddlot = jsonGetInt(it);
					++it;
					pIEXData->m_iNMSRule611 = jsonGetInt(it);
					break;
				case 'T': // 'T' last trade message
					++it;
					sDatetime = jsonGetString(it);
					++it;
					pIEXData->m_iNanoseconds = jsonGetLongLong(it);
					++it;
					sTicker = jsonGetString(it);
					pIEXData->m_strSymbol = sTicker.c_str();
					++it;
					pIEXData->m_dBidSize = jsonGetDouble(it);
					++it;
					pIEXData->m_dBidPrice = jsonGetDouble(it);
					++it;
					pIEXData->m_dMidPrice = jsonGetDouble(it);
					++it;
					pIEXData->m_dAskPrice = jsonGetDouble(it);
					++it;
					pIEXData->m_dAskSize = jsonGetDouble(it);
					++it;
					pIEXData->m_dLastPrice = jsonGetDouble(it);
					++it;
					pIEXData->m_dLastSize = jsonGetInt(it);
					++it;
					pIEXData->m_iHalted = jsonGetInt(it);
					++it;
					pIEXData->m_iISO = jsonGetInt(it);
					++it;
					pIEXData->m_iOddlot = jsonGetInt(it);
					++it;
					pIEXData->m_iNMSRule611 = jsonGetInt(it);
					break;
				case 'B': // 'B'trade break messages
					i++;
					break;
				default: // 错误
					return false;
					break;
				}
				gl_SystemData.PushTiingoIEXSocket(pIEXData);
				m_fReceivingData = true;
				break;
			case 'I': // authentication  {\"messageType\":\"I\",\"data\":{\"subscriptionId\":2563367},\"response\":{\"code\":200,\"message\":\"Success\"}}
				if (!jsonGetChild(&js, _T("data"), &js2)) return false;
				try {
					json js3 = js2.at(_T("tickers"));
					for (auto it2 = js3.begin(); it2 != js3.end(); ++it2) {
						strSymbol = jsonGetString(it2);
						m_vCurrentSymbol.push_back(strSymbol.c_str());
					}
				}
				catch (json::exception&) {
					ASSERT(GetSubscriptionId() == 0);
					SetSubscriptionId(jsonGetInt(&js2, _T("subscriptionId")));
				}
				break;
			case 'H': // Heart beat {\"messageType\":\"H\",\"response\":{\"code\":200,\"message\":\"HeartBeat\"}}
				// 无需处理
				break;
			default:
				return false;
				break;
			}
		}
		else {
			// ERROR
			return false;
		}
	}
	catch (json::exception& e) {
		ReportJSonErrorToSystemMessage(_T("Tiingo IEX WebSocket "), e.what());
		return false;
	}

	return true;
}
