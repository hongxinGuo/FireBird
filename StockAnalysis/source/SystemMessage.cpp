#include "SystemMessage.h"



CSystemMessage::CSystemMessage()
{
  static int siCounter = 0;
  if (siCounter > 1) { 
    TRACE("系统消息只允许一个实例\n");
    m_dequeInformationMessage.push_back(_T("错误：系统消息只允许一个实例"));
  }
  else siCounter++;

}


CSystemMessage::~CSystemMessage()
{
  m_dequeDayLineInfoMessage.clear();
  m_dequeTransactionMessage.clear();
  m_dequeInformationMessage.clear();
  m_dequeCancelSellMessage.clear();
  m_dequeTrace1Message.clear();
  m_dequeTrace2Message.clear();
}

void CSystemMessage::PushInformationMessage(CString str)
{
  CSingleLock singleLock(&m_InformationLock);
  singleLock.Lock();
  if (singleLock.IsLocked()) {
    m_dequeInformationMessage.push_back(str);
    singleLock.Unlock();
  }
}

CString CSystemMessage::PopInformationMessage(void)
{
  CString str;
  CSingleLock singleLock(&m_InformationLock);
  singleLock.Lock();
  if (singleLock.IsLocked()) {
    str = m_dequeInformationMessage.front();
    m_dequeInformationMessage.pop_front();
    singleLock.Unlock();
    return str;
  }
}

CString CSystemMessage::PopDayLineInfoMessage(void)
{
  CString str;
  CSingleLock singleLock(&m_DayLineInfoLock);
  singleLock.Lock();
  if (singleLock.IsLocked()) {
    str = m_dequeDayLineInfoMessage.front();
    m_dequeDayLineInfoMessage.pop_front();
    singleLock.Unlock();
    return str;
  }
}

CString CSystemMessage::PopTransactionMessage(void)
{
  CString str;
  CSingleLock singleLock(&m_TransactionLock);
  singleLock.Lock();
  if (singleLock.IsLocked()) {
    str = m_dequeTransactionMessage.front();
    m_dequeTransactionMessage.pop_front();
    singleLock.Unlock();
    return str;
  }
}

CString CSystemMessage::PopCancelSellMessage(void)
{
  CString str;
  CSingleLock singleLock(&m_CancelSellLock);
  singleLock.Lock();
  if (singleLock.IsLocked()) {
    str = m_dequeCancelSellMessage.front();
    m_dequeCancelSellMessage.pop_front();
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

long CSystemMessage::GetInformationDequeSize(void)
{
  CSingleLock singleLock(&m_InformationLock);
  singleLock.Lock();
  if (singleLock.IsLocked()) {
    long lCount = m_dequeInformationMessage.size();
    singleLock.Unlock();
    return lCount;
  }
  return 0;
}

long CSystemMessage::GetDayLineInfoDequeSize(void)
{
  CSingleLock singleLock(&m_DayLineInfoLock);
  singleLock.Lock();
  if (singleLock.IsLocked()) {
    long lCount = m_dequeDayLineInfoMessage.size();
    singleLock.Unlock();
    return lCount;
  }
  return 0;
}

long CSystemMessage::GetTransactionDequeSize(void)
{
  CSingleLock singleLock(&m_TransactionLock);
  singleLock.Lock();
  if (singleLock.IsLocked()) {
    long lCount = m_dequeTransactionMessage.size();
    singleLock.Unlock();
    return lCount;
  }
  return 0;
}

long CSystemMessage::GetCancelSellDequeSize(void)
{
  CSingleLock singleLock(&m_CancelSellLock);
  singleLock.Lock();
  if (singleLock.IsLocked()) {
    long lCount = m_dequeCancelSellMessage.size();
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

void CSystemMessage::PushDayLineInfoMessage(CString str)
{
  CSingleLock singleLock(&m_DayLineInfoLock);
  singleLock.Lock();
  if (singleLock.IsLocked()) {
    m_dequeDayLineInfoMessage.push_back(str);
    singleLock.Unlock();
  }
}

void CSystemMessage::PushTransactionMessage(CString str)
{
  CSingleLock singleLock(&m_TransactionLock);
  singleLock.Lock();
  if (singleLock.IsLocked()) {
    m_dequeTransactionMessage.push_back(str);
    singleLock.Unlock();
  }

}

void CSystemMessage::PushCancelSellMessage(CString str)
{
  CSingleLock singleLock(&m_CancelSellLock);
  singleLock.Lock();
  if (singleLock.IsLocked()) {
    m_dequeCancelSellMessage.push_back(str);
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

