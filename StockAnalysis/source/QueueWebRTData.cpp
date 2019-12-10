#include"globedef.h"

#include "QueueWebRTData.h"
#include "SystemMessage.h"

CQueueWebRTData::CQueueWebRTData() {
}

CQueueWebRTData::~CQueueWebRTData() {
}

void CQueueWebRTData::Reset(void) {
  m_MutexAccessWebRTData.lock();
  long lTotal = m_queueWebRTData.size();
  for (int i = 0; i < lTotal; i++) { // Çå¿Õ¶ÓÁÐ
    m_queueWebRTData.pop();
  }
  m_MutexAccessWebRTData.unlock();
}

void CQueueWebRTData::PushWebRTData(CWebDataReceivedPtr pData) {
  m_MutexAccessWebRTData.lock();
  m_queueWebRTData.push(pData);
  m_MutexAccessWebRTData.unlock();
}

CWebDataReceivedPtr CQueueWebRTData::PopWebRTData(void) {
  CWebDataReceivedPtr pData;
  m_MutexAccessWebRTData.lock();
  pData = m_queueWebRTData.front();
  m_queueWebRTData.pop();
  m_MutexAccessWebRTData.unlock();
  return pData;
}

long CQueueWebRTData::GetWebRTDataSize(void) {
  size_t size = 0;
  m_MutexAccessWebRTData.lock();
  size = m_queueWebRTData.size();
  m_MutexAccessWebRTData.unlock();
  return size;
}