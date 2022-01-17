///////////////////////////////////////////////////////////
//
// 单向队列，用于存储从网络上提取的字符串数据。由于提取数据采用线程模式，
// 故而此队列需要使用互斥区，以防止发生同步问题。
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

	// 通用接口函数
	void PushData(string data);
	void PushData(shared_ptr<string> pData);
	shared_ptr<string> PopData(void);
	size_t GetDataSize(void);

protected:
	queue<shared_ptr<string>> m_qData;
	mutex m_MutexAccessData;
};
