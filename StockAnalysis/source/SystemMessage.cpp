#include"pch.h"
#include"globedef.h"

#include "SystemMessage.h"

CSystemDeque::CSystemDeque() {
}

CSystemDeque::~CSystemDeque() {
  m_dequeMessage.clear();
}

CSystemMessage::~CSystemMessage() {
}

void CSystemDeque::Display(COutputList* pOutputList, CString strTime) {
  CString str, str2;
  const size_t lTotal = GetDequeSize();
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
  return str;     // 只能从这里返回
}

size_t CSystemDeque::GetDequeSize(void) {
  m_mutex.lock();
  const size_t lCount = m_dequeMessage.size();
  m_mutex.unlock();
  return lCount;
}

CSystemMessage::CSystemMessage() {
  static int siCounter = 0;
  if (siCounter++ > 0) {
    TRACE("系统消息只允许一个实例\n");
    gl_systemMessage.PushInformationMessage(_T("错误：系统不允许生成多个CSystemMessage实例"));
  }
}