#include"pch.h"
#include"globedef.h"

#include "QueueString.h"
#include "SystemMessage.h"

CQueueString::CQueueString() {
	m_lCounter = 0;
}

CQueueString::~CQueueString() {
	m_MutexAccessData.lock();
	ASSERT(m_lCounter == m_qData.size());
	m_qData.empty();
	m_MutexAccessData.unlock();
}

void CQueueString::Reset(void) {
	m_MutexAccessData.lock();
	const auto lTotal = m_qData.size();
	for (int i = 0; i < lTotal; i++) { // Çå¿Õ¶ÓÁÐ
		m_qData.pop();
	}
	m_lCounter = 0;
	m_MutexAccessData.unlock();
}

void CQueueString::PushData(string data) {
	shared_ptr<string> pData = make_shared<string>(data);

	PushData(pData);
}

void CQueueString::PushData(shared_ptr<string> pData) {
	m_MutexAccessData.lock();
	m_qData.push(pData);
	m_lCounter++;
	m_MutexAccessData.unlock();
}

shared_ptr<string> CQueueString::PopData(void) {
	m_MutexAccessData.lock();
	auto data = m_qData.front();
	m_qData.pop();
	m_lCounter--;
	m_MutexAccessData.unlock();
	return data;
}

size_t CQueueString::GetDataSize(void) {
	m_MutexAccessData.lock();
	auto size = m_qData.size();
	m_MutexAccessData.unlock();
	return size;
}