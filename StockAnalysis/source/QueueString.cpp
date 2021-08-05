#include"pch.h"
#include"globedef.h"

#include "QueueString.h"
#include "SystemMessage.h"

CQueueString::CQueueString() {
}

CQueueString::~CQueueString() {
}

void CQueueString::Reset(void) {
	m_MutexAccessData.lock();
	const size_t lTotal = m_qData.size();
	for (int i = 0; i < lTotal; i++) { // Çå¿Õ¶ÓÁÐ
		m_qData.pop();
	}
	m_MutexAccessData.unlock();
}

void CQueueString::PushData(shared_ptr<string> pData) {
	m_MutexAccessData.lock();
	m_qData.push(pData);
	m_MutexAccessData.unlock();
}

shared_ptr<string> CQueueString::PopData(void) {
	shared_ptr<string> pData;
	m_MutexAccessData.lock();
	pData = m_qData.front();
	m_qData.pop();
	m_MutexAccessData.unlock();
	return pData;
}

size_t CQueueString::GetDataSize(void) {
	size_t size = 0;
	m_MutexAccessData.lock();
	size = m_qData.size();
	m_MutexAccessData.unlock();
	return size;
}