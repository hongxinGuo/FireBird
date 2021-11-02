///////////////////////////////////////////////////////////
//
// 单向队列，用于存储从网络上提取的数据。由于提取数据采用线程模式，
// 故而此队列需要使用互斥区，以防止发生同步问题。
//
///////////////////////////////////////////////////////////
#pragma once

#include"afxmt.h"

#include"NeteaseDayLineWebData.h"

using namespace std;
#include<queue>
#include<mutex>

class CQueueDownLoadedNeteaseDayLine final
{
public:
	CQueueDownLoadedNeteaseDayLine();
	~CQueueDownLoadedNeteaseDayLine();
	void Reset(void);

	// 通用接口函数
	void PushData(CNeteaseDayLineWebDataPtr pData);
	CNeteaseDayLineWebDataPtr PopData(void);
	size_t GetDataSize(void);

protected:
	queue<CNeteaseDayLineWebDataPtr> m_qData;
	mutex m_MutexAccessData;
};
