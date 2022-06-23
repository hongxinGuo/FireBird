#include"pch.h"

#include "SystemMessage.h"
#include"OutputWnd.h"

CSystemDeque::CSystemDeque() {
}

CSystemDeque::~CSystemDeque() {
	m_dequeMessage.clear();
}

CSystemMessage::~CSystemMessage() {
}

void CSystemDeque::Display(COutputList* pOutputList, CString strTime) {
	CString str, str2;
	const size_t lTotal = Size();
	for (int i = 0; i < lTotal; i++) {
		str = PopMessage();
		str2 = strTime + _T(": ") + str;
		SysCallOutputListAddString(pOutputList, str2);
	}
}

void CSystemDeque::SysCallOutputListAddString(COutputList* pOutputList, CString str) {
	pOutputList->AddString(str);
}

void CSystemDeque::PushMessage(CString str) {
	m_mutex.lock();
	m_dequeMessage.push_back(str);
	m_mutex.unlock();
}

CString CSystemDeque::PopMessage(void) {
	CString str;
	m_mutex.lock();
	str = m_dequeMessage.front();
	m_dequeMessage.pop_front();
	m_mutex.unlock();
	return str;     // ֻ�ܴ����ﷵ��
}

size_t CSystemDeque::Size(void) {
	m_mutex.lock();
	const size_t lCount = m_dequeMessage.size();
	m_mutex.unlock();
	return lCount;
}

CSystemMessage::CSystemMessage() {
	static int siCounter = 0;
	if (siCounter++ > 0) {
		TRACE("ϵͳ��Ϣֻ����һ��ʵ��\n");
		gl_systemMessage.PushErrorMessage(_T("����ϵͳ���������ɶ��CSystemMessageʵ��"));
	}

	m_iProcessedFinnhubWebSocket = 0;
	m_iProcessedTiingoIEXWebSocket = 0;
	m_iProcessedTiingoCryptoWebSocket = 0;
	m_iProcessedTiingoForexWebSocket = 0;
}