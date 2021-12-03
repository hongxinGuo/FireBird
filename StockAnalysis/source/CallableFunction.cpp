#include"pch.h"

#include"globedef.h"
#include "CallableFunction.h"

#include"WebInquirer.h"

void FunctionProcessFinnhubWebSocket(const ix::WebSocketMessagePtr& msg) {
	switch (msg->type) {
	case ix::WebSocketMessageType::Message:
		// ��ϵͳ�˳�ʱ��ֹͣ����WebSocket�Ĺ�����Ҫʱ�䣬�ڴ��ڼ�˻ص���������ִ�У����洢���Ѿ������ˣ����³����ڴ�й©��
		// �ʶ���Ҫ�ж��Ƿ�ϵͳ�����˳���ֻ����û���˳�ϵͳʱ���ɴ洢���յ������ݣ���
		if (!gl_fExitingSystem) {
			gl_WebInquirer.pushFinnhubWebSocketData(msg->str);
			//gl_systemMessage.PushInnerSystemInformationMessage(msg->str.c_str());
		}
		break;
	case ix::WebSocketMessageType::Error:
		gl_systemMessage.PushInnerSystemInformationMessage(msg->errorInfo.reason.c_str());
		break;
	case ix::WebSocketMessageType::Open:
		gl_systemMessage.PushInnerSystemInformationMessage(_T("Finnhub WebSocket������"));
		break;
	case ix::WebSocketMessageType::Close:
		break;
	case ix::WebSocketMessageType::Fragment:
		break;
	case ix::WebSocketMessageType::Ping:
		break;
	case ix::WebSocketMessageType::Pong:
		break;
	default: // error
		break;
	}
}

void FunctionProcessTiingoIEXWebSocket(const ix::WebSocketMessagePtr& msg) {
	switch (msg->type) {
	case ix::WebSocketMessageType::Message:
		// ��ϵͳ�˳�ʱ��ֹͣ����WebSocket�Ĺ�����Ҫʱ�䣬�ڴ��ڼ�˻ص���������ִ�У����洢���Ѿ������ˣ����³����ڴ�й©��
		// �ʶ���Ҫ�ж��Ƿ�ϵͳ�����˳���ֻ����û���˳�ϵͳʱ���ɴ洢���յ������ݣ���
		if (!gl_fExitingSystem) {
			gl_WebInquirer.PushTiingoIEXWebSocketData(msg->str);
			//gl_systemMessage.PushInnerSystemInformationMessage(msg->str.c_str());
		}
		break;
	case ix::WebSocketMessageType::Error:
		gl_systemMessage.PushInnerSystemInformationMessage(msg->errorInfo.reason.c_str());
		break;
	case ix::WebSocketMessageType::Open:
		gl_systemMessage.PushInnerSystemInformationMessage(_T("Tiingo IEX WebSocket������"));
		break;
	case ix::WebSocketMessageType::Close:
		break;
	case ix::WebSocketMessageType::Fragment:
		break;
	case ix::WebSocketMessageType::Ping:
		break;
	case ix::WebSocketMessageType::Pong:
		//gl_systemMessage.PushInnerSystemInformationMessage(_T("Tiingo IEX WebSocket heart beat"));
		break;
	default: // error
		break;
	}
}

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
		gl_systemMessage.PushInnerSystemInformationMessage(msg->errorInfo.reason.c_str());
		break;
	case ix::WebSocketMessageType::Open:
		gl_systemMessage.PushInnerSystemInformationMessage(_T("Tiingo Crypto WebSocket������"));
		break;
	case ix::WebSocketMessageType::Close:
		break;
	case ix::WebSocketMessageType::Fragment:
		break;
	case ix::WebSocketMessageType::Ping:
		break;
	case ix::WebSocketMessageType::Pong:
		//gl_systemMessage.PushInnerSystemInformationMessage(_T("Tiingo Crypto WebSocket heart beat"));
		break;
	default: // error
		break;
	}
}

void FunctionProcessTiingoForexWebSocket(const ix::WebSocketMessagePtr& msg) {
	switch (msg->type) {
	case ix::WebSocketMessageType::Message:
		// ��ϵͳ�˳�ʱ��ֹͣ����WebSocket�Ĺ�����Ҫʱ�䣬�ڴ��ڼ�˻ص���������ִ�У����洢���Ѿ������ˣ����³����ڴ�й©��
		// �ʶ���Ҫ�ж��Ƿ�ϵͳ�����˳���ֻ����û���˳�ϵͳʱ���ɴ洢���յ������ݣ���
		if (!gl_fExitingSystem) {
			gl_WebInquirer.PushTiingoForexWebSocketData(msg->str);
			//gl_systemMessage.PushInnerSystemInformationMessage(msg->str.c_str());
		}
		break;
	case ix::WebSocketMessageType::Error:
		gl_systemMessage.PushInnerSystemInformationMessage(msg->errorInfo.reason.c_str());
		break;
	case ix::WebSocketMessageType::Open:
		gl_systemMessage.PushInnerSystemInformationMessage(_T("Tiingo Forex WebSocket������"));
		break;
	case ix::WebSocketMessageType::Close:
		break;
	case ix::WebSocketMessageType::Fragment:
		break;
	case ix::WebSocketMessageType::Ping:
		break;
	case ix::WebSocketMessageType::Pong:
		//gl_systemMessage.PushInnerSystemInformationMessage(_T("Tiingo Forex WebSocket heart beat"));
		break;
	default: // error
		break;
	}
}

bool CompareEPSSurprise(CEPSSurprisePtr& p1, CEPSSurprisePtr& p2) { return (p1->m_lDate < p2->m_lDate); }
bool CompareDayLineDate(CDayLinePtr& p1, CDayLinePtr& p2) { return p1->GetMarketDate() < p2->GetMarketDate(); }
bool CompareCountryList(CCountryPtr& p1, CCountryPtr& p2) { return p1->m_strCountry < p2->m_strCountry; }
bool CompareInsiderTransaction(CInsiderTransactionPtr& p1, CInsiderTransactionPtr& p2) { return p1->m_lTransactionDate < p2->m_lTransactionDate; }

bool CompareChinaStock(CChinaStockPtr p1, CChinaStockPtr p2) { return (p1->GetSymbol().Compare(p2->GetSymbol()) < 0); }
bool CompareWorldStock(CWorldStockPtr p1, CWorldStockPtr p2) { return (p1->GetSymbol().Compare(p2->GetSymbol()) < 0); }