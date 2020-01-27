#include"globedef.h"

#include "QueueWebData.h"
#include "SystemMessage.h"

CQueueWebData::CQueueWebData() {
}

CQueueWebData::~CQueueWebData() {
}

void CQueueWebData::Reset(void) {
  m_MutexAccessWebData.lock();
  long lTotal = m_qWebData.size();
  for (int i = 0; i < lTotal; i++) { // Çå¿Õ¶ÓÁÐ
    m_qWebData.pop();
  }
  m_MutexAccessWebData.unlock();
}

void CQueueWebData::PushWebData(CWebDataReceivedPtr pData) {
  m_MutexAccessWebData.lock();
  m_qWebData.push(pData);
  m_MutexAccessWebData.unlock();
}

CWebDataReceivedPtr CQueueWebData::PopWebData(void) {
  CWebDataReceivedPtr pData;
  m_MutexAccessWebData.lock();
  pData = m_qWebData.front();
  m_qWebData.pop();
  m_MutexAccessWebData.unlock();
  return pData;
}

long CQueueWebData::GetWebDataSize(void) {
  size_t size = 0;
  m_MutexAccessWebData.lock();
  size = m_qWebData.size();
  m_MutexAccessWebData.unlock();
  return size;
}