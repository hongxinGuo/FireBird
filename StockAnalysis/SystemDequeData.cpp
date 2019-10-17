#include"globedef.h"

#include "SystemDequeData.h"
#include "SystemMessage.h"

// 目前使用deque队列存储实时数据，故此变量初始值设置为假
bool gl_fUsingPriorityQueue = true;

CSystemQueueData::CSystemQueueData()
{
  static int siCounter = 0;
  if (siCounter++ > 0) {
    TRACE("系统数据队列只允许一个实例\n");
    gl_systemMessage.PushInformationMessage(_T("错误：系统数据队列只允许生成一个实例"));
  }
}

CSystemQueueData::~CSystemQueueData()
{
}

void CSystemQueueData::Reset(void)
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

void CSystemQueueData::PushRTData(CStockRTDataPtr pData) {
  if( gl_fUsingPriorityQueue)  PushPriorityRTData(pData);
  else PushDequeRTData(pData);
}

CStockRTDataPtr CSystemQueueData::PopRTData(void) {
  if( gl_fUsingPriorityQueue) return(PopPriorityRTData());
  else return(PopDequeRTData());
}

long CSystemQueueData::GetRTDataSize(void) {
  if(gl_fUsingPriorityQueue) return(GetPriorityRTDataSize());
  else return(GetDequeRTDataSize());
}

void CSystemQueueData::PushDequeRTData(CStockRTDataPtr pData)
{
  CSingleLock singleLock(&m_RTDataLock);
  singleLock.Lock();
  if (singleLock.IsLocked()) {
    m_queueRTStockData.push(pData);
    singleLock.Unlock();
  }
}

CStockRTDataPtr CSystemQueueData::PopDequeRTData(void)
{
  CStockRTDataPtr pData;
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

long CSystemQueueData::GetDequeRTDataSize(void)
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

void CSystemQueueData::PushPriorityRTData(CStockRTDataPtr pData)
{
  CSingleLock singleLock(&m_PriorityRTDataLock);
  singleLock.Lock();
  if (singleLock.IsLocked()) {
    m_priorityqueueRTStockData.push(pData);
    singleLock.Unlock();
  }
}

CStockRTDataPtr CSystemQueueData::PopPriorityRTData(void)
{
  CStockRTDataPtr pData;
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

long CSystemQueueData::GetPriorityRTDataSize(void)
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