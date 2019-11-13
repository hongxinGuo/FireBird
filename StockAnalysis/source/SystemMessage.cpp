#include"globedef.h"

#include "SystemMessage.h"

CSystemDeque::CSystemDeque() {
}

CSystemDeque::~CSystemDeque() {
  m_dequeMessage.clear();
}

CSystemMessage::~CSystemMessage()
{
}

void CSystemDeque::PushMessage(CString str)
{
  CSingleLock singleLock(&m_Lock);
  singleLock.Lock();
  if (singleLock.IsLocked()) {
    m_dequeMessage.push_back(str);
    singleLock.Unlock();
  }
}

CString CSystemDeque::PopMessage(void)
{
  CString str;
  CSingleLock singleLock(&m_Lock);
  singleLock.Lock();
  if (singleLock.IsLocked()) {  // ���û����ס�����߳���ֹ��һֱ�ȴ�
    str = m_dequeMessage.front();
    m_dequeMessage.pop_front();
    singleLock.Unlock();
    return str;     // ֻ�ܴ����ﷵ��
  }
  ASSERT(0);
  return ""; // �˷�֧������ִ�е���ֻΪ�������������ľ��������
}

long CSystemDeque::GetDequeSize(void)
{
  CSingleLock singleLock(&m_Lock);
  singleLock.Lock();
  if (singleLock.IsLocked()) {
    const long lCount = m_dequeMessage.size();
    singleLock.Unlock();
    return lCount;
  }
  ASSERT(0);
  return false; // �˷�֧������ִ�е���ֻΪ�������������ľ��������
}

CSystemMessage::CSystemMessage()
{
  static int siCounter = 0;
  if (siCounter++ > 0) {
    TRACE("ϵͳ��Ϣֻ����һ��ʵ��\n");
    gl_systemMessage.PushInnerSystemInformationMessage(_T("����ϵͳ���������ɶ��CSystemMessageʵ��"));
  }
}