#include "pch.h"

#include"globedef.h"
import FireBird.System.Message;
#include <ixwebsocket/IXWebSocket.h>
#include "FinnhubDataSource.h"
#include"JsonParse.h"
#include"simdjson.h"
#include "WorldMarket.h"
import FireBird.Accessory.JsonGetValue;
import FireBird.WebSocket.Finnhub;
import FireBird.Simdjson.GetValue;
import FireBird.Accessory.TimeConvert;
module FireBird.WebSocket.Finnhub;

using namespace simdjson;

void ProcessFinnhubWebSocket(const ix::WebSocketMessagePtr& msg) {
	string str;
	static bool s_bWebSocketClosing = false;
	gl_pFinnhubWebSocket->SetError(false);
	switch (msg->type) {
	case ix::WebSocketMessageType::Message:
		// ��ϵͳ�˳�ʱ��ֹͣ����WebSocket�Ĺ�����Ҫʱ�䣬�ڴ��ڼ�˻ص���������ִ�У����洢���Ѿ������ˣ����³����ڴ�й©��
		// �ʶ���Ҫ�ж��Ƿ�ϵͳ�����˳���ֻ����û���˳�ϵͳʱ���ɴ洢���յ������ݣ���
		s_bWebSocketClosing = false;
		if (!gl_systemConfiguration.IsExitingSystem()) {
			gl_pFinnhubWebSocket->PushData(msg->str);
		}
		break;
	case ix::WebSocketMessageType::Error:
		gl_pFinnhubWebSocket->SetError(true);
		str = _T("Finnhub WebSocket Error: ");
		str += msg->errorInfo.reason;
		gl_dailyWebSocketLogger->error("{}", str);
		gl_systemMessage.PushErrorMessage(str.c_str());
		gl_pFinnhubWebSocket->SetStatusCode(msg->errorInfo.http_status);
		if (msg->errorInfo.http_status == 429) { // ̫���ѯ�������ն����ڲ�ѯ��
			if (s_bWebSocketClosing) return;
			s_bWebSocketClosing = true;
			gl_pFinnhubWebSocket->TaskDisconnect();
			gl_systemConfiguration.SetUsingFinnhubWebSocket(false); // ֹͣ����
			str = _T("too many connections���ر�Finnhub Web Socket����");
			gl_systemMessage.PushInnerSystemInformationMessage(str.c_str());
			gl_systemMessage.PushErrorMessage(str.c_str());
			gl_dailyWebSocketLogger->error("{}", str);
			// finnhub webSocket��ʱ�����EC429��too many connection attempt�����������˻�ͬʱ�������¡���ʱ��Ҫ��ͣ���˻���������ά�������˻��������ܹ�˳��ִ��
			// 10���Ӻ��Զ��������ӡ�
			gl_pWorldMarket->AddTask(WORLD_MARKET_CONNECT_FINNHUB_WEB_SOCKET__, GetNextTime(gl_pWorldMarket->GetMarketTime(), 0, 10, 0));
		}
		break;
	case ix::WebSocketMessageType::Open:
		gl_systemMessage.PushWebSocketInfoMessage(_T("Finnhub WebSocket Open"));
		gl_dailyWebSocketLogger->info("Finnhub WebSocket Open");
		s_bWebSocketClosing = false;
		break;
	case ix::WebSocketMessageType::Close:
		str = _T("Finnhub WebSocket Close");
		if (msg->closeInfo.code != 1000) { // �������ر�
			str += _T(": ");
			str += msg->closeInfo.reason;
		}
		gl_dailyWebSocketLogger->info("{}", str);
		gl_systemMessage.PushWebSocketInfoMessage(str.c_str());
		break;
	case ix::WebSocketMessageType::Fragment:
		gl_systemMessage.PushWebSocketInfoMessage(_T("Finnhub WebSocket Fragment"));
		break;
	case ix::WebSocketMessageType::Ping:
		s_bWebSocketClosing = false;
		gl_pFinnhubWebSocket->SetHeartbeatTime(GetUTCTime());
		gl_systemMessage.PushWebSocketInfoMessage(_T("Finnhub WebSocket Ping"));
		break;
	case ix::WebSocketMessageType::Pong:
		s_bWebSocketClosing = false;
		gl_systemMessage.PushWebSocketInfoMessage(_T("Finnhub WebSocket Pong"));
		break;
	default: // error
		ASSERT(0);
		break;
	}
}

