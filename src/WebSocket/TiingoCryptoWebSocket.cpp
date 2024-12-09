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
		// ��ϵͳ�˳�ʱ��ֹͣ����WebSocket�Ĺ�����Ҫʱ�䣬�ڴ��ڼ�˻ص���������ִ�У����洢���Ѿ������ˣ����³����ڴ�й©��
		// �ʶ���Ҫ�ж��Ƿ�ϵͳ�����˳���ֻ����û���˳�ϵͳʱ���ɴ洢���յ������ݣ���
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
/// Tiingo Crypto������Դ��ʽ��wss://api.tiingo.com/crypto������Կ������͵ġ�
/// </summary>
/// 
void CTiingoCryptoWebSocket::Connect() {
	Connecting(m_url, ProcessTiingoCryptoWebSocket);
}

//////////////////////////////////////////////////////////////////////////////////////////////
//
// �������е�Crypto����ʱ��ÿ��������������50-100K������
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
	if (IsConnecting()) return; // ����������ӣ��򲻼�ظ�socket
	CVirtualWebSocket::MonitorWebSocket(gl_pTiingoDataSource->IsWebError(), gl_systemConfiguration.IsUsingTiingoCryptoWebSocket(), vSymbol);
}

///////////////////////////////////////////////////////////////////////
///
/// ��ʽ��
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
	message["eventData"]["thresholdLevel"] = 2; // thresholdLevel����Ч����Ϊ2����5
	for (auto str : vSymbol) {
		std::ranges::transform(str, str.begin(), ::tolower); // Tiingo webSocketʹ��Сд�ַ�
		vSymbol2.push_back(str);
	}
	message["eventData"]["tickers"] = vSymbol2;
	message["eventData"]["tickers"].emplace_back("dkaeth"); // tiingoʹ�õ�crypto������finnhub��ȫ��ͬ��
	message["eventData"]["tickers"].emplace_back("eurusd");// tiingoʹ�õ�crypto������finnhub��ȫ��ͬ��
	message["eventData"]["tickers"].emplace_back("ksmust");// tiingoʹ�õ�crypto������finnhub��ȫ��ͬ��

	return message.dump();
}

// <summary>
//
// https://api.tiingo.com/documentation/websockets/crypto
//
// �����ָ�ʽ��
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
			case 'I':  // �����֡�һ���Ǳ��浱ǰ��ѯ֤ȯ���룬��һ���Ǳ���ע����Ϣ
				js2 = jsonGetChild(js, _T("data"));
				try { //{"data":{"tickers":["*","uso","msft","tnk"],"thresholdLevel":"0"},"messageType":"I","response":{"code":200,"message":"Success"}}
					js3 = js2.at(_T("tickers"));
					for (auto it2 = js3.begin(); it2 != js3.end(); ++it2) { // �Ǵ��룺"data":{"tickers":["*","uso","msft","tnk"]
						strSymbol = jsonGetString(it2);
						m_vCurrentInquireSymbol.emplace_back(strSymbol.c_str());
					}
				} catch (nlohmann::ordered_json::exception&) { //ע����Ϣ��{"messageType":"I","response":{"code":200,"message":"Success"},"data":{"subscriptionId":2563396}}
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
				if (sService != _T("crypto_data")) return false; // ��ʽ�������˳�
				js2 = jsonGetChild(js, _T("data"));
				it = js2.begin();
				sMessageType = jsonGetString(it); // ��Q�����ߡ�T��
				if (sMessageType.at(0) == 'T') {	//last trade message {\"service\":\"crypto_data\",\"data\":[\"T\",\"jstusdt\",\"2021-08-10T23:56:55.237000+00:00\",\"huobi\",3952.5,0.062108],\"messageType\":\"A\"}
					pCryptoData->m_chMessageType = 'T';
					pCryptoData->m_sSymbol = jsonGetString(++it); // ֤ȯ����
					pCryptoData->m_sDateTime = jsonGetString(++it); // ʱ�䴮��"2019-07-05T15:49:15.157000+00:00"
					ss.str(pCryptoData->m_sDateTime);
					chrono::from_stream(ss, "%FT%H:%M:%9S%Ez", tpTime, &sString, &Minutes);
					pCryptoData->m_tpTime = chrono::time_point_cast<chrono::seconds>(tpTime);
					pCryptoData->m_strExchange = jsonGetString(++it); // ������
					pCryptoData->m_dLastSize = jsonGetDouble(++it); // ��������
					pCryptoData->m_dLastPrice = jsonGetDouble(++it); // ���¼۸�
				}
				else if (sMessageType.at(0) == 'Q') {		// 'Q' top-of-book update message.
					pCryptoData->m_chMessageType = 'Q';
					pCryptoData->m_sSymbol = jsonGetString(++it); // ֤ȯ����
					pCryptoData->m_sDateTime = jsonGetString(++it); // ʱ�䴮��"2019-07-05T15:49:15.157000+00:00"
					ss.str(pCryptoData->m_sDateTime);
					chrono::from_stream(ss, "%FT%H:%M:%9S%Ez", tpTime, &sString, &Minutes);
					pCryptoData->m_tpTime = chrono::time_point_cast<chrono::seconds>(tpTime);
					pCryptoData->m_strExchange = jsonGetString(++it);// ������
					pCryptoData->m_dBidSize = jsonGetDouble(++it); // �������
					pCryptoData->m_dBidPrice = jsonGetDouble(++it); // ���
					pCryptoData->m_dMidPrice = jsonGetDouble(++it); // �м�� ��BidPrice + AskPrice)/2
					pCryptoData->m_dAskSize = jsonGetDouble(++it); // ��������
					pCryptoData->m_dAskPrice = jsonGetDouble(++it); // ����
				}
				else {// ��ʽ����
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
			default: // ����
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
