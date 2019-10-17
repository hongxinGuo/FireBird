#include"globedef.h"

#include "SystemDequeData.h"
#include "SystemMessage.h"

// Ŀǰʹ��deque���д洢ʵʱ���ݣ��ʴ˱�����ʼֵ����Ϊ��
bool gl_fUsingPriorityQueue = true;

CSystemQueueData::CSystemQueueData()
{
  static int siCounter = 0;
  if (siCounter++ > 0) {
    TRACE("ϵͳ���ݶ���ֻ����һ��ʵ��\n");
    gl_systemMessage.PushInformationMessage(_T("����ϵͳ���ݶ���ֻ��������һ��ʵ��"));
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
  return nullptr; // �˷�֧������ִ�е���ֻΪ�������������ľ��������
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
  return false; // �˷�֧������ִ�е���ֻΪ�������������ľ��������
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
  return false; // �˷�֧������ִ�е���ֻΪ�������������ľ��������
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
  return false; // �˷�֧������ִ�е���ֻΪ�������������ľ��������
}