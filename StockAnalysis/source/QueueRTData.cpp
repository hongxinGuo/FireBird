#include"globedef.h"

#include "QueueRTData.h"
#include "SystemMessage.h"

CQueueRTData::CQueueRTData() {
}

CQueueRTData::~CQueueRTData() {
}

void CQueueRTData::Reset(void) {
  m_MutexAccessRTData.lock();
  long lTotal = m_qRTStockData.size();
  for (int i = 0; i < lTotal; i++) { // Çå¿Õ¶ÓÁÐ
    m_qRTStockData.pop();
  }
  m_MutexAccessRTData.unlock();
}

void CQueueRTData::PushRTData(CRTDataPtr pData) {
  m_MutexAccessRTData.lock();
  m_qRTStockData.push(pData);
  m_MutexAccessRTData.unlock();
}

CRTDataPtr CQueueRTData::PopRTData(void) {
  CRTDataPtr pData;
  m_MutexAccessRTData.lock();
  pData = m_qRTStockData.front();
  m_qRTStockData.pop();
  m_MutexAccessRTData.unlock();
  return pData;
}

long CQueueRTData::GetRTDataSize(void) {
  size_t size = 0;
  m_MutexAccessRTData.lock();
  size = m_qRTStockData.size();
  m_MutexAccessRTData.unlock();
  return size;
}