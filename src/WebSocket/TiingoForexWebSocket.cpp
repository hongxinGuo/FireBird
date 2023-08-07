#include "pch.h"

#include"JsonParse.h"
#include"JsonGetValue.h"
#include"TimeConvert.h"

#include "TiingoForexWebSocket.h"

#include "TiingoDataSource.h"

using namespace std;

void ProcessTiingoForexWebSocket(const ix::WebSocketMessagePtr& msg) {
	switch (msg->type) {
	case ix::WebSocketMessageType::Message:
		// ��ϵͳ�˳�ʱ��ֹͣ����WebSocket�Ĺ�����Ҫʱ�䣬�ڴ��ڼ�˻ص���������ִ�У����洢���Ѿ������ˣ����³����ڴ�й©��
		// �ʶ���Ҫ�ж��Ƿ�ϵͳ�����˳���ֻ����û���˳�ϵͳʱ���ɴ洢���յ������ݣ���
		if (!gl_systemConfiguration.IsExitingSystem()) { gl_tiingoForexWebSocket.PushData(msg->str); }
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

UINT ThreadConnectTiingoForexWebSocketAndSendMessage(not_null<CTiingoForexWebSocket*> pDataTiingoForexWebSocket, const vectorString& vSymbol) {
	static bool s_fConnecting = false;
	if (!s_fConnecting) {
		s_fConnecting = true;
		if (pDataTiingoForexWebSocket->ConnectWebSocketAndSendMessage(vSymbol)) {
			gl_systemMessage.PushInnerSystemInformationMessage(_T("����Tiingo Forex web socket����"));
		}
		s_fConnecting = false;
	}

	return 73;
}

CTiingoForexWebSocket::CTiingoForexWebSocket() {
	ASSERT(gl_systemConfiguration.IsInitialized());
	m_url = _T("wss://api.tiingo.com/fx");
}

CTiingoForexWebSocket::~CTiingoForexWebSocket() {}

/// <summary>
/// Tiingo Forex������Դ��ʽ��wss://api.tiingo.com/fx������Կ������͵ġ�
/// </summary>
/// <returns></returns>
void CTiingoForexWebSocket::Connect() {
	Connecting(m_url, ProcessTiingoForexWebSocket);
}

//////////////////////////////////////////////////////////////////////////////////////////////////
//
// 5״̬��ÿ�����100K���ҡ�
//
// ��Ч��thresholdLevelΪ��2�� 5�� 7
// thresholdLevel 5: ALL Top-of-Book updates
// thresholdLevel 7: A top-of-book update that is due to a change in either the bid/ask price or size.
// {"messageType":"E","response":{"code":400,"message":"thresholdLevel not valid}}
//
//////////////////////////////////////////////////////////////////////////////////////////////////
void CTiingoForexWebSocket::Send(const vectorString& vSymbol) {
	ASSERT(IsOpen());

	const string messageAuth(CreateMessage(vSymbol));
	ix::WebSocketSendInfo info = SendString(messageAuth);
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
///		 "tickers" : ["gbpaud","eurusd"]
///   }
/// }
///
///////////////////////////////////////////////////////////////////////
string CTiingoForexWebSocket::CreateMessage(const vectorString& vSymbol) {
	vectorString vSymbols;
	json jsonMessage;
	jsonMessage["eventName"] = _T("subscribe");
	jsonMessage["authorization"] = gl_pTiingoDataSource->GetInquiryToken();
	jsonMessage["eventData"]["thresholdLevel"] = 5; // //7��A top - of - book update that is due to a change in either the bid / ask price or size.
	for (auto str : vSymbol) {
		ranges::transform(str, str.begin(), ::tolower); // Tiingo webSocketʹ��Сд�ַ�
		vSymbols.push_back(str);
	}
	jsonMessage["eventData"]["tickers"] = vSymbols;
	jsonMessage["eventData"]["tickers"].emplace_back(_T("gbpaud"));// ���һ��Tiingo��ʽ�Ĵ��롣����Ŀǰ��ѡcryptoʹ�õ���finnhub��ʽ�Ĵ����ʽ����Ϊ��Ч���롣
	jsonMessage["eventData"]["tickers"].emplace_back(_T("eurusd"));// ���һ��Tiingo��ʽ�Ĵ��롣����Ŀǰ��ѡcryptoʹ�õ���finnhub��ʽ�Ĵ����ʽ����Ϊ��Ч���롣

	return jsonMessage.dump();
}

void CTiingoForexWebSocket::CreateThreadConnectWebSocketAndSendMessage(vectorString vSymbol) {
	thread thread1(ThreadConnectTiingoForexWebSocketAndSendMessage, this, vSymbol);
	thread1.detach();
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
//
// https://api.tiingo.com/documentation/websockets/forex
//
// �����ָ�ʽ��
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
			sType = jsonGetString(&js, _T("messageType"));
			chType = sType.at(0);
			switch (chType) {
			case 'I': // ע�� {\"messageType\":\"I\",\"response\":{\"code\":200,\"message\":\"Success\"},\"data\":{\"subscriptionId\":2563396}}
				js2 = jsonGetChild(&js, _T("data"));
				try {
					js3 = js2.at(_T("tickers"));
					for (auto it3 = js3.begin(); it3 != js3.end(); ++it3) {
						strSymbol = jsonGetString(it3);
						m_vCurrentSymbol.push_back(strSymbol.c_str());
					}
				}
				catch (json::exception&) {
					ASSERT(GetSubscriptionId() == 0);
					SetSubscriptionId(jsonGetInt(&js2, _T("subscriptionId")));
				}
				break;
			case 'H': // HeartBeat {"messageType":"H","response":{"code":200,"message":"HeartBeat"}}
				m_HeartbeatTime = GetUTCTime();
				js3 = jsonGetChild(&js, _T("response"));
				m_iStatusCode = js3.at(_T("code"));
				m_statusMessage = js3.at(_T("message"));
				break;
			case 'E':  //error message {"messageType":"E","response":{"code":400,"message":"thresholdLevel not valid}}
				js4 = jsonGetChild(&js, _T("response"));
				m_iStatusCode = js4.at(_T("code"));
				m_statusMessage = js4.at(_T("message"));
				break;
			case 'A': // new data
				sService = jsonGetString(&js, _T("service"));
				if (sService != _T("fx")) return false; // ֻ�д���
				pForexData = make_shared<CTiingoForexSocket>();
				js2 = jsonGetChild(&js, _T("data"));
				it = js2.begin();
				sMessageType = it->get<string>(); // �����ǡ�Q��
				pForexData->m_chMessageType = sMessageType.at(0);
				pForexData->m_sSymbol = jsonGetString(++it); // ֤ȯ����
				sDatetime = jsonGetString(++it); // ʱ�䴮��"2019-07-05T15:49:15.157000+00:00"
				pForexData->m_tTime = XferToTTime(sDatetime.c_str(), _T("%4d-%02d-%02dT%02d:%02d:%02d.%06d+%02d:%02d"));
				pForexData->m_dBidSize = jsonGetDouble(++it); // �������
				pForexData->m_dBidPrice = jsonGetDouble(++it); // ���
				pForexData->m_dMidPrice = jsonGetDouble(++it); // �м�� ��BidPrice + AskPrice)/2
				pForexData->m_dAskSize = jsonGetDouble(++it); // ��������
				pForexData->m_dAskPrice = jsonGetDouble(++it); // ����
				gl_SystemData.PushTiingoForexSocket(pForexData);
				m_fReceivingData = true;
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
	}
	catch (json::exception& e) {
		ReportJSonErrorToSystemMessage(_T("Tiingo Forex WebSocket "), e.what());
		return false;
	}

	return true;
}
