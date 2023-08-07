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
	return str; // ֻ�ܴ����ﷵ��
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
		TRACE("ϵͳ��Ϣֻ����һ��ʵ��\n");
		gl_systemMessage.PushErrorMessage(_T("����ϵͳ���������ɶ��CSystemMessageʵ��"));
	}

	m_iProcessedFinnhubWebSocket = 0;
	m_iProcessedTiingoIEXWebSocket = 0;
	m_iProcessedTiingoCryptoWebSocket = 0;
	m_iProcessedTiingoForexWebSocket = 0;
}
