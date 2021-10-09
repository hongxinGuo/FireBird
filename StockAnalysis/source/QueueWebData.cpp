#include"pch.h"
#include"globedef.h"

#include "QueueWebData.h"
#include "SystemMessage.h"

CQueueWebData::CQueueWebData() {
}

CQueueWebData::~CQueueWebData() {
}

void CQueueWebData::Reset(void) {
	m_MutexAccessData.lock();
	const size_t lTotal = m_qData.size();
	for (int i = 0; i < lTotal; i++) { // Çå¿Õ¶ÓÁÐ
		m_qData.pop();
	}
	m_MutexAccessData.unlock();
}

void CQueueWebData::PushData(CWebDataPtr pData) {
	m_MutexAccessData.lock();
	m_qData.push(pData);
	m_MutexAccessData.unlock();
}

CWebDataPtr CQueueWebData::PopData(void) {
	CWebDataPtr pData;
	m_MutexAccessData.lock();
	pData = m_qData.front();
	m_qData.pop();
	m_MutexAccessData.unlock();
	return pData;
}

size_t CQueueWebData::GetDataSize(void) {
	size_t size = 0;
	m_MutexAccessData.lock();
	size = m_qData.size();
	m_MutexAccessData.unlock();
	return size;
}