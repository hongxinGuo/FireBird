///////////////////////////////////////////////////////////
//
// ������У����ڴ洢����������ȡ�����ݡ�������ȡ���ݲ����߳�ģʽ��
// �ʶ��˶�����Ҫʹ�û��������Է�ֹ����ͬ�����⡣
//
// �����д洢��Ϊһ��ָ���������T������ָ�룬
//
///////////////////////////////////////////////////////////
#pragma once

#include<queue>
#include<mutex>
#include<memory>
using std::queue;
using std::shared_ptr;
using std::mutex;

template <typename T>
class CTemplateMutexAccessQueue {
public:
	CTemplateMutexAccessQueue() { }

	~CTemplateMutexAccessQueue() { }

	void Reset(void) {
		m_MutexAccessData.lock();
		const auto lTotal = m_qData.size();
		for (int i = 0; i < lTotal; i++) { m_qData.pop(); }
		m_MutexAccessData.unlock();
	}

	bool Empty() {
		m_MutexAccessData.lock();
		const bool fEmpty = m_qData.empty();
		m_MutexAccessData.unlock();
		return fEmpty;
	}

	// ͨ�ýӿں���
	void PushData(shared_ptr<T> pData) {
		m_MutexAccessData.lock();
		m_qData.push(pData);
		m_MutexAccessData.unlock();
	}

	shared_ptr<T> PopData(void) {
		ASSERT(m_qData.size() > 0);
		m_MutexAccessData.lock();
		shared_ptr<T> pData = m_qData.front();
		m_qData.pop();
		m_MutexAccessData.unlock();
		return pData;
	}

	size_t Size(void) {
		m_MutexAccessData.lock();
		const size_t size = m_qData.size();
		m_MutexAccessData.unlock();
		return size;
	}

protected:
	queue<shared_ptr<T>> m_qData;
	mutex m_MutexAccessData;
};