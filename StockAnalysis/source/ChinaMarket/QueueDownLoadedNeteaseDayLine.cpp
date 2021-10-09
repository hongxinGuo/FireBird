#include"pch.h"
#include"globedef.h"

#include "QueueDownLoadedNeteaseDayLine.h"
#include "SystemMessage.h"

CQueueDownLoadedNeteaseDayLine::CQueueDownLoadedNeteaseDayLine() {
}

CQueueDownLoadedNeteaseDayLine::~CQueueDownLoadedNeteaseDayLine() {
}

void CQueueDownLoadedNeteaseDayLine::Reset(void) {
	m_MutexAccessData.lock();
	const size_t lTotal = m_qData.size();
	for (int i = 0; i < lTotal; i++) { // Çå¿Õ¶ÓÁÐ
		m_qData.pop();
	}
	m_MutexAccessData.unlock();
}

void CQueueDownLoadedNeteaseDayLine::PushData(CDownLoadedNeteaseDayLinePtr pData) {
	m_MutexAccessData.lock();
	m_qData.push(pData);
	m_MutexAccessData.unlock();
}

CDownLoadedNeteaseDayLinePtr CQueueDownLoadedNeteaseDayLine::PopData(void) {
	CDownLoadedNeteaseDayLinePtr pData;
	m_MutexAccessData.lock();
	pData = m_qData.front();
	m_qData.pop();
	m_MutexAccessData.unlock();
	return pData;
}

size_t CQueueDownLoadedNeteaseDayLine::GetDataSize(void) {
	size_t size = 0;
	m_MutexAccessData.lock();
	size = m_qData.size();
	m_MutexAccessData.unlock();
	return size;
}