///////////////////////////////////////////////////////////
//
// ������У����ڴ洢����������ȡ�����ݡ�������ȡ���ݲ����߳�ģʽ��
// �ʶ��˶�����Ҫʹ�û��������Է�ֹ����ͬ�����⡣
//
///////////////////////////////////////////////////////////
#pragma once

#include"WebData.h"

using namespace std;
#include<queue>
#include<mutex>

class CQueueWebData final
{
public:
	CQueueWebData();
	~CQueueWebData();
	void Reset(void);

	// ͨ�ýӿں���
	void PushData(CWebDataPtr pData);
	CWebDataPtr PopData(void);
	size_t GetDataSize(void);

protected:
	queue<CWebDataPtr> m_qData;
	mutex m_MutexAccessData;
};
