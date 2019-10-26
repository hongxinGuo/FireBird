#include"stdafx.h"

#include"globedef.h"

#include"ThreadStatus.h"
#include"SystemMessage.h"

CThreadStatus::CThreadStatus() {
  static int siInstance = 0;
  if (siInstance++ > 0) {
    TRACE("系统状态只允许生成一个实例\n");
    gl_systemMessage.PushInformationMessage(_T("系统状态只允许生成一个实例"));
  }

  m_iNumberOfCalculatingRSThreads = 0;
}

void CThreadStatus::IncreaseNunberOfCalculatingRSThreads(void) {
  CSingleLock singleLock(&m_NumberOfCalculatingRSThreads);
  singleLock.Lock();
  if (singleLock.IsLocked()) {
    ASSERT(m_iNumberOfCalculatingRSThreads < gl_cMaxCalculatingRSThreads);
    m_iNumberOfCalculatingRSThreads++;
    singleLock.Unlock();
  }
}

void CThreadStatus::DecreaseNumberOfCalculatingRSThreads(void) {
  CSingleLock singleLock(&m_NumberOfCalculatingRSThreads);
  singleLock.Lock();
  if (singleLock.IsLocked()) {
    ASSERT(m_iNumberOfCalculatingRSThreads > 0);
    m_iNumberOfCalculatingRSThreads--;
    singleLock.Unlock();
  }
}

///////////////////////////////////////////////////////////////////////////////////////////
//
// 判断是否还有空余的线程可供使用。
//
// 目前采用16线程制，允许最多生成16个工作线程。
//
//
//////////////////////////////////////////////////////////////////////////////////////////
bool CThreadStatus::IsCalculatingRSThreadAvailable(void) {
  CSingleLock singleLock(&m_NumberOfCalculatingRSThreads);
  singleLock.Lock();
  if (singleLock.IsLocked()) {
    bool fFlag;
    if (m_iNumberOfCalculatingRSThreads >= gl_cMaxCalculatingRSThreads) { // 最多允许8个线程同时运行。更多的线程容易导致系统响应变慢，且没必要。
      fFlag = false;
    }
    else fFlag = true;
    singleLock.Unlock();
    return fFlag;
  }
  ASSERT(0);
  return false; // 此分支不可能执行到，只为了消除编译器的警告而存在
}

bool CThreadStatus::IsCalculatingRSThreadRunning(void) {
  CSingleLock singleLock(&m_NumberOfCalculatingRSThreads);
  singleLock.Lock();
  if (singleLock.IsLocked()) {
    bool fFlag;
    if (m_iNumberOfCalculatingRSThreads > 0) {
      fFlag = true;
    }
    else fFlag = false;
    singleLock.Unlock();

    return fFlag;
  }
  ASSERT(0);
  return false; // 此分支不可能执行到，只为了消除编译器的警告而存在
}