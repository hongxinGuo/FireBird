#include"pch.h"

#include "SystemMessage.h"
#include"OutputWnd.h"

CSystemDeque::CSystemDeque() {}

CSystemDeque::~CSystemDeque() {
	m_dequeMessage.clear();
}

CSystemMessage::~CSystemMessage() = default;

void CSystemDeque::Display(COutputList* pOutputList, const CString& strTime) {
	const size_t lTotal = Size();
	for (int i = 0; i < lTotal; i++) {
		CString str = PopMessage();
		CString str2 = strTime + _T(": ") + str;
		SysCallOutputListAddString(pOutputList, str2);
	}
}

void CSystemDeque::SysCallOutputListAddString(COutputList* pOutputList, const CString& str) {
	pOutputList->AddString(str);
}

void CSystemDeque::PushMessage(const CString& str) {
	m_mutex.lock();
	m_dequeMessage.push_back(str);
	m_mutex.unlock();
}

CString CSystemDeque::PopMessage() {
	m_mutex.lock();
	CString str = m_dequeMessage.front();
	m_dequeMessage.pop_front();
	m_mutex.unlock();
	return str; // 只能从这里返回
}

size_t CSystemDeque::Size() {
	m_mutex.lock();
	const size_t lCount = m_dequeMessage.size();
	m_mutex.unlock();
	return lCount;
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
