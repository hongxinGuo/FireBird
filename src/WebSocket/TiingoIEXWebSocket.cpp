#include "pch.h"

#include"JsonParse.h"
#include"JsonGetValue.h"

#include "TiingoIEXWebSocket.h"

#include "TiingoDataSource.h"

using namespace std;

void ProcessTiingoIEXWebSocket(const ix::WebSocketMessagePtr& msg) {
	gl_pTiingoIEXWebSocket->SetError(false);
	switch (msg->type) {
	case ix::WebSocketMessageType::Message:
		// ��ϵͳ�˳�ʱ��ֹͣ����WebSocket�Ĺ�����Ҫʱ�䣬�ڴ��ڼ�˻ص���������ִ�У����洢���Ѿ������ˣ����³����ڴ�й©��
		// �ʶ���Ҫ�ж��Ƿ�ϵͳ�����˳���ֻ����û���˳�ϵͳʱ���ɴ洢���յ������ݣ���
		if (!gl_systemConfiguration.IsExitingSystem()) {
			gl_pTiingoIEXWebSocket->PushData(msg->str);
		}
		break;
	case ix::WebSocketMessageType::Error:
		gl_pTiingoIEXWebSocket->SetError(true);
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

CTiingoIEXWebSocket::CTiingoIEXWebSocket() {
	ASSERT(gl_systemConfiguration.IsInitialized());
	m_url = _T("wss://api.tiingo.com/iex");
}

/// <summary>
/// Tiingo IEX������Դ��ʽ��wss://api.tiingo.com/iex������Կ������͵ġ�
/// </summary>
/// 
/// <returns></returns>
void CTiingoIEXWebSocket::Connect() {
	Connecting(m_url, ProcessTiingoIEXWebSocket);
}

void CTiingoIEXWebSocket::Send(const vectorString& vSymbol) {
	ASSERT(IsOpen());

	const string messageAuth(CreateMessage(vSymbol));
	ix::WebSocketSendInfo info = m_webSocket.send(messageAuth);

	gl_systemMessage.PushInnerSystemInformationMessage(messageAuth.c_str());
}

///////////////////////////////////////////////////////////////////////
///
/// ��ʽ��
/// {
///  "eventName" : "subscribe",
///  "authorization" : d897a00b7cfc2adffc630d23befd5316a4683156, // tiingo token
///  "eventData" :
///   {
///    "thresholdLevel" : 5,
///		 "tickers" : ["AAPL","MSFT", "RIG"]
///   }
/// }
///
///////////////////////////////////////////////////////////////////////
string CTiingoIEXWebSocket::CreateMessage(const vectorString& vSymbol) {
	vectorString vSymbols;
	json jsonMessage;
	jsonMessage["eventName"] = _T("subscribe");
	jsonMessage["authorization"] = gl_pTiingoDataSource->GetInquiryToken();
	jsonMessage["eventData"]["thresholdLevel"] = 5; // threshold����Ч����Ϊ0����5
	for (auto str : vSymbol) {
		ranges::transform(str, str.begin(), ::tolower); // Tiingo webSocketʹ��Сд�ַ�
		vSymbols.push_back(str);
	}
	jsonMessage["eventData"]["tickers"] = vSymbols;
	jsonMessage["eventData"]["tickers"].emplace_back(_T("rig"));// tiingoʹ�õ�stock������finnhub��ͬ
	jsonMessage["eventData"]["tickers"].emplace_back(_T("eurusd"));// tiingoʹ�õ�stock������finnhub��ͬ
	jsonMessage["eventData"]["tickers"].emplace_back(_T("spy"));// tiingoʹ�õ�stock������finnhub��ͬ
	jsonMessage["eventData"]["tickers"].emplace_back(_T("uso"));// tiingoʹ�õ�stock������finnhub��ͬ

	return jsonMessage.dump();
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// https://api.tiingo.com/documentation/websockets/iex
//
// �����ָ�ʽ��
// {"messageType":"I","data":{"subscriptionId":2563367},"response":{"code":200,"message":"Success"}}
// {"data":{"tickers":["*","uso","msft","tnk"],"thresholdLevel":"0"},"messageType":"I","response":{"code":200,"message":"Success"}}
// {"messageType":"H","response":{"code":200,"message":"HeartBeat"}}
// {"messageType":"A","service":"iex","data":["Q","2019-01-30T13:33:45.383129126-05:00",1548873225383129126,"vym",100,81.58,81.585,81.59,100,null,null,0,0,null,null,null]}
// {"messageType":"A","service":"iex","data":["T","2019-01-30T13:33:45.594808294-05:00",1548873225594808294,"wes",null,null,null,null,null,50.285,200,null,0,0,0,0]}
// {"messageType":"E","response":{"code":400,"message":"thresholdLevel not valid}}
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool CTiingoIEXWebSocket::ParseTiingoIEXWebSocketData(shared_ptr<string> pData) {
	string sSymbol;
	string strSymbol;
	string sTicker, sExchange, sValue;
	CTiingoIEXSocketPtr pIEXData = nullptr;

	try {
		if (json js; CreateJsonWithNlohmann(js, *pData)) {
			int i = 0;
			string sMessageType;
			char chType;
			string sService;
			string sType;
			json js2, js3, js4;
			json::iterator it;
			sType = jsonGetString(js, _T("messageType"));
			if (sType.empty()) return false;
			switch (sType.at(0)) {
			case 'A': // ��������
				pIEXData = make_shared<CTiingoIEXSocket>();
				sService = jsonGetString(js, _T("service"));
				if (sService != _T("iex")) return false; // �������Ϊ"iex"
				js2 = jsonGetChild(js, _T("data"));
				it = js2.begin();
				sMessageType = jsonGetString(it); // message type, 'Q'��'T'����'B'
				chType = sMessageType.at(0);
				pIEXData->m_chMessageType = chType;
				switch (chType) {
				case 'Q': // top-of-book update message
					pIEXData->m_sDateTime = jsonGetString(++it);
					pIEXData->m_iNanoseconds = jsonGetLongLong(++it);
					pIEXData->m_sSymbol = jsonGetString(++it);
					pIEXData->m_dBidSize = jsonGetDouble(++it);
					pIEXData->m_dBidPrice = jsonGetDouble(++it);
					pIEXData->m_dMidPrice = jsonGetDouble(++it);
					pIEXData->m_dAskPrice = jsonGetDouble(++it);
					pIEXData->m_dAskSize = jsonGetDouble(++it);
					pIEXData->m_dLastPrice = jsonGetDouble(++it);
					pIEXData->m_dLastSize = jsonGetDouble(++it);
					pIEXData->m_iAfterHour = jsonGetInt(++it);
					pIEXData->m_iISO = jsonGetInt(++it);
					pIEXData->m_iOddlot = jsonGetInt(++it);
					pIEXData->m_iNMSRule611 = jsonGetInt(++it);
					break;
				case 'T': // T last trade message
					pIEXData->m_sDateTime = jsonGetString(++it);
					pIEXData->m_iNanoseconds = jsonGetLongLong(++it);
					pIEXData->m_sSymbol = jsonGetString(++it);
					pIEXData->m_dBidSize = jsonGetDouble(++it);
					pIEXData->m_dBidPrice = jsonGetDouble(++it);
					pIEXData->m_dMidPrice = jsonGetDouble(++it);
					pIEXData->m_dAskPrice = jsonGetDouble(++it);
					pIEXData->m_dAskSize = jsonGetDouble(++it);
					pIEXData->m_dLastPrice = jsonGetDouble(++it);
					pIEXData->m_dLastSize = jsonGetInt(++it);
					pIEXData->m_iHalted = jsonGetInt(++it);
					pIEXData->m_iISO = jsonGetInt(++it);
					pIEXData->m_iOddlot = jsonGetInt(++it);
					pIEXData->m_iNMSRule611 = jsonGetInt(++it);
					break;
				case 'B': // 'B' trade break messages
					i++;
					break;
				default: // ����
					gl_systemMessage.PushInnerSystemInformationMessage(_T("Tiingo IEX WebSocket type error"));
					return false;
				}
				gl_SystemData.PushTiingoIEXSocket(pIEXData);
				m_HeartbeatTime = GetUTCTime();
				break;
			case 'I':  // �����֡�һ���Ǳ��浱ǰ��ѯ֤ȯ���룬��һ���Ǳ���ע����Ϣ
				js2 = jsonGetChild(js, _T("data"));
				try { // {"data":{"tickers":["*","uso","msft","tnk"],"thresholdLevel":"0"},"messageType":"I","response":{"code":200,"message":"Success"}}
					js3 = js2.at(_T("tickers"));
					for (auto it2 = js3.begin(); it2 != js3.end(); ++it2) {
						strSymbol = jsonGetString(it2);
						m_vCurrentInquireSymbol.emplace_back(strSymbol.c_str());
					}
				}
				catch (json::exception&) { // ע��ID {"messageType":"I","data":{"subscriptionId":2563367},"response":{"code":200,"message":"Success"}}
					ASSERT(GetSubscriptionId() == 0);
					SetSubscriptionId(jsonGetInt(&js2, _T("subscriptionId")));
				}
				break;
			case 'H': //Heart beat {\"messageType\":\"H\",\"response\":{\"code\":200,\"message\":\"HeartBeat\"}}
				js3 = jsonGetChild(js, _T("response"));
				m_iStatusCode = js3.at(_T("code"));
				m_statusMessage = js3.at(_T("message"));
				break;
			case 'E':  //error message {"messageType":"E","response":{"code":400,"message":"thresholdLevel not valid"}}
				js4 = jsonGetChild(js, _T("response"));
				m_iStatusCode = js4.at(_T("code"));
				m_statusMessage = js4.at(_T("message"));
				break;
			default:
				gl_systemMessage.PushInnerSystemInformationMessage(_T("Tiingo IEX WebSocket type error"));
				return false;
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
