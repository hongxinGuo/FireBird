#include "pch.h"

#include"JsonParse.h"
#include"JsonGetValue.h"

#include"TiingoWebInquiry.h"
#include "TiingoCryptoWebSocket.h"

#include<memory>
#include<thread>
using std::make_shared;
using std::thread;

void ProcessTiingoCryptoWebSocket(const ix::WebSocketMessagePtr& msg) {
	switch (msg->type) {
	case ix::WebSocketMessageType::Message:
		// ��ϵͳ�˳�ʱ��ֹͣ����WebSocket�Ĺ�����Ҫʱ�䣬�ڴ��ڼ�˻ص���������ִ�У����洢���Ѿ������ˣ����³����ڴ�й©��
		// �ʶ���Ҫ�ж��Ƿ�ϵͳ�����˳���ֻ����û���˳�ϵͳʱ���ɴ洢���յ������ݣ���
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

UINT ThreadConnectTiingoCryptoWebSocketAndSendMessage(not_null<CTiingoCryptoWebSocket*> pDataTiingoCryptoWebSocket, vectorString vSymbol) {
	if (static bool s_fConnecting = false; !s_fConnecting) {
		s_fConnecting = true;
		if (pDataTiingoCryptoWebSocket->ConnectWebSocketAndSendMessage(vSymbol)) {
			gl_systemMessage.PushInnerSystemInformationMessage(_T("����Tiingo Crypto web socket����"));
		}
		s_fConnecting = false;
	}

	return 73;
}

CTiingoCryptoWebSocket::CTiingoCryptoWebSocket() : CVirtualWebSocket() { m_url = _T("wss://api.tiingo.com/crypto"); }

/// <summary>
/// Tiingo Crypto������Դ��ʽ��wss://api.tiingo.com/crypto������Կ������͵ġ�
/// </summary>
/// <param name=""></param>
/// <returns></returns>
bool CTiingoCryptoWebSocket::Connect(void) {
	Connecting(m_url, ProcessTiingoCryptoWebSocket);

	return true;
}

//////////////////////////////////////////////////////////////////////////////////////////////
//
// �������е�Crypto����ʱ��ÿ��������������50-100K������
//
// thresholdlevel 2: Top-of-Book AND Last Trade updates.
// thresholdlevel 5: only Last Trade updates.
//
//////////////////////////////////////////////////////////////////////////////////////////////
bool CTiingoCryptoWebSocket::Send(vectorString vSymbol) {
	ASSERT(IsOpen());

	const string messageAuth(CreateMessage(vSymbol));

	ASSERT(IsOpen());

	ix::WebSocketSendInfo info = SendMessage(messageAuth);
	gl_systemMessage.PushInnerSystemInformationMessage(messageAuth.c_str());

	return true;
}

string CTiingoCryptoWebSocket::CreateMessage(vectorString vSymbol) {
	const string strPrefix = _T("{\"eventName\":\"subscribe\",\"authorization\":\"");
	const string strMiddle = _T("\",\"eventData\":{\"thresholdLevel\":2,\"tickers\":["); // 5��Trade Updates per-exchange.2��Top-of-Book quote updates as well as Trade updates. Both quote and trade updates are per-exchange
	const string strSuffix = _T("]}}"); // 5��Trade Updates per-exchange.2��Top-of-Book quote updates as well as Trade updates. Both quote and trade updates are per-exchange
	string strAuth = gl_pTiingoWebInquiry->GetInquiryToken().GetBuffer();

	strAuth.erase(strAuth.begin(), strAuth.begin() + 7);
	vSymbol.push_back(_T("dkaeth")); // ���һ��Tiingo��ʽ�Ĵ��롣����Ŀǰ��ѡcryptoʹ�õ���finnhub��ʽ�Ĵ����ʽ����Ϊ��Ч���롣
	vSymbol.push_back(_T("ksmust")); // ���һ��Tiingo��ʽ�Ĵ��롣����Ŀǰ��ѡcryptoʹ�õ���finnhub��ʽ�Ĵ����ʽ����Ϊ��Ч���롣
	const string strSymbols = CreateTiingoWebSocketSymbolString(vSymbol);

	string str = strPrefix + strAuth + strMiddle + strSymbols + strSuffix;

	return str;
}

bool CTiingoCryptoWebSocket::CreateThreadConnectWebSocketAndSendMessage(vectorString vSymbol) {
	thread thread1(ThreadConnectTiingoCryptoWebSocketAndSendMessage, this, vSymbol);
	thread1.detach();

	return true;
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
//
// </summary>
// <param name="pData"></param>
// <returns></returns>
bool CTiingoCryptoWebSocket::ParseTiingoCryptoWebSocketData(shared_ptr<string> pData) {
	string sSymbol;
	string strSymbol;
	string sTickers, sExchange;

	try {
		if (json js; NlohmannCreateJson(&js, *pData)) {
			CTiingoCryptoSocketPtr pCryptoData;
			string sService;
			string sDatetime;
			string sMessageType;
			char chType;
			string sType;
			json js2;
			json::iterator it;
			sType = jsonGetString(&js, _T("messageType"));
			chType = sType.at(0);
			switch (chType) {
			case 'I': // ע�� {\"messageType\":\"I\",\"response\":{\"code\":200,\"message\":\"Success\"},\"data\":{\"subscriptionId\":2563396}}
				// ����  {"data":{"tickers":["*","uso","msft","tnk"],"thresholdLevel":"0"},"messageType":"I","response":{"code":200,"message":"Success"}}
				js2 = jsonGetChild(&js, _T("data"));
				try {
					json js3 = js2.at(_T("tickers"));
					for (auto it2 = js3.begin(); it2 != js3.end(); ++it2) { // �Ǵ��룺"data":{"tickers":["*","uso","msft","tnk"]
						strSymbol = jsonGetString(it2);
						m_vCurrentSymbol.emplace_back(strSymbol.c_str());
					}
				}
				catch (json::exception&) { // ��ע����Ϣ��"data":{"subscriptionId":2563396}
					ASSERT(GetSubscriptionId() == 0);
					SetSubscriptionId(jsonGetInt(&js2, _T("subscriptionId")));
				}
				break;
			case 'H': // heart beat {\"messageType\":\"H\",\"response\":{\"code\":200,\"message\":\"HeartBeat\"}}
				// do nothing
				break;
			case 'A': // new data
				pCryptoData = make_shared<CTiingoCryptoSocket>();
				sService = jsonGetString(&js, _T("service"));
				if (sService != _T("crypto_data")) return false; // ��ʽ�������˳�
				js2 = jsonGetChild(&js, _T("data"));
				it = js2.begin();
				sMessageType = jsonGetString(it); // ��Q�����ߡ�T��
				if (sMessageType.at(0) == 'T') {
					//last trade message {\"service\":\"crypto_data\",\"data\":[\"T\",\"jstusdt\",\"2021-08-10T23:56:55.237000+00:00\",\"huobi\",3952.5,0.062108],\"messageType\":\"A\"}
					pCryptoData->m_chMessageType = 'T';
					++it;
					pCryptoData->m_sSymbol = jsonGetString(it); // ֤ȯ����
					++it;
					sDatetime = jsonGetString(it); // ʱ�䴮��"2019-07-05T15:49:15.157000+00:00"
					++it;
					sExchange = jsonGetString(it); // ������
					pCryptoData->m_strExchange = sExchange.c_str();
					++it;
					pCryptoData->m_dLastSize = jsonGetDouble(it); // ��������
					++it;
					pCryptoData->m_dLastPrice = jsonGetDouble(it); // ���¼۸�
				}
				else if (sMessageType.at(0) == 'Q') {
					// 'Q' top-of-book update message.
					pCryptoData->m_chMessageType = 'Q';
					++it;
					pCryptoData->m_sSymbol = jsonGetString(it); // ֤ȯ����
					++it;
					sDatetime = jsonGetString(it); // ʱ�䴮��"2019-07-05T15:49:15.157000+00:00"
					++it;
					sExchange = jsonGetString(it);// ������
					pCryptoData->m_strExchange = sExchange.c_str();
					++it;
					pCryptoData->m_dBidSize = jsonGetDouble(it); // �������
					++it;
					pCryptoData->m_dBidPrice = jsonGetDouble(it); // ���
					++it;
					pCryptoData->m_dMidPrice = jsonGetDouble(it); // �м�� ��BidPrice + AskPrice)/2
					++it;
					pCryptoData->m_dAskSize = jsonGetDouble(it); // ��������
					++it;
					pCryptoData->m_dAskPrice = jsonGetDouble(it); // ����
				}
				else {
					// ��ʽ����
					return false;
				}
				gl_SystemData.PushTiingoCryptoSocket(pCryptoData);
				m_fReceivingData = true;
				break;
			default: // ����
				return false;
				break;
			}
		}
		else { return false; }
	}
	catch (json::exception& e) {
		ReportJSonErrorToSystemMessage(_T("Tiingo Crypto WebSocket "), e.what());
		return false;
	}

	return true;
}