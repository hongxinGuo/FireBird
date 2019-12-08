#pragma once

#include"afxmt.h"

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
  void PushWebRTData(CWebRTDataPtr pData);
  CWebRTDataPtr PopWebRTData(void);
  long GetWebRTDataSize(void);

protected:
  queue<CWebRTDataPtr> m_queueWebRTData;
  mutex m_MutexAccessWebRTData;
};
