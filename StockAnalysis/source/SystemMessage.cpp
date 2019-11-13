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
  if (singleLock.IsLocked()) {  // 如果没有锁住，则线程终止，一直等待
    str = m_dequeMessage.front();
    m_dequeMessage.pop_front();
    singleLock.Unlock();
    return str;     // 只能从这里返回
  }
  ASSERT(0);
  return ""; // 此分支不可能执行到，只为了消除编译器的警告而存在
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
  return false; // 此分支不可能执行到，只为了消除编译器的警告而存在
}

CSystemMessage::CSystemMessage()
{
  static int siCounter = 0;
  if (siCounter++ > 0) {
    TRACE("系统消息只允许一个实例\n");
    gl_systemMessage.PushInnerSystemInformationMessage(_T("错误：系统不允许生成多个CSystemMessage实例"));
  }
}