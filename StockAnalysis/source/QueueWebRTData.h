///////////////////////////////////////////////////////////
//
// 单向队列，用于存储从网络上提取的数据。由于提取数据采用线程模式，
// 故而此队列需要使用互斥区，以防止发生同步问题。
//
///////////////////////////////////////////////////////////
#pragma once

#include"afxmt.h"

#include"WebDataReceived.h"

using namespace std;
#include<queue>
#include<mutex>

class CQueueWebRTData final
{
public:
  CQueueWebRTData();
  ~CQueueWebRTData();
  void Reset(void);

  // 通用接口函数
  void PushWebRTData(CWebDataReceivedPtr pData);
  CWebDataReceivedPtr PopWebRTData(void);
  long GetWebRTDataSize(void);

protected:
  queue<CWebDataReceivedPtr> m_queueWebRTData;
  mutex m_MutexAccessWebRTData;
};
