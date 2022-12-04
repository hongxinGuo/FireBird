#include "pch.h"

#include"JsonParse.h"
#include"ThreadStatus.h"

#include"WebInquirer.h"
#include "TiingoIEXWebSocket.h"

void ProcessTiingoIEXWebSocket(const ix::WebSocketMessagePtr& msg) {
	switch (msg->type) {
	case ix::WebSocketMessageType::Message:
		// ��ϵͳ�˳�ʱ��ֹͣ����WebSocket�Ĺ�����Ҫʱ�䣬�ڴ��ڼ�˻ص���������ִ�У����洢���Ѿ������ˣ����³����ڴ�й©��
		// �ʶ���Ҫ�ж��Ƿ�ϵͳ�����˳���ֻ����û���˳�ϵͳʱ���ɴ洢���յ������ݣ���
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
	static bool s_fConnecting = false;
	if (!s_fConnecting) {
		s_fConnecting = true;
		pDataTiingoIEXWebSocket->ConnectWebSocketAndSendMessage(vSymbol);
		s_fConnecting = false;
	}

	return 71;
}

CTiingoIEXWebSocket::CTiingoIEXWebSocket() : CVirtualWebSocket() {
	m_url = _T("wss://api.tiingo.com/iex");
}

CTiingoIEXWebSocket::~CTiingoIEXWebSocket(void) {
}

/// <summary>
/// Tiingo IEX������Դ��ʽ��wss://api.tiingo.com/iex������Կ������͵ġ�
/// </summary>
/// <param name=""></param>
/// <returns></returns>
bool CTiingoIEXWebSocket::Connect(void) {
	Connecting(m_url, ProcessTiingoIEXWebSocket);

	return true;
}

bool CTiingoIEXWebSocket::Send(vector<CString> vSymbol) {
	ASSERT(IsOpen());

	string messageAuth(CreateMessage(vSymbol));
	ix::WebSocketSendInfo info;

	ASSERT(IsOpen());

	info = SendMessage(messageAuth);
	gl_systemMessage.PushInnerSystemInformationMessage(messageAuth.c_str());

	return true;
}

CString CTiingoIEXWebSocket::CreateMessage(vector<CString> vSymbol) {
	CString str;
	CString strSymbols;
	CString strPreffix = _T("{\"eventName\":\"subscribe\",\"authorization\":\"");
	CString strMiddle = _T("\",\"eventData\":{\"thresholdLevel\":5,\"tickers\":[");
	CString strSuffix = _T("]}}");
	CString strAuth = gl_pTiingoWebInquiry->GetInquiryToken();
	strAuth = strAuth.Right(strAuth.GetLength() - 7);

	vSymbol.push_back(_T("rig")); // ���һ��Tiingo��ʽ�Ĵ��롣
	vSymbol.push_back(_T("aapl")); // ���һ��Tiingo��ʽ�Ĵ��롣
	strSymbols = CreateTiingoWebSocketSymbolString(vSymbol); // ȥ����������ַ�','

	str = strPreffix + strAuth + strMiddle + strSymbols + strSuffix;

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
// �����ָ�ʽ��
// {"messageType":"I","data":{"subscriptionId":2563367},"response":{"code":200,"message":"Success"}}
// {"data":{"tickers":["*","uso","msft","tnk"],"thresholdLevel":"0"},"messageType":"I","response":{"code":200,"message":"Success"}}
// {"messageType":"H","response":{"code":200,"message":"HeartBeat"}}
// {"messageType":"A","service":"iex","data":["Q","2019-01-30T13:33:45.383129126-05:00",1548873225383129126,"vym",100,81.58,81.585,81.59,100,null,null,0,0,null,null,null]}
// {"messageType":"A","service":"iex","data":["T","2019-01-30T13:33:45.594808294-05:00",1548873225594808294,"wes",null,null,null,null,null,50.285,200,null,0,0,0,0]}
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool CTiingoIEXWebSocket::ParseTiingoIEXWebSocketData(shared_ptr<string> pData) {
	ptree::iterator it;
	ptree pt, pt2, pt3, pt4;
	string sType, sSymbol, sService;
	char chType;
	string strSymbol;

	string sMessageType, sTicker, sExchange, sDatetime, sValue;
	int i = 0;
	CTiingoIEXSocketPtr pIEXData = nullptr;

	try {
		if (ParseWithPTree(pt, *pData)) {
			sType = ptreeGetString(pt, _T("messageType"));
			if (sType == _T("")) return false;
			switch (sType.at(0)) {
			case 'A': // ��������
				pIEXData = make_shared<CTiingoIEXSocket>();
				sService = ptreeGetString(pt, _T("service"));
				if (sService.compare(_T("iex")) != 0) return false; // �������Ϊ"iex"
				pt2 = pt.get_child(_T("data"));
				it = pt2.begin();
				pt3 = it->second;
				sMessageType = pt3.get_value<string>(); // message type, 'Q'��'T'����'B'
				chType = sMessageType.at(0);
				pIEXData->m_chMessageType = chType;
				switch (chType) {
				case 'Q':// top-of-book update message
					it++;
					pt3 = it->second;
					sDatetime = pt3.get_value<string>();
					it++;
					pt3 = it->second;
					pIEXData->m_iNanoseconds = pt3.get_value<INT64>();
					it++;
					pt3 = it->second;
					sTicker = pt3.get_value<string>();
					pIEXData->m_strSymbol = sTicker.c_str();
					it++;
					pt3 = it->second;
					sValue = pt3.get_value<string>();
					if (sValue.compare(_T("null")) != 0) {
						pIEXData->m_dBidSize = atof(sValue.c_str());
					}
					it++;
					pt3 = it->second;
					sValue = pt3.get_value<string>();
					if (sValue.compare(_T("null")) != 0) {
						pIEXData->m_dBidPrice = atof(sValue.c_str());
					}
					it++;
					pt3 = it->second;
					sValue = pt3.get_value<string>();
					if (sValue.compare(_T("null")) != 0) {
						pIEXData->m_dMidPrice = atof(sValue.c_str());
					}
					it++;
					pt3 = it->second;
					sValue = pt3.get_value<string>();
					if (sValue.compare(_T("null")) != 0) {
						pIEXData->m_dAskPrice = atof(sValue.c_str());
					}
					it++;
					pt3 = it->second;
					sValue = pt3.get_value<string>();
					if (sValue.compare(_T("null")) != 0) {
						pIEXData->m_dAskSize = atof(sValue.c_str());
					}
					it++;
					pt3 = it->second;
					sValue = pt3.get_value<string>();
					if (sValue.compare(_T("null")) != 0) {
						pIEXData->m_dLastPrice = atof(sValue.c_str());
					}
					it++;
					pt3 = it->second;
					sValue = pt3.get_value<string>();
					if (sValue.compare(_T("null")) != 0) {
						pIEXData->m_dLastSize = atof(sValue.c_str());
					}
					it++;
					pt3 = it->second;
					pIEXData->m_iHalted = pt3.get_value<int>();
					it++;
					pt3 = it->second;
					pIEXData->m_iAfterHour = pt3.get_value<int>();
					it++;
					pt3 = it->second;
					sValue = pt3.get_value<string>();
					if (sValue.compare(_T("null")) != 0) {
						pIEXData->m_iISO = atoi(sValue.c_str());
					}
					it++;
					pt3 = it->second;
					sValue = pt3.get_value<string>();
					if (sValue.compare(_T("null")) != 0) {
						pIEXData->m_iOddlot = atoi(sValue.c_str());
					}
					it++;
					pt3 = it->second;
					sValue = pt3.get_value<string>();
					if (sValue.compare(_T("null")) != 0) {
						pIEXData->m_iNMSRule611 = atoi(sValue.c_str());
					}
					break;
				case 'T':// 'T' last trade message
					it++;
					pt3 = it->second;
					sDatetime = pt3.get_value<string>();
					it++;
					pt3 = it->second;
					pIEXData->m_iNanoseconds = pt3.get_value<INT64>();
					it++;
					pt3 = it->second;
					sTicker = pt3.get_value<string>();
					pIEXData->m_strSymbol = sTicker.c_str();
					it++;
					pt3 = it->second;
					sValue = pt3.get_value<string>();
					if (sValue.compare(_T("null")) != 0) {
						pIEXData->m_dBidSize = atof(sValue.c_str());
					}
					it++;
					pt3 = it->second;
					sValue = pt3.get_value<string>();
					if (sValue.compare(_T("null")) != 0) {
						pIEXData->m_dBidPrice = atof(sValue.c_str());
					}
					it++;
					pt3 = it->second;
					sValue = pt3.get_value<string>();
					if (sValue.compare(_T("null")) != 0) {
						pIEXData->m_dMidPrice = atof(sValue.c_str());
					}
					it++;
					pt3 = it->second;
					sValue = pt3.get_value<string>();
					if (sValue.compare(_T("null")) != 0) {
						pIEXData->m_dAskPrice = atof(sValue.c_str());
					}
					it++;
					pt3 = it->second;
					sValue = pt3.get_value<string>();
					if (sValue.compare(_T("null")) != 0) {
						pIEXData->m_dAskSize = atof(sValue.c_str());
					}
					it++;
					pt3 = it->second;
					sValue = pt3.get_value<string>();
					if (sValue.compare(_T("null")) != 0) {
						pIEXData->m_dLastPrice = pt3.get_value<double>();
					}
					it++;
					pt3 = it->second;
					sValue = pt3.get_value<string>();
					if (sValue.compare(_T("null")) != 0) {
						pIEXData->m_dLastSize = atoi(sValue.c_str());
					}
					it++;
					pt3 = it->second;
					sValue = pt3.get_value<string>();
					if (sValue.compare(_T("null")) != 0) {
						pIEXData->m_iHalted = atoi(sValue.c_str());
					}
					it++;
					pt3 = it->second;
					sValue = pt3.get_value<string>();
					if (sValue.compare(_T("null")) != 0) {
						pIEXData->m_iISO = atoi(sValue.c_str());
					}
					it++;
					pt3 = it->second;
					sValue = pt3.get_value<string>();
					if (sValue.compare(_T("null")) != 0) {
						pIEXData->m_iOddlot = atoi(sValue.c_str());
					}
					it++;
					pt3 = it->second;
					sValue = pt3.get_value<string>();
					if (sValue.compare(_T("null")) != 0) {
						pIEXData->m_iNMSRule611 = atoi(sValue.c_str());
					}
					break;
				case 'B':// 'B'trade break messages
					i++;
					break;
				default: // ����
					return false;
					break;
				}
				gl_SystemData.PushTiingoIEXSocket(pIEXData);
				m_fReveivingData = true;
				break;
			case 'I':// authenization  {\"messageType\":\"I\",\"data\":{\"subscriptionId\":2563367},\"response\":{\"code\":200,\"message\":\"Success\"}}
				pt2 = pt.get_child(_T("data"));
				try {
					pt3 = pt2.get_child(_T("tickers"));
					for (ptree::iterator it4 = pt3.begin(); it4 != pt3.end(); it4++) {
						pt4 = it4->second;
						strSymbol = pt4.get_value<string>();
						m_vCurrentSymbol.push_back(strSymbol.c_str());
					}
				}
				catch (exception e) {
					ASSERT(GetSubscriptionId() == 0);
					SetSubscriptionId(ptreeGetInt(pt2, _T("subscriptionId")));
				}
				break;
			case 'H': // Heart beat {\"messageType\":\"H\",\"response\":{\"code\":200,\"message\":\"HeartBeat\"}}
				// ���账��
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
	catch (ptree_error& e) {
		ReportJSonErrorToSystemMessage(_T("Tiingo IEX WebSocket "), e);
		return false;
	}

	return true;
}