#include"globedef.h"

#include "QueueWebData.h"
#include "SystemMessage.h"

CQueueWebData::CQueueWebData() {
}

CQueueWebData::~CQueueWebData() {
}

void CQueueWebData::Reset(void) {
  m_MutexAccessWebData.lock();
  long lTotal = m_queueWebData.size();
  for (int i = 0; i < lTotal; i++) { // Çå¿Õ¶ÓÁÐ
    m_queueWebData.pop();
  }
  m_MutexAccessWebData.unlock();
}

void CQueueWebData::PushWebData(CWebDataReceivedPtr pData) {
  m_MutexAccessWebData.lock();
  m_queueWebData.push(pData);
  m_MutexAccessWebData.unlock();
}

CWebDataReceivedPtr CQueueWebData::PopWebData(void) {
  CWebDataReceivedPtr pData;
  m_MutexAccessWebData.lock();
  pData = m_queueWebData.front();
  m_queueWebData.pop();
  m_MutexAccessWebData.unlock();
  return pData;
}

long CQueueWebData::GetWebDataSize(void) {
  size_t size = 0;
  m_MutexAccessWebData.lock();
  size = m_queueWebData.size();
  m_MutexAccessWebData.unlock();
  return size;
}