CFinnhubWebSocket::CFinnhubWebSocket() {
	ASSERT(gl_systemConfiguration.IsInitialized());
	m_url = _T("wss://ws.finnhub.io");
}

/// <summary>
/// finnhub webSocket����Դ�ĸ�ʽ��wss://ws.finnhub.io/?token=c1i57rv48v6vit20lrc0��
/// </summary>
void CFinnhubWebSocket::Connect() {
	CString strToken = gl_pFinnhubDataSource->GetInquiryToken();
	strToken = "/?token=" + strToken;
	const string urlAndAuth = m_url + strToken.GetBuffer();

	Connecting(urlAndAuth, ProcessFinnhubWebSocket);
}

void CFinnhubWebSocket::Send(const vectorString& vSymbol) {
	string strMessage;

	ASSERT(IsOpen());
	for (long l = 0; l < vSymbol.size(); l++) {
		if (l >= 49) break; // note ����˻�ֻ֧�����50��֤ȯ����
		strMessage = CreateFinnhubWebSocketString(vSymbol.at(l));
		ix::WebSocketSendInfo info = m_webSocket.send(strMessage);
		ASSERT(info.success);
		gl_systemMessage.PushInnerSystemInformationMessage(strMessage.c_str());
	}
}

/// <summary>
/// Finnhub web socket��ʽ�� {"type":"subscribe","symbol":"���Ŵ�"},IEX�� Crypto, Forex��ʹ�ô˸�ʽ
/// ��{"type":"subscribe","symbol":"MSFT"}, {"type":"subscribe","symbol":"BINANCE:LTCBTC"}, {"type":"subscribe","symbol":"OANDA:AUD_SGD"}
/// </summary>
string CFinnhubWebSocket::CreateFinnhubWebSocketString(string sSymbol) {
	nlohmann::ordered_json symbol;
	symbol["type"] = _T("subscribe");
	symbol["symbol"] = sSymbol;
	return symbol.dump();
}

void CFinnhubWebSocket::MonitorWebSocket(const vectorString& vSymbol) {
	if (IsConnecting()) return; // ����������ӣ��򲻼�ظ�socket
	CVirtualWebSocket::MonitorWebSocket(gl_pFinnhubDataSource->IsWebError(), gl_systemConfiguration.IsUsingFinnhubWebSocket(), vSymbol);
}

/// <summary>
///
/// https://finnhub.io/docs/api/websocket-trades
///
/// Ŀǰ���֡�
/// ��ʽΪ��{"data":[{"c":null,"p":7296.89,"s":"BINANCE:BTCUSDT","t":1575526691134,"v":0.011467}],"type":"trade"}
///        {"type":"ping"}
///        {"msg":"Subscribing to too many symbols","type":"error"}
/// </summary>
/// <param name="pData"></param>
/// <returns></returns>
bool CFinnhubWebSocket::ParseFinnhubWebSocketData(shared_ptr<string> pData) {
	string sType, sSymbol, sMessage;
	string code;

	try {
		if (nlohmann::ordered_json pt; CreateJsonWithNlohmann(pt, *pData)) {
			sType = jsonGetString(&pt, _T("type"));
			if (sType == _T("trade")) { // {"data":[{"c":null,"p":7296.89,"s":"BINANCE:BTCUSDT","t":1575526691134,"v":0.011467}],"type":"trade"}
				nlohmann::ordered_json js2;
				nlohmann::ordered_json pt3;
				// ��������
				js2 = jsonGetChild(&pt, _T("data"));
				for (auto it = js2.begin(); it != js2.end(); ++it) {
					const auto pFinnhubDataPtr = make_shared<CFinnhubSocket>();
					pFinnhubDataPtr->m_sSymbol = jsonGetString(it, _T("s"));
					pt3 = jsonGetChild(it, _T("c"));
					for (auto it2 = pt3.begin(); it2 != pt3.end(); ++it2) {
						pFinnhubDataPtr->m_vCode.push_back(jsonGetString(it2));
					}
					pFinnhubDataPtr->m_dLastPrice = jsonGetDouble(it, _T("p"));
					pFinnhubDataPtr->m_dLastVolume = jsonGetDouble(it, _T("v"));
					pFinnhubDataPtr->m_iSeconds = jsonGetLongLong(it, _T("t"));
					gl_SystemData.PushFinnhubSocket(pFinnhubDataPtr);
				}
				m_HeartbeatTime = GetUTCTime();
			}
			else if (sType == _T("ping")) {	// ping  {\"type\":\"ping\"}
			}
			else if (sType == _T("error")) { // ERROR {\"msg\":\"Subscribing to too many symbols\",\"type\":\"error\"}
				sMessage = jsonGetString(&pt, _T("msg"));
				CString strMessage = "Finnhub WebSocket error message: ";
				strMessage += sMessage.c_str();
				gl_systemMessage.PushInnerSystemInformationMessage(strMessage);
				return false;
			}
			else { // new format or error
				CString strMsg = _T("Finnhub Web Socket type error: ");
				strMsg += sType.c_str();
				gl_systemMessage.PushInnerSystemInformationMessage(strMsg);
				return false;
			}
		}
		else {
			gl_systemMessage.PushInnerSystemInformationMessage(_T("Finnhub Web Socket json error"));
			return false;
		}
	} catch (nlohmann::ordered_json::exception& e) {
		ReportJSonErrorToSystemMessage(_T("Process One Finnhub WebSocketData "), e.what());
		return false;
	}

	return true;
}

