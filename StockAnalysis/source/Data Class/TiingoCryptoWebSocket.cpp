#include "pch.h"

#include"WebInquirer.h"
#include "TiingoCryptoWebSocket.h"

void FunctionProcessTiingoCryptoWebSocket(const ix::WebSocketMessagePtr& msg) {
	switch (msg->type) {
	case ix::WebSocketMessageType::Message:
		// ��ϵͳ�˳�ʱ��ֹͣ����WebSocket�Ĺ�����Ҫʱ�䣬�ڴ��ڼ�˻ص���������ִ�У����洢���Ѿ������ˣ����³����ڴ�й©��
		// �ʶ���Ҫ�ж��Ƿ�ϵͳ�����˳���ֻ����û���˳�ϵͳʱ���ɴ洢���յ������ݣ���
		if (!gl_fExitingSystem) {
			gl_WebInquirer.PushTiingoCryptoWebSocketData(msg->str);
			//gl_systemMessage.PushInnerSystemInformationMessage(msg->str.c_str());
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

UINT ThreadConnectingTiingoCryptoWebSocketAndSendMessage(not_null<CTiingoCryptoWebSocket*> pDataTiingoCryptoWebSocket, vector<CString> vSymbol) {
	gl_ThreadStatus.IncreaseSavingThread();
	pDataTiingoCryptoWebSocket->ConnectingWebSocketAndSendMessage(vSymbol);
	gl_ThreadStatus.DecreaseSavingThread();

	return 73;
}

CTiingoCryptoWebSocket::CTiingoCryptoWebSocket() : CVirtualWebSocket() {
}

CTiingoCryptoWebSocket::~CTiingoCryptoWebSocket(void) {
}

/// <summary>
/// Tiingo Crypto������Դ��ʽ��wss://api.tiingo.com/crypto������Կ������͵ġ�
/// </summary>
/// <param name=""></param>
/// <returns></returns>
bool CTiingoCryptoWebSocket::Connect(void) {
	std::string url("wss://api.tiingo.com/crypto");

	Connecting(url, FunctionProcessTiingoCryptoWebSocket);

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
bool CTiingoCryptoWebSocket::Send(vector<CString> vSymbol) {
	static bool sm_fSendAuth = true;
	CString str;
	CString strPreffix = _T("{\"eventName\":\"subscribe\",\"authorization\":\"");
	CString strMiddle = _T("\",\"eventData\":{\"thresholdLevel\":2,\"tickers\":["); // 5��Trade Updates per-exchange.2��Top-of-Book quote updates as well as Trade updates. Both quote and trade updates are per-exchange
	CString strSuffix = _T("]}}"); // 5��Trade Updates per-exchange.2��Top-of-Book quote updates as well as Trade updates. Both quote and trade updates are per-exchange
	CString strAuth = gl_pTiingoWebInquiry->GetInquiringStringSuffix();

	strAuth = strAuth.Right(strAuth.GetLength() - 7);
	vSymbol.push_back(_T("dkaeth")); // ���һ��Tiingo��ʽ�Ĵ��롣����Ŀǰ��ѡcryptoʹ�õ���finnhub��ʽ�Ĵ����ʽ����Ϊ��Ч���롣
	vSymbol.push_back(_T("ksmust")); // ���һ��Tiingo��ʽ�Ĵ��롣����Ŀǰ��ѡcryptoʹ�õ���finnhub��ʽ�Ĵ����ʽ����Ϊ��Ч���롣
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

bool CTiingoCryptoWebSocket::CreatingThreadConnectingWebSocketAndSendMessage(vector<CString> vSymbol) {
	thread thread1(ThreadConnectingTiingoCryptoWebSocketAndSendMessage, this, vSymbol);
	thread1.detach();

	return true;
}

/// <summary>
///
/// https://api.tiingo.com/documentation/websockets/crypto
///
/// �����ָ�ʽ��
/// {"messageType":"I","response":{"code":200,"message":"Success"},"data":{"subscriptionId":2563396}}
/// {"messageType":"H","response":{"code":200,"message":"HeartBeat"}}
/// {"messageType":"A","service":"crypto_data","data":["Q","neojpy","2019-01-30T18:03:40.195515+00:00","bitfinex",38.11162867,787.82,787.83,42.4153887,787.84]}
/// {"messageType":"A","service":"crypto_data","data":["T","evxbtc","2019-01-30T18:03:40.056000+00:00","binance",405.0,9.631e-05]}
///
/// </summary>
/// <param name="pData"></param>
/// <returns></returns>
bool CTiingoCryptoWebSocket::ParseTiingoCryptoWebSocketData(shared_ptr<string> pData) {
	ptree::iterator it;
	ptree pt, pt2, pt3;
	string sType, sSymbol;
	char chType;

	string sMessageType, sTickers, sExchange, sDatetime, sService;
	CTiingoCryptoSocketPtr pCryptoData = nullptr;

	try {
		if (ConvertToJSON(pt, *pData)) {
			sType = pt.get<string>(_T("messageType"));
			chType = sType.at(0);
			switch (chType) {
			case 'I': // ע�� {\"messageType\":\"I\",\"response\":{\"code\":200,\"message\":\"Success\"},\"data\":{\"subscriptionId\":2563396}}
				pt2 = pt.get_child(_T("data"));
				ASSERT(GetSubscriptionId() == 0);
				SetSubscriptionId(pt2.get<int>(_T("subscriptionId")));
				break;
			case 'H': // heart beat {\"messageType\":\"H\",\"response\":{\"code\":200,\"message\":\"HeartBeat\"}}
				// do nothing
				break;
			case 'A': // new data
				pCryptoData = make_shared<CTiingoCryptoSocket>();
				sService = pt.get<string>(_T("service"));
				if (sService.compare(_T("crypto_data")) != 0) return false; // ��ʽ�������˳�
				pt2 = pt.get_child(_T("data"));
				it = pt2.begin();
				pt3 = it->second;
				sMessageType = pt3.get_value<string>(); // ��Q�����ߡ�T��
				if (sMessageType.at(0) == 'T') { //last trade message {\"service\":\"crypto_data\",\"data\":[\"T\",\"jstusdt\",\"2021-08-10T23:56:55.237000+00:00\",\"huobi\",3952.5,0.062108],\"messageType\":\"A\"}
					pCryptoData->m_chMessageType = 'T';
					it++;
					pt3 = it->second;
					sTickers = pt3.get_value<string>(); // ֤ȯ����
					pCryptoData->m_strSymbol = sTickers.c_str();
					it++;
					pt3 = it->second;
					sDatetime = pt3.get_value<string>(); // ʱ�䴮��"2019-07-05T15:49:15.157000+00:00"
					it++;
					pt3 = it->second;
					sExchange = pt3.get_value<string>(); // ������
					pCryptoData->m_strExchange = sExchange.c_str();
					it++;
					pt3 = it->second;
					pCryptoData->m_dLastSize = pt3.get_value<double>(); // ��������
					it++;
					pt3 = it->second;
					pCryptoData->m_dLastPrice = pt3.get_value<double>(); // ���¼۸�
				}
				else if (sMessageType.at(0) == 'Q') { // 'Q' top-of-book update message.
					pCryptoData->m_chMessageType = 'Q';
					it++;
					pt3 = it->second;
					sTickers = pt3.get_value<string>(); // ֤ȯ����
					pCryptoData->m_strSymbol = sTickers.c_str();
					it++;
					pt3 = it->second;
					sDatetime = pt3.get_value<string>(); // ʱ�䴮��"2019-07-05T15:49:15.157000+00:00"
					it++;
					pt3 = it->second;
					sExchange = pt3.get_value<string>(); // ������
					pCryptoData->m_strExchange = sExchange.c_str();
					it++;
					pt3 = it->second;
					pCryptoData->m_dBidSize = pt3.get_value<double>(); // �������
					it++;
					pt3 = it->second;
					pCryptoData->m_dBidPrice = pt3.get_value<double>(); // ���
					it++;
					pt3 = it->second;
					pCryptoData->m_dMidPrice = pt3.get_value<double>(); // �м�� ��BidPrice + AskPrice)/2
					it++;
					pt3 = it->second;
					pCryptoData->m_dAskSize = pt3.get_value<double>(); // ��������
					it++;
					pt3 = it->second;
					pCryptoData->m_dAskPrice = pt3.get_value<double>(); // ����
				}
				else { // ��ʽ����
					return false;
				}
				gl_SystemData.PushTiingoCryptoSocket(pCryptoData);
				break;
			default: // ����
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