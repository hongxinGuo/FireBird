#include"globedef.h"

#include "QueueRTData.h"
#include "SystemMessage.h"

// Ŀǰʹ��deque���д洢ʵʱ���ݣ��ʴ˱�����ʼֵ����Ϊ�١��ڲ���ʹ��priority_queue�ɹ��󣬼�����deque(�˱�ʶ��ԶΪ�棩��
#define __fUsingPriorityQueue__

CQueueRTData::CQueueRTData()
{
  static int siCounter = 0;
  if (siCounter++ > 0) {
    TRACE("ϵͳ���ݶ���ֻ����һ��ʵ��\n");
    gl_systemMessage.PushInformationMessage(_T("����ϵͳ���ݶ���ֻ��������һ��ʵ��"));
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
    for (int i = 0; i < lTotal; i++) { // ��ն���
      m_queueRTStockData.pop();
    }
    singleLock.Unlock();
  }

  CSingleLock singleLockPriorityQueue(&m_PriorityRTDataLock);
  singleLockPriorityQueue.Lock();
  if (singleLockPriorityQueue.IsLocked()) {
    long lTotal = m_priorityqueueRTStockData.size();
    for (int i = 0; i < lTotal; i++) { // ��ն���
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
  return nullptr; // �˷�֧������ִ�е���ֻΪ�������������ľ��������
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
  return false; // �˷�֧������ִ�е���ֻΪ�������������ľ��������
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
  return false; // �˷�֧������ִ�е���ֻΪ�������������ľ��������
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
  return false; // �˷�֧������ִ�е���ֻΪ�������������ľ��������
}