#include"globedef.h"

#include "QueueWebRTData.h"
#include "SystemMessage.h"

CQueueWebRTData::CQueueWebRTData() {
}

CQueueWebRTData::~CQueueWebRTData() {
}

void CQueueWebRTData::Reset(void) {
  m_MutexAccessData.lock();
  const size_t lTotal = m_qStockData.size();
  for (int i = 0; i < lTotal; i++) { // Çå¿Õ¶ÓÁÐ
    m_qStockData.pop();
  }
  m_MutexAccessData.unlock();
}

void CQueueWebRTData::PushData(CWebRTDataPtr pData) {
  m_MutexAccessData.lock();
  m_qStockData.push(pData);
  m_MutexAccessData.unlock();
}

CWebRTDataPtr CQueueWebRTData::PopData(void) {
  CWebRTDataPtr pData;
  m_MutexAccessData.lock();
  pData = m_qStockData.front();
  m_qStockData.pop();
  m_MutexAccessData.unlock();
  return pData;
}

size_t CQueueWebRTData::GetDataSize(void) {
  size_t size = 0;
  m_MutexAccessData.lock();
  size = m_qStockData.size();
  m_MutexAccessData.unlock();
  return size;
}