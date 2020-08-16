#pragma once

#include"WebRTData.h"

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
  void PushData(CWebRTDataPtr pData);
  CWebRTDataPtr PopData(void);
  size_t GetDataSize(void);

protected:
  queue<CWebRTDataPtr> m_qStockData;
  mutex m_MutexAccessData;
};
