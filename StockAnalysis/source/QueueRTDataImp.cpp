#include"globedef.h"

#include "QueueRTDataImp.h"
#include "SystemMessage.h"

CQueueRTDataImp::CQueueRTDataImp() {
}

CQueueRTDataImp::~CQueueRTDataImp() {
}

void CQueueRTDataImp::Reset(void) {
  m_MutexAccessRTData.lock();
  long lTotal = m_queueRTStockData.size();
  for (int i = 0; i < lTotal; i++) { // Çå¿Õ¶ÓÁÐ
    m_queueRTStockData.pop();
  }
  m_MutexAccessRTData.unlock();
}

void CQueueRTDataImp::PushRTData(CRTDataPtr pData) {
  m_MutexAccessRTData.lock();
  m_queueRTStockData.push(pData);
  m_MutexAccessRTData.unlock();
}

CRTDataPtr CQueueRTDataImp::PopRTData(void) {
  CRTDataPtr pData;
  m_MutexAccessRTData.lock();
  pData = m_queueRTStockData.front();
  m_queueRTStockData.pop();
  m_MutexAccessRTData.unlock();
  return pData;
}

long CQueueRTDataImp::GetRTDataSize(void) {
  size_t size = 0;
  m_MutexAccessRTData.lock();
  size = m_queueRTStockData.size();
  m_MutexAccessRTData.unlock();
  return size;
}