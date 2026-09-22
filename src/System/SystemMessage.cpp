#include"pch.h"

#include "SystemMessage.h"

#include"OutputWnd.h"
#include "SystemConfiguration.h"

using std::map;
using std::binary_semaphore;

namespace {
	binary_semaphore s_RTDataStockCode{ 1 };
	binary_semaphore s_DayLineDataStockCode{ 1 };
	binary_semaphore s_CurrentFinnhubWebSocketStake{ 1 };
	binary_semaphore s_CurrentTiingoWebSocketIEX{ 1 };
	binary_semaphore s_CurrentTiingoWebSocketCrypto{ 1 };
	binary_semaphore s_CurrentTiingoWebSocketForex{ 1 };
	binary_semaphore s_sReadCurrentFinnhubFunction{ 1 };
	binary_semaphore s_sReadCurrentTiingoFunction{ 1 };
	binary_semaphore s_sReadCurrentAlphaVantageFunction{ 1 };
	binary_semaphore s_sReadCurrentAlpacaFunction{ 1 };
}

void CSystemMessage::SetStockCodeForInquiringRTData(const string& strStockCode) {
	s_RTDataStockCode.acquire();
	m_sStockCodeForInquiringRTData = strStockCode;
	s_RTDataStockCode.release();
}

string CSystemMessage::GetStockCodeForInquiringRTData() const {
	s_RTDataStockCode.acquire();
	string s = m_sStockCodeForInquiringRTData;
	s_RTDataStockCode.release();
	return s;
}

void CSystemMessage::SetStockCodeForInquiryDayLine(const string& strStockCode) {
	s_DayLineDataStockCode.acquire();
	m_sStockCodeForInquiryDayLine = strStockCode;
	s_DayLineDataStockCode.release();
}

string CSystemMessage::GetStockCodeForInquiryDayLine() const {
	s_DayLineDataStockCode.acquire();
	string s = m_sStockCodeForInquiryDayLine;
	s_DayLineDataStockCode.release();
	return s;
}

void CSystemMessage::SetCurrentFinnhubWebSocketStake(const string& s) {
	s_CurrentFinnhubWebSocketStake.acquire();
	m_sCurrentFinnhubWebSocketStake = s;
	s_CurrentFinnhubWebSocketStake.release();
}

string CSystemMessage::GetCurrentFinnhubWebSocketStake() const {
	s_CurrentFinnhubWebSocketStake.acquire();
	string s = m_sCurrentFinnhubWebSocketStake;
	s_CurrentFinnhubWebSocketStake.release();
	return s;
}

void CSystemMessage::SetCurrentTiingoWebSocketIEX(const string& s) {
	s_CurrentTiingoWebSocketIEX.acquire();
	m_sCurrentTiingoWebSocketIEX = s;
	s_CurrentTiingoWebSocketIEX.release();
}

string CSystemMessage::GetCurrentTiingoWebSocketIEX() const {
	s_CurrentTiingoWebSocketIEX.acquire();
	string s = m_sCurrentTiingoWebSocketIEX;
	s_CurrentTiingoWebSocketIEX.release();
	return s;
}

void CSystemMessage::SetCurrentTiingoWebSocketForex(const string& s) {
	s_CurrentTiingoWebSocketForex.acquire();
	m_sCurrentTiingoWebSocketForex = s;
	s_CurrentTiingoWebSocketForex.release();
}

string CSystemMessage::GetCurrentTiingoWebSocketForex() const {
	s_CurrentTiingoWebSocketForex.acquire();
	string s = m_sCurrentTiingoWebSocketForex;
	s_CurrentTiingoWebSocketForex.release();
	return s;
}

void CSystemMessage::SetCurrentTiingoWebSocketCrypto(const string& s) {
	s_CurrentTiingoWebSocketCrypto.acquire();
	m_sCurrentTiingoWebSocketCrypto = s;
	s_CurrentTiingoWebSocketCrypto.release();
}

string CSystemMessage::GetCurrentTiingoWebSocketCrypto() const {
	s_CurrentTiingoWebSocketCrypto.acquire();
	string s = m_sCurrentTiingoWebSocketCrypto;
	s_CurrentTiingoWebSocketCrypto.release();
	return s;
}

string CSystemMessage::GetCurrentFinnhubFunction() const {
	s_sReadCurrentFinnhubFunction.acquire();
	string str = m_sCurrentFinnhubFunction;
	s_sReadCurrentFinnhubFunction.release();
	return str;
}

void CSystemMessage::SetCurrentFinnhubFunction(const string& str) {
	s_sReadCurrentFinnhubFunction.acquire();
	m_sCurrentFinnhubFunction = str;
	s_sReadCurrentFinnhubFunction.release();
}

string CSystemMessage::GetCurrentTiingoFunction() const {
	s_sReadCurrentTiingoFunction.acquire();
	string str = m_sCurrentTiingoFunction;
	s_sReadCurrentTiingoFunction.release();
	return str;
}

void CSystemMessage::SetCurrentTiingoFunction(const string& str) {
	s_sReadCurrentTiingoFunction.acquire();
	m_sCurrentTiingoFunction = str;
	s_sReadCurrentTiingoFunction.release();
}

string CSystemMessage::GetCurrentAlphaVantageFunction() const {
	s_sReadCurrentAlphaVantageFunction.acquire();
	string str = m_sCurrentAlphaVantageFunction;
	s_sReadCurrentAlphaVantageFunction.release();
	return str;
}

void CSystemMessage::SetCurrentAlphaVantageFunction(const string& str) {
	s_sReadCurrentAlphaVantageFunction.acquire();
	m_sCurrentAlphaVantageFunction = str;
	s_sReadCurrentAlphaVantageFunction.release();
}

string CSystemMessage::GetCurrentAlpacaFunction() const {
	s_sReadCurrentAlpacaFunction.acquire();
	string str = m_sCurrentAlpacaFunction;
	s_sReadCurrentAlpacaFunction.release();
	return str;
}

void CSystemMessage::SetCurrentAlpacaFunction(const string& str) {
	s_sReadCurrentAlpacaFunction.acquire();
	m_sCurrentAlpacaFunction = str;
	s_sReadCurrentAlpacaFunction.release();
}

void CSystemDeque::Display(COutputList* pOutputList, const string& strTime) {
	size_t lTotal = Size();
	lTotal = std::min<size_t>(lTotal, pOutputList->GetLineNumber());
	for (size_t i = 0; i < lTotal; i++) {
		string str2 = strTime + ": " + PopMessage();
		SysCallOutputListAddString(pOutputList, str2);
	}
}

void CSystemDeque::SysCallOutputListAddString(COutputList* pOutputList, const string& str) {
	pOutputList->AppendString(str);
}

void CSystemDeque::PushMessage(const string& str) {
	m_queueMessage.enqueue(str);
}

string CSystemDeque::PopMessage() {
	string str;
	m_queueMessage.try_dequeue(str);
	return str;
}

size_t CSystemDeque::Size() const {
	return m_queueMessage.size_approx();
}

CSystemMessage::CSystemMessage() {
	ABSL_DCHECK(gl_systemConfiguration.IsInitialized());
	if (static int siCounter = 0; siCounter++ > 0) {
		ABSL_DLOG(INFO) << "系统消息只允许一个实例";
		gl_systemMessage.PushErrorMessage("错误：系统不允许生成多个CSystemMessage实例");
	}

	m_iProcessedFinnhubWebSocket = 0;
	m_iProcessedTiingoIEXWebSocket = 0;
	m_iProcessedTiingoCryptoWebSocket = 0;
	m_iProcessedTiingoForexWebSocket = 0;
}
