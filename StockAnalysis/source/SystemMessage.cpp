#include "SystemMessage.h"



CSystemMessage::CSystemMessage()
{
  static int siCounter = 0;
  if (siCounter > 1) { 
    TRACE("系统消息只允许一个实例\n");
  }
  else siCounter++;

}


CSystemMessage::~CSystemMessage()
{
  m_dequeDataBaseMessage.clear();
  m_dequeFindMessage.clear();
  m_dequeOutputMessage.clear();
  m_dequeWarningMessage.clear();
}

void CSystemMessage::PushOutputMessage(CString str)
{
  CSingleLock singleLock(&m_OutputLock);
  singleLock.Lock();
  if (singleLock.IsLocked()) {
    m_dequeOutputMessage.push_back(str);
    singleLock.Unlock();
  }
}

CString CSystemMessage::PopOutputMessage(void)
{
  CString str;
  CSingleLock singleLock(&m_OutputLock);
  singleLock.Lock();
  if (singleLock.IsLocked()) {
    str = m_dequeOutputMessage.front();
    m_dequeOutputMessage.pop_front();
    singleLock.Unlock();
    return str;
  }
}

CString CSystemMessage::PopDataBaseMessage(void)
{
  CString str;
  CSingleLock singleLock(&m_OutputLock);
  singleLock.Lock();
  if (singleLock.IsLocked()) {
    str = m_dequeOutputMessage.front();
    m_dequeOutputMessage.pop_front();
    singleLock.Unlock();
    return str;
  }
}

CString CSystemMessage::PopFindMessage(void)
{
  CString str;
  CSingleLock singleLock(&m_FindLock);
  singleLock.Lock();
  if (singleLock.IsLocked()) {
    str = m_dequeFindMessage.front();
    m_dequeFindMessage.pop_front();
    singleLock.Unlock();
    return str;
  }
}

CString CSystemMessage::PopWarningMessage(void)
{
  CString str;
  CSingleLock singleLock(&m_WarningLock);
  singleLock.Lock();
  if (singleLock.IsLocked()) {
    str = m_dequeWarningMessage.front();
    m_dequeWarningMessage.pop_front();
    singleLock.Unlock();
    return str;
  }
}

CString CSystemMessage::PopTrace1Message(void)
{
  CString str;
  CSingleLock singleLock(&m_Trace1Lock);
  singleLock.Lock();
  if (singleLock.IsLocked()) {
    str = m_dequeTrace1Message.front();
    m_dequeTrace1Message.pop_front();
    singleLock.Unlock();
    return str;
  }
}

CString CSystemMessage::PopTrace2Message(void)
{
  CString str;
  CSingleLock singleLock(&m_Trace2Lock);
  singleLock.Lock();
  if (singleLock.IsLocked()) {
    str = m_dequeTrace2Message.front();
    m_dequeTrace2Message.pop_front();
    singleLock.Unlock();
    return str;
  }
}

long CSystemMessage::GetOutputDequeSize(void)
{
  CSingleLock singleLock(&m_OutputLock);
  singleLock.Lock();
  if (singleLock.IsLocked()) {
    long lCount = m_dequeOutputMessage.size();
    singleLock.Unlock();
    return lCount;
  }
  return 0;
}

long CSystemMessage::GetDataBaseDequeSize(void)
{
  CSingleLock singleLock(&m_DataBaseLock);
  singleLock.Lock();
  if (singleLock.IsLocked()) {
    long lCount = m_dequeDataBaseMessage.size();
    singleLock.Unlock();
    return lCount;
  }
  return 0;
}

long CSystemMessage::GetFindDequeSize(void)
{
  CSingleLock singleLock(&m_FindLock);
  singleLock.Lock();
  if (singleLock.IsLocked()) {
    long lCount = m_dequeFindMessage.size();
    singleLock.Unlock();
    return lCount;
  }
  return 0;
}

long CSystemMessage::GetWarningDequeSize(void)
{
  CSingleLock singleLock(&m_WarningLock);
  singleLock.Lock();
  if (singleLock.IsLocked()) {
    long lCount = m_dequeWarningMessage.size();
    singleLock.Unlock();
    return lCount;
  }
  return 0;
}

long CSystemMessage::GetTrace1DequeSize(void)
{
  CSingleLock singleLock(&m_Trace1Lock);
  singleLock.Lock();
  if (singleLock.IsLocked()) {
    long lCount = m_dequeTrace1Message.size();
    singleLock.Unlock();
    return lCount;
  }
  return 0;
}

long CSystemMessage::GetTrace2DequeSize(void)
{
  CSingleLock singleLock(&m_Trace2Lock);
  singleLock.Lock();
  if (singleLock.IsLocked()) {
    long lCount = m_dequeTrace2Message.size();
    singleLock.Unlock();
    return lCount;
  }
  return 0;
}

void CSystemMessage::PushDataBaseMessage(CString str)
{
  CSingleLock singleLock(&m_DataBaseLock);
  singleLock.Lock();
  if (singleLock.IsLocked()) {
    m_dequeDataBaseMessage.push_back(str);
    singleLock.Unlock();
  }
}

void CSystemMessage::PushFindMessage(CString str)
{
  CSingleLock singleLock(&m_FindLock);
  singleLock.Lock();
  if (singleLock.IsLocked()) {
    m_dequeFindMessage.push_back(str);
    singleLock.Unlock();
  }

}

void CSystemMessage::PushWarningMessage(CString str)
{
  CSingleLock singleLock(&m_WarningLock);
  singleLock.Lock();
  if (singleLock.IsLocked()) {
    m_dequeWarningMessage.push_back(str);
    singleLock.Unlock();
  }
}

void CSystemMessage::PushTrace1Message(CString str)
{
  CSingleLock singleLock(&m_Trace1Lock);
  singleLock.Lock();
  if (singleLock.IsLocked()) {
    m_dequeTrace1Message.push_back(str);
    singleLock.Unlock();
  }
}

void CSystemMessage::PushTrace2Message(CString str)
{
  CSingleLock singleLock(&m_Trace2Lock);
  singleLock.Lock();
  if (singleLock.IsLocked()) {
    m_dequeTrace2Message.push_back(str);
    singleLock.Unlock();
  }
}

