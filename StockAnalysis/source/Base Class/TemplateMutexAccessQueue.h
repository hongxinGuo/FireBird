///////////////////////////////////////////////////////////
//
// ������У����ڴ洢����������ȡ�����ݡ�������ȡ���ݲ����߳�ģʽ��
// �ʶ��˶�����Ҫʹ�û��������Է�ֹ����ͬ�����⡣
//
///////////////////////////////////////////////////////////
#pragma once

using namespace std;
#include<queue>
#include<mutex>

template <typename T> class CTemplateMutexAccessQueue {
public:
	CTemplateMutexAccessQueue() {}
	~CTemplateMutexAccessQueue() {}
	void Reset(void) { m_MutexAccessData.lock(); const auto lTotal = m_qData.size(); for (int i = 0; i < lTotal; i++) { m_qData.pop(); }	m_MutexAccessData.unlock(); }

	// ͨ�ýӿں���
	void PushData(T data) { m_MutexAccessData.lock(); m_qData.push(data); m_MutexAccessData.unlock(); }
	T PopData(void) { T data;	m_MutexAccessData.lock();	data = m_qData.front(); m_qData.pop();	m_MutexAccessData.unlock();	return data; }
	size_t GetDataSize(void) { size_t size = 0;	m_MutexAccessData.lock();	size = m_qData.size(); m_MutexAccessData.unlock(); return size; }

protected:
	queue<T> m_qData;
	mutex m_MutexAccessData;
};
