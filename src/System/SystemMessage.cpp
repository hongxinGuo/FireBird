#include"pch.h"

#include "SystemMessage.h"
#include"OutputWnd.h"

CSystemDeque::CSystemDeque() {}

CSystemDeque::~CSystemDeque() {}

CSystemMessage::~CSystemMessage() = default;

void CSystemDeque::Display(COutputList* pOutputList, const CString& strTime) {
	const size_t lTotal = Size();
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
