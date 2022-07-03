#include "pch.h"

#include"accessory.h"
#include"JsonParse.h"
#include"ThreadStatus.h"

#include"WebInquirer.h"
#include "TiingoForexWebSocket.h"

void ProcessTiingoForexWebSocket(const ix::WebSocketMessagePtr& msg) {
	switch (msg->type) {
	case ix::WebSocketMessageType::Message:
		// ��ϵͳ�˳�ʱ��ֹͣ����WebSocket�Ĺ�����Ҫʱ�䣬�ڴ��ڼ�˻ص���������ִ�У����洢���Ѿ������ˣ����³����ڴ�й©��
		// �ʶ���Ҫ�ж��Ƿ�ϵͳ�����˳���ֻ����û���˳�ϵͳʱ���ɴ洢���յ������ݣ���
		if (!gl_systemStatus.IsExitingSystem()) {
			gl_WebInquirer.PushTiingoForexWebSocketData(msg->str);
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

UINT ThreadConnectTiingoForexWebSocketAndSendMessage(not_null<CTiingoForexWebSocket*> pDataTiingoForexWebSocket, vector<CString> vSymbol) {
	static bool s_fConnecting = false;
	if (!s_fConnecting) {
		s_fConnecting = true;
		gl_ThreadStatus.IncreaseSavingThread();
		pDataTiingoForexWebSocket->ConnectWebSocketAndSendMessage(vSymbol);
		gl_ThreadStatus.DecreaseSavingThread();
		s_fConnecting = false;
	}

	return 73;
}

CTiingoForexWebSocket::CTiingoForexWebSocket() : CVirtualWebSocket() {
	m_url = _T("wss://api.tiingo.com/fx");
}

CTiingoForexWebSocket::~CTiingoForexWebSocket(void) {
}

/// <summary>
/// Tiingo Forex������Դ��ʽ��wss://api.tiingo.com/fx������Կ������͵ġ�
/// </summary>
/// <param name=""></param>
/// <returns></returns>
bool CTiingoForexWebSocket::Connect(void) {
	Connecting(m_url, ProcessTiingoForexWebSocket);
	return true;
}

//////////////////////////////////////////////////////////////////////////////////////////////////
//
// 5״̬��ÿ�����100K���ҡ�
//
// threshlodlevel 5: ALL Top-of-Book updates
// thresholdlevel 7: A top-of-book update that is due to a change in either the bid/ask price or size.
//
//////////////////////////////////////////////////////////////////////////////////////////////////
bool CTiingoForexWebSocket::Send(vector<CString> vSymbol) {
	ASSERT(IsOpen());

	string messageAuth(CreateMessage(vSymbol));
	ix::WebSocketSendInfo info = Sending(messageAuth);
	gl_systemMessage.PushInnerSystemInformationMessage(messageAuth.c_str());

	return true;
}

CString CTiingoForexWebSocket::CreateMessage(vector<CString> vSymbol) {
	CString str;
	CString strPreffix = _T("{\"eventName\":\"subscribe\",\"authorization\":\"");
	CString strMiddle = _T("\",\"eventData\":{\"thresholdLevel\":5,\"tickers\":["); //7��A top - of - book update that is due to a change in either the bid / ask price or size.
	CString strSuffix = _T("]}}"); //7��A top - of - book update that is due to a change in either the bid / ask price or size.
	CString strAuth = gl_pTiingoWebInquiry->GetInquiringStringSuffix();
	strAuth = strAuth.Right(strAuth.GetLength() - 7);

	vSymbol.push_back(_T("gbpaud")); // ���һ��Tiingo��ʽ�Ĵ��롣����Ŀǰ��ѡcryptoʹ�õ���finnhub��ʽ�Ĵ����ʽ����Ϊ��Ч���롣
	vSymbol.push_back(_T("eurusd")); // ���һ��Tiingo��ʽ�Ĵ��롣����Ŀǰ��ѡcryptoʹ�õ���finnhub��ʽ�Ĵ����ʽ����Ϊ��Ч���롣
	CString strSymbols = CreateTiingoWebSocketSymbolString(vSymbol);

	str = strPreffix + strAuth + strMiddle + strSymbols + strSuffix;

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
// �����ָ�ʽ��
// {"messageType":"I","response":{"code":200,"message":"Success"},"data":{"subscriptionId":2563396}}
// {"data":{"tickers":["*","uso","msft","tnk"],"thresholdLevel":"0"},"messageType":"I","response":{"code":200,"message":"Success"}}
// {"messageType":"H","response":{"code":200,"message":"HeartBeat"}}
// {"messageType":"A","service":"fx","data":["Q","eurnok","2019-07-05T15:49:15.157000+00:00",5000000.0,9.6764,9.678135,5000000.0,9.67987]}
// {"messageType":"A","service":"fx","data":["Q","gbpaud","2019-07-05T15:49:15.236000+00:00",1000000.0,1.79457,1.79477,5000000.0,1.79497]}
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool CTiingoForexWebSocket::ParseTiingoForexWebSocketData(shared_ptr<string> pData) {
	ptree pt, pt2, pt3, pt4;
	ptree::iterator it;
	string sType, sSymbol, sService;
	char chType;
	string strSymbol;

	string sMessageType, sTickers, sDatetime;
	CTiingoForexSocketPtr pForexData = nullptr;

	try {
		if (ParseWithPTree(pt, *pData)) {
			sType = ptreeGetString(pt, _T("messageType"));
			chType = sType.at(0);
			switch (chType) {
			case 'I': // ע�� {\"messageType\":\"I\",\"response\":{\"code\":200,\"message\":\"Success\"},\"data\":{\"subscriptionId\":2563396}}
				pt2 = pt.get_child(_T("data"));
				try {
					pt3 = pt2.get_child(_T("tickers"));
					for (ptree::iterator it3 = pt3.begin(); it3 != pt3.end(); it3++) {
						pt4 = it3->second;
						strSymbol = pt4.get_value<string>();
						m_vCurrentSymbol.push_back(strSymbol.c_str());
					}
				}
				catch (exception e) {
					ASSERT(GetSubscriptionId() == 0);
					SetSubscriptionId(ptreeGetInt(pt2, _T("subscriptionId")));
				}
				break;
			case 'H': // HeartBeat {"messageType":"H","response":{"code":200,"message":"HeartBeat"}}
				// do nothing
				break;
			case 'A': // new data
				sService = ptreeGetString(pt, _T("service"));
				if (sService.compare(_T("fx")) != 0) return false; // ֻ�д���
				pForexData = make_shared<CTiingoForexSocket>();
				pt2 = pt.get_child(_T("data"));
				it = pt2.begin();
				pt3 = it->second;
				sMessageType = pt3.get_value<string>(); // �����ǡ�Q��
				pForexData->m_chMessageType = sMessageType.at(0);
				it++;
				pt3 = it->second;
				sTickers = pt3.get_value<string>(); // ֤ȯ����
				pForexData->m_strSymbol = sTickers.c_str();
				it++;
				pt3 = it->second;
				sDatetime = pt3.get_value<string>(); // ʱ�䴮��"2019-07-05T15:49:15.157000+00:00"
				it++;
				pt3 = it->second;
				pForexData->m_dBidSize = pt3.get_value<double>(); // �������
				it++;
				pt3 = it->second;
				pForexData->m_dBidPrice = pt3.get_value<double>(); // ���
				it++;
				pt3 = it->second;
				pForexData->m_dMidPrice = pt3.get_value<double>(); // �м�� ��BidPrice + AskPrice)/2
				it++;
				pt3 = it->second;
				pForexData->m_dAskSize = pt3.get_value<double>(); // ��������
				it++;
				pt3 = it->second;
				pForexData->m_dAskPrice = pt3.get_value<double>(); // ����
				gl_SystemData.PushTiingoForexSocket(pForexData);
				m_fReveivingData = true;
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