///
/// https://finnhub.io/docs/api/websocket-trades
///
///Ŀǰ���֡�
// ��ʽΪ��{"data":[{"c":null,"p":7296.89,"s":"BINANCE:BTCUSDT","t":1575526691134,"v":0.011467}],"type":"trade"}
//				 {"data": [{"c":["1", "12", "24"], "p" : 7296.89, "s" : "BINANCE:BTCUSDT", "t" : 1575526691134, "v" : 0.011467}] , "type" : "trade"}
///        {"type":"ping"}
///        {"msg":"Subscribing to too many symbols","type":"error"}
/// <param name="pData"></param>
/// <returns></returns>
static ondemand::document s_docFinnhubWebSocket; // ʹ�þ�̬�����Լ��ٽ���
static ondemand::parser s_parserFinnhubWebSocket;// ʹ�þ�̬�����Լ��ٽ���
bool CFinnhubWebSocket::ParseFinnhubWebSocketDataWithSidmjson(const shared_ptr<string>& pData) {
	try {
		const padded_string jsonPadded(*pData);
		s_docFinnhubWebSocket = s_parserFinnhubWebSocket.iterate(jsonPadded).value();
		const string_view sType = jsonGetStringView(s_docFinnhubWebSocket, "type");
		if (sType == _T("trade")) { // {"data":[{"c":null,"p":7296.89,"s":"BINANCE:BTCUSDT","t":1575526691134,"v":0.011467}],"type":"trade"}
			auto data_array = jsonGetArray(s_docFinnhubWebSocket, "data");
			for (auto item : data_array) {
				auto itemValue = item.value();
				const auto pFinnhubDataPtr = make_shared<CFinnhubSocket>();
				auto condition_array = jsonGetArray(itemValue, "c");
				for (auto condition_item : condition_array) {
					const string_view s5 = jsonGetStringView(condition_item.value());
					string_view s6 = s5.substr(0, s5.length());
					string s2(s6);
					pFinnhubDataPtr->m_vCode.push_back(s2);
				}
				pFinnhubDataPtr->m_dLastPrice = jsonGetDouble(itemValue, "p");
				const string_view symbol = jsonGetStringView(itemValue, "s");
				const string_view symbol2 = symbol.substr(0, symbol.length());
				pFinnhubDataPtr->m_sSymbol = symbol2;
				pFinnhubDataPtr->m_iSeconds = jsonGetInt64(itemValue, "t");
				pFinnhubDataPtr->m_dLastVolume = jsonGetDouble(itemValue, "v");
				gl_SystemData.PushFinnhubSocket(pFinnhubDataPtr);
			}
			m_HeartbeatTime = GetUTCTime();
		}
		else if (sType == _T("ping")) {	// ping  {\"type\":\"ping\"}
		}
		else if (sType == _T("error")) {// ERROR {\"msg\":\"Subscribing to too many symbols\",\"type\":\"error\"}
			const string_view message = s_docFinnhubWebSocket["msg"].get_string();
			const string_view m2 = message.substr(0, message.length());
			const string sMessage(m2);
			CString strMessage = "Finnhub WebSocket error message: ";
			strMessage += sMessage.c_str();
			gl_systemMessage.PushInnerSystemInformationMessage(strMessage);
			return false;
		}
		else { // new format or error
			return false;
		}
	} catch (simdjson_error& error) {
		ReportJSonErrorToSystemMessage(_T("Process One Finnhub WebSocketData "), error.what());
		return false;
	}

	return true;
}
