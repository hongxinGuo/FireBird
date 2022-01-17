///////////////////////////////////////////////////////////
//
// ������У����ڴ洢����������ȡ���ַ������ݡ�������ȡ���ݲ����߳�ģʽ��
// �ʶ��˶�����Ҫʹ�û��������Է�ֹ����ͬ�����⡣
//
///////////////////////////////////////////////////////////
#pragma once

using namespace std;
#include<queue>
#include<mutex>
#include<string>

class CQueueString final
{
public:
	CQueueString();
	~CQueueString();
	void Reset(void);

	// ͨ�ýӿں���
	void PushData(string data);
	void PushData(shared_ptr<string> pData);
	shared_ptr<string> PopData(void);
	size_t GetDataSize(void);

protected:
	queue<shared_ptr<string>> m_qData;
	mutex m_MutexAccessData;
};
