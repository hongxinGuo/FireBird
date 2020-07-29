#pragma once

#include"WebRTData.h"

using namespace std;
#include<queue>
#include<mutex>

class CQueueRTData final
{
public:
  CQueueRTData();
  ~CQueueRTData();
  void Reset(void);

  // 通用接口函数
  void PushRTData(CWebRTDataPtr pData);
  CWebRTDataPtr PopRTData(void);
  size_t GetRTDataSize(void);

protected:
  queue<CWebRTDataPtr> m_qRTStockData;
  mutex m_MutexAccessRTData;
};
