#include"globedef.h"

#include "QueueRTData.h"
#include "SystemMessage.h"

// 目前使用deque队列存储实时数据，故此变量初始值设置为假。在测试使用priority_queue成功后，即弃用deque(此标识永远为真）。
#define __fUsingPriorityQueue__

CQueueRTData::CQueueRTData()
{
  static int siCounter = 0;
  if (siCounter++ > 0) {
    TRACE("系统数据队列只允许一个实例\n");
    gl_systemMessage.PushInformationMessage(_T("错误：系统数据队列只允许生成一个实例"));
  }
}

CQueueRTData::~CQueueRTData()
{
}

void CQueueRTData::Reset(void)
{
  CSingleLock singleLock(&m_RTDataLock);
  singleLock.Lock();
  if (singleLock.IsLocked()) {
    long lTotal = m_queueRTStockData.size();
    for (int i = 0; i < lTotal; i++) { // 清空队列
      m_queueRTStockData.pop();
    }
    singleLock.Unlock();
  }

  CSingleLock singleLockPriorityQueue(&m_PriorityRTDataLock);
  singleLockPriorityQueue.Lock();
  if (singleLockPriorityQueue.IsLocked()) {
    long lTotal = m_priorityqueueRTStockData.size();
    for (int i = 0; i < lTotal; i++) { // 清空队列
      m_priorityqueueRTStockData.pop();
    }
    singleLockPriorityQueue.Unlock();
  }
}

void CQueueRTData::PushRTData(CRTDataPtr pData) {
#ifdef __fUsingPriorityQueue__
  PushPriorityRTData(pData);
#else
  PushDequeRTData(pData);
#endif
}

CRTDataPtr CQueueRTData::PopRTData(void) {
#ifdef __fUsingPriorityQueue__
  return(PopPriorityRTData());
#else
  return(PopDequeRTData());
#endif
}

long CQueueRTData::GetRTDataSize(void) {
#ifdef __fUsingPriorityQueue__
  return(GetPriorityRTDataSize());
#else
  return(GetDequeRTDataSize());
#endif
}

void CQueueRTData::PushDequeRTData(CRTDataPtr pData)
{
  CSingleLock singleLock(&m_RTDataLock);
  singleLock.Lock();
  if (singleLock.IsLocked()) {
    m_queueRTStockData.push(pData);
    singleLock.Unlock();
  }
}

CRTDataPtr CQueueRTData::PopDequeRTData(void)
{
  CRTDataPtr pData;
  CSingleLock singleLock(&m_RTDataLock);
  singleLock.Lock();
  if (singleLock.IsLocked()) {
    pData = m_queueRTStockData.front();
    m_queueRTStockData.pop();
    singleLock.Unlock();
    return pData;
  }
  ASSERT(0);
  return nullptr; // 此分支不可能执行到，只为了消除编译器的警告而存在
}

long CQueueRTData::GetDequeRTDataSize(void)
{
  CSingleLock singleLock(&m_RTDataLock);
  singleLock.Lock();
  if (singleLock.IsLocked()) {
    const long lCount = m_queueRTStockData.size();
    singleLock.Unlock();
    return lCount;
  }
  ASSERT(0);
  return false; // 此分支不可能执行到，只为了消除编译器的警告而存在
}

void CQueueRTData::PushPriorityRTData(CRTDataPtr pData)
{
  CSingleLock singleLock(&m_PriorityRTDataLock);
  singleLock.Lock();
  if (singleLock.IsLocked()) {
    m_priorityqueueRTStockData.push(pData);
    singleLock.Unlock();
  }
}

CRTDataPtr CQueueRTData::PopPriorityRTData(void)
{
  static CRTDataPtr pLastData = nullptr;
  CRTDataPtr pData;
  CSingleLock singleLock(&m_PriorityRTDataLock);
  singleLock.Lock();
  if (singleLock.IsLocked()) {
    pData = m_priorityqueueRTStockData.top();
    m_priorityqueueRTStockData.pop();
    singleLock.Unlock();
    return pData;
  }
  ASSERT(0);
  return false; // 此分支不可能执行到，只为了消除编译器的警告而存在
}

long CQueueRTData::GetPriorityRTDataSize(void)
{
  CSingleLock singleLock(&m_PriorityRTDataLock);
  singleLock.Lock();
  if (singleLock.IsLocked()) {
    const long lCount = m_priorityqueueRTStockData.size();
    singleLock.Unlock();
    return lCount;
  }
  ASSERT(0);
  return false; // 此分支不可能执行到，只为了消除编译器的警告而存在
}