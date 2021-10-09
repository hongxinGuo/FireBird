///////////////////////////////////////////////////////////
//
// ������У����ڴ洢����������ȡ�����ݡ�������ȡ���ݲ����߳�ģʽ��
// �ʶ��˶�����Ҫʹ�û��������Է�ֹ����ͬ�����⡣
//
///////////////////////////////////////////////////////////
#pragma once

#include"afxmt.h"

#include"DownLoadedNeteaseDayLine.h"

using namespace std;
#include<queue>
#include<mutex>

class CQueueDownLoadedNeteaseDayLine final
{
public:
	CQueueDownLoadedNeteaseDayLine();
	~CQueueDownLoadedNeteaseDayLine();
	void Reset(void);

	// ͨ�ýӿں���
	void PushData(CDownLoadedNeteaseDayLinePtr pData);
	CDownLoadedNeteaseDayLinePtr PopData(void);
	size_t GetDataSize(void);

protected:
	queue<CDownLoadedNeteaseDayLinePtr> m_qData;
	mutex m_MutexAccessData;
};
