#include"globedef.h"

#include "SystemMessage.h"

CSystemDeque::CSystemDeque() {
}

CSystemDeque::~CSystemDeque() {
  m_dequeMessage.clear();
}

CSystemMessage::~CSystemMessage() {
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

long CSystemDeque::GetDequeSize(void) {
  m_mutex.lock();
  const long lCount = m_dequeMessage.size();
  m_mutex.unlock();
  return lCount;
}

CSystemMessage::CSystemMessage() {
  static int siCounter = 0;
  if (siCounter++ > 0) {
    TRACE("ϵͳ��Ϣֻ����һ��ʵ��\n");
    gl_systemMessage.PushInformationMessage(_T("����ϵͳ���������ɶ��CSystemMessageʵ��"));
  }
}