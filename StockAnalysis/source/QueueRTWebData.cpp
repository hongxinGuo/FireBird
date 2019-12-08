#include"globedef.h"

#include "QueueRTWebData.h"
#include "SystemMessage.h"

CQueueRTWebData::CQueueRTWebData() {
}

CQueueRTWebData::~CQueueRTWebData() {
}

void CQueueRTWebData::Reset(void) {
  m_MutexAccessRTWebData.lock();
  long lTotal = m_queueRTWebData.size();
  for (int i = 0; i < lTotal; i++) { // Çå¿Õ¶ÓÁÐ
    m_queueRTWebData.pop();
  }
  m_MutexAccessRTWebData.unlock();
}

void CQueueRTWebData::PushRTWebData(CRTWebDataPtr pData) {
  m_MutexAccessRTWebData.lock();
  m_queueRTWebData.push(pData);
  m_MutexAccessRTWebData.unlock();
}

CRTWebDataPtr CQueueRTWebData::PopRTWebData(void) {
  CRTWebDataPtr pData;
  m_MutexAccessRTWebData.lock();
  pData = m_queueRTWebData.front();
  m_queueRTWebData.pop();
  m_MutexAccessRTWebData.unlock();
  return pData;
}

long CQueueRTWebData::GetRTWebDataSize(void) {
  size_t size = 0;
  m_MutexAccessRTWebData.lock();
  size = m_queueRTWebData.size();
  m_MutexAccessRTWebData.unlock();
  return size;
}