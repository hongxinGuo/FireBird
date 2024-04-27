#include"pch.h"

#include "SystemMessage.h"
#include"OutputWnd.h"

using std::binary_semaphore;

binary_semaphore s_RTDataStockCode{1};
binary_semaphore s_DayLineDataStockCode{1};
binary_semaphore s_CurrentFinnhubWebSocketStake{1};
binary_semaphore s_CurrentTiingoWebSocketIEX{1};
binary_semaphore s_CurrentTiingoWebSocketCrypto{1};
binary_semaphore s_CurrentTiingoWebSocketForex{1};

CSystemDeque::CSystemDeque() {}

CSystemDeque::~CSystemDeque() {}

CSystemMessage::~CSystemMessage() = default;

void CSystemMessage::SetStockCodeForInquiringRTData(const CString& strStockCode) {
	s_RTDataStockCode.acquire();
	m_strStockCodeForInquiringRTData = strStockCode;
	s_RTDataStockCode.release();
}

CString CSystemMessage::GetStockCodeForInquiringRTData() const {
	s_RTDataStockCode.acquire();
	CString s = m_strStockCodeForInquiringRTData;
	s_RTDataStockCode.release();
	return s;
}

void CSystemMessage::SetStockCodeForInquiryDayLine(const CString& strStockCode) {
	s_DayLineDataStockCode.acquire();
	m_strStockCodeForInquiryDayLine = strStockCode;
	s_DayLineDataStockCode.release();
}

CString CSystemMessage::GetStockCodeForInquiryDayLine() const {
	s_DayLineDataStockCode.acquire();
	CString s = m_strStockCodeForInquiryDayLine;
	s_DayLineDataStockCode.release();
	return s;
}

void CSystemMessage::SetCurrentFinnhubWebSocketStake(const CString& s) {
	s_CurrentFinnhubWebSocketStake.acquire();
	m_strCurrentFinnhubWebSocketStake = s;
	s_CurrentFinnhubWebSocketStake.release();
}

CString CSystemMessage::GetCurrentFinnhubWebSocketStake() const {
	s_CurrentFinnhubWebSocketStake.acquire();
	CString s = m_strCurrentFinnhubWebSocketStake;
	s_CurrentFinnhubWebSocketStake.release();
	return s;
}

void CSystemMessage::SetCurrentTiingoWebSocketIEX(const CString& s) {
	s_CurrentTiingoWebSocketIEX.acquire();
	m_strCurrentTiingoWebSocketIEX = s;
	s_CurrentTiingoWebSocketIEX.release();
}

CString CSystemMessage::GetCurrentTiingoWebSocketIEX() const {
	s_CurrentTiingoWebSocketIEX.acquire();
	CString s = m_strCurrentTiingoWebSocketIEX;
	s_CurrentTiingoWebSocketIEX.release();
	return s;
}

void CSystemMessage::SetCurrentTiingoWebSocketForex(const CString& s) {
	s_CurrentTiingoWebSocketForex.acquire();
	m_strCurrentTiingoWebSocketForex = s;
	s_CurrentTiingoWebSocketForex.release();
}

CString CSystemMessage::GetCurrentTiingoWebSocketForex() const {
	s_CurrentTiingoWebSocketForex.acquire();
	CString s = m_strCurrentTiingoWebSocketForex;
	s_CurrentTiingoWebSocketForex.release();
	return s;
}

void CSystemMessage::SetCurrentTiingoWebSocketCrypto(const CString& s) {
	s_CurrentTiingoWebSocketCrypto.acquire();
	m_strCurrentTiingoWebSocketCrypto = s;
	s_CurrentTiingoWebSocketCrypto.release();
}

CString CSystemMessage::GetCurrentTiingoWebSocketCrypto() const {
	s_CurrentTiingoWebSocketCrypto.acquire();
	CString s = m_strCurrentTiingoWebSocketCrypto;
	s_CurrentTiingoWebSocketCrypto.release();
	return s;
}

void CSystemDeque::Display(COutputList* pOutputList, const CString& strTime) {
	size_t lTotal = Size();
	if (lTotal > pOutputList->GetLineNumber()) lTotal = pOutputList->GetLineNumber();
	for (int i = 0; i < lTotal; i++) {
		CString str2 = strTime + _T(": ") + PopMessage();
		SysCallOutputListAddString(pOutputList, str2);
	}
}

void CSystemDeque::SysCallOutputListAddString(COutputList* pOutputList, const CString& str) {
	pOutputList->AddString(str);
}

void CSystemDeque::PushMessage(const CString& str) {
	m_queueMessage.enqueue(str.GetString());
}

CString CSystemDeque::PopMessage() {
	string str;
	if (m_queueMessage.try_dequeue(str)) {
		return str.c_str();
	}
	else return _T("");
}

size_t CSystemDeque::Size() const {
	return m_queueMessage.size_approx();
}

CSystemMessage::CSystemMessage() {
	ASSERT(gl_systemConfiguration.IsInitialized());
	if (static int siCounter = 0; siCounter++ > 0) {
		TRACE("系统消息只允许一个实例\n");
		gl_systemMessage.PushErrorMessage(_T("错误：系统不允许生成多个CSystemMessage实例"));
	}

	m_iProcessedFinnhubWebSocket = 0;
	m_iProcessedTiingoIEXWebSocket = 0;
	m_iProcessedTiingoCryptoWebSocket = 0;
	m_iProcessedTiingoForexWebSocket = 0;
}
